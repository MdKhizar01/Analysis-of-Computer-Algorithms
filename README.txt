# CSCE 5150 Project – Flight Graph Generator and Route Search

## Group Information
- Group Number: 1
- Team Members: 
1. Mohammed, Momin Khizar Uddin - 11837335
2. Teja Nagendra Sirigineedi - 11825070
3. Chopra Sai Arani - 11807499
4. Deepthi Tanubuddi - 11842466 

We have uploaded the project to UNT Cell linux servers
Navigate to our project:
cd /nfs/home/STUDENTS/mm3445  -  this is my directory
cd "Project 2"  -  Project Folder


## Files Included
- `graphGen.cpp` — Flight data generator.
- `WA.cpp` — Route searching and graph representation.
- `city.name` — List of cities (input data).
- `flight.txt` — List of flights generated.
- `report.pdf` — Written report.
- `README.md` — This file.
- `screenshots/` — Folder with test results and outputs.

---

## Compilation Instructions

### To Compile graphGen.cpp

g++ graphGen.cpp -o graphGen
g++ WA.cpp -o routeSearch
Running Instructions
To Run graphGen
./graphGen <number_of_cities>
<number_of_cities> should be between 1 and 140.

Example:

./graphGen 100
This will generate a flight.txt file based on randomly connected cities.

To Run WA
Do this in cell machine or linux

Command: ./routeSearch
Followed by specific queries:

Query 1 – Shortest Path with Connection Limit

Command: ./routeSearch 1 "Source City" "Destination City" MaxConnections

Example:
Command 1: ./routeSearch 1 "Seoul, South Korea" "Toronto, Canada" 3
Command 2: ./routeSearch 1 "London, United Kingdom" "Hanoi, Vietnam" 6

Query 2 – Path through Two Intermediate Cities
./routeSearch 2 "Source City" through "Intermediate City 1" and "Intermediate City 2" to "Destination City"

Example:
Command 1: ./routeSearch 2 "Hong Kong, SAR" through "New Delhi, India" and "Lisbon, Portugal" to "Dublin, Ireland"
Command 2: ./routeSearch 2 "Hong Kong, SAR" through "Dublin, Ireland" and "Vienna, Austria" to "Santiago, Chile"

Query 3 – Best Meeting Point for Three Travelers
./routeSearch 4 "City 1" "City 2" "City 3"

Example:
Command 1: ./routeSearch 4 "Mumbai, India" "Singapore, Singapore" "Shanghai, People's Republic of China"
Command 2: ./routeSearch 4 "Seoul, South Korea" "Toronto, Canada" "Luxembourg, Luxembourg"
