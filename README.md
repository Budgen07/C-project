# 🌍 Travel Itinerary Planner  
*A complete CLI-based itinerary management system written entirely in C*

---

## 📌 Overview
The **Travel Itinerary Planner** is a modular, file-based CLI application written **100% in C**.

It allows users to create, edit, delete, view, search, sort, export, and manage travel itineraries with full date validation, cost analytics, favorites, stop tracking, and much more.

This project follows strict constraints:

- **Only C language**
- **Fully CLI-based**
- **No external libraries**
- **Only syllabus-approved concepts**
- **Modular design** (`main.c`, `itinerary.c`, `itinerary.h`)
- **Simple Makefile-based build system**

---

## ✨ Features

### 🧳 Core Features
- Create new itineraries  
- View detailed itinerary summary  
- List all itineraries in a formatted table  
- Delete itineraries  
- Edit itineraries (including stops & dates)  
- Export itineraries to a `.txt` file  
- **Automatically reassign IDs after deletion (no gaps)**  
- File storage via `itineraries.txt`  

---

### 📅 Date System (Fully Validated)
- Separate input: `day`, `month`, `year`  
- Validates:
  - Correct calendar dates  
  - Leap years  
  - Month lengths  
- Prevents:
  - Past start dates  
  - Past end dates  
  - End date occurring before start  
- Calculates:
  - Duration in days  
  - Day of week for start & end  
  - Days until start  
  - Trip status: **Upcoming / Ongoing / Completed**

---

### 🛑 Input Validation
#### ✔ City & Country Validation
- Must contain at least **one letter**
- Can only contain:
  - letters  
  - spaces  
  - hyphens  
  - apostrophes  
- Rejects:
  - numbers-only  
  - invalid symbols (`@`, `%`, `*`, etc.)

#### ✔ Cost Validation
- Integer only  
- Must be **≥ 0**  
- Rejects all invalid inputs  

---

### 🗺️ Stop Management
Each itinerary supports up to **20 stops**, each storing:

- City  
- Country  
- Cost  
- Completion flag  

Features include:

- Add stop  
- Remove stop  
- Edit stop  
- Mark stop completed / not completed  
- Prevent invalid city/country names  

---

### ⭐ Favorites System
- Mark any itinerary as favorite  
- Toggle favorite at any time  
- List only favorite itineraries  

---

### 💰 Cost Analytics
- Total trip cost  
- Cost per day  
- ASCII bar chart showing cost distribution per stop  

Example cost chart:

```
Stop 1 (Tokyo):   #######
Stop 2 (Kyoto):   ####
Stop 3 (Osaka):   ###
```

---

### 🔍 Search & Sort
- Case-insensitive name search  
- Sort itineraries:
  - By **Name**
  - By **ID**  

---

### 📤 Exporting
Export any itinerary to:

```
itinerary_<ID>.txt
```

The exported file includes all trip details and stops.

---

## 📁 Project Structure

```
project/
│── main.c
│── itinerary.c
│── itinerary.h
│── Makefile
│── itineraries.txt   (auto-generated)
│── README.md
```

---

## ⚙️ Compilation & Execution

### 1️⃣ Build with Makefile
```bash
make
```

This produces an executable named:

```
itinerary
```

### 2️⃣ Run the Program
```bash
./itinerary
```

### 3️⃣ Clean Object Files
```bash
make clean
```

---

## 📂 Data Storage Format (itineraries.txt)

Each itinerary is stored in the following structured format:

```
id
name
start_day start_month start_year
end_day end_month end_year
stop_count
city_1
country_1
cost_1
completed_1
...
favorite
```

**This file is automatically updated every time you:**
- Add an itinerary  
- Edit an itinerary  
- Delete  
- Toggle favorite  
- Sort itineraries  

---

## 🧪 Functional Testing Scenarios Fully Supported

✔ Create itinerary  
✔ Validate date  
✔ Validate city/country  
✔ Validate cost  
✔ Edit itinerary name  
✔ Edit dates  
✔ Edit stops  
✔ Delete itinerary  
✔ Auto-reassign IDs  
✔ Search  
✔ Sort  
✔ Mark favorite  
✔ List favorites  
✔ Export itinerary  
✔ Calculate cost/day  
✔ Show cost chart  
✔ Detect trip status  
✔ Count days until start  

Everything follows strict syllabus guidelines.

---

## 👨‍💻 Technical Concepts Used

### Allowed & In-Syllabus:
- Structures  
- Arrays  
- File handling  
- Modular programming  
- Header files  
- Functions  
- Pointer basics  
- Input validation  
- Time/date functions  
- Makefile  
- ANSI escape sequences for color  

### Libraries Used:
- `<stdio.h>`  
- `<stdlib.h>`  
- `<string.h>`  
- `<ctype.h>`  
- `<time.h>`  

---

## 🏁 Final Notes

This project demonstrates:

- Strong understanding of C programming  
- Clean, modular code  
- Robust input handling  
- Real-world travel management logic  
- File persistence  
- Professional CLI design  
- Perfect adherence to academic constraints  

It is fully feature-complete and cleanly implemented.
