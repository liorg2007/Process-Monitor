//
// Created by lior on 7/5/24.
//

#include "ProcessRetriever.h"

#include <iostream>


ProcessRetreiver::ProcessRetreiver()
 : memTotal_(GetMemTotal()) {

}

int ProcessRetreiver::GetMemTotal() const {
    std::ifstream meminfoFile(MEMINFO_FILE_PATH);
    std::string firstLine;

    if(!meminfoFile.is_open())
        throw std::runtime_error(MEMINFO_FILE_ERROR);

    std::getline(meminfoFile, firstLine); //The MemTotal field is in the first line

    return std::stoi(firstLine.substr(firstLine.find_first_of(DIGITS), firstLine.find_last_of(DIGITS)));
}

