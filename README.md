# Hospital-Appointment-System-using-DSA
🏥 Hospital Appointment Scheduling System (C++ Console Project)

This is a C++ console application that simulates a real-world hospital appointment system. It allows patients to book time slots, prioritizes emergency cases, maintains logs, and provides multiple utility features to assist hospital staff.

📌 Features

➕ Add new patient appointments

🚑 Process emergency patients based on severity

❌ Cancel appointments using patient ID

🔍 Search appointment details by patient ID

📋 View all appointments slot-wise

👴 View patients above a certain age

📝 Log all activities into a log.txt file automatically

⚙️ Technologies Used

💻 Language: C++

🧠 Concepts: Structures, Classes, Linked Lists, Unordered Maps, Priority Queues, File Handling

🛠️ Tools: VS Code / Code::Blocks, MinGW Compiler

🧱 Data Structures Used

🔗 Linked List – Stores patients in each time slot

🗺️ Unordered Map – Maps time slots to appointment lists

🧮 Priority Queue – Maintains emergency patients sorted by severity

🧾 File Handling – Logs all operations to log.txt

🆔 ID Map (unordered_map) – Enables fast lookup for search and cancellation

🏗️ System Architecture User (CLI Input)   ↓ AppointmentManager (Core Logic Layer)   ↓       ↓ Slot Map (Normal)   Emergency Queue (Severity-based)   ↓       ↓      Log File (Persistent Tracking)

📁 File Structure

main.cpp – Full source code

README.txt – Project documentation

🚀 How to Run

Download or clone the repository

Open a terminal or command prompt

Compile the source code using a C++ compiler:   g++ main.cpp -o appointment

Run the executable:   ./appointment

Use the menu interface to interact with the system

🎓 Learnings

Hands-on implementation of core data structures

Real-time system design and slot-based logic

Emergency case handling using queues

File logging and persistent storage in C++

Writing clean, menu-driven C++ applications

📄 License This project is intended for academic use and learning. Feel free to use or extend it with credit.
