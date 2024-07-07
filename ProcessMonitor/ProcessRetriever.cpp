//
// Created by lior on 7/5/24.
//

#include "ProcessRetriever.h"


ProcessRetreiver::ProcessRetreiver()
  :memTotal_(GetMemTotal()) {

}

std::list<Process> ProcessRetreiver::GetRunningProcesses() {
	std::list<Process> processes;

	for(const auto& entry : std::filesystem::directory_iterator(PROC_DIR_PATH)) {
		if(!IsNumeric(entry.path().filename().string()) ||
			!entry.is_directory())
			continue;

		std::ifstream statusFile(entry.path().string() + STATUS_FILE_PATH);

		std::string procName = GetProcessName(statusFile);
		processes.emplace_back(
			entry.path().filename().string(),
			procName,
			GetProcessCPU_Usage(entry),
			GetProcessMemoryUsage(statusFile));

	}
	return processes;
}

bool ProcessRetreiver::IsNumeric(const std::string_view &string) const {
	for(const auto& letter : string) {
		if(!std::isdigit(letter))
			return false;
	}

	return true;
}

int ProcessRetreiver::GetMemTotal() const {
	std::ifstream meminfoFile(MEMINFO_FILE_PATH);
	std::string firstLine;

	if(!meminfoFile.is_open())
		throw std::runtime_error(MEMINFO_FILE_ERROR);

	std::getline(meminfoFile, firstLine); //The MemTotal field is in the first line

	return GetIntegerValFromLine(firstLine);
}

std::string ProcessRetreiver::GetProcessName(std::ifstream &statusFile) const {
	std::string firstLine;
	std::getline(statusFile, firstLine);
	return GetStringValFromLine(firstLine);
}

double ProcessRetreiver::GetProcessMemoryUsage(std::ifstream &statusFile) const {
	std::string line;
	std::getline(statusFile, line);

	while(!line.starts_with(VM_RSS_KEY)) {
		if(statusFile.eof())
			return 0;
		std::getline(statusFile, line);
	}

	return GetIntegerValFromLine(line) / static_cast<double>(memTotal_) * 100;
}

double ProcessRetreiver::GetProcessCPU_Usage(const std::filesystem::directory_entry &dir)  {
	std::ifstream statFile(dir.path().string() + STAT_FILE_PATH);
	std::string line;
	std::getline(statFile, line);
	double utime, stime, startTime;
	std::string trashValue;

	std::stringstream ss(line);

	for(int i = 0; i<UTIME_INDEX; i++)
		ss >> trashValue;

	ss >> utime;
	ss >> stime; //stime is one index after utime

	for(int i = UTIME_INDEX + 1; i<START_TIME_INDEX; i++)
		ss >> trashValue;

	ss >> startTime;

	//clock tick into seconds
	utime /= CLK_TCK;
	stime /= CLK_TCK;
	startTime /= CLK_TCK;
	double upTime = GetSystemUpTime();
	return (utime + stime) * 100 / (GetSystemUpTime() - startTime);
}

double ProcessRetreiver::GetSystemUpTime() {
	std::ifstream uptimeFile(UPTIME_DIR_PATH);
	std::string line;
	std::getline(uptimeFile, line);

	return std::stod(line.substr(0, line.find_first_of(" "))); //first number in the uptime file
}

/*
format:
Key:    value

amount of spaces is unknown
 */
std::string ProcessRetreiver::GetStringValFromLine(const std::string &line) const {
	return line.substr(line.find_last_of("\t") + 1);
}

int ProcessRetreiver::GetIntegerValFromLine(const std::string &line) const {
	return std::stoi(line.substr(line.find_first_of(DIGITS), line.find_last_of(DIGITS)));
}



