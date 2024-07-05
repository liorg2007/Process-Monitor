//
// Created by lior on 7/5/24.
//

#include "ProcessRetriever.h"


ProcessRetreiver::ProcessRetreiver()
 : memTotal_(GetMemTotal()) {

}

std::vector<Process> ProcessRetreiver::GetRunningProcesses() const {
    std::vector<Process> processes;
    for(const auto& entry : std::filesystem::directory_iterator(PROC_DIR_PATH)) {
        if(!IsNumeric(entry.path().filename().string()) ||
            !entry.is_directory())
            continue;

        processes.emplace_back(
            std::stoi(entry.path().filename().string()),
            GetProcessName(entry),
            GetProcessCPU_Usage(entry),
            GetProcessMemoryUsage(entry));
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

    return std::stoi(firstLine.substr(firstLine.find_first_of(DIGITS), firstLine.find_last_of(DIGITS)));
}

