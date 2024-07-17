//
// Created by lior on 7/7/24.
//

#include "DataDisplay.h"

#include <iostream>
#include <unistd.h>

DataDisplay::DataDisplay() {
	initscr();

	//	initialize the windows for the boxes
	input_box_window_ = newwin(kInputBoxHeight, kBoxWidth, kInputBoxPos, 0);
	notification_box_window_ = newwin(kNotificationBoxHeight, kBoxWidth, kNotificationBoxPos, 0);
	process_box_window_ = newwin(kProcessBoxHeight, kBoxWidth, kProcessBoxPos, 0);

	refresh();
}

DataDisplay::~DataDisplay() {
	//	delete the memory of the windows
	delwin(input_box_window_);
	delwin(notification_box_window_);
	delwin(process_box_window_);

	endwin();
}

void DataDisplay::DisplayProcceses() {

}

