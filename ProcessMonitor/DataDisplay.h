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



class DataDisplay {
public:
	explicit DataDisplay(unsigned int refresh_delay);
	~DataDisplay();


private:
	//	box dimensions
	auto static constexpr kBoxWidth = 70;
	auto static constexpr kInputBoxHeight = 3;
	auto static constexpr kProcessBoxHeight = 52;

	//	box positions
	auto static constexpr kInputBoxPos = 0;
	auto static constexpr kProcessBoxPos = 6;

	//	process box positions
	auto static constexpr kPidPos = 1;
	auto static constexpr kNamePos = 8;
	auto static constexpr kCpuPos = 48;
	auto static constexpr kMemPos = 57;

	auto static constexpr kProcViewLimit = 50;

	void RunDisplay();

	void ProcessUserInput();
	void RetreiveAndShowProcessesThread();

	void InitProcessBox();
	void InitInputBox();

	void IncViewShift(int shift);
	int GetViewShift();

	std::vector<Process> retreived_processes_;
	ProcessRetreiver retreiver_;

	WINDOW* input_box_window_;
	WINDOW* process_box_window_;

	std::mutex screen_init_mtx_;
	std::mutex process_view_shift_mtx_;


	int process_view_shift_;

	unsigned int refresh_delay_;
	std::atomic<bool> keep_display_;
};



#endif //DATADISPLAY_H
