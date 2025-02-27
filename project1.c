#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Function prototypes
void  FCFS(int* burst_times, int num_process);
void  SJF (int* burst_times, int num_process);
void  RR  (int* burst_times, int num_process, int quantum);

// Main function
int main() {

    // Declare variables
    int* burst_times;
    int num_processes;

    // Ask the user for how many processes to run
    // then allocate memory for the burst times.
    printf("How many processes are you running? ");
    scanf("%d", &num_processes);
    burst_times = (int*)malloc(num_processes * sizeof(int));

    // Enter the burst times for each process.
    for (int i = 0; i < num_processes; i++) {
        printf("Enter the burst time for process %i of %i: ", i + 1, num_processes);
        scanf("%d", &burst_times[i]);
    }

    // Print out what was typed.
    printf("The following processes will arive at time 0 with these burst times:");
    for (int i = 0; i < num_processes; i++) {
        printf(" %i", burst_times[i]);
    }
    printf("\n");

    // Call each scheduling algorithm
    FCFS(burst_times, num_processes);
    SJF(burst_times, num_processes);
    RR(burst_times, num_processes, 2);
    RR(burst_times, num_processes, 4);
    RR(burst_times, num_processes, 6);
    RR(burst_times, num_processes, 8);
    RR(burst_times, num_processes, 10);
    RR(burst_times, num_processes, 12);

    free(burst_times);
    return 0;
}

// FCFS
void FCFS(int* burst_times, int num_processes) {
    // Declare variables
    int time_elapsed = 0;
    float avg_waiting_time = 0.0f;
    float avg_turnaround_time = 0.0f;

// Calculate each process's waiting and turnaround times
// in a for loop.
printf("The times for FCFS are as follows:\n");
for (int i = 0; i < num_processes; i++) {
    int burst_time = burst_times[i];
    time_elapsed += burst_time;

    int turnaround_time = time_elapsed;
    int waiting_time = turnaround_time - burst_time;
    
    // Print out waiting and turnaround time
    printf("Process %i:\tBT: %i\tWT: %i\tTAT: %i\n", i + 1, burst_time, waiting_time, turnaround_time);

    // Add waiting time and turnaround time for average calculation
    avg_waiting_time += waiting_time;
    avg_turnaround_time += turnaround_time;
  }

  // Print out avg waiting and turnaround time
  avg_waiting_time /= (float)num_processes;
  avg_turnaround_time /= (float)num_processes;
  printf("Avg WT: %f\tAvg TAT: %f\n", avg_waiting_time, avg_turnaround_time);

  return;
}

// SJF
void SJF(int* burst_times, int num_processes) {

    // Declare pointers and variables
    int* sorted_burst_times = (int*)malloc(num_processes * sizeof(int));
    int* process_ids = (int*)malloc(num_processes * sizeof(int));
    int time_elapsed = 0;
    float avg_waiting_time = 0.0f;
    float avg_turnaround_time = 0.0f;

    // Copy burst times into a new array to be sorted
    // and write down process IDs.
    for (int i = 0; i < num_processes; i++) {
        sorted_burst_times[i] = burst_times[i];
        process_ids[i] = i;
    }

    // Sort burst times using selection sort, but also
    // rearrange process IDs so they correspond with
    // their respective burst times 
    for(int i = 0; i < num_processes - 1; i++) {
        int index_of_smallest = i;
        for (int j = i + 1; j < num_processes; j++) {
            if (sorted_burst_times[j] < sorted_burst_times[index_of_smallest]) {
                index_of_smallest = j;
            }
        }

        // Swap burst times
        int temp = sorted_burst_times[i];
        sorted_burst_times[i] = sorted_burst_times[index_of_smallest];
        sorted_burst_times[index_of_smallest] = temp;

        // Swap PIDs
        temp = process_ids[i];
        process_ids[i] = process_ids[index_of_smallest];
        process_ids[index_of_smallest] = temp;
    }

// Calculate each process's waiting and turnaround times
// in a for loop.
printf("The times for SJF are as follows:\n");
for (int i = 0; i < num_processes; i++) {
    int burst_time = sorted_burst_times[i];
    time_elapsed += burst_time;

    int turnaround_time = time_elapsed;
    int waiting_time = turnaround_time - burst_time;
    
    // Print out waiting and turnaround time
    printf("Process %i:\tBT: %i\tWT: %i\tTAT: %i\n", process_ids[i] + 1, burst_time, waiting_time, turnaround_time);

    // Add waiting time and turnaround time for average calculation
    avg_waiting_time += waiting_time;
    avg_turnaround_time += turnaround_time;
  }

  // Print out avg waiting and turnaround time
  avg_waiting_time /= (float)num_processes;
  avg_turnaround_time /= (float)num_processes;
  printf("Avg WT: %f\tAvg TAT: %f\n", avg_waiting_time, avg_turnaround_time);
  
  free(sorted_burst_times);
  free(process_ids);
  return;
}

// RR
void RR(int* burst_times, int num_processes, int quantum) {

    // Declare variables and pointers
    int* time_remaining = (int*)malloc(num_processes * sizeof(int));
    int time_elapsed = 0;
    float avg_waiting_time = 0.0f;
    float avg_turnaround_time = 0.0f;
    bool all_processes_complete = false;

    // Copy burst times to time remaining
    for (int i = 0; i < num_processes; i++) {
        time_remaining[i] = burst_times[i];
    }

    // Calculate each process's waiting and turnaround time.
    printf("The times for RR (quantum = %i are as follows:\n" , quantum);
    while (!all_processes_complete) {
        // First for loop
        // For each process with time remaining, decrement the quantum
        // from that time. If the process reaches zero this way, calculate
        // that process's waiting and turnaround time.
        for (int i = 0; i < num_processes; i++) {
            // If a process has zero time remaining, skip it.
            if (time_remaining[i] == 0) continue;


            // Calculate the current quantum: it's either the quantum or 
            // the process's remaining time, whichever is smaller.
            int current_quantum = quantum;
            if (time_remaining[i] < current_quantum) {
                current_quantum = time_remaining[i];
            }

            // Increment the elapsed time and decrement the time remaining
            time_elapsed += current_quantum;
            time_remaining[i] -= current_quantum;

            // If a process's remaining time reaches zero this way,
            // calculate and print its waiting and turnaround time.
            if (time_remaining[i] == 0) {
                int burst_time = burst_times[i];
                int turnaround_time = time_elapsed;
                int waiting_time = turnaround_time - burst_time;
            
            // Print out waiting and turnaround time
            printf("Process %i:\tBT: %i\tWT: %i\tTAT: %i\n", i + 1, burst_time, waiting_time, turnaround_time);
        
            // Add waiting time and turnaround time for average calculation
            avg_waiting_time += waiting_time;
            avg_turnaround_time += turnaround_time;
          }

        }

        // Second for loop
        // Check whether all processes have remaining times of zero.
        // If so, then the program can break out of the while loop.
        all_processes_complete = true;
        for (int i = 0; i < num_processes; i++) {
            all_processes_complete = all_processes_complete && time_remaining[i] == 0;
        }
    }

      // Print out avg waiting and turnaround time
    avg_waiting_time /= (float)num_processes;
    avg_turnaround_time /= (float)num_processes;
    printf("Avg WT: %f\tAvg TAT: %f\n", avg_waiting_time, avg_turnaround_time);

    free(time_remaining);
    return;
}