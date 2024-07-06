#ifndef __DRIVER_H__
#define __DRIVER_H__

#define MY_IOCTL_PRINT0		_IOC(_IOC_NONE, 'k', 1, 0)
#define MY_IOCTL_PRINT1		_IOC(_IOC_NONE, 'k', 2, 0)
#define MY_IOCTL_PRINT2		_IOC(_IOC_NONE, 'k', 3, 0)
#define MY_IOCTL_INC_0		_IOC(_IOC_NONE, 'k', 4, 0)
#define MY_IOCTL_INC_1		_IOC(_IOC_NONE, 'k', 5, 0)
#define MY_IOCTL_INC_2		_IOC(_IOC_NONE, 'k', 6, 0)
#define MY_IOCTL_INC_3		_IOC(_IOC_NONE, 'k', 7, 0)
#define MY_IOCTL_INC_4		_IOC(_IOC_NONE, 'k', 8, 0)
#define MY_IOCTL_INC_ALL	_IOC(_IOC_NONE, 'k', 9, 0)
#define MY_IOCTL_INT		_IOC(_IOC_NONE, 'k', 10, 0)
#define MY_IOCTL_SUM		_IOC(_IOC_NONE, 'k', 11, 0)

#define MY_IOCTL_RESET		_IOC(_IOC_NONE, 'k', 12, 0)
/* TODO: if you need synchronization, define new ioctl cmd for lock/unlock */
#endif
