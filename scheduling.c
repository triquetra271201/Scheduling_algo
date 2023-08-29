// Non Pre-emptive Priority Scheduling With Round Robin
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <math.h>

#define PROCESS_LIMIT 128
#define QUANTUM_TIME 4

struct Process{
    int num;
    int burst_count;
    int priority;
    int arrival_time;
    int CPU_bursts[128];
    int IO_bursts[128];
    int burst_index; 
    int remaining_time; 
};

struct Schedule{
    int id;
    int cpio;
    int start_time;
    int end_time;
};

struct Process ProcessArray[PROCESS_LIMIT];
struct Schedule SchedArray[PROCESS_LIMIT];
int total_proc = 0;
int schedIndex = 0;

void getProcesses(){
    // gets processes from the text file and stores it in ProcessArray
    FILE* filePointer = fopen("scheduling.txt","r");
    fscanf(filePointer, "%d", &total_proc);

    for (int i=0; i<total_proc; i++){
        fscanf(filePointer, "%d %d %d", &ProcessArray[i].burst_count, &ProcessArray[i].priority, &ProcessArray[i].arrival_time);

        int burst_cpu = ProcessArray[i].burst_count;
        for (int j=0; j<ProcessArray[i].burst_count; j++){
            fscanf(filePointer, "%d", &ProcessArray[i].CPU_bursts[j]);
            burst_cpu--;
            if (burst_cpu != 0) fscanf(filePointer, "%d", &ProcessArray[i].IO_bursts[j]);
        }

        ProcessArray[i].num = i;
        ProcessArray[i].burst_index = 0;
        ProcessArray[i].remaining_time = 0;
    }

    fclose(filePointer);
}

int selectNextProcess(int currentTime){
    // selects next process to execute in CPU according to priority and arrival
    int maxPriority = total_proc;
    int maxPriorityIndex = -1;
    for (int i=0; i<total_proc; i++){
        if(ProcessArray[i].num == -1) continue;
        if (ProcessArray[i].arrival_time <= currentTime){
            if (ProcessArray[i].priority < maxPriority){
                maxPriority = ProcessArray[i].priority;
                maxPriorityIndex = i;
            }
        }
    }
    return maxPriorityIndex;
}

int nextArrivalTime(int currentTime){
    // if ready queue is empty find next earliest arrival of process
    int nextTime = INT_MAX;
    for (int i=0; i<total_proc; i++){
        if (ProcessArray[i].arrival_time > currentTime && ProcessArray[i].arrival_time < nextTime){
            nextTime = ProcessArray[i].arrival_time;
        }
    } 
    return nextTime;
}

int main(int argc, char* argv[]){
    getProcesses();

    srand(time(NULL));
    int remainingProc = total_proc;
    int currentTime = 0; 
    while (remainingProc > 0){
        int factor = (rand() % 41) + 60; // random number between 60 and 100
        int procIndex = selectNextProcess(currentTime);
        struct Process P = ProcessArray[procIndex];
        if (procIndex == -1){
            // ready queue is empty, finding idle time
            int nextTime = nextArrivalTime(currentTime);
            SchedArray[schedIndex] = (struct Schedule){.id = -1, .cpio = 0, .start_time = currentTime, .end_time = nextTime};
            currentTime = nextTime;
            schedIndex++;
        }

        else{
           int cpuBurst;
           if (P.remaining_time == 0){
            cpuBurst = (int)ceil((P.CPU_bursts[P.burst_index] * factor)/100);
           }
           else{
            cpuBurst = P.remaining_time;
            P.remaining_time = 0;
           }

           int ioBurst = P.IO_bursts[P.burst_index];
           
           if (cpuBurst <= QUANTUM_TIME) {

            SchedArray[schedIndex] = (struct Schedule){.id = P.num, .cpio = 0, .start_time = currentTime, .end_time = currentTime + cpuBurst};
            currentTime = currentTime + cpuBurst;
            schedIndex++;
            if(ioBurst) {
                SchedArray[schedIndex] = (struct Schedule){.id = P.num, .cpio = 1, .start_time = currentTime, .end_time = currentTime + ioBurst};
                schedIndex++;
            }
            P.burst_index++;
            P.arrival_time = currentTime + ioBurst;

            }

            else{

            SchedArray[schedIndex] = (struct Schedule){.id = P.num, .cpio = 0, .start_time = currentTime, .end_time = currentTime + QUANTUM_TIME};
            schedIndex++;
            currentTime = currentTime + QUANTUM_TIME;
            P.remaining_time = cpuBurst - QUANTUM_TIME;
            P.arrival_time = currentTime;

            }
            // end of process
            if (P.burst_index == P.burst_count){
                P.num = -1;
                remainingProc--;
            }

        ProcessArray[procIndex] = P;  
        } 
    }

    printf("CPU Schedule: \n");
    for (int i=0; i<schedIndex; i++){
        struct Schedule S = SchedArray[i];
        
        if (S.cpio == 0){
                if (S.id == -1){
                    printf("IDLE %d -> %d\n", S.start_time, S.end_time);
                }
                else{
                    printf("Process %d: %d -> %d\n", S.id, S.start_time, S.end_time);
                }
        }
    }
    printf("\nI/O Schedule: \n");
    for (int i=0; i<schedIndex; i++){
        struct Schedule S = SchedArray[i];
        
        if (S.cpio == 1){
            printf("Process %d: %d -> %d\n", S.id, S.start_time, S.end_time);
        }
    }
    return 0;
}



