// using required header files
#include<bits/stdc++.h>
using namespace std;

// creating a structure for each prcoess which contains all necessary details
struct process
{
    int pid;
    int processing_time;
    int period;
    int deadline;
    int arrival_time;
    int remaining_time;
    float priority;
};

// creating a structure to keep track of waiting time for each process
struct waiting
{
    int waiting_time_total;
    int number_of_processes;
};

// initializing values of a waiting structure and returning it
waiting init_waiting()
{
    waiting neww;
    neww.number_of_processes = 0;
    neww.waiting_time_total = 0;
    return neww;
}

// a function to add waiting time to its mapped waiting structure given the process ID
void add_time(map<int, waiting> &m, int _pid, int time)
{
    map<int, waiting>::iterator itr;
    itr = m.find(_pid);
    if(itr != m.end())
    {
        itr->second.number_of_processes += 1;
        itr->second.waiting_time_total += time;
    }
}

// initializing values of a process strucutre and returning it
process init_process(int _pid, int _processing_time, int _period, int _arrival_time)
{
    process newp;
    newp.pid = _pid;
    newp.processing_time = _processing_time;
    newp.period = _period;
    newp.deadline = _arrival_time + _period;
    newp.arrival_time = _arrival_time;
    newp.priority = 1.0/_period;
    newp.remaining_time = _processing_time;

    return newp;
}

// a function to print basic details of given process (used while debugging)
void print_process(process p)
{
    cout << "Process ID:    " << p.pid << endl;
    cout << "Arrival time:  " << p.arrival_time << endl;
    cout << "Deadline time: " << p.deadline << endl;
    cout << "Priority:      " << p.priority << endl;
}

/* a function to return the process with highest priority given the ready queue
this function is used to select a process once a process is completed or terminated
*/
process get_highest_priority(vector<process> &list)
{
    float max_priority = 0.0;
    int max_priority_index = -1;
    // to find the process with maximum priority and its corresponding index
    for(int i=0; i<list.size(); i++)
    {
        if(list[i].priority > max_priority)
        {
            max_priority = list[i].priority;
            max_priority_index = i;
        }
    }
    // extracting required process
    process return_process = list[max_priority_index];
    list.erase(list.begin() + max_priority_index);
    return return_process;
}

