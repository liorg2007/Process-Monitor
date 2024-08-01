//
// Created by lior on 7/7/24.
//

#include "DataDisplay.h"

#include <future>
#include <iostream>
#include <thread>
#include <unistd.h>

DataDisplay::DataDisplay(unsigned int refresh_delay)
	: refresh_delay_(refresh_delay), keep_display_(true) , process_view_shift_(0), sort_option_(KEYS::SORT_CPU), isDec_(true) {
	initscr();
	noecho();
	curs_set(0);
	//	initialize the windows for the boxes
	help_box_window_ = newwin(kInputBoxHeight, kBoxWidth, kInputBoxPos, 0);
	process_box_window_ = newwin(kProcessBoxHeight, kBoxWidth, kProcessBoxPos, 0);

	nodelay(process_box_window_, TRUE);

	leaveok(process_box_window_, TRUE);
	leaveok(help_box_window_, TRUE);


	refresh();
	RunDisplay();
}

DataDisplay::~DataDisplay() {
	//	delete the memory of the windows
	delwin(help_box_window_);
	delwin(process_box_window_);

	endwin();
}

void DataDisplay::RunDisplay() {
	std::thread t(&DataDisplay::ProcessUserInput, this);
	RetreiveAndShowProcessesThread();
	t.join();
}

void DataDisplay::ProcessUserInput() {
	int c;
	InitHelpBox();

	keypad(help_box_window_, true);

	do {
		c = wgetch(help_box_window_);

		switch(c) {
			case static_cast<int>(KEYS::MOVE_DOWN):
				IncViewShift(10);
				break;
			case static_cast<int>(KEYS::MOVE_UP):
				IncViewShift(-10);
				break;
			case static_cast<int>(KEYS::SORT_CPU):
			case static_cast<int>(KEYS::SORT_MEM):
			case static_cast<int>(KEYS::SORT_PID):
			case static_cast<int>(KEYS::SORT_NAME): {
				std::lock_guard lock(process_sort_mutex_);
				sort_option_ = static_cast<KEYS>(c);
			}
			break;
			case static_cast<int>(KEYS::SORT_DEC):
			case static_cast<int>(KEYS::SORT_ACCEND): {
				std::lock_guard lock(process_sort_mutex_);
				isDec_ = static_cast<KEYS>(c) == KEYS::SORT_DEC;
			}
			break;
			case static_cast<int>(KEYS::CHANGE_REFRESH):
				beep();
				ChangeRefresh();
			case static_cast<int>(KEYS::QUIT):
				break;
		}

	} while(c != static_cast<int>(KEYS::QUIT));
	keep_display_ = false;
}

void DataDisplay::RetreiveAndShowProcessesThread() {
	while(keep_display_) {
		std::thread notificationThread;
		retreived_processes_ = retreiver_.GetRunningProcesses();	//	get the new processes

		//show the processesw

		InitProcessBox();

		int vecSize = retreived_processes_.size();
		int viewShift = GetViewShift();
		KEYS sortOption;
		bool isDecend;

		{
			std::lock_guard lock(process_sort_mutex_);
			sortOption = sort_option_;
			isDecend = isDec_;
		}

		SortProcesses(sortOption, isDecend);

		for(int i = 0, row = 2; row <= kProcViewLimit && i < vecSize; row++, i++) {
			auto& proc = retreived_processes_.at((viewShift + i) % vecSize);
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
	}
}

void DataDisplay::SortProcesses(KEYS sortOption, bool isDecending) {
	std::function<bool(const Process&, const Process&)>  comparator;

	switch (sortOption) {
		case KEYS::SORT_CPU: // Sort by 'value'
			comparator = [](const Process &a, const Process &b) {
				return a.CPU_Usage < b.CPU_Usage;
		};
		break;
		case KEYS::SORT_NAME: // Sort by 'name'
			comparator = [](const Process &a, const Process &b) {
				return a.name < b.name;
		};
		break;
		case KEYS::SORT_PID: // Sort by 'score'
			comparator = [](const Process &a, const Process &b) {
				return stoi(a.PID) < stoi(b.PID);
		};
		break;
		case KEYS::SORT_MEM: // Sort by 'score'
			comparator = [](const Process &a, const Process &b) {
				return a.memUsage < b.memUsage;
		};

	}

	if (isDecending) {
		std::ranges::sort(retreived_processes_, [comparator](const Process &a, const Process &b) {
				return comparator(b, a); // Reverse the comparator
		});
	} else {
		std::ranges::sort(retreived_processes_, comparator);
	}
}

void DataDisplay::ChangeRefresh() {
	std::string printingString = "Enter new refresh time(seconds): ";
	wattron(help_box_window_, A_REVERSE);
	mvwprintw(help_box_window_, 4, 1, printingString.c_str());
	{
		std::lock_guard lock(screen_init_mtx_);
		wrefresh(help_box_window_);
	}
	int refresh = 0;
	int c;

	do {
		c = wgetch(help_box_window_);

		if(c == KEY_BACKSPACE && refresh > 0) {
			beep();
			refresh /= 10;
		}
		else if(c >= '0' && c <= '9')
			refresh = refresh*10 + c - '0';
		else
			continue;

		winsdelln(help_box_window_, 4);
		mvwprintw(help_box_window_, 4, 1, (printingString + std::to_string(refresh)).c_str());
		{
			std::lock_guard lock(screen_init_mtx_);
			wrefresh(help_box_window_);
		}

	}while (c != '\n');
	beep();

	wattroff(help_box_window_, A_REVERSE);
	InitHelpBox();
	refresh_delay_ = refresh == 0? refresh_delay_ : refresh;
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

void DataDisplay::InitHelpBox() {
	werase(help_box_window_);
	box(help_box_window_, 0, 0);

	wmove(help_box_window_, 4, 1);
	mvwprintw(help_box_window_, 1, 1, "Move view: arrow up/down");
	mvwprintw(help_box_window_, 2, 1, "Sort: p-PID c-CPU%% m-MEM%% n-NAME a-Ascending d-Descending");
	mvwprintw(help_box_window_, 3, 1, "Change refresh rate: r");
	mvwprintw(help_box_window_, 4, 1, "Quit - q");

	{
		std::lock_guard lock(screen_init_mtx_);
		wrefresh(help_box_window_);
	}
}

void DataDisplay::IncViewShift(int shift) {
	std::lock_guard lock(process_view_shift_mtx_);
	process_view_shift_ += shift;

	if(process_view_shift_ < 0)
		process_view_shift_ = 0;
}

int DataDisplay::GetViewShift() {
	std::lock_guard lock(process_view_shift_mtx_);
	return process_view_shift_;
}

