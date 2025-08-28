#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "message.h"
#include "mount_dirs.h"

#define VERSION "1.0"

int main() {
	printf("\x1b[32;1mnoa-initcpiofs v%s\x1b[0m\n", VERSION);

	info("Mounting directories...\n");
	if (!mount_dirs()) {
		error("Mount failed. Enterning to loop...\n");
		while (1) {
			pause();
		}
	}


	info("Executing getty...\n");
	while (1) {
		if (system("/usr/bin/getty 38400 tty1 linux") == -1) {
			error("Bash reserve error: %s", strerror(errno));
			while (1) {
				pause();
			}
		}
	}

	return 0;
}
