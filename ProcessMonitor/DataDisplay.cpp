//
// Created by lior on 7/7/24.
//

#include "DataDisplay.h"

#include <future>
#include <iostream>
#include <thread>
#include <unistd.h>

DataDisplay::DataDisplay(unsigned int refresh_delay)
	: refresh_delay_(refresh_delay), keep_display_(true) , process_view_shift_(100){
	initscr();

	//	initialize the windows for the boxes
	input_box_window_ = newwin(kInputBoxHeight, kBoxWidth, kInputBoxPos, 0);
	notification_box_window_ = newwin(kNotificationBoxHeight, kBoxWidth, kNotificationBoxPos, 0);
	process_box_window_ = newwin(kProcessBoxHeight, kBoxWidth, kProcessBoxPos, 0);

	refresh();
	RunDisplay();
}

DataDisplay::~DataDisplay() {
	//	delete the memory of the windows
	delwin(input_box_window_);
	delwin(notification_box_window_);
	delwin(process_box_window_);

	endwin();
}

void DataDisplay::RunDisplay() {

	RetreiveAndShowProcessesThread();
}

void DataDisplay::RetreiveAndShowProcessesThread() {
	while(keep_display_) {
		std::thread notificationThread;
		retreived_processes_ = retreiver_.GetRunningProcesses();	//	get the new processes

		notificationThread = std::thread(&DataDisplay::ShowNotifications, this);
		//show the processesw

		InitProcessBox();

		for(int i = process_view_shift_, row = 2; row <= kProcViewLimit; row++, i++) {
			auto& proc = retreived_processes_.at(i);
			mvwprintw(process_box_window_, row, kPidPos, proc.PID.c_str());
			mvwprintw(process_box_window_, row, kNamePos, proc.name.c_str());
			mvwprintw(process_box_window_, row, kCpuPos, "%.2f%%", proc.CPU_Usage);
			mvwprintw(process_box_window_, row, kMemPos, "%.2f%%", proc.memUsage);
		}
		{
			std::lock_guard lock(screen_init_mtx_);
			wrefresh(process_box_window_);
		}

		sleep(refresh_delay_);
		notificationThread.join();
	}
}

void DataDisplay::ShowNotifications() {
	std::unordered_set<Process> previous_processes(retreived_processes_.begin(), retreived_processes_.end());	//	save the previous processes to track changes
	InitNotificationBox();
	if(previous_processes.empty())
		return;

	NewOrClosedProcesses update = GetNewOrClosedProcesses(retreived_processes_, previous_processes);

	if(update.closed_processes.empty() && update.new_processes.empty())
		return;

}

void DataDisplay::InitProcessBox() {
	werase(process_box_window_);
	box(process_box_window_, 0, 0);
	mvwprintw(process_box_window_, 1, kPidPos, "PID");
	mvwprintw(process_box_window_, 1, kNamePos, "NAME");
	mvwprintw(process_box_window_, 1, kCpuPos, "CPU%%");
	mvwprintw(process_box_window_, 1, kMemPos, "MEMORY%%");
	{
		std::lock_guard lock(screen_init_mtx_);
		wrefresh(process_box_window_);
	}
}

void DataDisplay::InitNotificationBox() {
	werase(notification_box_window_);
	box(notification_box_window_, 0, 0);
	{
		std::lock_guard lock(screen_init_mtx_);
		wrefresh(notification_box_window_);
	}
}

NewOrClosedProcesses DataDisplay::GetNewOrClosedProcesses(
		const std::vector<Process> &retreived_processes,
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

