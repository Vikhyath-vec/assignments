Instructions to run the file in Linux and get required output:

    1. Download the files SrcAssgn4-cas-cs20btech11056.cpp, SrcAssgn4-tas-cs20btech11056.cpp, SrcAssgn4-cas-bounded-cs20btech11056.cpp
       These are the required C++ program files which implement mutual exclusion algorithms.
       Without loss of generality, further instructions will be specific to cas mutual exclusion algorithm (SrcAssgn4-cas-cs20btech11056.cpp). For running other files, instructions are similar.
    2. To run the code, go to any terminal and find the directory which contains the above downloaded file.
    3. Run the command "g++ SrcAssgn4-cas-cs20btech11056.cpp -o cas -lpthread" to compile.
    4. Before running above compiled code, create a file named "inp-params.txt" in the same directory as aforementioned source file which contains necessary input data in the following format:
            <number of threads> <number of times each thread enters critical section> <lambda1> <lambda2>
       For example, a valid input would look like:
            100 10 0.5 0.2
    5. Use the command "./cas" to run the executable.
    6. The code will create two files.
       (i) CAS-Log.txt which contains the logs of all threads.
       (ii) CAS-Stats.txt which contains important stats such as average and worst-case waiting time.
    7. To test TAS mutual exclusion algorithm, run the command "g++ SrcAssgn4-tas-cs20btech11056.cpp -o tas -lpthread" and then the command "./tas" to run the executable.
    8. To test Bounded waiting CAS mutual exclusion algorithm, run the command "g++ SrcAssgn4-cas-bounded-cs20btech11056.cpp -o casb -lpthread" and then the command "./casb" to run the executable.