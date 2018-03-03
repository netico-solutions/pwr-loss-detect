#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>


#define PWRLOSS_MAGIC		's'
#define PWRLOSS_NOTIFY		_IOW(PWRLOSS_MAGIC, 2, int)

int main() {

	int fd;
	int power;

	fd = open("/dev/pwrloss_device");
	printf("entered while loop\n");
    while(1) {
	
		while(sleep(10000)) {

		    ioctl(fd, PWRLOSS_NOTIFY, (int32_t *) &power);
		    printf("Value is: %d", power);
		}
	
	}	
}
