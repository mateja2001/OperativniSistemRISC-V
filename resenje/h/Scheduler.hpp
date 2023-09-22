#ifndef PROJECT_BASE_V1_1_SCHEDULER_HPP
#define PROJECT_BASE_V1_1_SCHEDULER_HPP
class PCB;
class Scheduler{
private:
    static PCB* readyHead;
public:
    static PCB* get();
    static void put(PCB* pcb);

};




#endif //PROJECT_BASE_V1_1_SCHEDULER_HPP

