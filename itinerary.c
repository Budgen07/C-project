#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "itinerary.h"

Itinerary itineraries[MAX_ITINERARIES];
int count = 0;
int next_id = 1;

int safeInt() {
    int x;
    while (scanf("%d", &x) != 1) {
        printf("Invalid input. Try again: ");
        while (getchar() != '\n');
    }
    while(getchar()!='\n');
    return x;
}

void loadData() {
    FILE *file = fopen(FILE_NAME, "r");
    if (!file) return;

    count = 0;
    while (fscanf(file, "%d", &itineraries[count].id) == 1) {
        fscanf(file, " %[^\n]", itineraries[count].name);
        fscanf(file, " %[^\n]", itineraries[count].start_date);
        fscanf(file, " %[^\n]", itineraries[count].end_date);
        fscanf(file, "%d", &itineraries[count].stop_count);

        for (int i = 0; i < itineraries[count].stop_count; i++) {
            fscanf(file, " %[^\n]", itineraries[count].stops[i].city);
            fscanf(file, " %[^\n]", itineraries[count].stops[i].country);
            fscanf(file, " %[^\n]", itineraries[count].stops[i].cost);
        }

        if (itineraries[count].id >= next_id)
            next_id = itineraries[count].id + 1;

        count++;
    }
    fclose(file);
}

void saveData() {
    FILE *file = fopen(FILE_NAME, "w");

    for (int i = 0; i < count; i++) {
        fprintf(file, "%d\n", itineraries[i].id);
        fprintf(file, "%s\n", itineraries[i].name);
        fprintf(file, "%s\n", itineraries[i].start_date);
        fprintf(file, "%s\n", itineraries[i].end_date);
        fprintf(file, "%d\n", itineraries[i].stop_count);

        for (int j = 0; j < itineraries[i].stop_count; j++) {
            fprintf(file, "%s\n%s\n%s\n",
                itineraries[i].stops[j].city,
                itineraries[i].stops[j].country,
                itineraries[i].stops[j].cost
            );
        }
    }

    fclose(file);
}

void addItinerary() {
    Itinerary it;
    it.id = next_id++;

    printf("Enter itinerary name: ");
    fgets(it.name, MAX_LEN, stdin);
    strtok(it.name, "\n");

    printf("Start date: ");
    fgets(it.start_date, MAX_LEN, stdin);
    strtok(it.start_date, "\n");

    printf("End date: ");
    fgets(it.end_date, MAX_LEN, stdin);
    strtok(it.end_date, "\n");

    printf("Number of stops (1-%d): ", MAX_STOPS);
    it.stop_count = safeInt();
    if (it.stop_count < 1) it.stop_count = 1;
    if (it.stop_count > MAX_STOPS) it.stop_count = MAX_STOPS;

    for (int i = 0; i < it.stop_count; i++) {
        printf("Stop %d city: ", i + 1);
        fgets(it.stops[i].city, MAX_LEN, stdin);
        strtok(it.stops[i].city, "\n");

        printf("Stop %d country: ", i + 1);
        fgets(it.stops[i].country, MAX_LEN, stdin);
        strtok(it.stops[i].country, "\n");

        printf("Stop %d cost: ", i + 1);
        fgets(it.stops[i].cost, MAX_LEN, stdin);
        strtok(it.stops[i].cost, "\n");
    }

    itineraries[count++] = it;
    saveData();
    printf("Added successfully!\n");
}

void listItineraries() {
    if (count == 0) {
        printf("\nNo itineraries found.\n");
        return;
    }

    printf("\n======================================================================\n");
    printf("| %-4s | %-30s |\n", "ID", "Name");
    printf("======================================================================\n");

    for (int i = 0; i < count; i++) {
        printf("| %-4d | %-30s |\n", itineraries[i].id, itineraries[i].name);
    }

    printf("======================================================================\n");
}


void viewItinerary() {
    printf("Enter ID: ");
    int id = safeInt();

    for (int i = 0; i < count; i++) {
        if (itineraries[i].id == id) {
            printf("\n=============== ITINERARY DETAILS ===============\n");
printf("ID: %d\n", itineraries[i].id);
printf("Name: %s\n", itineraries[i].name);
printf("Start Date: %s\n", itineraries[i].start_date);
printf("End Date:   %s\n", itineraries[i].end_date);
printf("-------------------------------------------------\n");

for (int j = 0; j < itineraries[i].stop_count; j++) {
    printf("Stop %-2d | %-12s | %-12s | Cost: %s\n",
           j+1,
           itineraries[i].stops[j].city,
           itineraries[i].stops[j].country,
           itineraries[i].stops[j].cost
    );
}
printf("=================================================\n");

            return;
        }
    }
    printf("Not found.\n");
}

void deleteItinerary() {
    printf("Enter ID to delete: ");
    int id = safeInt();

    for (int i = 0; i < count; i++) {
        if (itineraries[i].id == id) {
            for (int j = i; j < count - 1; j++)
                itineraries[j] = itineraries[j + 1];
            count--;
            saveData();
            printf("Deleted.\n");
            return;
        }
    }
    printf("Not found.\n");
}

void searchItineraries() {
    char key[MAX_LEN];
    printf("Enter keyword: ");
    fgets(key, MAX_LEN, stdin);
    strtok(key, "\n");

    for (char *p = key; *p; p++) *p = tolower(*p);

    printf("\nResults:\n");
    for (int i = 0; i < count; i++) {
        char temp[MAX_LEN];
        strcpy(temp, itineraries[i].name);
        for (char *p = temp; *p; p++) *p = tolower(*p);

        if (strstr(temp, key)) {
            printf("%d | %s\n", itineraries[i].id, itineraries[i].name);
        }
    }
}

void sortByName() {
    for (int i = 0; i < count - 1; i++)
        for (int j = i + 1; j < count; j++)
            if (strcmp(itineraries[i].name, itineraries[j].name) > 0) {
                Itinerary temp = itineraries[i];
                itineraries[i] = itineraries[j];
                itineraries[j] = temp;
            }
    saveData();
    printf("Sorted by name.\n");
}

void sortByID() {
    for (int i = 0; i < count - 1; i++)
        for (int j = i + 1; j < count; j++)
            if (itineraries[i].id > itineraries[j].id) {
                Itinerary temp = itineraries[i];
                itineraries[i] = itineraries[j];
                itineraries[j] = temp;
            }
    saveData();
    printf("Sorted by ID.\n");
}
