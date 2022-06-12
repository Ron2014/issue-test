#include <stdio.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
// #include <sys/ioctl.h>
// #include <net/if.h>

int main(int argc, char *argv[])
{
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    // int args = 0;
    // ioctl(fileno(stdin), FIONREAD, &args);
    // ioctl(fileno(stdin), FIONWRITE, args);

    char c;
    c = getchar();

    int iRet;
    fd_set fds_read;
    struct timeval stWaitTime = {0, 100};

    while(1)
    {
        FD_ZERO(&fds_read);
        FD_SET(fileno(stdin), &fds_read);
        // printf("%d\n", FD_ISSET(fileno(stdin), &fds_read));

        int iOpenFds = select(FD_SETSIZE, &fds_read, NULL, NULL, &stWaitTime);
        // printf("%d %d\n", iOpenFds, FD_ISSET(fileno(stdin), &fds_read));

        if (iOpenFds > 0 && FD_ISSET(fileno(stdin), &fds_read))
        {
            iRet = read(fileno(stdin), (void *)&c, 1);
            if (iRet)
            {
                printf("0x%x\n", c);
            }
        }

        usleep(1);
    }
    return 0;
}