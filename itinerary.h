#ifndef ITINERARY_H
#define ITINERARY_H

#define MAX_ITINERARIES 100
#define MAX_STOPS 10
#define MAX_LEN 50
#define FILE_NAME "itineraries.txt"

typedef struct {
    char city[MAX_LEN];
    char country[MAX_LEN];
    char cost[MAX_LEN];
} Stop;

typedef struct {
    int id;
    char name[MAX_LEN];
    char start_date[MAX_LEN];
    char end_date[MAX_LEN];
    int stop_count;
    Stop stops[MAX_STOPS];
} Itinerary;

void loadData();
void saveData();
void addItinerary();
void listItineraries();
void viewItinerary();
void deleteItinerary();
void searchItineraries();
void sortByName();
void sortByID();
int safeInt();

#endif
