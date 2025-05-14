==============================================================================
Multithreaded Integer Classification Program
==============================================================================

Author      : Halmuhammet Muhamedorazov  
Date        : 04/25/2024  
Version     : 1.0  
C++ Version : g++ (GCC) 4.8.5 20150623 (Red Hat 4.8.5-16)  
==============================================================================

Description:
-------------
This multithreaded C++ program reads integers from an input file and classifies 
them concurrently using multiple threads. The classification includes checking 
if a number is:

- Even or Odd  
- Positive or Negative  
- A Perfect Square  
- A Perfect Cube  

The results are written to corresponding output files. The program utilizes 
a shared queue and implements synchronization using `std::mutex` and 
`std::condition_variable` to manage thread communication.

------------------------------------------------------------------------------
Compilation Instructions:
--------------------------
Make sure you are using a compiler that supports C++11 or above.

To compile this program, use the following command:

    g++ -std=c++11 main.cpp -o classify -pthread

------------------------------------------------------------------------------
Execution Instructions:
------------------------
Ensure the input file is placed correctly and accessible at the path 
hardcoded in the program:

    C:\Users\muhsm\CLionProjects\assignment_4_version7\input_file.txt

Then run the compiled executable:

    ./classify

------------------------------------------------------------------------------
Input:
-------
- `input_file.txt`: A text file containing a list of integers, one per line.

------------------------------------------------------------------------------
Output:
--------
The program generates the following files in the same project directory:

- `even.out`     : Contains all even numbers  
- `odd.out`      : Contains all odd numbers  
- `positive.out` : Contains all positive numbers  
- `negative.out` : Contains all negative numbers  
- `square.out`   : Contains perfect squares and 0  
- `cube.out`     : Contains perfect cubes (positive or negative)

------------------------------------------------------------------------------
Threading Model:
-----------------
- 1 Reader Thread:
    Reads numbers from the input file and inserts them into a shared queue.
    
- 2 Worker Threads:
    Wait for data in the queue and concurrently process and classify the numbers.

Note:
You can increase the number of worker threads by uncommenting additional worker 
thread lines in the `main()` function.

------------------------------------------------------------------------------
Performance:
-------------
- The program measures and displays the total execution time in milliseconds.
- It uses a buffer of fixed size (500 elements) to control the flow between 
  the producer (reader) and consumers (workers).

------------------------------------------------------------------------------
Important Notes:
-----------------
- Ensure all output file paths are valid and writable.
- Update the file paths in the source code if running in a different environment.
- The program assumes that the input contains only valid integers.

------------------------------------------------------------------------------
