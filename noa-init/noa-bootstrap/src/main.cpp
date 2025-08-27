#include <cstdlib>
#include <iostream>
#include "mount_dirs.hpp"
#include <sys/types.h>
#include <unistd.h>


bool check_pid_and_root() {
	uid_t euid = getuid();
	pid_t pid = getpid();

	if (euid != 0) {
		std::cerr << "Not running in root: don't call noa-bootstrap in userland\n"; 
		std::exit(EXIT_FAILURE);
	}

	if (pid != 1) {
		std::cerr << "Do not run noa-bootstrap in userland\n";
		std::exit(EXIT_FAILURE);
	}

	return true;
}


int main() {
	check_pid_and_root();


	return 1;
}
