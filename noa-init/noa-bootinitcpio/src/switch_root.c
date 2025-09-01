#include "kutil.h"
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/mount.h>
#include <linux/limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <linux/loop.h>
#include <fcntl.h>
#include <dirent.h>

#include "kutil.h"
#include "switch_root.h"
#include "message.h"

// 旧ルートファイルシステムを削除する関数
int delete_contents(const char *directory) {
	DIR *dir;
	struct dirent *d;
	char path[PATH_MAX];
	struct stat statbuf;

	if (!(dir = opendir(directory))) {
		return -1;
	}

	while ((d = readdir(dir))) {
		if (!strcmp(d->d_name, ".") || !strcmp(d->d_name, ".."))
			continue;

		snprintf(path, sizeof(path), "%s/%s", directory, d->d_name);

		if (lstat(path, &statbuf) == -1) {
			continue;
		}

		if (S_ISDIR(statbuf.st_mode)) {
			delete_contents(path);
			rmdir(path);
		} else {
			unlink(path);
		}
	}

	closedir(dir);
	return 0;
}

int mount_with_loop(const char *image_file, const char *mountpoint, const char *fstype) {
	int loop_fd;
	char loop_device[32];
	int loop_num = -1;

	// 利用可能なループデバイスを探す
	info("Checking avalilable loop device... ");
	fflush(stdout);
	struct loop_info64 loopinfo;
	for (int i = 0; i < 16; i++) {
		snprintf(loop_device, sizeof(loop_device), "/dev/loop%d", i);
		loop_fd = open(loop_device, O_RDWR);

		if (loop_fd >= 0) {
			if (ioctl(loop_fd, LOOP_GET_STATUS64, &loopinfo) == -1 && errno == ENXIO) {
				// 未使用
				printf("\x1b[32;1m%s\x1b[0m", loop_device);
				fflush(stdout);
				loop_num = i;
				goto found_loop;
			} else {
				// busyなら開放する
				char filename[PATH_MAX];
				memset(filename, 0, sizeof(filename));
				strncpy(filename, (char*)loopinfo.lo_file_name, sizeof(loopinfo.lo_file_name));
				printf("\x1b[31;1m%s\x1b[0m(%s) ", loop_device, filename);
				fflush(stdout);
				close(loop_fd);
				free_loop_device(loop_device);
			}
		}
	}
	
	found_loop:
		printf("\n");

		if (loop_num == -1) {
			error(format_string("Loop device not found.\n"));
			return -1;
		}

		info(format_string("Using loop device %s\n", loop_device));

		// イメージファイルをオープン
		int image_fd = open(image_file, O_RDONLY);
		if (image_fd < 0) {
			error(format_string("Cannot open image file: %s\n", strerror(errno)));
			close(loop_fd);
			return -1;
		}

		// ループデバイスにイメージファイルを関連付け
		if (ioctl(loop_fd, LOOP_SET_FD, image_fd) < 0) {
			error(format_string("LOOP_SET_FD failed: %s\n", strerror(errno)));
			close(image_fd);
			close(loop_fd);
			return -1;
		}

		close(image_fd);
		close(loop_fd);

		info(format_string("Mounting loop device %s...\n", loop_device));

		if (mount(loop_device, mountpoint, fstype, MS_RDONLY, NULL) != 0) {
			error(format_string("Failed to mount loop device: %s\n", strerror(errno)));

			loop_fd = open(loop_device, O_RDWR);
			if (loop_fd >= 0) {
				ioctl(loop_fd, LOOP_CLR_FD, 0);
				close(loop_fd);
			}
			return -1;
		}

		return 0;
}

int switch_root(const char *newroot, const char *init) {
	struct stat st1, st2;

	// 新しいルートが存在することを確認
	if (stat(newroot, &st1) != 0 || !S_ISDIR(st1.st_mode)) {
		error(format_string("New root not fonud: %s\n", newroot));
		return -1;
	}

	// initプログラムが存在することを確認
	char init_path[PATH_MAX];
	snprintf(init_path, sizeof(init_path), "%s%s", newroot, init);
	if (stat(init_path, &st2) != 0) {
		error(format_string("Init program not found: %s\n", strerror(errno)));
		return -1;
	}

	// 新しいルートと現在のルートが同じデバイス上にないことを確認
	if (st1.st_dev == st2.st_dev) {
		error("The new root must be a different filesystem\n");
		return -1;
	}

	// 新しいルートディレクトリにchdirする
	if (chdir(newroot) != 0) {
		error("chdir failed: %s\n", strerror(errno));
		return -1;
	}

	// 新しいルートを / にマウント
	if (mount(".", "/", NULL, MS_MOVE, NULL) != 0) {
		error("mount MS_MOVE failed: %s\n", strerror(errno));
		return -1;
	}

	// 新しいルートにchrootする
	if (chroot(".") != 0) {
		error("chroot failed: %s\n", strerror(errno));
		return -1;
	}

	// 作業ディレクトリを / に設定
	if (chdir("/") != 0) {
		error("final chdir failed: %s\n", strerror(errno));
		return -1;
	}

	// 旧ルートファイルシステムのマウントポイントを削除
	// (通常は /mnt/root などにマウントされている)

	return 0;
}

void free_loop_device(const char *loopdev) {
    int fd = open(loopdev, O_RDWR);
    if (fd < 0) {
        error("open");
        return;
    }
    if (ioctl(fd, LOOP_CLR_FD, 0) < 0) {
        error("LOOP_CLR_FD");
    } else {
		info(format_string("%s freed\n", loopdev));
    }
    close(fd);
}