#ifndef NETICO_PWRLOSS_IOCTL_
#define NETICO_PWRLOSS_IOCTL_

#include <linux/ioctl.h>

#define PWRLOSS_NAME        "pwr-loss-gpio"

#define PWRLOSS_MAGIC       's'

#define PWRLOSS_GET_PID     _IOW(PWRLOSS_MAGIC, 1, signed long long)

#define PWRLOSS_NOTIFY      _IOR(PWRLOSS_MAGIC, 2, int)

#endif /* NETICO_PWRLOSS_IOCTL_ */
