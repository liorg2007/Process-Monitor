//
// Created by lior on 7/7/24.
//st

#ifndef DATADISPLAY_H
#define DATADISPLAY_H

#include <ncurses.h>
#include <unordered_set>
#include <atomic>
#include <shared_mutex>

#include "ProcessRetriever.h"

struct NewOrClosedProcesses {
	std::vector<Process> closed_processes;
	std::vector<Process> new_processes;

	NewOrClosedProcesses(std::vector<Process>& new_processes, const std::unordered_set<Process>& closed_processes_set)
		:	new_processes(std::move(new_processes)){
		closed_processes.reserve(closed_processes_set.size());
		for(auto& process : closed_processes_set)
			closed_processes.push_back(process);
	}
};


class DataDisplay {
public:
	explicit DataDisplay(unsigned int refresh_delay);
	~DataDisplay();


private:
	//	box dimensions
	auto static constexpr kBoxWidth = 70;
	auto static constexpr kInputBoxHeight = 3;
	auto static constexpr kProcessBoxHeight = 52;
	auto static constexpr kNotificationBoxHeight = 3;

	//	box positions
	auto static constexpr kInputBoxPos = 0;
	auto static constexpr kNotificationBoxPos = 3;
	auto static constexpr kProcessBoxPos = 6;

	//	process box positions
	auto static constexpr kPidPos = 1;
	auto static constexpr kNamePos = 6;
	auto static constexpr kCpuPos = 46;
	auto static constexpr kMemPos = 55;

	auto static constexpr kProcViewLimit = 50;

	void RunDisplay();

	void RetreiveAndShowProcessesThread();
	void ShowNotifications();

	void InitProcessBox();
	void InitNotificationBox();

	static NewOrClosedProcesses GetNewOrClosedProcesses(const std::vector<Process>& retreived_processes, const std::unordered_set<Process>& previous_processes);

	std::vector<Process> retreived_processes_;
	ProcessRetreiver retreiver_;

	WINDOW* input_box_window_;
	WINDOW* process_box_window_;
	WINDOW* notification_box_window_;

	std::mutex screen_init_mtx_;

	int process_view_shift_;

	unsigned int refresh_delay_;
	std::atomic<bool> keep_display_;
};



#endif //DATADISPLAY_H
