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
	~DataDisplay();

	void DisplayProcceses();

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

	std::vector<Process> retreived_processes_;
	ProcessRetreiver retreiver_;
	WINDOW* input_box_window_;
	WINDOW* process_box_window_;
	WINDOW* notification_box_window_;
};



#endif //DATADISPLAY_H
