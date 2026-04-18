#include <iostream>
#include <vector>

// Typedefs
typedef std::vector<int> ResourceVector;
typedef std::vector<std::vector<int>> ResourceMatrix;
typedef std::vector<bool> Bitmap;

// Function prototype
// Returns whether one vector is less than another
bool LessThan(const std::vector<int>& v1, const std::vector<int>& v2);

// Function prototype
// Returns whether all processes have terminated
bool AllProcessesTerminated(const std::vector<bool>& processes); 

// Function prototype
// Prints a resource vector
void PrintResourceVector(const ResourceVector& v);

// Function prototype
// Detects deadlock
void DetectDeadlock(const ResourceVector& a, const ResourceMatrix& c, const ResourceMatrix&r);

// Main function
int main() {
  
    // Currently available resources
    ResourceVector available = { 1, 1, 0, 1 };

    // Allocated resources
    ResourceMatrix current =  {
        { 1, 0, 1, 0 },
        { 1, 1, 0, 0 },
        { 0, 1, 1, 1 },
    };

    // Allocated resources
    ResourceMatrix requested =  {
        { 1, 1, 0, 1 },
        { 1, 1, 1, 1 },
        { 1, 0, 0, 0 },
    };

    // Call DetectDeadlock
    DetectDeadlock(available, current, requested);

    return 0;
}

// Function definition
// Returns whether one vector is less than another
bool LessThan(const std::vector<int>& v1, const std::vector<int>& v2) {
  bool is_less_than = true;
  for (int i = 0; i < v1.size(); i++) {
    is_less_than &= v1[i] <= v2[i];
  }
  return is_less_than;
}

// Function definition
// Returns whether all processes have terminated
bool AllProcessesTerminated(const std::vector<bool>& processes) {
    bool all_terminated = true;
    for (int i = 0; i < processes.size(); i++) {
      all_terminated &= processes[i];
    }
    return all_terminated;
}

// Function defintion
// Prints a resource vector
void PrintResourceVector(const ResourceVector& v) {
  for (int i = 0; i < v.size(); i++) {
    std::cout << v[i] << " ";
  }
}

// Function definition
// Detects deadlock
void DetectDeadlock(const ResourceVector& a, const ResourceMatrix& c, const ResourceMatrix&r) {
  
  int num_resources = a.size();
  int num_processes = c.size();

  Bitmap is_terminated;
  for (int i = 0; i < num_processes; i++) {
    is_terminated.push_back(false);
  }

  ResourceVector a_copy;
  for (int i = 0; i < num_resources; i++) {
    a_copy.push_back(a[i]);
  }

  while (true) {

    // Check whether every process has been marked as terminated
    if (AllProcessesTerminated(is_terminated)) {
        // Print out that there's no deadlock, then break
        std::cout << "No deadlock detected.\n";
        break;
    }

    // Find a process's request that can be satisfied
    int process_index = -1;
    for (int i = 0; i < num_processes; i++) {
        // Check whether current process is marked as terminated
        // If so, continue
        if (is_terminated[i]) {
            continue;
        }

        // Check whether request is less than or equal to available
        // resources. If so, save the current index. If not, continue.
        if (LessThan(r[i], a_copy)) {
            process_index = i;
            break;
        } else {
            continue;
        }
    }

    // Check whether a satisfiable request has been found.
    // If so, mark that process as terminated and make its alocated
    // resources available. If not, then there are deadlocked processes.
    if (process_index >= 0) {
      // Add back allocated resources to available resources
      for (int i = 0; i < num_resources; i++) {
        a_copy[i] += c[process_index][i];
      }

      // Mark process as terminated
      is_terminated[process_index] = true;
      std::cout << "Process " << process_index << " marked as terminated.\n";

    } else {
      std::cout << "Deadlock detected. The following processes are deadlocked:\n";
      for (int i = 0; i < num_processes; i++) {
        if(!is_terminated[i]) {
            std::cout << "Process " << i << std::endl;
        }
      }
      std::cout << std::endl;

      // Print out each process's request and allocated resources
      for (int i = 0; i < num_processes; i++) {
        if(!is_terminated[i]) {
            std::cout << "Allocated resources of process " << i << ":\n";
            PrintResourceVector(c[i]);
            std::cout << std::endl;
            std::cout << "Request of process " << i << ":\n";
            PrintResourceVector(r[i]);
            std::cout << std::endl << std::endl;
        }
      }

      break;
    }
  }
}