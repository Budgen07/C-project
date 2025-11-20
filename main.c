#include <stdio.h>
#include "itinerary.h"

int main() {
    int choice;

    loadData();

    while (1) {
        clearScreen();
        printf(CYAN BOLD "========== TRAVEL ITINERARY PLANNER ==========\n" RESET);
        printf("1. Add Itinerary\n");
        printf("2. List All Itineraries\n");
        printf("3. View Itinerary\n");
        printf("4. Delete Itinerary\n");
        printf("5. Search Itineraries\n");
        printf("6. Sort by Name\n");
        printf("7. Sort by ID\n");
        printf("8. Edit Itinerary\n");
        printf("9. View Favorite Trips\n");
        printf("10. Toggle Favorite\n");
        printf("11. Export Itinerary to Text File\n");
        printf("0. Exit\n");
        printf("----------------------------------------------\n");
        printf("Enter choice: ");

        choice = safeInt();

        switch (choice) {
            case 1:
                addItinerary();
                pauseScreen();
                break;
            case 2:
                listItineraries();
                pauseScreen();
                break;
            case 3:
                viewItinerary();
                pauseScreen();
                break;
            case 4:
                deleteItinerary();
                pauseScreen();
                break;
            case 5:
                searchItineraries();
                pauseScreen();
                break;
            case 6:
                sortByName();
                pauseScreen();
                break;
            case 7:
                sortByID();
                pauseScreen();
                break;
            case 8:
                editItinerary();
                pauseScreen();
                break;
            case 9:
                listFavorites();
                pauseScreen();
                break;
            case 10:
                toggleFavorite();
                pauseScreen();
                break;
            case 11:
                exportItinerary();
                pauseScreen();
                break;
            case 0:
                printf(GREEN "\nGoodbye!\n" RESET);
                return 0;
            default:
                printf(RED "Invalid choice.\n" RESET);
                pauseScreen();
        }
    }

    return 0;
}
