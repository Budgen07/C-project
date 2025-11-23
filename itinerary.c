#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include "itinerary.h"

Itinerary itineraries[MAX_ITINERARIES];
int count = 0;
int next_id = 1;

// ========================= BASIC HELPERS ============================

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

// ========================= NEW: INPUT HELPERS WITH BACK =====================
// Returns 1 on success, 0 on parse error. *isBack is set to 1 if user requested back via "B" or "b".
int askStringWithBack(const char *prompt, char *out, int maxlen, int *isBack) {
    char buf[512];
    *isBack = 0;
    printf("%s (enter 'B' to go back)\n", prompt);
    if (!fgets(buf, sizeof(buf), stdin)) return 0;
    // strip newline
    buf[strcspn(buf, "\n")] = '\0';
    // check back
    if ((strlen(buf) == 1) && (buf[0] == 'B' || buf[0] == 'b')) {
        *isBack = 1;
        return 1;
    }
    // normal copy
    strncpy(out, buf, maxlen - 1);
    out[maxlen - 1] = '\0';
    return 1;
}

// For numeric input: read a line; if "B" or "b" => isBack=1; else parse integer into *out.
// Returns 1 if parse succeeded (or back requested), 0 otherwise.
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

// ========================= STRING VALIDATION ============================

// Check if string is not empty and has actual content
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
        if (!(isalpha(c) || isspace(c) || c == '-' || c == '\'')) {
            return 0;
        }
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
        if (c < 0) {
            printf(RED "Cost must be non-negative.\n" RESET);
        } else {
            return c;
        }
    }
}

// ========================= DATE VALIDATION ============================

int isLeap(int year) {
    return (year % 400 == 0) ||
           (year % 4 == 0 && year % 100 != 0);
}

int isValidDate(int d, int m, int y) {
    if (y < 1 || m < 1 || m > 12 || d < 1) return 0;

    int daysInMonth[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    if (m == 2 && isLeap(y))
        return d <= 29;
    else
        return d <= daysInMonth[m];
}

int isPastDate(int d, int m, int y) {
    time_t t = time(NULL);
    struct tm *today = localtime(&t);

    int td = today->tm_mday;
    int tm = today->tm_mon + 1;
    int ty = today->tm_year + 1900;

    if (y < ty) return 1;
    if (y == ty && m < tm) return 1;
    if (y == ty && m == tm && d < td) return 1;
    return 0;
}

// ========================= DATE / TIME MATH ============================

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
    date.tm_mday = d;
    date.tm_mon  = m - 1;
    date.tm_year = y - 1900;
    mktime(&date);

    const char *names[] = {
        "Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"
    };
    return names[date.tm_wday];
}

int daysUntilStart(Itinerary it) {
    time_t t = time(NULL);
    struct tm *today = localtime(&t);

    int td = today->tm_mday;
    int tm = today->tm_mon + 1;
    int ty = today->tm_year + 1900;

    int todayDays = dateToDays(td, tm, ty);
    int startDays = dateToDays(it.start_day, it.start_month, it.start_year);
    return startDays - todayDays;
}

const char* tripStatus(Itinerary it) {
    time_t t = time(NULL);
    struct tm *today = localtime(&t);

    int td = today->tm_mday;
    int tm = today->tm_mon + 1;
    int ty = today->tm_year + 1900;

    int todayDays = dateToDays(td, tm, ty);
    int startDays = dateToDays(it.start_day, it.start_month, it.start_year);
    int endDays   = dateToDays(it.end_day, it.end_month, it.end_year);

    if (todayDays < startDays) return "Upcoming";
    if (todayDays > endDays)   return "Completed";
    return "Ongoing";
}

// ========================= COST HELPERS ============================

int totalCost(Itinerary it) {
    int sum = 0;
    for (int i = 0; i < it.stop_count; i++)
        sum += it.stops[i].cost;
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
        printf(MAGENTA); // Bar color
        for (int j = 0; j < bars; j++) printf("#");
        printf(RESET "\n");
    }
}

