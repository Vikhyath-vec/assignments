Instructions to execute the code on Linux:

    1. Download the files Assgn3-RMScs20btech11056.cpp and Assgn3-EDFcs20btech11056.cpp
       These are the required cpp files to calculate logs and statistics of a given set of processes using RMS and EDF scheduling algorithms respectively.
    2. To run the codes, go to any terminal and find the directory which contains the above downloaded files.
    3. Run the command "g++ Assgn3-RMScs20btech11056.cpp -o Assgn3-RMScs20btech11056" to compile the file which uses RMS scheduling algorithm.
       Similarily, run the command "g++ Assgn3-EDFcs20btech11056.cpp -o Assgn3-EDFcs20btech11056" to compile the file which uses EDF scheduling algorithm.
    4. Before running above compiled codes, create a file named "inp-params.txt" in the same directory as aforementioned source files. This file contains necessary input data in the following format:
            <number of processes = n>
            <process id, processing time, period, number of repeats>
            .
            .
            .
            (n such lines)
       For example, a valid input would look like:
            2
            1 20 50 3
            2 35 60 3
    5. Use the command "./Assgn3-RMScs20btech11056" to run the executable which uses RMS scheduling.
       Similarily, use the command "./Assgn3-EDFcs20btech11056" to run the executable which uses EDF scheduling.
    6. Running the RMS executable will result in two files namely "RM-Logs.txt" and "RM-Stats.txt" whereas running the EDF executable will result in two files namely "EDF-Logs.txt" and "EDF-Stats.txt".
       (i) The Logs file contains logs of all the processes mentioned in the input file according to the scheduling algorithm used.
       (ii) The Stats file contains statistics like total number of processes, average waiting time, and more for the scheduling algorithm used on all the processes mentioned in the input file.