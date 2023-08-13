#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/select.h>
#include <sys/time.h>

int down = 0;


void *startKeyListener(void* args) {
    const char* devicePath = "/dev/input/event5"; // Replace X with the correct event number
    int fd = open(devicePath, O_RDONLY);
    if (fd == -1) {
        perror("Error opening device");
        return NULL;
    }

    struct input_event event;

    while (1) {
        // Set up the file descriptor set for select()
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(fd, &fds);

        // Set up the timeout
        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 100000; // 100 ms

        // Wait for input with a timeout
        int ready = select(fd + 1, &fds, NULL, NULL, &timeout);

        if (ready == -1) {
            perror("Error in select");
            break;
        } else if (ready == 0) {
            // Timeout: no input event
            continue;
        }

        // Input event available, read it
        ssize_t bytesRead = read(fd, &event, sizeof(event));
        if (bytesRead == sizeof(event)) {
            if (event.type == EV_KEY && event.value == 0) {
                printf("Key up: %d\n", event.code);
                down = 0;
            } else if (event.type == EV_KEY && event.value == 1) {
                printf("Key down: %d\n", event.code);
                down = event.code;
            }
        }
    }

    close(fd);

    return NULL;
}

