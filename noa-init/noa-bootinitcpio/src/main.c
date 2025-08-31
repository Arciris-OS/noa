#include <asm-generic/errno-base.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <linux/limits.h>

#include "message.h"
#include "kutil.h"
#include "mount_dirs.h"
#include "switch_root.h"
#include <sys/stat.h>
#include <sys/mount.h>

#define VERSION "1.0"

int swroot() {
	const char *device = "/rootfs.squashfs";
	const char *mountpoint = "/mnt/rootfs";
	const char *fstype = "squashfs";
	const char *init = "/sbin/init";
	const char *actual_device = device;

	info(format_string("mkdir %s\n", mountpoint));
	if (mkdir(mountpoint, 0755) != 0 && errno == EEXIST) {
		error(format_string("mkdir failed: %s\n", strerror(errno)));
		return 1;
	}

	if (strncmp(actual_device, "/dev/", 5) != 0) {
		// if file. loopback mount
		if (mount_with_loop(actual_device, mountpoint, fstype) != 0) {
			error(format_string("loop mount failed: %s\n", strerror(errno)));
			return 1;
		}
	} else {
		// if device file. mount normally
		if (mount(actual_device, mountpoint, fstype, 0, NULL) != 0) {
			error(format_string("mount failed: %s\n", strerror(errno)));
			return 1;
		}
	}
	info(format_string("Mounted device %s -> %s\n", actual_device, mountpoint));

	info(format_string("switching to rootfs....\n"));

	if (switch_root(mountpoint, init) != 0) {
		error(format_string("switch_root failed: %s\n", strerror(errno)));
		umount(mountpoint);
		return 1;
	}

	info("Starting noa-bootstrap...");

	char *init_args[] = {(char*)init, NULL};
	execv(init, init_args);

	error(format_string("execv failed: %s\n", strerror(errno)));
	return 1;
}

int main() {
	printf("\x1b[32;1mnoa-initcpiofs v%s\x1b[0m\n", VERSION);

	info("Mounting directories... ");
	if (!mount_dirs()) {
		error("Mount failed. Enterning to loop...\n");
		while (1) {
			pause();
		}
	}

	if (swroot()) {
		error("swroot() failed. Enterning to initramfs getty...\n");
		while (1) {
			if (system("/usr/bin/getty 38400 tty1 linux")) {
				continue;
			}
		}
	}

	return 0;
}
