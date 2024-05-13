#include <stdio.h>
#include <poll.h>

int     main(void)
{
    int             num_events;
    int             pollin_happened;
    struct pollfd   pfds[1];

    pfds[0].fd = 0;
    pfds[0].events = POLLIN;

    printf("Hit RETURN or wait 2.5 seconds for timeout\n");

    num_events = poll(pfds, 1, 2500);
    printf("num_events: %d", num_events);

    if (num_events == 0)
    {
        printf("Poll timed out!\n");
    }
    else 
    {
        pollin_happened = pfds[0].revents & POLLIN;
        if (pollin_happened)
            printf("File descriptor %d is ready to read\n", pfds[0].fd);
        else 
            printf("Unexpected event ocurred: %d\n", pfds[0].revents);
    }

    return (0);
}