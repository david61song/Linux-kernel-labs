/* Direct System Call example code 
 * syscall (SYSCALL_NUMBER, FILEDES, MSG, BYTES_TO_WRITE); // write() syscall
 */

#include <unistd.h>
#include <sys/syscall.h>

int main(void){
    ssize_t bytes_written;

    bytes_written = syscall(1, 1, "Hello, World!", 15);
    return 0;
}