// ========================= LOAD & SAVE ============================

void loadData() {
    FILE *file = fopen(FILE_NAME, "r");
    if (!file) return;

    count = 0;
    while (count < MAX_ITINERARIES && fscanf(file, "%d", &itineraries[count].id) == 1) {
        fscanf(file, " %[^\n]", itineraries[count].name);

        fscanf(file, "%d %d %d",
            &itineraries[count].start_day,
            &itineraries[count].start_month,
            &itineraries[count].start_year);

        fscanf(file, "%d %d %d",
            &itineraries[count].end_day,
            &itineraries[count].end_month,
            &itineraries[count].end_year);

        fscanf(file, "%d", &itineraries[count].stop_count);

        for (int i = 0; i < itineraries[count].stop_count; i++) {
            fscanf(file, " %[^\n]", itineraries[count].stops[i].city);
            fscanf(file, " %[^\n]", itineraries[count].stops[i].country);
            fscanf(file, "%d", &itineraries[count].stops[i].cost);
            fscanf(file, "%d", &itineraries[count].stops[i].completed);
        }

        fscanf(file, "%d", &itineraries[count].favorite);

        if (itineraries[count].id >= next_id)
            next_id = itineraries[count].id + 1;

        count++;
    }
    fclose(file);
}

void saveData() {
    FILE *file = fopen(FILE_NAME, "w");
    if (!file) {
        printf(RED "Error opening file for saving.\n" RESET);
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%d\n", itineraries[i].id);
        fprintf(file, "%s\n", itineraries[i].name);

        fprintf(file, "%d %d %d\n",
            itineraries[i].start_day,
            itineraries[i].start_month,
            itineraries[i].start_year);

        fprintf(file, "%d %d %d\n",
            itineraries[i].end_day,
            itineraries[i].end_month,
            itineraries[i].end_year);

        fprintf(file, "%d\n", itineraries[i].stop_count);

        for (int j = 0; j < itineraries[i].stop_count; j++) {
            fprintf(file, "%s\n", itineraries[i].stops[j].city);
            fprintf(file, "%s\n", itineraries[i].stops[j].country);
            fprintf(file, "%d\n", itineraries[i].stops[j].cost);
            fprintf(file, "%d\n", itineraries[i].stops[j].completed);
        }

        fprintf(file, "%d\n", itineraries[i].favorite);
    }

    fclose(file);
}

// ========================= CORE FEATURES ============================

