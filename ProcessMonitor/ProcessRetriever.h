//
// Created by lior on 7/5/24.
//

#ifndef PROCESSRETRIEVER_H
#define PROCESSRETRIEVER_H

#include <sched.h>
#include <string>
#include <vector>
#include <fstream>

struct Process {
    const pid_t PID;
    const std::string name;
    const double CPUUsage;
    const double memUsage;

    Process(const pid_t pid, const std::string& n, const double cpu, const double mem)
        :PID(pid), name(n), CPUUsage(cpu), memUsage(mem)
    {}
};

class ProcessRetreiver
{
public:
    ProcessRetreiver();
    [[nodiscard]] std::vector<Process> getRunningProcesses() const;

private:
    int m_MemTotal;
};



#endif //PROCESSRETRIEVER_H
