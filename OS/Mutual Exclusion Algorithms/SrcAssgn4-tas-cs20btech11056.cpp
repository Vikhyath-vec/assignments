// including necessary libraries
#include <iostream>
#include <atomic>
#include <unistd.h>
#include <fstream>
#include <random>
#include <sys/time.h>
#include <vector>
#include <chrono>
#include <stdlib.h>
#include <thread>

using namespace std; 
using namespace chrono;

// global variables to store input data
int number_of_threads, number_of_repetitions;
double lambda1, lambda2;

// atomic flag lock used in mutual exclusion
atomic_flag lock = ATOMIC_FLAG_INIT;

// to sotre statistics
double total_waiting_time;
double maximum_waiting_time = 0;

// to generate random numbers
default_random_engine generator(time(NULL));

// required exponential distributions
exponential_distribution<double> dist1(1.0/lambda1);
exponential_distribution<double> dist2(1.0/lambda2);

// log file
FILE *output = fopen("TAS-Log.txt", "w");

// function to return current time (precision upto micro seconds) as a string
string current_time()
{
    time_t t;
    struct tm *time_info;
    time(&t);
    time_info = localtime(&t);
    struct timeval tv;
    gettimeofday(&tv, NULL);
    char return_str[20];
    sprintf(return_str, "%.2d:%.2d:%.2d:%.3zu:%.3zu", time_info->tm_hour, time_info->tm_min, time_info->tm_sec, tv.tv_usec/1000, tv.tv_usec%1000);
    string return_time(return_str);
    return return_time;

}

// function to return suffix for given number
string suffix(int i)
{
    if(i%10 == 1 && i!=11)
    {
        return "st";
    }
    else if(i%10 == 2 && i!=12)
    {
        return "nd";
    }
    else if(i%10 == 3 && i!=13)
    {
        return "rd";
    }
    return "th";
}

void testCS(int thread_id)
{
    int i;
    // to store times before and after waiting period
    steady_clock::time_point begin_waiting_time, end_waiting_time;
    // a thread entering the critical section multiple times
    for(i=0; i<number_of_repetitions; i++)
    {
        // entry section - to acquire the lock
        string request_time = current_time();
        fprintf(output, "%d %s CS Requested at %s by thread %d\n", i+1, suffix(i+1).c_str(), request_time.c_str(), thread_id);
        
        // storing the time just before it starts to wait for the lock
        begin_waiting_time = steady_clock::now();
        // trying to acquire the lock
        while(atomic_flag_test_and_set(&lock));
        // loop breaks once lock is acquired and we store time at which it does so
        // critical section begins
        end_waiting_time = steady_clock::now();
        fprintf(output, "%d %s CS Entered at %s by thread %d\n", i+1, suffix(i+1).c_str(), current_time().c_str(), thread_id);
        duration<double, milli> waiting_time = end_waiting_time - begin_waiting_time;
        total_waiting_time += waiting_time.count(); // adding waiting time
        maximum_waiting_time = max(maximum_waiting_time, waiting_time.count()); // comparing current waiting time with maximum waiting time
        usleep(dist1(generator) * 1e6); // to simulate some time-consuming processes inside the critical section.
        fprintf(output, "%d %s CS Exited at %s by thread %d\n", i+1, suffix(i+1).c_str(), current_time().c_str(), thread_id);
        // critical section ends
        lock.clear();
        // remainder section
        usleep(dist2(generator) * 1e6); // to simulate some time-consuming processes inside the remainder section
    }
    
}

int main() 
{
    int i;
    ifstream inputs("inp-params.txt");
    // taking necessary inputs
    inputs >> number_of_threads >> number_of_repetitions >> lambda1 >> lambda2;
    inputs.close();
    fprintf(output, "TAS ME Output:\n");
    // creating required number of threads
    vector<thread> threads(number_of_threads);
    vector<int> thread_ids(number_of_threads);
    // initializing threads and passing the function
    for(i = 0; i < number_of_threads; i++)
    {
        thread_ids[i] = i + 1;
        threads[i] = thread(testCS, i+1);
    }
    // waiting for threads to terminate and join with the main thread
    for(i = 0; i < number_of_threads; i++)
    {
        threads[i].join();
    }

    // printing required statistics
    ofstream stats_output("TAS-Stats.txt");

    stats_output << "Average waiting time = " << total_waiting_time/(number_of_threads*number_of_repetitions) << endl;
    stats_output << "Maximum waiting time = " << maximum_waiting_time << endl;
    stats_output.close();
    return 0;
}