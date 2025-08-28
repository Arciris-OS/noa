#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "message.h"
#include "mount_dirs.h"

bool check_pid_and_root() {
	uid_t euid = getuid();
	pid_t pid = getpid();

	if (euid != 0) {
		error("Not running in root: don't call noa-bootstrap in userland\n");
		exit(EXIT_FAILURE);
	}

	if (pid != 1) {
		error("Do not run noa-bootstrap in userland\n");
		exit(EXIT_FAILURE);
	}

	return true;
}


int main() {
	/* if (check_pid_and_root()) */ 
	/* 	return 1; */

	info("Mounting directories...\n");
	if (!mount_dirs()) {
		error("Mount failed. Enterning to loop...\n");
		while (1) {
			pause();
		}

	}


	info("Enterning to Debug loop...\n");
	while (1) {
		pause();
	}

	return 0;
}
