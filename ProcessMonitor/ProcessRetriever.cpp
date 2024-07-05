//
// Created by lior on 7/5/24.
//

#include "ProcessRetriever.h"

#include <algorithm>


ProcessRetreiver::ProcessRetreiver()
 : memTotal_(GetMemTotal()) {

}

std::vector<Process> ProcessRetreiver::GetRunningProcesses() const {
    std::vector<Process> processes;
    for(const auto& entry : std::filesystem::directory_iterator(PROC_DIR_PATH)) {
        if(!IsNumeric(entry.path().filename().string()) ||
            !entry.is_directory())
            continue;

        std::ifstream statusFile(entry.path().string() + STATUS_FILE_PATH);

        processes.emplace_back(
            std::stoi(entry.path().filename().string()),
            GetProcessName(statusFile),
            GetProcessCPU_Usage(entry),
            GetProcessMemoryUsage(statusFile));
    }
    return processes;
}

bool ProcessRetreiver::IsNumeric(const std::string &string) const {
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

    while(!line.starts_with(VM_RSS_KEY))
        std::getline(statusFile, line);

    return GetIntegerValFromLine(line) / static_cast<double>(memTotal_) * 100;
}

/*
format:
Key:    value

amount of spaces is unknown
 */
std::string ProcessRetreiver::GetStringValFromLine(const std::string &line) const {
    return line.substr(line.find_last_of(" "));
}

int ProcessRetreiver::GetIntegerValFromLine(const std::string &line) const {
    return std::stoi(line.substr(line.find_first_of(DIGITS), line.find_last_of(DIGITS)));
}



