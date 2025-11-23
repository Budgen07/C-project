#include <stdio.h>
#include "common.h"
#include "storage.h"
#include "utils.h"

void loadData() {
    FILE *file = fopen(FILE_NAME, "r");
    if (!file) return;

    count = 0;
    while (count < MAX_ITINERARIES && fscanf(file, "%d", &itineraries[count].id) == 1) {
        fscanf(file, " %[^\n]", itineraries[count].name);
        fscanf(file, "%d %d %d", &itineraries[count].start_day, &itineraries[count].start_month, &itineraries[count].start_year);
        fscanf(file, "%d %d %d", &itineraries[count].end_day, &itineraries[count].end_month, &itineraries[count].end_year);
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
        fprintf(file, "%d %d %d\n", itineraries[i].start_day, itineraries[i].start_month, itineraries[i].start_year);
        fprintf(file, "%d %d %d\n", itineraries[i].end_day, itineraries[i].end_month, itineraries[i].end_year);
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
            fprintf(f, "Start: %02d-%02d-%04d (%s)\n", it.start_day, it.start_month, it.start_year, weekdayName(it.start_day, it.start_month, it.start_year));
            fprintf(f, "End:   %02d-%02d-%04d (%s)\n", it.end_day, it.end_month, it.end_year, weekdayName(it.end_day, it.end_month, it.end_year));
            fprintf(f, "Status: %s\n", tripStatus(it));
            fprintf(f, "Duration: %d days\n", tripDuration(it));
            fprintf(f, "Total cost: %d\n", totalCost(it));
            fprintf(f, "Favorite: %s\n", it.favorite ? "Yes" : "No");
            fprintf(f, "\nStops:\n");
            for (int j = 0; j < it.stop_count; j++) {
                fprintf(f, "Stop %d: %s, %s | cost=%d | completed=%s\n", j+1, it.stops[j].city, it.stops[j].country, it.stops[j].cost, it.stops[j].completed ? "Yes" : "No");
            }
            fclose(f);
            printf(GREEN "✓ Exported to file: %s\n" RESET, filename);
            return;
        }
    }
    printf(RED "Not found.\n" RESET);
}