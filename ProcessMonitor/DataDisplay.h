//
// Created by lior on 7/7/24.
//st

#ifndef DATADISPLAY_H
#define DATADISPLAY_H

#include <ncurses.h>
#include <unordered_set>
#include <atomic>
#include <map>
#include <shared_mutex>

#include "ProcessRetriever.h"

enum class KEYS {
	MOVE_UP = KEY_UP,
	MOVE_DOWN = KEY_DOWN,
	SORT_PID = 'p',
	SORT_CPU = 'c',
	SORT_MEM = 'm',
	SORT_NAME = 'n',
	SORT_DEC = 'd',
	SORT_ACCEND = 'a',
	CHANGE_REFRESH = 'r',
	QUIT = 'q'
};


class DataDisplay {
public:
	explicit DataDisplay(unsigned int refresh_delay);
	~DataDisplay();


private:
	//	box dimensions
	auto static constexpr kBoxWidth = 70;
	auto static constexpr kInputBoxHeight = 6;
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
	void SortProcesses(KEYS sortOption, bool isDecending);
	void ChangeRefresh();

	void InitProcessBox();
	void InitHelpBox();

	void IncViewShift(int shift);
	int GetViewShift();

	std::vector<Process> retreived_processes_;
	ProcessRetreiver retreiver_;

	WINDOW* help_box_window_;
	WINDOW* process_box_window_;

	std::mutex screen_init_mtx_;
	std::mutex process_view_shift_mtx_;
	std::mutex process_sort_mutex_;

	KEYS sort_option_;
	bool isDec_;
	int process_view_shift_;

	unsigned int refresh_delay_;
	std::atomic<bool> keep_display_;
};



#endif //DATADISPLAY_H
