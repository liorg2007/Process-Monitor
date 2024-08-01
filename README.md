## Process Monitor - A C++ ncurses application

This project is a process monitor written in C++ for Linux systems. It retrieves information about running processes from the `/proc` directory and displays it on the screen using the ncurses library.

### Features

* Retrieves process information from `/proc`
* Displays processes in a user-friendly interface using ncurses
* Two threads:
    * One for user input (sorting, navigation)
    * One for displaying processes
* User can sort processes by various criteria (PID, CPU usage, memory usage, name)
* User can traverse the process list

### Dependencies

* cmake 3.25 and higher
[Get the most recent version of cmake, follow the first answer in method 'A'](https://askubuntu.com/questions/355565/how-do-i-install-the-latest-version-of-cmake-from-the-command-line)
* ncurses library (`libncurses5-dev` on Debian/Ubuntu)

```bash
sudo apt-get install libncurses5-dev libncursesw5-dev
```
* A c++ compiler
```bash
sudo apt update && sudo apt install build-essential
```

### Building

1. Clone this repository or download the source code.
2. Make sure you have the required dependencies installed.
3. Create a build directory 
``` bash
cd ProcessMonitor
mkdir build
```
4. Change directory to the build
``` bash
cd build
```
5. Build the project
``` bash
cmake ..
make
```

### Running

1. Open a terminal window.
2. Navigate to the directory `/ProcessMonitor/build`
3. Run the program:

```bash
./ProcessMonitor
```

### Usage

The application displays a list of running processes. You can interact with the list using the following keys:

* Arrow keys (Up/Down): Navigate through the process list
* `p`: Sort processes by PID
* `m`: Sort processes by memory usage
* `c`: Sort processes by CPU usage
* `n`: Sort processes by name
* `d`: Sort processes in descending order
* `a`: Sort processes in ascending order
* `r`: Edit the refresh rate
* `q`: Quit the application

