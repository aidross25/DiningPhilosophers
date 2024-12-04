# Dining Philosophers Problem Simulation with Docker

## Overview
This program simulates the **Dining Philosophers Problem** using multithreading in C. The simulation involves five philosophers who alternate between thinking and eating. Philosophers need two objects (represented as "forks") to eat, and they pick up and put down the forks while ensuring no deadlocks occur. The program runs for a specified duration and tracks the number of meals eaten by each philosopher.

The program is packaged in a **Docker** container to ensure a consistent runtime environment across different systems.

---

## Prerequisites
To run this program, you need:
- **Docker** installed on your machine. [Install Docker](https://www.docker.com/products/docker-desktop).
- Basic knowledge of using Docker commands.

---

## Building and Running the Program

1. **Clone or Download the Project**  
   Ensure the `dining_philosophers.c` source file and `Dockerfile` are in the same directory.

2. **Dockerfile**  
   Here's a part of the `Dockerfile` that shows the command line input for the program:
   ```Dockerfile

   # Default command to run the program
   CMD ["./dining_philosophers", "30"]
   
## Report
| Run Time (seconds) | Minimum Meals | Maximum Meals | Average Meals |
|--------------------|---------------|---------------|---------------|
| 30                 | 4             | 6             | 5.20          |
| 40                 | 5             | 7             | 6.20          |
| 50                 | 7             | 8             | 7.80          |
| 60                 | 9             | 10            | 9.40          |
| 70                 | 10            | 10            | 10.00         |
| 80                 | 10            | 10            | 10.00         |
| 90                 | 10            | 10            | 10.00         |

