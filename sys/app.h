#ifndef PWRLOSS_APP
#define PWRLOSS_APP

#include <sys/ioctl.h>


#define PWRLOSS_MAGIC		's'
#define PWRLOSS_READ        _IOR(PWRLOSS_MAGIC, 3, int)

#endif
