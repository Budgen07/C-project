#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

int safeInt() {
    int x;
    while (scanf("%d", &x) != 1) {
        printf(RED "Invalid input. Please enter an integer: " RESET);
        while (getchar() != '\n');
    }
    while (getchar() != '\n');
    return x;
}

void pauseScreen() {
    printf(YELLOW "\nPress ENTER to continue..." RESET);
    while (getchar() != '\n');
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

int askStringWithBack(const char *prompt, char *out, int maxlen, int *isBack) {
    char buf[512];
    *isBack = 0;
    printf("%s (enter 'B' to go back)\n", prompt);
    if (!fgets(buf, sizeof(buf), stdin)) return 0;
    buf[strcspn(buf, "\n")] = '\0';
    if ((strlen(buf) == 1) && (buf[0] == 'B' || buf[0] == 'b')) {
        *isBack = 1;
        return 1;
    }
    strncpy(out, buf, maxlen - 1);
    out[maxlen - 1] = '\0';
    return 1;
}

int askIntWithBack(const char *prompt, int *out, int *isBack) {
    char buf[128];
    *isBack = 0;
    printf("%s (enter 'B' to go back)\n", prompt);
    if (!fgets(buf, sizeof(buf), stdin)) return 0;
    buf[strcspn(buf, "\n")] = '\0';
    if ((strlen(buf) == 1) && (buf[0] == 'B' || buf[0] == 'b')) {
        *isBack = 1;
        return 1;
    }
    char *endptr;
    long val = strtol(buf, &endptr, 10);
    if (endptr == buf || *endptr != '\0') {
        printf(RED "Invalid integer input.\n" RESET);
        return 0;
    }
    *out = (int)val;
    return 1;
}

int isValidName(const char *s) {
    int len = 0;
    for (int i = 0; s[i] != '\0'; i++) {
        if (!isspace((unsigned char)s[i])) len++;
    }
    return len > 0;
}

int isValidCityOrCountry(const char *s) {
    int len = 0;
    int only_digits = 1;
    for (int i = 0; s[i] != '\0'; i++) {
        unsigned char c = (unsigned char)s[i];
        if (!isspace(c)) len++;
        if (isalpha(c)) only_digits = 0;
        if (!(isalpha(c) || isspace(c) || c == '-' || c == '\'')) return 0;
    }
    if (len == 0) return 0;
    if (only_digits) return 0;
    return 1;
}

int readValidatedCost() {
    int c;
    while (1) {
        printf(BLUE "Enter cost (integer, >= 0): " RESET);
        c = safeInt();
        if (c < 0) printf(RED "Cost must be non-negative.\n" RESET);
        else return c;
    }
}

int isLeap(int year) {
    return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
}

int isValidDate(int d, int m, int y) {
    if (y < 1 || m < 1 || m > 12 || d < 1) return 0;
    int daysInMonth[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    if (m == 2 && isLeap(y)) return d <= 29;
    else return d <= daysInMonth[m];
}

int isPastDate(int d, int m, int y) {
    time_t t = time(NULL);
    struct tm *today = localtime(&t);
    int td = today->tm_mday, tm = today->tm_mon + 1, ty = today->tm_year + 1900;
    if (y < ty) return 1;
    if (y == ty && m < tm) return 1;
    if (y == ty && m == tm && d < td) return 1;
    return 0;
}

int dateToDays(int d, int m, int y) {
    if (m < 3) { y--; m += 12; }
    return 365*y + y/4 - y/100 + y/400 + (153*m - 457)/5 + d - 306;
}

int tripDuration(Itinerary it) {
    int start = dateToDays(it.start_day, it.start_month, it.start_year);
    int end   = dateToDays(it.end_day, it.end_month, it.end_year);
    return (end - start) + 1;
}

const char* weekdayName(int d, int m, int y) {
    struct tm date = {0};
    date.tm_mday = d; date.tm_mon = m - 1; date.tm_year = y - 1900;
    mktime(&date);
    const char *names[] = {"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
    return names[date.tm_wday];
}

int daysUntilStart(Itinerary it) {
    time_t t = time(NULL);
    struct tm *today = localtime(&t);
    int startDays = dateToDays(it.start_day, it.start_month, it.start_year);
    int todayDays = dateToDays(today->tm_mday, today->tm_mon + 1, today->tm_year + 1900);
    return startDays - todayDays;
}

const char* tripStatus(Itinerary it) {
    time_t t = time(NULL);
    struct tm *today = localtime(&t);
    int todayDays = dateToDays(today->tm_mday, today->tm_mon + 1, today->tm_year + 1900);
    int startDays = dateToDays(it.start_day, it.start_month, it.start_year);
    int endDays   = dateToDays(it.end_day, it.end_month, it.end_year);

    if (todayDays < startDays) return "Upcoming";
    if (todayDays > endDays)   return "Completed";
    return "Ongoing";
}

int totalCost(Itinerary it) {
    int sum = 0;
    for (int i = 0; i < it.stop_count; i++) sum += it.stops[i].cost;
    return sum;
}

double costPerDay(Itinerary it) {
    int dur = tripDuration(it);
    if (dur <= 0) return 0.0;
    return (double)totalCost(it) / (double)dur;
}

void showCostChart(Itinerary it) {
    int total = totalCost(it);
    if (total <= 0) {
        printf(YELLOW "No cost chart available (total cost = 0).\n" RESET);
        return;
    }
    printf(CYAN "\nCost distribution by stop:\n" RESET);
    for (int i = 0; i < it.stop_count; i++) {
        double pct = 100.0 * it.stops[i].cost / total;
        int bars = (int)(pct / 5.0);
        printf("Stop %2d (%s): %6.2f%% ", i+1, it.stops[i].city, pct);
        printf(MAGENTA);
        for (int j = 0; j < bars; j++) printf("#");
        printf(RESET "\n");
    }
}