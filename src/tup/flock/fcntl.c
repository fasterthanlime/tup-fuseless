/* vim: set ts=8 sw=8 sts=8 noet tw=78:
 *
 * tup - A file-based build system
 *
 * Copyright (C) 2011-2017  Mike Shal <marfey@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#define _ATFILE_SOURCE
#define _GNU_SOURCE
#include "tup/flock.h"
#include "tup/config.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_BG_RED        "\x1b[41m"
#define ANSI_BG_GREEN      "\x1b[42m"
#define ANSI_BG_YELLOW     "\x1b[43m"
#define ANSI_BG_BLUE       "\x1b[44m"
#define ANSI_BG_MAGENTA    "\x1b[45m"
#define ANSI_BG_CYAN       "\x1b[46m"
#define NUM_COLORS 12

#define ANSI_COLOR_RESET   "\x1b[0m"

const char *escape_start() {
	int colornum = getpid() % 6;
	switch (colornum) {
		case 0:  return ANSI_COLOR_RED;
		case 1:  return ANSI_COLOR_GREEN;
		case 2:  return ANSI_COLOR_YELLOW;
		case 3:  return ANSI_COLOR_BLUE;
		case 4:  return ANSI_COLOR_MAGENTA;
		case 5:  return ANSI_COLOR_CYAN;
		case 6:  return ANSI_BG_RED;
		case 7:  return ANSI_BG_GREEN;
		case 8:  return ANSI_BG_YELLOW;
		case 9:  return ANSI_BG_BLUE;
		case 10: return ANSI_BG_MAGENTA;
		case 11: return ANSI_BG_CYAN;
	}
}

const char *escape_end() {
	return ANSI_COLOR_RESET;
}

int tup_lock_open(const char *lockname, tup_lock_t *lock)
{
	int fd;

	fd = openat(tup_top_fd(), lockname, O_RDWR | O_CREAT, 0666);
	if(fd < 0) {
		perror(lockname);
		fprintf(stderr, "tup error: Unable to open lockfile.\n");
		return -1;
	}
	*lock = fd;
	return 0;
}

void tup_lock_close(tup_lock_t lock)
{
	if(close(lock) < 0) {
		perror("close(lock)");
	}
}

int tup_flock(tup_lock_t fd)
{
	if(flock(fd, LOCK_EX) < 0) {
		return -1;
	}
	return 0;
}

/* Returns: -1 error, 0 got lock, 1 would block */
int tup_try_flock(tup_lock_t fd)
{
	if(flock(fd, LOCK_EX|LOCK_NB) < 0) {
		if (errno == EWOULDBLOCK)
			return 1;
		perror("tup_try_flock");
		return -1;
	}
}

int tup_unflock(tup_lock_t fd)
{
	if(flock(fd, LOCK_UN) < 0) {
		return -1;
	}
	return 0;
}

int tup_wait_flock(tup_lock_t fd)
{
	while(1) {
		if(flock(fd, LOCK_EX|LOCK_NB) < 0) {
			if (errno == EWOULDBLOCK) {
				usleep(10000);
				continue;
			}
			perror("tup_wait_flock");
			return -1;
		}

		break;
	}
	return 0;
}

int tup_flock2(int fd)
{
	if(flock(fd, LOCK_EX) < 0) {
		perror("flock LOCK_EX)");
		return -1;
	}
	return 0;
}

int tup_unflock2(int fd)
{
	if(flock(fd, LOCK_UN) < 0) {
		perror("flock LOCK_UN)");
		return -1;
	}
	return 0;
}