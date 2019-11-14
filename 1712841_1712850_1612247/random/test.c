#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

int main(void)
{
    int stt, randomNumber, fd;

    printf("Starting random number\n");

    fd = open("/dev/source", O_RDONLY);
    if (fd < 0){
        perror("Failed to open random device\n");
        return errno;
    }

    stt = read(fd, &randomNumber, sizeof(randomNumber));
    if (stt < 0){
        perror("Failed to read random device\n");
        return errno;
    }

    printf("Random number is: %d\n",randomNumber);

    printf("End of the program! See you soon!\n");
    return 0;
}
