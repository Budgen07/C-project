🌍 Travel Itinerary Planner v2.0
A highly modular, file-based CLI itinerary management system written in pure C.

📌 Overview
The Travel Itinerary Planner v2.0 is a refactored, robust CLI application. It has been upgraded from a monolithic structure to a modular architecture, separating concerns into logic, storage, utilities, and interface management.

It allows users to plan trips with granular detail, including stop tracking, budget analytics, date validation, and city statistics, all while adhering to strict C programming constraints.

✨ Key Features
🚀 Core Management
CRUD Operations: Create, Read, Update, and Delete itineraries.

Smart ID Management: IDs are automatically re-sequenced upon deletion to prevent gaps.

Search & Sort: Search by name (case-insensitive) or sort by Name/ID.

Persistent Storage: All data is saved automatically to itineraries.txt.

🧠 Smart Input & Navigation
"Back" Functionality: Enter B at almost any prompt to go back to the previous step (perfect for correcting mistakes during entry).

Robust Validation:

Rejects invalid dates (e.g., Feb 30th, past dates).

Rejects non-numeric costs or invalid city names.

📊 Analytics & Stats
City Statistics: View a generated report of:

Top cities visited (Past trips).

Upcoming destinations (Future trips).

Cost Visualization: ASCII bar charts visualizing cost distribution per stop.

Trip Status: Automatically tags trips as Upcoming, Ongoing, or Completed.

🛡️ Safety Features
Reset Protection: The "Reset All" feature requires solving a randomized math problem to prevent accidental deletion.

File Export: Export specific itineraries to individual text files for sharing/printing.

📂 Modular Project Structure
The code is organized into logical modules for better maintainability and readability:

Plaintext

.
├── main.c           # Entry point; handles the main menu loop.
├── common.h         # Shared data structures (structs) and global constants.
├── manager.c/h      # Business logic (Add, Edit, Delete, Search, List).
├── storage.c/h      # File I/O operations (Load, Save, Export).
├── utils.c/h        # Helper functions (Validation, Date Math, Input handling).
├── Makefile         # Automated build script.
├── itineraries.txt  # Database file (auto-generated).
└── README.md        # Documentation.
⚙️ Compilation & Execution
Prerequisites
GCC Compiler

Make

1️⃣ Build the Project
Run the following command in the terminal:

Bash

make
2️⃣ Run the Application
Bash

./travel_app
(Note: On Windows, use travel_app.exe)

3️⃣ Clean Build Files
To remove object files and the executable:

Bash

make clean
📝 Usage Guide
Date System
The system uses a custom date engine that calculates:

Duration: Total days of the trip.

Countdown: Days remaining until the trip starts.

Weekdays: Automatically determines the day of the week (e.g., "Monday").

Cost Management
Total Cost: Sum of all stops.

Daily Average: Calculates Total Cost / Duration.

Visuals:

Plaintext

Stop 1 (Paris):   25.50%  #####
Stop 2 (London):  74.50%  ###############
Stop Management
Each itinerary supports up to 20 stops, tracking:

City & Country

Cost per stop

Completion status (Done/Not Done)

💾 Data Storage Format
Data is stored in itineraries.txt in a human-readable format. You can backup this file to save your data.

Format Structure:

Plaintext

[ID]
[Name]
[Start Date]
[End Date]
[Stop Count]
[Stop 1 City]
[Stop 1 Country]
[Stop 1 Cost]
[Stop 1 Completed]
...
[Favorite Flag]
👨‍💻 Technical Highlights
This project demonstrates advanced C concepts within academic constraints:

Separation of Concerns: Logic is split across manager, storage, and utils.

Header Guards: Prevents circular dependency errors.

Pointers & Memory: Efficient passing of structures by reference.

Preprocessor Macros: Used for cross-platform screen clearing and ANSI colors.

Make Automation: Simplifies the compilation of multiple source files.

Safe Travels! ✈️
