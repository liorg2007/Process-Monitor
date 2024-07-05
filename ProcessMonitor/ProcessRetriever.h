//
// Created by lior on 7/5/24.
//

#ifndef PROCESSRETRIEVER_H
#define PROCESSRETRIEVER_H

#include <sched.h>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>

struct Process {
    const pid_t PID;
    const std::string name;
    const double CPU_Usage;
    const double memUsage;

    Process(const pid_t PID, const std::string& name, const double CPU_Usage, const double memUsage)
        :PID(PID), name(name), CPU_Usage(CPU_Usage), memUsage(memUsage)
    {}
};

class ProcessRetreiver
{
public:
    ProcessRetreiver();

    [[nodiscard]] std::vector<Process> GetRunningProcesses() const;

private:
    static constexpr auto DIGITS = "0123456789";
    static constexpr auto MEMINFO_FILE_PATH = "/proc/meminfo";
    static constexpr std::filesystem::path PROC_DIR_PATH{"/proc"};

    static constexpr auto MEMINFO_FILE_ERROR = "Can't open /proc/meminfo";

    bool IsNumeric(const std::string& string) const;
    [[nodiscard]] int GetMemTotal() const;
    std::string GetProcessName(const std::filesystem::directory_entry& dir) const;
    double GetProcessMemoryUsage(const std::filesystem::directory_entry& dir) const;
    double GetProcessCPU_Usage(const std::filesystem::directory_entry& dir) const;

    const int memTotal_;
};



#endif //PROCESSRETRIEVER_H
