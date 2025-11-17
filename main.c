#include <stdio.h>
#include "itinerary.h"

int main() {
    int choice;
    loadData();

    while (1) {
       printf("\n=====================================\n");
printf("      TRAVEL ITINERARY PLANNER\n");
printf("=====================================\n");
printf("1. Add Itinerary\n");
printf("2. List Itineraries\n");
printf("3. View Itinerary\n");
printf("4. Delete Itinerary\n");
printf("5. Search Itinerary\n");
printf("6. Sort by Name\n");
printf("7. Sort by ID\n");
printf("0. Exit\n");
printf("=====================================\n");
printf("Enter choice: ");


        choice = safeInt();

        if (choice == 1) addItinerary();
        else if (choice == 2) listItineraries();
        else if (choice == 3) viewItinerary();
        else if (choice == 4) deleteItinerary();
        else if (choice == 5) searchItineraries();
        else if (choice == 6) sortByName();
        else if (choice == 7) sortByID();
        else if (choice == 0) break;
        else printf("Invalid choice.\n");
    }

    printf("Goodbye!\n");
    return 0;
}
