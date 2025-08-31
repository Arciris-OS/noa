#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/mount.h>
#include <unistd.h>

#include "mount_dirs.h"
#include "message.h"
#include "kutil.h"


bool mount_dirs() {
	// mount proc
	fflush(stdout);
	if (mount("proc", "/proc", "proc", 0, NULL) == -1) {
		error(format_string("Failed to mount /proc: %s\n", strerror(errno)));
		return false;
	}
	printf("/proc ");

	fflush(stdout);
	if (mount("sys", "/sys", "sysfs", 0, NULL) == -1) {
		error(format_string("\nFailed to mount /sys: %s\n", strerror(errno)));
		return false;
	}
	printf("/sys ");

	fflush(stdout);
	if (mount("udev", "/dev", "devtmpfs", 0, NULL) == -1) {
		error(format_string("\nFailed to mount /dev: %s\n", strerror(errno)));
		return false;
	}
	printf("/dev\n");

	return true;
}
