#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>


#define PWRLOSS_MAGIC		's'
#define PWRLOSS_READ		_IOR(PWRLOSS_MAGIC, 2, int)

int main() {

	int fd;
	int power = 234;
    int retval = 64;

	fd = open("/dev/pwrloss_device", O_RDWR);
	if(retval = ioctl(fd, PWRLOSS_READ, (int32_t *) &power) < 0) {
        printf("ioctl failed and errno is: %s\n", strerror(retval));
    }
	
    printf("Value is: %d\n", power);
}
