#ifndef COMMON_H
#define COMMON_H

#include <time.h>

// Constants
#define MAX_ITINERARIES 100
#define MAX_STOPS 20
#define MAX_LEN 100
#define FILE_NAME "itineraries.txt"

// ================= VIBRANT COLORS =================
#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN    "\033[1;36m"
#define WHITE   "\033[1;37m"

// Structures
typedef struct {
    char city[MAX_LEN];
    char country[MAX_LEN];
    int cost;         
    int completed;    
} Stop;

typedef struct {
    int id;
    char name[MAX_LEN];
    int start_day, start_month, start_year;
    int end_day, end_month, end_year;
    int stop_count;
    Stop stops[MAX_STOPS];
    int favorite;     
} Itinerary;

// Global Variables (defined in main.c, accessed everywhere via extern)
extern Itinerary itineraries[MAX_ITINERARIES];
extern int count;
extern int next_id;

#endif