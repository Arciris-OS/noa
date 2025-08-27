#include <cstring>
#include <iostream>
#include <cerrno>
#include <ostream>
#include <sys/mount.h>

#include "mount_dirs.hpp"


bool Mounts::mount_dirs() {
	// mount proc
	if (mount("proc", "/proc", "proc", 0, NULL) == -1) {
		std::cerr << "Failed to mount /proc: " 
			<< std::strerror(errno) 
			<< std::endl;
		return false;
	}

	if (mount("sysfs", "/sys", "sys", 0, NULL) == -1) {
		std::cerr << "Failed to mount /sys: " 
			<< std::strerror(errno) 
			<< std::endl;
		return false;
	}

	if (mount("udev", "/dev", "devtmpfs", 0, NULL) == -1) {
		std::cerr << "Failed to mount /dev: " 
			<< std::strerror(errno) 
			<< std::endl;
		return false;
	}

	return true;
}
