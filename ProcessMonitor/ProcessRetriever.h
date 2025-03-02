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
#include <list>

#include <string>

struct Process {
	std::string PID;
	std::string name;
	double CPU_Usage;
	double memUsage;
	double startTime;

	Process(const std::string& PID, const std::string& name, const double CPU_Usage, const double memUsage, const double startTime)
			: PID(PID), name(name), CPU_Usage(CPU_Usage), memUsage(memUsage), startTime(startTime)
	{}

	bool operator==(const Process &other) const {
		return startTime == other.startTime && PID == other.PID;
	}

	Process(const Process& other)
			: PID(other.PID), name(other.name), CPU_Usage(other.CPU_Usage), memUsage(other.memUsage), startTime(other.startTime) {
	}

	Process& operator=(Process&& other) noexcept {
		if (this != &other) {
			PID = std::move(other.PID);
			name = std::move(other.name);
			CPU_Usage = other.CPU_Usage;
			memUsage = other.memUsage;
			startTime = other.startTime;

			// Set other members to default values
			other.CPU_Usage = 0;
			other.memUsage = 0;
			other.startTime = 0;
		}
		return *this;
	}
};


template<>
struct std::hash<Process> {
	auto operator()( const Process& x ) const
	{ return std::hash< std::string >()( x.PID + "_" +  std::to_string(x.startTime)); }
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
	[[nodiscard]] std::pair<double, double> GetProcessCPU_UsageAndStartTime(const std::filesystem::directory_entry& dir);
	[[nodiscard]] double GetSystemUpTime();
	std::string GetStringValFromLine(const std::string& line) const;
	int GetIntegerValFromLine(const std::string& line) const;

	const int memTotal_;
};



#endif //PROCESSRETRIEVER_H
