//
// Created by lior on 7/5/24.
//

#ifndef PROCESSRETRIEVER_H
#define PROCESSRETRIEVER_H

#include <sched.h>

#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
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

	[[nodiscard]] std::vector<Process> GetRunningProcesses();

private:
	static constexpr auto DIGITS = "0123456789";
	static constexpr auto MEMINFO_FILE_PATH = "/proc/meminfo";
	const std::filesystem::path PROC_DIR_PATH{"/proc"};
	static constexpr auto STATUS_FILE_PATH = "/status";
	static constexpr auto STAT_FILE_PATH = "/stat";
	static constexpr auto UPTIME_DIR_PATH = "/proc/uptime";
	static constexpr auto VM_RSS_KEY = "VmRSS";
	static constexpr auto CLK_TCK = 100;
	static constexpr auto UTIME_INDEX = 13;
	static constexpr auto START_TIME_INDEX = 20;

	static constexpr auto MEMINFO_FILE_ERROR = "Can't open /proc/meminfo";

	[[nodiscard]] bool IsNumeric(const std::string_view& string) const;
	[[nodiscard]] int GetMemTotal() const;
	[[nodiscard]] std::string GetProcessName(std::ifstream& statusFile) const;
	[[nodiscard]] double GetProcessMemoryUsage(std::ifstream& statusFile) const;
	[[nodiscard]] double GetProcessCPU_Usage(const std::filesystem::directory_entry& dir);
	[[nodiscard]] double GetSystemUpTime();
	std::string GetStringValFromLine(const std::string& line) const;
	int GetIntegerValFromLine(const std::string& line) const;

	const int memTotal_;
};



#endif //PROCESSRETRIEVER_H
