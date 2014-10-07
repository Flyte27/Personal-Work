/*
 *Created By: Alex Payne
 *Data class which stores information related to a process
 */
#ifndef PROCESS_H
#define PROCESS_H
#include "stdio.h"
#include <iostream>

class Process
{

public:
    Process();                  //Default constructor
    Process(int id, int arrTime, int CPUtime, int freq, int dur);   //Constructor used with given attributes
    //Getter Methods
    int getArrivalTime();
    int getCPUTime();
    int getFreq();
    int getDur();
    int getCPUused();
    int getID();
    int getQueue();
    int getIOdone();
    int getProg();
    int getTimeSlot();
    int getStartCPU();
    //Setter Methods
    void setProg(int prog);
    void setSlot(int timeS);
    void setStart(int start);
    void setIOdone(int io);
    void setQueue(int curQ);
    void setCPUused(int newCPU);
    void toString();

private:
    int _ID;            //Process ID
    int _arriveTime;    //Process arrival time
    int _CPUTime;       //CPU time to be done
    int _CPUused;       //CPU time already used
    int _IOfreq;        //Frequency of I/O burst
    int _IOduration;    //Duration of a I/O burst
    int _curQueue;      //Current queue storing process
    int _IOdone;        //Amount of I/O already done in an I/O burst
    int _progress;      //Amount of CPU needed for CPU burst
    int _timeSlot;      //Amount of CPU given for time slice
    int _startCPU;      //Amount of CPU done at start of time slice
};

#endif // PROCESS_H