void addItinerary() {
    if (count >= MAX_ITINERARIES) {
        printf(RED "Cannot add more itineraries (limit reached).\n" RESET);
        return;
    }

    Itinerary it;
    it.id = next_id++;

    // Step-based input so user can go BACK one step at any time.
    int step = 1;
    int current_stop = 0; // for step 5
    while (step <= 6) {
        int isBack = 0;
        char temp[MAX_LEN];

        if (step == 1) {
            // Name
            while (1) {
                if (!askStringWithBack(BLUE "Enter itinerary name:" RESET, temp, MAX_LEN, &isBack)) {
                    printf(RED "Input error. Try again.\n" RESET);
                    continue;
                }
                if (isBack) {
                    // back at step 1 => cancel add
                    printf(YELLOW "Add cancelled. Returning to main menu.\n" RESET);
                    next_id--; // undo id allocation
                    return;
                }
                if (isValidName(temp)) {
                    strcpy(it.name, temp);
                    break;
                }
                printf(RED "Invalid name. Name cannot be empty.\n" RESET);
            }
            step++;
        } else if (step == 2) {
            // Start date
            int d,m,y;
            while (1) {
                printf(BLUE "Enter start date (dd mm yyyy) (or B to go back): " RESET);
                char line[128];
                if (!fgets(line, sizeof(line), stdin)) { printf(RED "Input error.\n" RESET); continue; }
                line[strcspn(line, "\n")] = '\0';
                if ((strlen(line) == 1) && (line[0]=='B' || line[0]=='b')) { isBack = 1; break; }
                if (sscanf(line, "%d %d %d", &d, &m, &y) != 3) { printf(RED "Invalid format. Use: dd mm yyyy\n" RESET); continue; }
                if (!isValidDate(d,m,y)) { printf(RED "Invalid calendar date.\n" RESET); continue; }
                if (isPastDate(d,m,y)) { printf(RED "Start date cannot be in the past.\n" RESET); continue; }
                it.start_day = d; it.start_month = m; it.start_year = y;
                break;
            }
            if (isBack) { step--; continue; }
            step++;
        } else if (step == 3) {
            // End date
            int d,m,y;
            while (1) {
                printf(BLUE "Enter end date (dd mm yyyy) (or B to go back): " RESET);
                char line[128];
                if (!fgets(line, sizeof(line), stdin)) { printf(RED "Input error.\n" RESET); continue; }
                line[strcspn(line, "\n")] = '\0';
                if ((strlen(line) == 1) && (line[0]=='B' || line[0]=='b')) { isBack = 1; break; }
                if (sscanf(line, "%d %d %d", &d, &m, &y) != 3) { printf(RED "Invalid format. Use: dd mm yyyy\n" RESET); continue; }
                if (!isValidDate(d,m,y)) { printf(RED "Invalid calendar date.\n" RESET); continue; }
                if (isPastDate(d,m,y)) { printf(RED "End date cannot be in the past.\n" RESET); continue; }
                it.end_day = d; it.end_month = m; it.end_year = y;
                break;
            }
            if (isBack) { step--; continue; }
            if (tripDuration(it) < 1) { printf(RED "End date cannot be before start date.\n" RESET); return; }
            step++;
        } else if (step == 4) {
            // Number of stops
            int sc;
            while (1) {
                if (!askIntWithBack(BLUE "Number of stops (1-20):" RESET, &sc, &isBack)) {
                    printf(RED "Invalid input. Try again.\n" RESET);
                    continue;
                }
                if (isBack) { break; }
                if (sc < 1) sc = 1;
                if (sc > MAX_STOPS) sc = MAX_STOPS;
                it.stop_count = sc;
                break;
            }
            if (isBack) { step--; continue; }
            step++;
            current_stop = 0;
        } else if (step == 5) {
            // Stops input (iterate stops, each supports back to previous stop)
            while (current_stop < it.stop_count) {
                int thisBack = 0;
                // city
                while (1) {
                    char prompt[160];
                    snprintf(prompt, sizeof(prompt), BLUE "\nStop %d city:" RESET, current_stop + 1);
                    if (!askStringWithBack(prompt, temp, MAX_LEN, &thisBack)) { printf(RED "Input error.\n" RESET); continue; }
                    if (thisBack) {
                        if (current_stop == 0) {
                            // go back to step 4 (stop count)
                            step = 4;
                            break;
                        } else {
                            // go back to previous stop
                            current_stop--;
                            break;
                        }
                    }
                    if (!isValidCityOrCountry(temp)) { printf(RED "Invalid city name. Try again.\n" RESET); continue; }
                    strcpy(it.stops[current_stop].city, temp);
                    break;
                }
                if (step != 5) break; // handled back to step
                if ((int)strlen(it.stops[current_stop].city) == 0) continue; // moved back

                // country
                while (1) {
                    char prompt[160];
                    snprintf(prompt, sizeof(prompt), BLUE "Stop %d country:" RESET, current_stop + 1);
                    if (!askStringWithBack(prompt, temp, MAX_LEN, &thisBack)) { printf(RED "Input error.\n" RESET); continue; }
                    if (thisBack) { /* go back to city input for this stop */ continue; }
                    if (!isValidCityOrCountry(temp)) { printf(RED "Invalid country name. Try again.\n" RESET); continue; }
                    strcpy(it.stops[current_stop].country, temp);
                    break;
                }

                // cost
                while (1) {
                    char prompt[160];
                    snprintf(prompt, sizeof(prompt), BLUE "Stop %d cost (integer >=0):" RESET, current_stop + 1);
                    int cost;
                    int costBack = 0;
                    if (!askIntWithBack(prompt, &cost, &costBack)) { printf(RED "Invalid input.\n" RESET); continue; }
                    if (costBack) { /* go back to country input */ break; }
                    if (cost < 0) { printf(RED "Cost must be non-negative.\n" RESET); continue; }
                    it.stops[current_stop].cost = cost;
                    it.stops[current_stop].completed = 0;
                    break;
                }

                // If user used back on cost (costBack), we returned to country entry above;
                // otherwise move to next stop
                if (strlen(it.stops[current_stop].city) > 0 && strlen(it.stops[current_stop].country) > 0)
                    current_stop++;
            }

            if (step != 5) continue; // we jumped back to earlier step
            // if reached here, all stops entered
            step++;
        } else if (step == 6) {
            // favorite
            int fav;
            while (1) {
                if (!askIntWithBack(BLUE "\nMark as favorite? (1 = yes, 0 = no):" RESET, &fav, &isBack)) {
                    printf(RED "Invalid input.\n" RESET); continue;
                }
                if (isBack) { step--; break; }
                if (fav != 1) fav = 0;
                it.favorite = fav;
                break;
            }
            if (isBack) continue;
            step++;
        }
    } // end while(steps)

    // finished collecting everything
    itineraries[count++] = it;
    saveData();

    printf(GREEN "\n✓ Itinerary created successfully!\n" RESET);
    printf(CYAN "Assigned ID: %d\n" RESET, it.id);
}

