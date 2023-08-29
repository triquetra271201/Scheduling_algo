Structure of file
---------------
Total no. of processes
#CPU_bursts   Priority  Arrival_time   CPU1  IO1  CPU2  IO2 ... CPUx
#CPU_bursts   Priority  Arrival_time   CPU1  IO1  CPU2  IO2 ... CPUx
.
.
.



getProcesses() - parses the file and stores it in struct Process in ProcessArray

selectNextProcess() - selecting which process to execute next 
input -> current time 
output -> index of process in ProcessArray
checks which processes have already arrived at the current time and among them which is the highest (least numerically) priority. 
The index for that process is returned

nextArrivalTime () - In case no process has arrived before current time to find idle time we find the process which has the earliest arrival time 
among all processes.


main()
get next process index 
if idle then find next arrival time and move to that time 
else -
    if Process.remaining_time == 0 then full cpu burst is executed 
    else remaining time is cpu burst, set remaining time to 0

if cpuburst < Quantum time full cpu burst executed, goes to IO and comes back 
else partial execution, update remaining time and arrival time = current time

printing schedule

procIndex == -1 => ready queue is empty, IDLE 
Process.num == -1 => Process finished
Schedule.id = -1 => IDLE cpu 

