//
// Created by lior on 7/7/24.
//st

#ifndef DATADISPLAY_H
#define DATADISPLAY_H

#include <ncurses.h>
#include <unordered_set>

#include "ProcessRetriever.h"

struct NewOrClosedProcesses {
	std::vector<Process> closed_processes;
	std::vector<Process> new_processes;

	NewOrClosedProcesses(const std::unordered_set<Process>& new_processes_set, std::vector<Process>& closed_processes)
		:	closed_processes(std::move(closed_processes)){
		new_processes.reserve(new_processes_set.size());
		for(auto& process : new_processes_set)
			new_processes.push_back(process);
	}
};


class DataDisplay {
public:
	explicit DataDisplay(double refresh_delay);
	~DataDisplay();


private:
	//	box dimensions
	auto static constexpr kBoxWidth = 100;
	auto static constexpr kInputBoxHeight = 3;
	auto static constexpr kProcessBoxHeight = 50;
	auto static constexpr kNotificationBoxHeight = 3;

	//	box positions
	auto static constexpr kInputBoxPos = 0;
	auto static constexpr kNotificationBoxPos = 3;
	auto static constexpr kProcessBoxPos = 6;

	void DisplayProcceses();
	static NewOrClosedProcesses GetNewOrClosedProcesses(const std::list<Process>& retreived_processes, const std::unordered_set<Process>& previous_processes);

	std::list<Process> retreived_processes_;
	ProcessRetreiver retreiver_;

	WINDOW* input_box_window_;
	WINDOW* process_box_window_;
	WINDOW* notification_box_window_;

	double refresh_delay_;
	bool keep_display_;
};



#endif //DATADISPLAY_H
