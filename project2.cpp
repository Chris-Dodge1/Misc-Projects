#include <vector>       // For storing page numbers
#include <list>         // For list data structure
#include <iostream>     // For outputting text
#include <algorithm>    // For std::find

void FIFO(std::vector<int>& ref_string, int num_pages_frames);
void LRU(std::vector<int>& ref_string, int num_pages_frames);
void OPT(std::vector<int>& ref_string, int num_pages_frames);
int FindPageUsedLast(std::vector<int>& ref_string, std::list<int>& opt_list, int pos_in_ref_string);
std::vector<int> SimplifyReferenceString(std::vector<int>& ref_string);

int main() {
    // Declare page frame count
    // Declare refrence string from the slides
    // int num_pages_frames = 3;
    // std::vector<int> ref_string = { 7,0,1,2,0, 3,0,4,2,3, 0,3,2,1,2, 0,1,7,0,1 };

    // int num_pages_frames;
    std::vector<int> ref_string;

    // std::cout << "Enter the number of page frames: ";
    // std::cin >> num_pages_frames;

    bool is_input_complete = false;
    while (!is_input_complete) {
        std::cout << "Enter a page number or enter -1 to signify end of string: ";
        int next_page_number;
        std::cin >> next_page_number;
        if (next_page_number == -1) {
            is_input_complete = true;
        } else {
            ref_string.push_back(next_page_number);
        }
    }
    
    ref_string = SimplifyReferenceString(ref_string);

    // Print out the refrence string
    std::cout << "Refrence string is as follows: \n";
    for (int i = 0; i < ref_string.size(); i++) {
        std::cout << ref_string[i] << " ";
    }
    std::cout << std::endl << std::endl;

    // Call the page replacement algorithms
    // FIFO(ref_string, num_page_frames);
    // LRU(ref_string, num_page_frames);
    // OPT(ref_string, num_page_frames);

    for (int i = 1; i <= 8; i++) {
        FIFO(ref_string, i);
        LRU(ref_string, i);
        OPT(ref_string, i);
        std::cout << std::endl;
    }

    return 0;
}

// Count the number of page faults for FIFO page replacement
void FIFO(std::vector<int>& ref_string, int num_pages_frames) {

    // Initialize the queue and page fault count
    std::list<int> fifo_queue;
    int num_page_faults = 0;

    // For loop
    for (int i = 0; i < ref_string.size(); i++) {
        int page_number = ref_string[i];

        // Search the queue for the current page number
        auto iter = std::find(fifo_queue.begin(), fifo_queue.end(), page_number);

        // If page is not found...
        if (iter == fifo_queue.end()) {
            // Increment page faults by 1
            num_page_faults++;

            // Push page number to the front of the queue
            fifo_queue.push_front(page_number);

            // If queue is larger than the number of page frames,
            // pop the page number at the back of the queue
            if (fifo_queue.size() > num_pages_frames) {
                fifo_queue.pop_back();
            }
        }
    }

    std::cout << "Using FIFO with " << num_pages_frames << 
    " page frames yielded " << num_page_faults << " page faults.\n";
}

// Count the number of page faults for LRU
void LRU(std::vector<int>& ref_string, int num_pages_frames) {

    // Initialize the stack and page fault count
    std::list<int> lru_stack;
    int num_page_faults = 0;

    // For loop
    for (int i = 0; i < ref_string.size(); i++) {
        int page_number = ref_string[i];

        // Search the stack for the current page number
        auto iter = std::find(lru_stack.begin(), lru_stack.end(), page_number);

        if (iter != lru_stack.end()) {
          // Page is found within stack; move it to the top of the stack
          lru_stack.erase(iter);
          lru_stack.push_front(page_number);
        } else {
            // Page is not within the stack
            // Increment page fault count by 1
            num_page_faults++;

            // Push missing page to the top of the stack
            lru_stack.push_front(page_number);

            // If the stacks size is larger than the number of page frames,
            // pop the page that's at the bottom of the stack
            if (lru_stack.size() > num_pages_frames) {
                lru_stack.pop_back();
            }
        }
    }

    std::cout << "Using LRU with " << num_pages_frames << 
    " page frames yielded " << num_page_faults << " page faults.\n";
}

// Count the number of page faults for OPT
void OPT(std::vector<int>& ref_string, int num_pages_frames) {

    // Initialize the stack and page fault count
    std::list<int> opt_list;
    int num_page_faults = 0;

    // For loop
    for (int i = 0; i < ref_string.size(); i++) {
        int page_number = ref_string[i];

        // Search the list for the current page number
        auto iter = std::find(opt_list.begin(), opt_list.end(), page_number);

        if (opt_list.size() < num_pages_frames && iter == opt_list.end()) {
            // If the page number was not found and there's room to add a new page,
            // add it to the list and increment the page fault count.
            opt_list.push_front(page_number);
            num_page_faults++;
        } else if ( opt_list.size() == num_pages_frames && iter == opt_list.end()) {
          // If the page number was not found and there is no more room for a 
          // new page...
          // increment the number of page faults
          num_page_faults++;

          // Find which page in the OPT list will be used LAST!
          int page_used_last = FindPageUsedLast(ref_string, opt_list, i);

          // Find that page number in the list using std::find()
          auto iter2 = std::find(opt_list.begin(), opt_list.end(), page_used_last);

          // Remove that page number from the opt list
          opt_list.erase(iter2);

          // Add the new page number to the list
          opt_list.push_front(page_number);
        }
    }

    std::cout << "Using OPT with " << num_pages_frames << 
    " page frames yielded " << num_page_faults << " page faults.\n";
}

// Helper function for OPT
int FindPageUsedLast(std::vector<int>& ref_string, std::list<int>& opt_list, int pos_in_ref_string){

    // Initialize distance and current page number
    int found_page_number = -1;
    int found_distance = -1000000;  // Set to a large negative number to ensure it's updated

    // Iterate through the OPT list. For each number, find how far its next
    // use is.
    for (auto it = opt_list.begin(); it != opt_list.end(); ++it) {
        int curr_page_number = *it;

        // Iterate through the current reference string starting from the current 
        // position in OPT to find that page's next use. On e found, break. If not
        // found, then the distance to the next use will be infinitely far away.
        int curr_distance = 1000000;
        for (int i = pos_in_ref_string; i < ref_string.size(); i++) {
            if (curr_page_number == ref_string[i]) {
                curr_distance = i - pos_in_ref_string;
                break;
            }
        }

        // Update the page that will be used last 
        if (curr_distance > found_distance) {
            found_page_number = curr_page_number;
            found_distance = curr_distance;
        }
    }

    return found_page_number;
} 

std::vector<int> SimplifyReferenceString(std::vector<int>& ref_string) {

    int prev_page_number = ref_string[0];
    std::vector<int> new_ref_string = { prev_page_number};
    for (int i = 1; i < ref_string.size(); i++) {
        if (prev_page_number == ref_string[i]) {
            continue;
        } else {
            prev_page_number = ref_string[i];
            new_ref_string.push_back(prev_page_number);
        }
    }

    return new_ref_string;
}