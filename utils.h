#ifndef UTILS_H    // <--- MAKE SURE THIS SAYS UTILS_H
#define UTILS_H    // <--- MAKE SURE THIS SAYS UTILS_H

#include "common.h"

// Input Helpers
int safeInt();
void pauseScreen();
void clearScreen();
int askStringWithBack(const char *prompt, char *out, int maxlen, int *isBack);
int askIntWithBack(const char *prompt, int *out, int *isBack);

// String Validation
int isValidName(const char *s);
int isValidCityOrCountry(const char *s);
int readValidatedCost();

// Date Helpers
int isLeap(int year);
int isValidDate(int d, int m, int y);
int isPastDate(int d, int m, int y);
int dateToDays(int d, int m, int y);
const char* weekdayName(int d, int m, int y);

// Trip Math
int tripDuration(Itinerary it);
int daysUntilStart(Itinerary it);
const char* tripStatus(Itinerary it);
int totalCost(Itinerary it);
double costPerDay(Itinerary it);
void showCostChart(Itinerary it);

#endif