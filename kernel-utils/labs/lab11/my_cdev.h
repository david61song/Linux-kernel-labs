#ifndef __PSOCDEV_H__
#define __PSOCDEV_H__		1

#include <asm/ioctl.h>

#define BUFFER_SIZE		256

#define MODULE_NAME		"mycdev"
#define MESSAGE			"hello\n"
#define IOCTL_MESSAGE		"Hello ioctl"

/* for ioctl */

#define MY_IOCTL_PRINT		_IOC(_IOC_NONE, 'k', 1, 0)
#define MY_IOCTL_SET_BUFFER	_IOC(_IOC_WRITE, 'k', 2, BUFFER_SIZE)
#define MY_IOCTL_GET_BUFFER	_IOC(_IOC_READ, 'k', 3, BUFFER_SIZE)



#endif