void listItineraries() {
    if (count == 0) {
        printf(RED "\nNo itineraries found.\n" RESET);
        return;
    }

    printf(MAGENTA "\n================= All Itineraries =================\n" RESET);
    printf(YELLOW "| %-4s | %-30s | %-10s | %-9s |\n" RESET, "ID", "Name", "Status", "Favorite");
    printf(MAGENTA "-------------------------------------------------------------\n" RESET);

    for (int i = 0; i < count; i++) {
        const char* stat = tripStatus(itineraries[i]);
        char color[20];

        // Dynamic coloring based on status
        if (strcmp(stat, "Completed") == 0) strcpy(color, GREEN);
        else if (strcmp(stat, "Upcoming") == 0) strcpy(color, CYAN);
        else strcpy(color, YELLOW); // Ongoing

        // ALIGNMENT FIX: Truncate name to 27 chars + "..." if too long
        char dispName[35];
        if (strlen(itineraries[i].name) > 30) {
            strncpy(dispName, itineraries[i].name, 27);
            dispName[27] = '\0';
            strcat(dispName, "...");
        } else {
            strcpy(dispName, itineraries[i].name);
        }

        printf("| %-4d | %-30s | %s%-10s" RESET " | %-9s |\n",
               itineraries[i].id,
               dispName, // Use truncated name
               color, stat,
               itineraries[i].favorite ? "Yes" : "No");
    }
}

