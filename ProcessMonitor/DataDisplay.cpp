//
// Created by lior on 7/7/24.
//

#include "DataDisplay.h"

#include <iostream>
#include <unistd.h>

DataDisplay::DataDisplay(double refresh_delay)
	: refresh_delay_(refresh_delay), keep_display_(true) {
	// initscr();
	//
	// //	initialize the windows for the boxes
	// input_box_window_ = newwin(kInputBoxHeight, kBoxWidth, kInputBoxPos, 0);
	// notification_box_window_ = newwin(kNotificationBoxHeight, kBoxWidth, kNotificationBoxPos, 0);
	// process_box_window_ = newwin(kProcessBoxHeight, kBoxWidth, kProcessBoxPos, 0);
	//
	// refresh();
	DisplayProcceses();
}

DataDisplay::~DataDisplay() {
	//	delete the memory of the windows
	delwin(input_box_window_);
	delwin(notification_box_window_);
	delwin(process_box_window_);

	endwin();
}

void DataDisplay::DisplayProcceses() {
	while(keep_display_) {
		std::unordered_set<Process> previous_processes(retreived_processes_.begin(), retreived_processes_.end());	//	save the previous processes to track changes
		retreived_processes_ = retreiver_.GetRunningProcesses();	//	get the new processes

		if(!previous_processes.empty()) {
			NewOrClosedProcesses update = GetNewOrClosedProcesses(retreived_processes_, previous_processes);
		}

	}
}

NewOrClosedProcesses DataDisplay::GetNewOrClosedProcesses(
		const std::list<Process> &retreived_processes,
		const std::unordered_set<Process> &previous_processes) {

	std::vector<Process> new_processes;
	std::unordered_set<Process> previous_processes_copy = previous_processes;

	new_processes.reserve(previous_processes.size());

	for(const auto& process : retreived_processes) {
		if(!previous_processes_copy.contains(process)) {
			new_processes.push_back(process);
		}
		else {
			previous_processes_copy.erase(process);
		}

	}

	return {new_processes, previous_processes_copy};
}

