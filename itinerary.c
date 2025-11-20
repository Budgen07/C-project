#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
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

// ========================= STRING VALIDATION ============================

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
        printf("Enter cost (integer, >= 0): ");
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
        for (int j = 0; j < bars; j++) printf("#");
        printf("\n");
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

    printf("\nEnter itinerary name: ");
    fgets(it.name, MAX_LEN, stdin);
    strtok(it.name, "\n");

    while (1) {
        printf("\nEnter start date (dd mm yyyy): ");
        scanf("%d %d %d", &it.start_day, &it.start_month, &it.start_year);
        while (getchar() != '\n');

        if (!isValidDate(it.start_day, it.start_month, it.start_year))
            printf(RED "Invalid calendar date.\n" RESET);
        else if (isPastDate(it.start_day, it.start_month, it.start_year))
            printf(RED "Start date cannot be in the past.\n" RESET);
        else
            break;
    }

    while (1) {
        printf("\nEnter end date (dd mm yyyy): ");
        scanf("%d %d %d", &it.end_day, &it.end_month, &it.end_year);
        while (getchar() != '\n');

        if (!isValidDate(it.end_day, it.end_month, it.end_year))
            printf(RED "Invalid calendar date.\n" RESET);
        else if (isPastDate(it.end_day, it.end_month, it.end_year))
            printf(RED "End date cannot be in the past.\n" RESET);
        else
            break;
    }

    if (tripDuration(it) < 1) {
        printf(RED "End date cannot be before start date.\n" RESET);
        return;
    }

    printf("\nNumber of stops (1-%d): ", MAX_STOPS);
    it.stop_count = safeInt();
    if (it.stop_count < 1) it.stop_count = 1;
    if (it.stop_count > MAX_STOPS) it.stop_count = MAX_STOPS;

    for (int i = 0; i < it.stop_count; i++) {
        while (1) {
            printf("\nStop %d city: ", i+1);
            fgets(it.stops[i].city, MAX_LEN, stdin);
            strtok(it.stops[i].city, "\n");
            if (!isValidCityOrCountry(it.stops[i].city))
                printf(RED "Invalid city name. Try again.\n" RESET);
            else break;
        }

        while (1) {
            printf("Stop %d country: ", i+1);
            fgets(it.stops[i].country, MAX_LEN, stdin);
            strtok(it.stops[i].country, "\n");
            if (!isValidCityOrCountry(it.stops[i].country))
                printf(RED "Invalid country name. Try again.\n" RESET);
            else break;
        }

        it.stops[i].cost = readValidatedCost();
        it.stops[i].completed = 0;
    }

    printf("\nMark as favorite? (1 = yes, 0 = no): ");
    it.favorite = safeInt();
    if (it.favorite != 1) it.favorite = 0;

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

    printf(CYAN BOLD "\n================= All Itineraries =================\n" RESET);
    printf("| %-4s | %-30s | %-10s | %-9s |\n", "ID", "Name", "Status", "Favorite");
    printf("-------------------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        printf("| %-4d | %-30s | %-10s | %-9s |\n",
               itineraries[i].id,
               itineraries[i].name,
               tripStatus(itineraries[i]),
               itineraries[i].favorite ? "Yes" : "No");
    }
}

void viewItinerary() {
    printf("Enter itinerary ID: ");
    int id = safeInt();

    for (int i = 0; i < count; i++) {
        if (itineraries[i].id == id) {
            Itinerary it = itineraries[i];
            int dur = tripDuration(it);
            int days_start = daysUntilStart(it);

            printf(CYAN BOLD "\n========== ITINERARY DETAILS ==========\n" RESET);
            printf("ID:        %d\n", it.id);
            printf("Name:      %s\n", it.name);
            printf("Start:     %02d-%02d-%04d (%s)\n",
                   it.start_day, it.start_month, it.start_year,
                   weekdayName(it.start_day, it.start_month, it.start_year));
            printf("End:       %02d-%02d-%04d (%s)\n",
                   it.end_day, it.end_month, it.end_year,
                   weekdayName(it.end_day, it.end_month, it.end_year));

            printf("Status:    %s\n", tripStatus(it));
            if (days_start > 0)
                printf("Begins in: %d days\n", days_start);
            else if (days_start == 0)
                printf("Begins:    Today\n");

            printf("Duration:  %d days\n", dur);
            printf("Total cost: %d\n", totalCost(it));
            printf("Cost/day:  %.2f\n", costPerDay(it));
            printf("Favorite:  %s\n", it.favorite ? "Yes" : "No");

            printf("\nStops:\n");
            printf("-------------------------------------------------------\n");
            printf("| %-4s | %-15s | %-15s | %-8s | %-9s |\n",
                   "No.", "City", "Country", "Cost", "Done?");
            printf("-------------------------------------------------------\n");

            for (int j = 0; j < it.stop_count; j++) {
                printf("| %-4d | %-15s | %-15s | %-8d | %-9s |\n",
                       j+1,
                       it.stops[j].city,
                       it.stops[j].country,
                       it.stops[j].cost,
                       it.stops[j].completed ? "Yes" : "No");
            }
            printf("-------------------------------------------------------\n");

            showCostChart(it);
            return;
        }
    }
    printf(RED "Itinerary with that ID not found.\n" RESET);
}

void deleteItinerary() {
    printf("Enter ID to delete: ");
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

    // AUTO-REALLOCATE IDs (NEW FEATURE)
    for (int i = 0; i < count; i++)
        itineraries[i].id = i + 1;

    next_id = count + 1;

    saveData();
    printf(GREEN "✓ Deleted and IDs rearranged.\n" RESET);
}

void searchItineraries() {
    char key[MAX_LEN];
    printf("Enter search keyword: ");
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
    printf(CYAN "\nFavorite itineraries:\n" RESET);
    for (int i = 0; i < count; i++) {
        if (itineraries[i].favorite) {
            printf("%d | %s\n", itineraries[i].id, itineraries[i].name);
            found = 1;
        }
    }
    if (!found) printf(YELLOW "No favorites yet.\n" RESET);
}

void toggleFavorite() {
    printf("Enter ID to toggle favorite: ");
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
    printf("Enter ID to export: ");
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
    printf("Enter ID to edit: ");
    int id = safeInt();

    for (int i = 0; i < count; i++) {
        if (itineraries[i].id == id) {
            Itinerary *it = &itineraries[i];
            char temp[MAX_LEN];

            printf(CYAN "\nEditing itinerary: %s\n" RESET, it->name);

            printf("New name (ENTER to keep '%s'): ", it->name);
            fgets(temp, MAX_LEN, stdin);
            if (temp[0] != '\n') {
                strtok(temp, "\n");
                strcpy(it->name, temp);
            }

            int d,m,y;

            printf("New start date (dd mm yyyy, 0 0 0 to keep): ");
            scanf("%d %d %d", &d, &m, &y);
            while (getchar() != '\n');
            if (!(d==0 && m==0 && y==0)) {
                if (!isValidDate(d,m,y) || isPastDate(d,m,y)) {
                    printf(RED "Invalid start date.\n" RESET);
                    return;
                }
                it->start_day = d; it->start_month = m; it->start_year = y;
            }

            printf("New end date (dd mm yyyy, 0 0 0 to keep): ");
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
                printf("\n--- Stop Editor for '%s' ---\n", it->name);
                printf("1. Edit stop\n");
                printf("2. Add stop\n");
                printf("3. Remove stop\n");
                printf("4. Toggle stop completed\n");
                printf("0. Done\n");
                printf("Choice: ");

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
