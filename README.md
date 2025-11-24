# SMART-STUDY-SCHEDULE-OPTIMIZER
This project implements a task scheduling system in C++ that generates an optimized daily work plan based on:

✅ Task dependencies
✅ Difficulty level
✅ Deadline urgency
✅ Task duration
✅ Limited working hours per day (default: 8 hours)

It ensures that:

Tasks are completed only after their dependencies

Higher-priority tasks are scheduled earlier

No more than the allowed daily working hours are used

A valid topological order is followed (detects dependency cycles)

🚀 Features
🔹 Task Input System

Users can enter:

Task name

Deadline (in days)

Difficulty (1–10)

Required hours

Dependencies

🔹 Priority Calculation

Each task receives a priority score:

priority = (1 / deadline) + difficulty


Meaning:

Tasks with closer deadlines and higher difficulty get higher priority.

🔹 Topological Sorting

The program:

Builds a dependency graph

Computes task order using Kahn’s algorithm

Detects cycles in dependencies

🔹 Max Heap Scheduling

A custom max heap selects the highest-priority available task that fits in remaining daily hours.

🔹 Daily Schedule Output

The system generates:

-----FINAL DAILY SCHEDULE-----
Day 1: TaskA TaskC
Day 2: TaskB
...

🛠️ Technologies Used

C++ (Object-Oriented Design)

Custom:

Linked List-based Queue

Max Heap implementation

Graph Theory (Topological Sort)

Greedy Scheduling Logic

📂 Project Structure
main.cpp


Contains:

Task struct

Queue class

MaxHeap class

Graph builder

Topological sorting

Scheduler

Console I/O system

🧠 How It Works (Flow)

User enters tasks + dependencies

Program assigns priority scores

Builds a dependency graph

Performs topological sorting

Schedules tasks daily:

8 hours max per day

Picks highest priority available tasks

Prints final schedule

▶️ How to Run
Compile
g++ main.cpp -o scheduler

Run
./scheduler

✅ Example Input
enter number of tasks: 3
enter task name: A
enter deadline(in days): 2
enter difficulty(1 to 10): 5
enter hours required: 4
number of dependencies for this task: 0

enter task name: B
enter deadline(in days): 1
enter difficulty(1 to 10): 7
enter hours required: 3
number of dependencies for this task: 1
enter dependency indexes: 0

enter task name: C
...

✅ Output Example
-----FINAL DAILY SCHEDULE-----
Day 1: A B
Day 2: C

⚠️ Limitations / Future Improvements

No save/load functionality

No GUI

Priority formula can be improved

Currently stops scheduling when a task doesn't fit remaining hours

🏗️ Future Enhancements (Planned)

🔸 Better priority model
🔸 Automatic time splitting for long tasks
🔸 File-based task input
🔸 Web or desktop UI
🔸 Visualization of dependency graph

📜 License

This project is open-source and free to use.
