#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>


#define DEV_PATH "/dev/mycdev"
#define NUM_TRY 5

int main(){
    int fd;
    char buf[100];

    fd = open(DEV_PATH, O_RDWR);

    if (fd < 0){
	perror("Open");
    }

    for (int i = 0 ; i < NUM_TRY ; i ++){
	write(fd, "Hello", 6);
	read(fd, buf, 6);
    }

    return 0;

}
