/*
 *Created By: Alex Payne
 *Main method class which takes in a trace file, and schedules input in a round robin format
 */

#include "process.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <queue>

int main()
{
    std::string fileN, currentLine;
    std::string tokens[5];
    int timeSlice[2];
    int procCount = 0;
    std::vector <Process> proc;
    std::queue <Process> q1;        //Ready queue for first queue
    std::queue <Process> q2;        //Ready queue for second queue
    std::queue <Process> ioToQ1;    //Input/Output queue leading to first queue
    std::queue <Process> ioToQ2;    //Input/Output queue leading to second queue
    std::queue <Process> cpu;       //Queue of currently running process
    std::cout << "Please input trace file: ";
    std::cin >> fileN;
    std::cout << "Enter time slice for queue 0: ";
    std::cin >> timeSlice[0];
    std::cout << "Enter time slice for queue 1: ";
    std::cin >> timeSlice[1];
    std::ifstream traceFile(fileN.c_str());
    int total = 0;

    while (getline(traceFile, currentLine)){      //Loops through file, reading in each process properties
        std::stringstream ss(currentLine);
        for (int i = 0; i < 5; ++i){
            getline (ss, tokens[i], ' ');
        }
        Process temp(atoi(tokens[0].c_str()), atoi(tokens[1].c_str()), atoi(tokens[2].c_str()), atoi(tokens[3].c_str()), atoi(tokens[4].c_str()));  //Creates a new process to be scheduled
        temp.setSlot(-1);
        proc.push_back(temp);
        total = total + atoi(tokens[1].c_str()) + atoi(tokens[2].c_str()) + atoi(tokens[3].c_str()) * atoi(tokens[4].c_str());    //Adds to the maximum total amount of time to be taken
        ++procCount;
    }

    Process tempP;
    int entered[proc.size()];
    for (int i = 0; i < procCount; ++i){
        entered[i] = 0;
    }

    std::ofstream newFile((fileN + ".out").c_str());
    int time = 0;
    while (time < total){   //Loops until all processes have been scheduled.
        for (int i = 0; i < procCount; ++i){    //Loops through all initial processes to be inserted into scheduler
            if (entered[i] == 0 && proc.at(i).getArrivalTime() <= time){    //Inserts a process when it has arrived, if it has not yet been inserted
                q1.push(proc.at(i));
                entered[i] = 1;
            }
        }

        if (!ioToQ1.empty()){   //If the I/O queue which leads to first ready queue is not empty
            for (int i = ioToQ1.size(); i > 0; --i){    //Loops through all processes in the I/O queue
                tempP = ioToQ1.front();
                ioToQ1.pop();
                tempP.setIOdone(tempP.getIOdone() + 1);
                if (tempP.getCPUTime() == tempP.getCPUused() && tempP.getDur() == tempP.getIOdone()){   //If the process has finished all CPU and I/O bursts
                    newFile << "Process " << tempP.getID() << " ended at time " << time+1 << "." << std::endl;   //Outputs to user the finish time for the process
                }
                if (tempP.getIOdone() == tempP.getDur()+1 || total - time == 1){    //If a process has finished its current I/O burst, push it back into first ready queue
                    if (tempP.getCPUTime() != tempP.getCPUused()){
                        tempP.setIOdone(0);
                        tempP.setQueue(0);
                        tempP.setSlot(-1);
                        q1.push(tempP);
                    }
                }
                else ioToQ1.push(tempP);    //Otherwise, the process has not finished the current I/O burst, pushes it back into the I/O queue
            }
        }

        if (!ioToQ2.empty()){   //If the I/O queue which leads to second ready queue is not empty
            for (int i = ioToQ2.size(); i > 0; --i){    //Loops through all processes in the I/O queue
                tempP = ioToQ2.front();
                ioToQ2.pop();
                tempP.setIOdone(tempP.getIOdone() + 1);
                if (tempP.getCPUTime() == tempP.getCPUused() && tempP.getDur() == tempP.getIOdone()){   //If the process has finished all CPU and I/O bursts
                    newFile << "Process " << tempP.getID() << " ended at time " << time+1 << "." << std::endl;   //Outputs to user the finish time for the process
                }
                if (tempP.getIOdone() == tempP.getDur()+1 || total - time == 1){    //If a process has finished its current I/O burst, push it back into second ready queue
                    if (tempP.getCPUTime() != tempP.getCPUused()){
                        tempP.setIOdone(0);
                        tempP.setQueue(1);
                        tempP.setSlot(-1);
                        q1.push(tempP);
                    }
                }
                else ioToQ2.push(tempP);    //Otherwise, the process has not finished the current I/O burst, pushes it back into the I/O queue
            }
        }

        if (!q1.empty() && cpu.empty()){    //If the first ready queue is not empty and the execute queue is empty, pushes the next process to the CPU
            tempP = q1.front();
            tempP.setSlot(-1);
            tempP.setQueue(0);
            cpu.push(tempP);
            q1.pop();
        }

        else if (!q2.empty() && cpu.empty()){   //If the second ready queue is not empty and the sexecute queue is empty, pushes the next process to the CPU
            tempP = q2.front();
            tempP.setSlot(-1);
            tempP.setQueue(1);
            cpu.push(tempP);
            q2.pop();
        }

        if (!cpu.empty()){      //If there is a process being executed/to be executed
            tempP = cpu.front();
            cpu.pop();
            int curTimeSlice;
            if (tempP.getQueue() == 0) curTimeSlice = timeSlice[0];     //Decides which timeslice to be using
            else curTimeSlice = timeSlice[1];
            if (tempP.getTimeSlot() == -1){    //If it is just starting being executed
                tempP.setStart(tempP.getCPUused());
                tempP.setProg(tempP.getCPUTime()/tempP.getFreq() - tempP.getCPUused()%(tempP.getCPUTime()/tempP.getFreq()));    //Gets the progress needed to complete the CPU burst
                if ((tempP.getCPUTime() - tempP.getCPUused()) < tempP.getProg()) tempP.setProg(tempP.getCPUTime() - tempP.getCPUused());   //Checks if on an unusual CPU burst
                if (tempP.getProg() <= curTimeSlice){      //If the CPU burst is within the queue's time slice constraint
                    tempP.setSlot(tempP.getProg());
                    tempP.setCPUused(tempP.getCPUused() + 1);
                    if (tempP.getCPUused()%(tempP.getCPUTime()/tempP.getFreq()) == 0){      //If the program has finished the process's CPU burst
                        if (tempP.getCPUTime() == tempP.getCPUused()){                      //If the program has finished the last CPU burst

                            if ((tempP.getCPUTime()/tempP.getFreq())*tempP.getFreq() == tempP.getCPUused()){            //If the program has a usual or unusual last CPU burst
                                if (tempP.getProg() <= tempP.getTimeSlot()) ioToQ1.push(tempP);              //Checks which I/O queue to send a process with a usual CPU burst
                                else ioToQ2.push(tempP);
                            }
                            else{

                                newFile << "Process " << tempP.getID() << " ended at time " << time+1 << "." << std::endl;   //Outputs to user the finish time for the process
                            }
                        }
                        else{       //If the CPU burst was not the last cpu burst
                            if (tempP.getProg() <= tempP.getTimeSlot() && tempP.getQueue() != 1){
                                tempP.setSlot(-1);
                                ioToQ1.push(tempP);
                            }
                            else {
                                ioToQ2.push(tempP);
                                tempP.setSlot(-1);
                            }
                        }
                    }
                    else{       //Returns process to the execute queue if it is not finished it's CPU burst
                        cpu.push(tempP);
                    }
                }
                else{       //If the time slice is smaller than CPU burst
                    tempP.setSlot(curTimeSlice);
                    tempP.setCPUused(tempP.getCPUused() + 1);
                    if (curTimeSlice == 1) q2.push(tempP);
                    else cpu.push(tempP);
                }
            }
            else{   //If the process has already started execution
                tempP.setCPUused(tempP.getCPUused() + 1);

                if (tempP.getCPUused() == (tempP.getStartCPU() + tempP.getTimeSlot())){ //If the process has finished its CPU burst

                    if (tempP.getCPUTime() == tempP.getCPUused()){                      //If the program has finished the last CPU burst
                        std::cout << "arrival out" << std::endl;
                        if (0 == (tempP.getCPUTime() - (tempP.getCPUTime()/tempP.getFreq())*tempP.getFreq())){            //If the program has a usual or unusual last CPU burst
                            if (tempP.getProg() <= tempP.getTimeSlot()) ioToQ1.push(tempP);              //Checks which I/O queue to send a process with a usual CPU burst
                            else ioToQ2.push(tempP);
                        }
                        else{
                            newFile << "Process " << tempP.getID() << " ended at time " << time+1 << "." << std::endl;   //Outputs to user the finish time for the process
                        }
                    }
                    else{       //If the CPU burst was not the last cpu burst
                        if (tempP.getCPUused() != tempP.getStartCPU() + tempP.getProg()){
                            if (tempP.getProg() <= tempP.getTimeSlot()) q1.push(tempP);
                            else q2.push(tempP);
                        }
                        else{
                            if (tempP.getProg() <= tempP.getTimeSlot()) ioToQ1.push(tempP);
                            else ioToQ2.push(tempP);
                        }
                    }
                    tempP.setSlot(-1);
                }
                else{   //If the process has not finished its time slice
                    cpu.push(tempP);
                }
            }
        }
        ++time;
    }
    std::cout << "Output placed in " << (fileN + ".out") << std::endl;
    newFile.close();
}
