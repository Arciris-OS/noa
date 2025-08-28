#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/mount.h>

#include "mount_dirs.h"
#include "message.h"
#include "kutil.h"


bool mount_dirs() {
	// mount proc
	if (mount("proc", "/proc", "proc", 0, NULL) == -1) {
		error(format_string("Failed to mount /proc: %s\n", strerror(errno)));
		return false;
	}

	if (mount("sysfs", "/sys", "sys", 0, NULL) == -1) {
		error(format_string("Failed to mount /mount: %s\n", strerror(errno)));
		return false;
	}

	if (mount("udev", "/dev", "devtmpfs", 0, NULL) == -1) {
		error(format_string("Failed to mount /dev: %s\n", strerror(errno)));
		return false;
	}

	return true;
}
