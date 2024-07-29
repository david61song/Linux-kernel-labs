#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define FILENAME "/root/sample.txt"
#define BUFFER_SIZE 256

int main() {
    int fd;
    ssize_t read_size;
    char buffer[BUFFER_SIZE];
    const char *write_string = "Raspbian Linux!\n";

    // Open file
    fd = open(FILENAME, O_RDWR);
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    // Read and print initial content
    read_size = read(fd, buffer, BUFFER_SIZE);
    if (read_size > 0) {
        printf("Initial content:\n%s", buffer);
    }

    // Write new content
    if (write(fd, write_string, strlen(write_string)) == -1) {
        perror("Error writing to file");
        close(fd);
        return 1;
    }

    // Reset file position to beginning
    if (lseek(fd, 0, SEEK_SET) == -1) {
        perror("Error resetting file position");
        close(fd);
        return 1;
    }

    // Read and print updated content
    memset(buffer, 0, BUFFER_SIZE);
    read_size = read(fd, buffer, BUFFER_SIZE);
    if (read_size > 0) {
        printf("\nUpdated content:\n%s", buffer);
    }

    // Close file
    close(fd);

    return 0;
}
