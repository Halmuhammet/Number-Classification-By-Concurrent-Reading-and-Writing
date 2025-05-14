/*=============================================================================*
 * Title        : main.cpp
 * Description  : This is a multithreading program. It utilizes reader and worker function using multiple threads concurrently.
 * Author       : Halmuhammet Muhamedorazov
 * Date         : 04/25/2024
 * Version      : 1.0
 * Usage        : Compile and run this program using the GNUC++ compiler
 * Notes        : To compile the program, don't forget to include -pthread to link the C++ threading libraries.
 * C++ Version  : g++ (GCC) 4.8.5 20150623 (Red Hat 4.8.5-16)
 * =============================================================================*/

// Import necessary files

#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <cmath>
#include <chrono>
using namespace std;

queue<int> buffer; // shared queue to store int numbers
mutex mtx;
condition_variable cv_worker; // condition variable for worker threads
condition_variable cv_reader; // condition varable for the reader
bool endOfInputFile = false;  // initialize the end of file to false
int bufferSize = 500;


// this function reads input file line by line and turns them into integer type and stores them in the shared buffer
void reader(ifstream& file) {
    int number;
    while(file >> number){
        // lock the shared buffer to make sure only one thread at a time can access it
        unique_lock<mutex> lock(mtx);
        if(buffer.size() >= bufferSize){
            cv_reader.wait(lock);
        }
        buffer.push(number);
        // when a number is pushed into shared buffer, notify the worker threads to wake up
        cv_worker.notify_all();
    }
    unique_lock<mutex> lock(mtx);
    // use a variable to indicate if the reader reached the end of file
    endOfInputFile = true;
    // then notify the worker threads
    cv_worker.notify_all();
}

// worker function classifies the numbers and directly writes them into its assciated files
void worker(ofstream& evenFile, ofstream& oddFile,ofstream& positiveFile,ofstream& negativeFile,ofstream& squareFile, ofstream& cubeFile){
    while(true){
        // lock the shared resources to make sure only one thread at a time accesses them
        unique_lock<mutex> lock(mtx);
        // wait if the shared buffer is empty and the reader has not reached the end of file yet
        while(buffer.empty() && !endOfInputFile) {
            cv_worker.wait(lock);
        }
        // break the loop if end of file reached and the buffer is empty
        if(endOfInputFile == true && buffer.empty()){
            break;
        }
        if(buffer.size() <= 100 && endOfInputFile == false){
            cv_reader.notify_one();
        }
        // if the buffer has greater or equal to 1000 elements, then process in 1000 elements
        if(buffer.size() >= 100){
            // process for 100 times
            for(int i = 0; i < 100; i++){
                // get the front element in the shared buffer
                int frontElementOfBuffer = buffer.front();
                buffer.pop();
                // classify and write the numbers into their associated files
                if(frontElementOfBuffer > 0){
                    positiveFile << frontElementOfBuffer << endl;
                    // if the result from cbrt function is anything other than a whole number, then it is not perfect square
                    if(fmod(sqrt(frontElementOfBuffer), 1) == 0){
                        squareFile << frontElementOfBuffer << endl;
                    }
                }//check for negative
                else if(frontElementOfBuffer < 0){
                    negativeFile << frontElementOfBuffer << endl;
                }// if the number is zero, then put it into square file
                else{
                    squareFile << frontElementOfBuffer << endl;
                }//check for even
                if(frontElementOfBuffer % 2 == 0){
                    evenFile << frontElementOfBuffer << endl;
                }// otherwise, the number is odd
                else{
                    oddFile << frontElementOfBuffer << endl;
                }
                // the result from cbrt function is anything other than a whole number, then it is not perfect cube
                if(fmod(cbrt(fabs(frontElementOfBuffer)), 1) == 0){
                    cubeFile << frontElementOfBuffer << endl;
                }
            }
        }// otherwise, do process numbers one-by-one
        else{
            int frontElementOfBuffer = buffer.front();
            buffer.pop();
            if(frontElementOfBuffer > 0){
                positiveFile << frontElementOfBuffer << endl;
                // if the result from cbrt function is anything other than a whole number, then it is not perfect square
                if(fmod(sqrt(frontElementOfBuffer), 1) == 0){
                    squareFile << frontElementOfBuffer << endl;
                }
            }//check for negative
            else if(frontElementOfBuffer < 0){
                negativeFile << frontElementOfBuffer << endl;
            }// if number is zero, put it into square file
            else{
                squareFile <<  frontElementOfBuffer << endl;
            }// check for even
            if(frontElementOfBuffer % 2 == 0){
                evenFile << frontElementOfBuffer << endl;
            }//otherwise, the number is odd
            else{
                oddFile << frontElementOfBuffer << endl;
            }
            // the result from cbrt function is anything other than a whole number, then it is not perfect cube
            if(fmod(cbrt(frontElementOfBuffer), 1) == 0){
                cubeFile << frontElementOfBuffer << endl;
            }
        }
    }
}


int main() {
    // Start measuring time
    auto start = std::chrono::high_resolution_clock::now();
    //open the file to read and check if it exists
    ifstream input_file("C:\\Users\\muhsm\\CLionProjects\\assignment_4_version7\\input_file.txt");
    if(!input_file.is_open()){
        cout << "File does not exist." << endl;
    }

    int number;
    ofstream evenFile("C:\\Users\\muhsm\\CLionProjects\\assignment_4_version7\\even.out");
    ofstream oddFile("C:\\Users\\muhsm\\CLionProjects\\assignment_4_version7\\odd.out");
    ofstream positiveFile("C:\\Users\\muhsm\\CLionProjects\\assignment_4_version7\\positive.out");
    ofstream negativeFile("C:\\Users\\muhsm\\CLionProjects\\assignment_4_version7\\negative.out");
    ofstream squareFile("C:\\Users\\muhsm\\CLionProjects\\assignment_4_version7\\square.out");
    ofstream cubeFile("C:\\Users\\muhsm\\CLionProjects\\assignment_4_version7\\cube.out");
    // prepare the output files


    // have 1 reader thread and 2 worker threads
    thread read(reader, ref(input_file));
    thread worker1(worker, ref(evenFile), ref(oddFile), ref(positiveFile), ref(negativeFile), ref(squareFile), ref(cubeFile));
    thread worker2(worker, ref(evenFile), ref(oddFile), ref(positiveFile), ref(negativeFile), ref(squareFile), ref(cubeFile));
//    thread worker3(worker, ref(evenFile), ref(oddFile), ref(positiveFile), ref(negativeFile), ref(squareFile), ref(cubeFile));

    // join threads
    read.join();
    worker1.join();
    worker2.join();
//    worker3.join();


    // close the files
    input_file.close();
    evenFile.close();
    oddFile.close();
    positiveFile.close();
    negativeFile.close();
    squareFile.close();
    cubeFile.close();


    // Stop measuring time
    auto end = std::chrono::high_resolution_clock::now();
    // Calculate duration
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    // Output the duration in milliseconds
    std::cout << "Execution time: " << duration.count() << " milliseconds" << std::endl;
    return 0;
}

