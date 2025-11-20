#ifndef ITINERARY_H
#define ITINERARY_H

#include <time.h>

#define MAX_ITINERARIES 100
#define MAX_STOPS 20
#define MAX_LEN 100
#define FILE_NAME "itineraries.txt"

// CLI Colors (ANSI)
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

typedef struct {
    char city[MAX_LEN];
    char country[MAX_LEN];
    int cost;         // integer cost (>= 0)
    int completed;    // 0 = not done, 1 = completed
} Stop;

typedef struct {
    int id;
    char name[MAX_LEN];

    int start_day, start_month, start_year;
    int end_day, end_month, end_year;

    int stop_count;
    Stop stops[MAX_STOPS];

    int favorite;     // 0 = no, 1 = yes
} Itinerary;

// Core functions
void loadData();
void saveData();
void addItinerary();
void listItineraries();
void viewItinerary();
void deleteItinerary();
void searchItineraries();
void sortByName();
void sortByID();
void editItinerary();
void listFavorites();
void toggleFavorite();
void exportItinerary();

// Helpers
int safeInt();
void pauseScreen();
void clearScreen();

// Validation and date helpers
int isValidDate(int d, int m, int y);
int isPastDate(int d, int m, int y);
int isLeap(int year);
int dateToDays(int d, int m, int y);
int tripDuration(Itinerary it);
const char* weekdayName(int d, int m, int y);
int totalCost(Itinerary it);
double costPerDay(Itinerary it);

// Extra helpers
const char* tripStatus(Itinerary it); // Upcoming/Ongoing/Completed
int daysUntilStart(Itinerary it);     // negative if already started
int isValidCityOrCountry(const char *s);
int readValidatedCost();
void showCostChart(Itinerary it);

#endif
