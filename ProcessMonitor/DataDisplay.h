//
// Created by lior on 7/7/24.
//

#ifndef DATADISPLAY_H
#define DATADISPLAY_H

#include <ncurses.h>
#include <unordered_set>

#include "ProcessRetriever.h"

class DataDisplay {
public:
	DataDisplay();

	void DisplayProcceses();

private:
	std::unordered_set<Process> processes_;
	ProcessRetreiver retreiver_;
};



#endif //DATADISPLAY_H
