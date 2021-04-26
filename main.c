// Hunter Flory 1001792779
// main.c is a program that forks a child process from parent, 
//counts down from 10 (once per second) while printing the time 
//remaining, and returns to the parent process which prints
// "countdown complete". 

// for bonus points, a system signal handler and alarm signal will
// be used instead of "sleep()"

// structure of forking process used from 'fork.c' in course github

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define TEN_SECONDS 10

// create signal handler for use with alarm in main()
// signal handler will print when alarm is finished
void alarm_handler(int sig)
{
    printf("BRRRRRRING! Alarm %d finished from signal handler.\n", sig);
    alarm(1);
}

/*
    fork() a child and print a message from parent when child
    is finished.
*/

int main(int argc, char *argv[])
{

    // set up signal handler
    struct sigaction act;
    memset(&act, '\0', sizeof(act));
    act.sa_handler = &alarm_handler;
    if (sigaction(SIGALRM, &act, NULL) < 0)
    {
        perror("sigaction: ");
        return 1;
    }
    
    pid_t pid = fork(); // process ID
    
    if (pid == -1)
    {
        // if fork() returns -1, an error occurred
        perror("fork failed: ");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        // when fork returns 0, we are in child process!
        // Set an alarm for 10 seconds. 
        // Count down from 10, printing each second.
        int count;
        alarm(TEN_SECONDS);
        for (count = 10; count > 0; count--)
        {
            printf("%d seconds remaining from main function.\n", count);
            sleep(1); // wait 1 second
        }
        
    }
    else
    {
        // if anything else is returned, we are back in parent process.
        int status;
        
        // force the parent process to wait until the chile process exits
        // print "countdown complete"
        waitpid(pid, &status, 0);
        printf("Countdown complete.\n");
        fflush(NULL);
    }
    return EXIT_SUCCESS;
}
