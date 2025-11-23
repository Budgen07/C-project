#include <stdio.h>
#include "itinerary.h"

int main() {
    int choice;

    loadData();

    while (1) {
        clearScreen();
        // New Vibrant Header
        printf(MAGENTA "=================================================\n" RESET);
        printf(CYAN    "          TRAVEL ITINERARY PLANNER v2.0          \n" RESET);
        printf(MAGENTA "=================================================\n" RESET);

        printf(GREEN " 1." RESET "  Add Itinerary\n");
        printf(CYAN  " 2." RESET "  List All Itineraries\n");
        printf(YELLOW " 3." RESET "  View Itinerary Details\n");
        printf(RED    " 4." RESET "  Delete Itinerary\n");
        printf(MAGENTA " 5." RESET "  Search Itineraries\n");
        printf(BLUE   " 6." RESET "  Sort by Name\n");
        printf(BLUE   " 7." RESET "  Sort by ID\n");
        printf(CYAN   " 8." RESET "  Edit Itinerary\n");
        printf(YELLOW " 9." RESET "  View Favorite Trips\n");
        printf(GREEN "10." RESET "  Toggle Favorite\n");
        printf(MAGENTA "11." RESET "  Export to File\n");
        printf(YELLOW "12." RESET "  City Stats (Visited/Upcoming)\n"); // New Option
        printf(RED    "13." RESET "  Reset ALL itineraries (danger!)\n"); // Reset option
        printf(WHITE  " 0." RESET "  Exit\n");
        printf(MAGENTA "-------------------------------------------------\n" RESET);
        printf(BLUE "Enter choice: " RESET);

        choice = safeInt();

        switch (choice) {
            case 1: addItinerary(); pauseScreen(); break;
            case 2: listItineraries(); pauseScreen(); break;
            case 3: viewItinerary(); pauseScreen(); break;
            case 4: deleteItinerary(); pauseScreen(); break;
            case 5: searchItineraries(); pauseScreen(); break;
            case 6: sortByName(); pauseScreen(); break;
            case 7: sortByID(); pauseScreen(); break;
            case 8: editItinerary(); pauseScreen(); break;
            case 9: listFavorites(); pauseScreen(); break;
            case 10: toggleFavorite(); pauseScreen(); break;
            case 11: exportItinerary(); pauseScreen(); break;
            case 12: viewCityStats(); pauseScreen(); break; // New Call
            case 13: resetAllItineraries(); pauseScreen(); break; // Reset
            case 0:
                printf(GREEN "\nGoodbye! Safe travels.\n" RESET);
                return 0;
            default:
                printf(RED "Invalid choice.\n" RESET);
                pauseScreen();
        }
    }

    return 0;
}