void viewItinerary() {
    printf(BLUE "Enter itinerary ID: " RESET);
    int id = safeInt();

    for (int i = 0; i < count; i++) {
        if (itineraries[i].id == id) {
            Itinerary it = itineraries[i];
            int dur = tripDuration(it);
            int days_start = daysUntilStart(it);

            printf(CYAN "\n========== ITINERARY DETAILS ==========\n" RESET);
            printf(WHITE "ID:        " RESET "%d\n", it.id);
            printf(WHITE "Name:      " RESET "%s\n", it.name);
            printf(WHITE "Start:     " RESET "%02d-%02d-%04d (%s)\n",
                   it.start_day, it.start_month, it.start_year,
                   weekdayName(it.start_day, it.start_month, it.start_year));
            printf(WHITE "End:       " RESET "%02d-%02d-%04d (%s)\n",
                   it.end_day, it.end_month, it.end_year,
                   weekdayName(it.end_day, it.end_month, it.end_year));

            printf(WHITE "Status:    " RESET "%s\n", tripStatus(it));
            if (days_start > 0)
                printf(WHITE "Begins in: " RESET "%d days\n", days_start);
            else if (days_start == 0)
                printf(WHITE "Begins:    " RESET "Today\n");

            printf(WHITE "Duration:  " RESET "%d days\n", dur);
            printf(WHITE "Total cost:" RESET " %d\n", totalCost(it));
            printf(WHITE "Cost/day:  " RESET "%.2f\n", costPerDay(it));
            printf(WHITE "Favorite:  " RESET "%s\n", it.favorite ? "Yes" : "No");

            printf(YELLOW "\nStops:\n" RESET);
            printf(MAGENTA "-------------------------------------------------------\n" RESET);
            printf(YELLOW "| %-4s | %-15s | %-15s | %-8s | %-9s |\n" RESET,
                   "No.", "City", "Country", "Cost", "Done?");
            printf(MAGENTA "-------------------------------------------------------\n" RESET);

            for (int j = 0; j < it.stop_count; j++) {
                printf("| %-4d | %-15s | %-15s | %-8d | %-9s |\n",
                       j+1,
                       it.stops[j].city,
                       it.stops[j].country,
                       it.stops[j].cost,
                       it.stops[j].completed ? "Yes" : "No");
            }
            printf(MAGENTA "-------------------------------------------------------\n" RESET);

            showCostChart(it);
            return;
        }
    }
    printf(RED "Itinerary with that ID not found.\n" RESET);
}

