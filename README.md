# C-project
Itinerary Planner
# 🌍 Travel Itinerary Planner (C Project)

## 📘 Overview
This is a command-line **Travel Itinerary Planner** written entirely in C as part of an academic project.

It allows users to:
- Add itineraries
- List them in a formatted table
- View details
- Delete entries
- Search itineraries
- Sort itineraries
- Save everything in a text file

The project uses **only basic C concepts from the syllabus** — no advanced libraries, dynamic memory, or complex parsing.

---

## ✨ Features
✔ Add itineraries  
✔ List itineraries in a formatted table  
✔ View itinerary details  
✔ Delete itineraries  
✔ Search itineraries (case-insensitive)  
✔ Sort by name or ID  
✔ Data persistence using text file  
✔ Fully CLI based  
✔ Pure C — no external dependencies  
✔ Includes simple Makefile  
❌ No pointers, malloc, structs of pointers, linked lists  
❌ No third-party libraries or GUI

---

## 🧠 Concepts Used (Allowed in Course)
- Variables
- Arrays
- Structs
- Functions
- Header files
- Control flow (if/else, loops)
- Basic file I/O (fopen, fprintf, fscanf, fclose)
- Modular programming
- Makefile usage

---

## 🗂 Project Structure
```
TravelItinerary/
│
├── main.c
├── itinerary.c
├── itinerary.h
├── Makefile
└── itineraries.txt   ← auto-created at runtime
```

---

## 🛠 How to Compile & Run

### 📌 Compile
```
make
```

### ▶ Run
Linux/macOS:
```
./itinerary
```

Windows:
```
itinerary.exe
```

### 🧹 Clean build files
```
make clean
```

---

## 📁 Data Storage
All saved itineraries are stored in:
```
itineraries.txt
```
- Auto-generated
- Human-readable
- No binary format used

---

## 📊 Example Operations in Program
```
1 → Add itinerary
2 → List itineraries
3 → View itinerary
4 → Delete itinerary
5 → Search itinerary
6 → Sort by name
7 → Sort by ID
0 → Exit
```

---

## 📋 Grading Requirements Satisfied
| Requirement | Status |
|-------------|--------|
| File I/O | ✔ Yes |
| Persistent storage | ✔ Yes |
| Structs | ✔ Yes |
| Modular programming | ✔ Yes |
| Makefile | ✔ Yes |
| Non-trivial logic | ✔ Yes |
| CLI only | ✔ Yes |
| No external libraries | ✔ Yes |
| Uses only allowed concepts | ✔ Yes |

---

## 👤 Team Members
(Add before submission)
```
1. Name – Contribution
2. Name – Contribution
3. Name – Contribution
```

---

## 🏁 End of Document
This project is ready for:
- Submission
- Viva demonstration
- PDF export (via browser or MD→PDF converter)
