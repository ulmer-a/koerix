// SPDX-FileCopyrightText: 2005-2020 Rich Felker, et al.
// SPDX-License-Identifier: MIT

#include <sys/ioctl.h>
#include <stdarg.h>
#include <errno.h>

int _ioctl(int fd, size_t req, void* arg);

int ioctl(int fd, size_t req, ...)
{
	void *arg;
	va_list ap;
	va_start(ap, req);
	arg = va_arg(ap, void *);
	va_end(ap);

	int r = _ioctl(fd, req, arg);
	/*if (SIOCGSTAMP != SIOCGSTAMP_OLD && req && r==-ENOTTY) {
		for (int i=0; i<sizeof compat_map/sizeof *compat_map; i++) {
			if (compat_map[i].new_req != req) continue;
			union {
				long long align;
				char buf[256];
			} u;
			convert_ioctl_struct(&compat_map[i], u.buf, arg, W);
			r = __syscall(SYS_ioctl, fd, compat_map[i].old_req, u.buf);
			if (r<0) break;
			convert_ioctl_struct(&compat_map[i], u.buf, arg, R);
			break;
		}
	}
	return __syscall_ret(r);*/
    return r;
}
