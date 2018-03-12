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
#define MAX_RETRY_COUNT     6
#define POWER_ON            1
#define POWER_OFF           0


int send_def_value_to_driver(int power);
int recv_value_from_driver(void);


/*int send_def_value_to_driver(power) {
    int fd;
    int retval;

	fd = open("/dev/pwrloss_device", O_WRONLY);
    
    if(retval = ioctl(fd, PWRLOSS_WRITE, (int32_t *) &power) < 0) {
        printf("ioctl failed and errno is: %s\n", strerror(retval));
    }
}*/

int recv_value_from_driver() {
	int fd;
	int power;
    int retval;

	fd = open("/dev/pwrloss_device", O_RDONLY);
	
    if(retval = ioctl(fd, PWRLOSS_READ, (int32_t *) &power) < 0) {
        printf("ioctl failed and errno is: %s\n", strerror(retval));
    }
    printf("Power reading is: %d\n", &power);
}

int main() {
    int retry_count = 0;
    
    while(1) {
        recv_value_from_driver();
        sleep(1);
    }
}
