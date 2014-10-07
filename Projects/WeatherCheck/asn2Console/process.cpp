/*
 *Created By: Alex Payne
 *Data class which stores information related to a process
 */
#include "process.h"
#include "stdio.h"
#include <iostream>

//Constructors
Process::Process(){
    _ID = 0;
    _arriveTime = 0;
    _CPUTime = 0;
    _IOfreq = 0;
    _IOduration = 0;
    _CPUused = 0;
}

Process::Process(int id, int arrTime, int CPUtime, int freq, int dur)
{
    _ID = id;
    _arriveTime = arrTime;
    _CPUTime = CPUtime;
    _IOfreq = freq;
    _IOduration = dur;
    _CPUused = 0;
    _curQueue = 0;
    _IOdone = 0;
}

//Getter Methods
int Process::getArrivalTime(){
    return _arriveTime;
}

int Process::getCPUTime(){
    return _CPUTime;
}

int Process::getFreq(){
    return _IOfreq;
}

int Process::getDur(){
    return _IOduration;
}

int Process::getCPUused(){
    return _CPUused;
}

int Process::getID(){
    return _ID;
}

int Process::getQueue(){
    return _curQueue;
}

int Process::getIOdone(){
    return _IOdone;
}

int Process::getProg(){
    return _progress;
}

int Process::getTimeSlot(){
    return _timeSlot;
}

int Process::getStartCPU(){
    return _startCPU;
}

//Setter Methods
void Process::setProg(int prog){
    _progress = prog;
}

void Process::setSlot(int timeS){
    _timeSlot = timeS;
}

void Process::setStart(int start){
    _startCPU = start;
}

void Process::setIOdone(int io){
    _IOdone = io;
}

void Process::setQueue(int curQ){
    _curQueue = curQ;
}

void Process::setCPUused(int newCPU){
    _CPUused = newCPU;
}

//Outputs basic process data to user
void Process::toString(){
    std::cout << _ID << " " << _arriveTime << " " << _CPUTime << " " << _IOfreq << " " << _IOduration << std::endl;
}
