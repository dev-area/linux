#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

int h;

void async(int s)
{
        char c;
        int x = 0;
        printf("signal\n"); fflush(stdout);
        /*while (read(h, &c, 1) == 1) x++; */
         read(h, &c, 1);
        printf("read: %c\n", c); /* fflush(stdout); */
        if (fcntl(h, F_SETFL, O_NONBLOCK) < 0) perror("fcntl"), exit(1);
        if (fcntl(h, F_SETFL, O_ASYNC | O_NONBLOCK) < 0) perror("fcntl"), exit(1);
        sleep(1);
}

int main(void)
{
        h = open("/dev/tty", O_RDONLY | O_NONBLOCK);
        if (h < 0) perror("open"), exit(1);
        signal(SIGIO, async);
        if (fcntl(h, F_SETFL, O_ASYNC | O_NONBLOCK) < 0) perror("fcntl"), exit(1);
        while (1) sleep(1);
}