void deleteItinerary() {
    printf(BLUE "Enter ID to delete: " RESET);
    int id = safeInt();

    int index = -1;
    for (int i = 0; i < count; i++) {
        if (itineraries[i].id == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf(RED "Not found.\n" RESET);
        return;
    }

    // Shift left
    for (int i = index; i < count - 1; i++)
        itineraries[i] = itineraries[i+1];

    count--;

    // AUTO-REALLOCATE IDs
    for (int i = 0; i < count; i++)
        itineraries[i].id = i + 1;

    next_id = count + 1;

    saveData();
    printf(GREEN "✓ Deleted and IDs rearranged.\n" RESET);
}

void searchItineraries() {
    char key[MAX_LEN];
    printf(BLUE "Enter search keyword: " RESET);
    fgets(key, MAX_LEN, stdin);
    strtok(key, "\n");

    char keylower[MAX_LEN];
    strcpy(keylower, key);
    for (int i = 0; keylower[i]; i++)
        keylower[i] = (char)tolower((unsigned char)keylower[i]);

    int found = 0;
    printf(CYAN "\nSearch results:\n" RESET);
    for (int i = 0; i < count; i++) {
        char nameLower[MAX_LEN];
        strcpy(nameLower, itineraries[i].name);
        for (int j = 0; nameLower[j]; j++)
            nameLower[j] = (char)tolower((unsigned char)nameLower[j]);

        if (strstr(nameLower, keylower)) {
            printf("%d | %s\n", itineraries[i].id, itineraries[i].name);
            found = 1;
        }
    }
    if (!found) printf(YELLOW "No matching itineraries.\n" RESET);
}

void sortByName() {
    for (int i = 0; i < count - 1; i++)
        for (int j = i + 1; j < count; j++)
            if (strcmp(itineraries[i].name, itineraries[j].name) > 0) {
                Itinerary tmp = itineraries[i];
                itineraries[i] = itineraries[j];
                itineraries[j] = tmp;
            }
    saveData();
    printf(GREEN "✓ Sorted by name.\n" RESET);
}

void sortByID() {
    for (int i = 0; i < count - 1; i++)
        for (int j = i + 1; j < count; j++)
            if (itineraries[i].id > itineraries[j].id) {
                Itinerary tmp = itineraries[i];
                itineraries[i] = itineraries[j];
                itineraries[j] = tmp;
            }
    saveData();
    printf(GREEN "✓ Sorted by ID.\n" RESET);
}

void listFavorites() {
    int found = 0;
    printf(MAGENTA "\nFavorite itineraries:\n" RESET);
    for (int i = 0; i < count; i++) {
        if (itineraries[i].favorite) {
            printf(YELLOW "★ %d | %s\n" RESET, itineraries[i].id, itineraries[i].name);
            found = 1;
        }
    }
    if (!found) printf(YELLOW "No favorites yet.\n" RESET);
}

void toggleFavorite() {
    printf(BLUE "Enter ID to toggle favorite: " RESET);
    int id = safeInt();

    for (int i = 0; i < count; i++) {
        if (itineraries[i].id == id) {
            itineraries[i].favorite = !itineraries[i].favorite;
            saveData();
            printf(GREEN "Favorite status changed.\n" RESET);
            return;
        }
    }
    printf(RED "Not found.\n" RESET);
}

void exportItinerary() {
    printf(BLUE "Enter ID to export: " RESET);
    int id = safeInt();

    for (int i = 0; i < count; i++) {
        if (itineraries[i].id == id) {
            char filename[64];
            snprintf(filename, sizeof(filename), "itinerary_%d.txt", id);
            FILE *f = fopen(filename, "w");
            if (!f) {
                printf(RED "Error opening export file.\n" RESET);
                return;
            }
            Itinerary it = itineraries[i];
            fprintf(f, "Itinerary ID: %d\n", it.id);
            fprintf(f, "Name: %s\n", it.name);
            fprintf(f, "Start: %02d-%02d-%04d (%s)\n",
                    it.start_day, it.start_month, it.start_year,
                    weekdayName(it.start_day, it.start_month, it.start_year));
            fprintf(f, "End:   %02d-%02d-%04d (%s)\n",
                    it.end_day, it.end_month, it.end_year,
                    weekdayName(it.end_day, it.end_month, it.end_year));
            fprintf(f, "Status: %s\n", tripStatus(it));
            fprintf(f, "Duration: %d days\n", tripDuration(it));
            fprintf(f, "Total cost: %d\n", totalCost(it));
            fprintf(f, "Favorite: %s\n", it.favorite ? "Yes" : "No");
            fprintf(f, "\nStops:\n");
            for (int j = 0; j < it.stop_count; j++) {
                fprintf(f, "Stop %d: %s, %s | cost=%d | completed=%s\n",
                        j+1,
                        it.stops[j].city,
                        it.stops[j].country,
                        it.stops[j].cost,
                        it.stops[j].completed ? "Yes" : "No");
            }
            fclose(f);
            printf(GREEN "✓ Exported to file: %s\n" RESET, filename);
            return;
        }
    }
    printf(RED "Not found.\n" RESET);
}

void editItinerary() {
    printf(BLUE "Enter ID to edit: " RESET);
    int id = safeInt();

    for (int i = 0; i < count; i++) {
        if (itineraries[i].id == id) {
            Itinerary *it = &itineraries[i];
            char temp[MAX_LEN];

            printf(CYAN "\nEditing itinerary: %s\n" RESET, it->name);

            printf(BLUE "New name (ENTER to keep '%s'): " RESET, it->name);
            fgets(temp, MAX_LEN, stdin);
            if (temp[0] != '\n') {
                strtok(temp, "\n");
                if (isValidName(temp)) {
                    strcpy(it->name, temp);
                } else {
                    printf(RED "Invalid name ignored. Keeping old name.\n" RESET);
                }
            }

            int d,m,y;

            printf(BLUE "New start date (dd mm yyyy, 0 0 0 to keep): " RESET);
            scanf("%d %d %d", &d, &m, &y);
            while (getchar() != '\n');
            if (!(d==0 && m==0 && y==0)) {
                if (!isValidDate(d,m,y) || isPastDate(d,m,y)) {
                    printf(RED "Invalid start date.\n" RESET);
                    return;
                }
                it->start_day = d; it->start_month = m; it->start_year = y;
            }

            printf(BLUE "New end date (dd mm yyyy, 0 0 0 to keep): " RESET);
            scanf("%d %d %d", &d, &m, &y);
            while (getchar() != '\n');
            if (!(d==0 && m==0 && y==0)) {
                if (!isValidDate(d,m,y) || isPastDate(d,m,y)) {
                    printf(RED "Invalid end date.\n" RESET);
                    return;
                }
                it->end_day = d; it->end_month = m; it->end_year = y;
            }

            if (tripDuration(*it) < 1) {
                printf(RED "End date cannot be before start.\n" RESET);
                return;
            }

            while (1) {
                printf(CYAN "\n--- Stop Editor for '%s' ---\n" RESET, it->name);
                printf("1. Edit stop\n");
                printf("2. Add stop\n");
                printf("3. Remove stop\n");
                printf("4. Toggle stop completed\n");
                printf("0. Done\n");
                printf(BLUE "Choice: " RESET);

                int choice = safeInt();
                if (choice == 1) {
                    printf("Enter stop number to edit (1-%d): ", it->stop_count);
                    int s = safeInt() - 1;
                    if (s < 0 || s >= it->stop_count) {
                        printf(RED "Invalid stop.\n" RESET);
                        continue;
                    }
                    printf("New city (ENTER to keep '%s'): ", it->stops[s].city);
                    fgets(temp, MAX_LEN, stdin);
                    if (temp[0] != '\n') { strtok(temp, "\n"); if (isValidCityOrCountry(temp)) strcpy(it->stops[s].city, temp); }

                    printf("New country (ENTER to keep '%s'): ", it->stops[s].country);
                    fgets(temp, MAX_LEN, stdin);
                    if (temp[0] != '\n') { strtok(temp, "\n"); if (isValidCityOrCountry(temp)) strcpy(it->stops[s].country, temp); }

                    printf("New cost (0 to keep %d): ", it->stops[s].cost);
                    int c = safeInt();
                    if (c > 0) it->stops[s].cost = c;

                } else if (choice == 2) {
                    if (it->stop_count >= MAX_STOPS) {
                        printf(RED "Cannot add more stops.\n" RESET);
                        continue;
                    }
                    int s = it->stop_count++;
                    while (1) {
                        printf("City: ");
                        fgets(it->stops[s].city, MAX_LEN, stdin);
                        strtok(it->stops[s].city, "\n");
                        if (isValidCityOrCountry(it->stops[s].city)) break;
                        printf(RED "Invalid city.\n" RESET);
                    }
                    while (1) {
                        printf("Country: ");
                        fgets(it->stops[s].country, MAX_LEN, stdin);
                        strtok(it->stops[s].country, "\n");
                        if (isValidCityOrCountry(it->stops[s].country)) break;
                        printf(RED "Invalid country.\n" RESET);
                    }
                    it->stops[s].cost = readValidatedCost();
                    it->stops[s].completed = 0;

                } else if (choice == 3) {
                    printf("Stop to remove (1-%d): ", it->stop_count);
                    int s = safeInt() - 1;
                    if (s < 0 || s >= it->stop_count) {
                        printf(RED "Invalid stop.\n" RESET);
                        continue;
                    }
                    for (int j = s; j < it->stop_count - 1; j++)
                        it->stops[j] = it->stops[j+1];
                    it->stop_count--;

                } else if (choice == 4) {
                    printf("Stop to toggle completed: ");
                    int s = safeInt() - 1;
                    if (s < 0 || s >= it->stop_count) {
                        printf(RED "Invalid stop.\n" RESET);
                        continue;
                    }
                    it->stops[s].completed = !it->stops[s].completed;

                } else if (choice == 0) {
                    break;
                } else {
                    printf(RED "Invalid choice.\n" RESET);
                }
            }

            saveData();
            printf(GREEN "✓ Itinerary updated.\n" RESET);
            return;
        }
    }

    printf(RED "ID not found.\n" RESET);
}

// ========================= CITY STATS ============================

typedef struct {
    char name[MAX_LEN];
    int count;
} CityStat;

void viewCityStats() {
    printf(CYAN "\n========== CITY STATISTICS ==========\n" RESET);
    printf("1. Cities Visited (Past/Completed Trips)\n");
    printf("2. Future Destinations (Upcoming Trips)\n");
    printf(BLUE "Choice: " RESET);

    int mode = safeInt();
    if (mode != 1 && mode != 2) {
        printf(RED "Invalid mode.\n" RESET);
        return;
    }

    CityStat stats[MAX_ITINERARIES * MAX_STOPS];
    int uniqueCount = 0;
    int totalStopsFound = 0;

    for (int i = 0; i < count; i++) {
        const char* status = tripStatus(itineraries[i]);
        int isMatch = 0;

        if (mode == 1 && strcmp(status, "Completed") == 0) isMatch = 1;
        if (mode == 2 && strcmp(status, "Upcoming") == 0) isMatch = 1;

        if (isMatch) {
            for (int j = 0; j < itineraries[i].stop_count; j++) {
                totalStopsFound++;
                char currentCity[MAX_LEN];
                strcpy(currentCity, itineraries[i].stops[j].city);

                char currentLower[MAX_LEN];
                strcpy(currentLower, currentCity);
                for(int k=0; currentLower[k]; k++)
                    currentLower[k] = (char)tolower((unsigned char)currentLower[k]);

                int found = -1;
                for (int k = 0; k < uniqueCount; k++) {
                    char storedLower[MAX_LEN];
                    strcpy(storedLower, stats[k].name);
                    for(int z=0; storedLower[z]; z++)
                        storedLower[z] = (char)tolower((unsigned char)storedLower[z]);

                    if (strcmp(currentLower, storedLower) == 0) {
                        found = k;
                        break;
                    }
                }

                if (found != -1) {
                    stats[found].count++;
                } else {
                    strcpy(stats[uniqueCount].name, currentCity);
                    stats[uniqueCount].count = 1;
                    uniqueCount++;
                }
            }
        }
    }

    if (uniqueCount == 0) {
        printf(YELLOW "\nNo cities found for this category.\n" RESET);
        return;
    }

    // Sort Descending
    for (int i = 0; i < uniqueCount - 1; i++) {
        for (int j = i + 1; j < uniqueCount; j++) {
            if (stats[j].count > stats[i].count) {
                CityStat temp = stats[i];
                stats[i] = stats[j];
                stats[j] = temp;
            }
        }
    }

    if (mode == 1) printf(GREEN "\n=== CITIES VISITED (PAST TRIPS) ===\n" RESET);
    else printf(CYAN "\n=== FUTURE DESTINATIONS (UPCOMING) ===\n" RESET);

    printf(YELLOW "%-20s | %-10s\n" RESET, "City", "Visits");
    printf(MAGENTA "--------------------------------\n" RESET);
    for (int i = 0; i < uniqueCount; i++) {
        printf("%-20s | %d\n", stats[i].name, stats[i].count);
    }
    printf(MAGENTA "--------------------------------\n" RESET);
    printf(BLUE "Total stops analyzed: %d\n" RESET, totalStopsFound);
}

// ========================= RESET FEATURE ============================

void resetAllItineraries() {
    // Simple math question confirmation
    srand((unsigned int)time(NULL));
    int a = rand() % 10 + 1;
    int b = rand() % 10 + 1;
    int correct = a + b;
    printf(RED "\nWARNING: This will DELETE ALL itineraries permanently!\n" RESET);
    printf(YELLOW "To confirm, answer: %d + %d = ? (enter the number): " RESET, a, b);
    int ans = safeInt();
    if (ans != correct) {
        printf(RED "Wrong answer. Reset aborted.\n" RESET);
        return;
    }

    // clear memory
    count = 0;
    next_id = 1;
    // remove saved file content
    FILE *f = fopen(FILE_NAME, "w");
    if (f) fclose(f);
    printf(GREEN "✓ All itineraries deleted.\n" RESET);
}
