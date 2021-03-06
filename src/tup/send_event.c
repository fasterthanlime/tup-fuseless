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

#define _GNU_SOURCE
#include "tup/access_event.h"
#include "tup/flock.h"
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include <dlfcn.h>

static int write_all(int sd, const void *buf, size_t len);

#define MAX_MESSAGE_SIZE PIPE_BUF

void tup_send_event(const char *file, int len, const char *file2, int len2, int at)
{
	static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	static int tupsd;
	static int lockfd;
	static char buf[MAX_MESSAGE_SIZE];
	struct access_event event;

	if (pthread_mutex_lock(&mutex) < 0) {
		fprintf(stderr, "tup internal error: could not lock mutex\n");
		exit(1);
	}

	if(!file) {
		fprintf(stderr, "tup internal error: file can't be NUL\n");
		exit(1);
	}
	if(!file2) {
		fprintf(stderr, "tup internal error: file2 can't be NUL\n");
		exit(1);
	}
	if(!lockfd) {
		char *path;

		path = getenv(TUP_LOCK_NAME);
		if(!path) {
			fprintf(stderr, "tup: Unable to get '%s' "
				"path from the environment.\n", TUP_LOCK_NAME);
			exit(1);
		}
		lockfd = strtol(path, NULL, 0);
		if(lockfd <= 0) {
			fprintf(stderr, "tup: Unable to get valid file lock.\n");
			exit(1);
		}
	}

	if(!tupsd) {
		char *path;

		path = getenv(TUP_SERVER_NAME);
		if(!path) {
			fprintf(stderr, "tup: Unable to get '%s' "
				"path from the environment.\n", TUP_SERVER_NAME);
			exit(1);
		}

		tupsd = strtol(path, NULL, 0);
		if(tupsd <= 0) {
			fprintf(stderr, "tup: Unable to get valid socket descriptor.\n");
			exit(1);
		}
	}

	event.at = at;
	event.len = len;
	event.len2 = len2;

	char *cur = buf;
	int buflen = 0;

  int	final_len = sizeof(event) + event.len + event.len2;
	if (final_len >= sizeof(buf)) {
		fprintf(stderr, "cannot send event about %s (too large)\n", file);
	} else {
		memcpy(cur + buflen, &event, sizeof(event));
		buflen += sizeof(event);

		memcpy(cur + buflen, file, event.len);
		buflen += event.len;

		memcpy(cur + buflen, file2, event.len2);
		buflen += event.len2;

		if(tup_flock2(lockfd) < 0) {
			perror("flock(lockfd)");
			exit(1);
		}

		if(write_all(tupsd, buf, buflen) < 0) {
			perror("write_all(buf)");
			exit(1);
		}

		if(tup_unflock2(lockfd) < 0) {
			perror("tup_unflock(lockfd)");
			exit(1);
		}
	}

	if (pthread_mutex_unlock(&mutex) < 0) {
		fprintf(stderr, "tup internal error: could not lock mutex\n");
		exit(1);
	}
}

static int write_all(int sd, const void *buf, size_t len)
{
	size_t sent = 0;
	const char *cur = buf;

	while(sent < len) {
		int rc;
		rc = write(sd, cur + sent, len - sent);
		if(rc < 0) {
			perror("write");
			return -1;
		}
		sent += rc;
	}
	return 0;
}