// main function begins
int main()
{

    // n for number of processes
    // i and j are variables used to keep count in loops
    int n, i, j;
    int _pid, _processing_time, _period, _repeats; // variables to store data once it is read from input file

    // opening required files for input and output
    ifstream input("inp-params.txt");
    ofstream output1("RM-Log.txt");
    ofstream output2("RM-Stats.txt");

    // variables to store overall data
    int number_of_processes, successful_processes, terminated_processes, waiting_time;
    number_of_processes = successful_processes = terminated_processes = waiting_time = 0;
    input >> n;

    // creating 2 vectors: one to store details of all processes as input is taken and another to store details of all processes sorted according to their arrival time and priority
    vector<process> all_processes, sorted_processes;
    // initializing waiting map and its corresponding iterator
    map<int, waiting> waiting_map;
    map<int, waiting>::iterator itr;

    // taking input of all processes
    for(i=1; i<=n; i++)
    {
        input >> _pid >> _processing_time >> _period >> _repeats;
        waiting_map.insert(pair<int, waiting>(_pid, init_waiting()));
        output1 << "Process P" << _pid << ": processing time=" << _processing_time << \
        "; deadline:" << _period << "; period:" << _period << " joined the system at time 0" << endl;
        for(j=1; j<=_repeats; j++)
        {
            process temp = init_process(_pid, _processing_time, _period, (j-1)*_period);
            all_processes.push_back(temp);
            sorted_processes.push_back(temp);
            number_of_processes += 1;
        }
    }
    
    // sorting all processes based on their arrival time and priority
    for(i=0; i<all_processes.size()-1; i++)
    {
        for(j=0; j<all_processes.size()-i-1; j++)
        {
            if(sorted_processes[j].arrival_time > sorted_processes[j+1].arrival_time)
            {
                swap(sorted_processes[j], sorted_processes[j+1]);
            }
            else if(sorted_processes[j].arrival_time == sorted_processes[j+1].arrival_time)
            {
                if(sorted_processes[j].priority < sorted_processes[j+1].priority)
                {
                    swap(sorted_processes[j], sorted_processes[j+1]);
                }
            }
        }
    }
    // just to print all processes details once input is taken
    // for(i=0; i<all_processes.size(); i++)
    // {
    //     print_process(all_processes[i]);
    // }
    // cout << endl;
    // for(i=0; i<all_processes.size(); i++)
    // {
    //     print_process(sorted_processes[i]);
    // }

    // initializing the ready queue which will contain all the processes that have come into the system and are ready to execute
    vector<process> ready_queue;

    // initializing a variable to keep track of simulated time which is strictly increasing
    int current_time = 0;
    int flag, end_flag=0;
    process current_process; // a variable to store details of the process which is currently being executed

    // since we sorted the processes based on arrival time and priority, the first process to be executed will be first in the list
    ready_queue.push_back(sorted_processes[0]);
    sorted_processes.erase(sorted_processes.begin()); // removing it from the sorted list
    // cout << endl;

    // starting the loop which will take care of selecting and running processes until all are completed 
    while(!sorted_processes.empty() + !ready_queue.empty() > 0)
    {
        // if there are processes still in the ready queue and in the sorted list
        if(!sorted_processes.empty() && !ready_queue.empty())
        {
            // extract process with highest priority from ready queue
            current_process = get_highest_priority(ready_queue);
            // a check to see if the selected process can meet its deadline
            // if it fails to meet the deadline, it will be terminated immediately and let the next process run
            if(current_time + current_process.remaining_time > current_process.deadline)
            {
                output1 << "Process P" << current_process.pid << " could not meet its deadline at time " << current_process.deadline << " and thus got terminated" << endl;
                
                add_time(waiting_map, current_process.pid, current_process.deadline - current_process.arrival_time - current_process.processing_time);
                terminated_processes += 1;
                // considering the waiting time for a terminated process until the point from whereafter, it will be terminated no matter when it starts running
            }
            // process can still meet its deadline
            else
            {
                // extracting processes from sorted list which must be in the ready queue that is its arrival time is not greater than the current time
                while(!sorted_processes.empty())
                {
                    if(sorted_processes[0].arrival_time <= current_time)
                    {
                        ready_queue.push_back(sorted_processes[0]);
                        sorted_processes.erase(sorted_processes.begin());
                    }
                    else
                    {
                        break;
                    }
                }
                // extracting a process with highest priority from ready queue just to compare with existing current process as new processes have been added
                process new_proc = get_highest_priority(ready_queue);
                if(current_process.priority >= new_proc.priority)
                {
                    ready_queue.push_back(new_proc);
                }
                else
                {
                    ready_queue.push_back(current_process);
                    current_process = new_proc;
                }

                if(current_process.remaining_time != current_process.processing_time) // this means the process had been preempted earlier and is continuing now
                {
                    output1 << "Process P" << current_process.pid << " resumes execution at time " << current_time + 1 << "." << endl;
                }
                else // process being executed for the first time
                {
                    output1 << "Process P" << current_process.pid << " starts execution at time " << current_time + 1 << "." << endl;    
                }
                // a flag to check if any process with higher priority will enter the ready while the process is running
                flag = 0; 
                while(!sorted_processes.empty() && sorted_processes[0].arrival_time < current_time + current_process.remaining_time)
                {
                    // pushing the processes as they get added to the ready queue while a process is being executed
                    process new_process = sorted_processes[0];
                    ready_queue.push_back(sorted_processes[0]);
                    sorted_processes.erase(sorted_processes.begin());
                    // if we have reached the last process
                    if(sorted_processes.size() == 0 && ready_queue.size() == 1)
                    {
                        end_flag = 1;
                    }
                    // if we find a process with higher priority which enters the ready queue while a process is running
                    // thus the current process will be preempted
                    if(new_process.priority > current_process.priority)
                    {
                        flag = 1;
                        current_process.remaining_time -= (new_process.arrival_time - current_time); // updating remaining processing time
                        ready_queue.push_back(current_process);
                        current_time = new_process.arrival_time;
                        output1 << "Process P" << current_process.pid << " is preempted by Process P" << new_process.pid << " at time " \
                        << current_time << ". Remaining process time:" << current_process.remaining_time << endl;
                        break;
                    }
                }
                if(flag == 1) // some process preempted
                {
                    continue;
                }
                if(flag == 0) // no process preempted and thus it is completed
                {
                    current_time += current_process.remaining_time;
                    output1 << "Process P" << current_process.pid << " finishes execution at time " << current_time << "." << endl;
                    add_time(waiting_map, current_process.pid, current_time - current_process.arrival_time - current_process.processing_time);
                }
                if(end_flag == 1) // all processes have been executed and there is only one process remaining
                {
                    current_process = get_highest_priority(ready_queue);
                    output1 << "Process P" << current_process.pid << " starts execution at time " << current_time + 1 << "." << endl;
                    output1 << "Process P" << current_process.pid << " finishes execution at time " << current_time + current_process.remaining_time << "." << endl;
                    add_time(waiting_map, current_process.pid, current_time - current_process.arrival_time);
                    // exit(0);
                    break;

                }
            }
        }

        // if ready queue is empty but there are processes still in sorted list meaning the CPU will be idle for sometime
        else if(!sorted_processes.empty() == 1 && !ready_queue.empty() == 0)
        {
            // if the arrival time of next process is more than current time
            if(sorted_processes[0].arrival_time > current_time)
            {
                output1 << "CPU is idle till time " << sorted_processes[0].arrival_time << "." << endl;
            }
            // extracting first process from sorted list to continue execution after CPU has been idle
            current_time = sorted_processes[0].arrival_time;
            ready_queue.push_back(sorted_processes[0]);
            sorted_processes.erase(sorted_processes.begin());
            if(sorted_processes.empty()) // if sorted list is empty, then we have extracted the last process
            {
                current_process = get_highest_priority(ready_queue);
                // to check if the process miseed its deadline or not
                if(current_time+current_process.remaining_time > current_process.deadline) // last process missed its deadline
                {
                    output1 << "Process P" << current_process.pid << " could not meet its deadline at time " << current_process.deadline << " and thus got terminated" << endl;
                    terminated_processes += 1;
                    // add_time(waiting_map, current_process.pid, current_time - current_process.arrival_time - current_process.processing_time + current_process.remaining_time);
                    add_time(waiting_map, current_process.pid, current_process.deadline - current_process.arrival_time - current_process.processing_time);
                }
                else // printing last process
                {
                    output1 << "Process P" << current_process.pid << " starts execution at time " << current_time + 1 << "." << endl;
                    output1 << "Process P" << current_process.pid << " finishes execution at time " << current_time + current_process.remaining_time << "." << endl;
                    add_time(waiting_map, current_process.pid, current_time - current_process.arrival_time);
                    // cout << 1 << endl;
                    // exit(0);
                    break;
                }
            }
        }
        // if ready queue is not empty while sorted list is empty, then all processes have entered the ready queue and now all processes can run without being preempted
        else if(!sorted_processes.empty() == 0 && !ready_queue.empty() == 1)
        {
            // extracting process with highest priority until ready queue is empty too
            while(!ready_queue.empty() == 1)
            {
                current_process = get_highest_priority(ready_queue);
                if(current_time+current_process.remaining_time > current_process.deadline) // process missed its deadline
                {
                    output1 << "Process P" << current_process.pid << " could not meet its deadline at time " << current_process.deadline << " and thus got terminated." << endl;
                    // add_time(waiting_map, current_process.pid, current_time - current_process.arrival_time - current_process.processing_time + current_process.remaining_time);
                    add_time(waiting_map, current_process.pid, current_process.deadline - current_process.arrival_time - current_process.processing_time);
                    terminated_processes += 1;
                }
                else // process did not miss deadline and runs without being preempted
                {
                    output1 << "Process P" << current_process.pid << " starts execution at time " << current_time + 1 << "." << endl;
                    output1 << "Process P" << current_process.pid << " finishes execution at time " << current_time + current_process.remaining_time << "." << endl;
                    add_time(waiting_map, current_process.pid, current_time - current_process.arrival_time);
                    current_time += current_process.remaining_time; // updating time
                }
            }
        }

    }

    // calculating and printing required statistics
    successful_processes = number_of_processes - terminated_processes;
    waiting_time = 0;
    output2 << "Number of processes that came into the system: " << number_of_processes << endl;
    output2 << "Number of processes that successfully completed: " << successful_processes << endl;
    output2 << "Number of processes that missed their deadlines: " << terminated_processes << endl;
    output2 << "Average waiting time for each process is given below" << endl;
    for(itr = waiting_map.begin(); itr != waiting_map.end(); itr++)
    {
        output2 << "Process P" << itr->first << " Average waiting time: " << (float)itr->second.waiting_time_total / itr->second.number_of_processes << endl;
        waiting_time += itr->second.waiting_time_total;
    }
    output2 << "Average waiting time: " << (float)waiting_time / number_of_processes << endl;
    return 0;

    // end of program
}