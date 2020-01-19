#include "aux.h"
#include "config.h"

#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>


#define SEC_PER_MIN 60
typedef enum Mode { SECONDS, MINUTES } Mode;

Mode g_mode = MINUTES;
timer_t g_timer;

int setup();

void usage()
{
    puts("Usage: timestream [OPTION]");
    puts("Stream time aligned to seconds/minutes.");
    puts("");
    puts("Options");
    puts("  -h, --help       show help");
    puts("  -s               output in seconds. Default: minutes");
    puts("  -v, --version    print version");
}

void version()
{
    printf("%s\n", TIMESTREAM_VERSION_FULL);
}

int parse_args(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
            usage();
            exit(0);
        } else if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--version")) {
            version();
            exit(0);
        } else if (!strcmp(argv[i], "-s")) {
            g_mode = SECONDS;
        } else {
            err("unknown option '%s'.\nTry --help for more information.", argv[i]);
            return -1;
        }
    }

    return 0;
}

int get_time(struct timespec *ts)
{
    int ret = 0;

    ret = clock_gettime(CLOCK_REALTIME, ts);
    if (ret < 0) {
        perror("failed to get clock");
        return ret;
    }

    return ret;
}

void sig_handler(int sig)
{
    int ret;
    struct timespec ts;
    struct tm tm;

    if (sig == SIGUSR1) {
        get_time(&ts);
        ret = clock_gettime(CLOCK_REALTIME, &ts);
        if (ret < 0) {
            perror("failed to get clock");
            return;
        }
        dbg("Latency: %ldns", ts.tv_nsec);

        /* Note: it is not advised to use printf function in the signal handlers but
         * this one is called no more frequent than once a second. */
        localtime_r(&ts.tv_sec, &tm);
        if (g_mode == MINUTES)
            printf("%02d:%02d\n", tm.tm_hour, tm.tm_min);
        else
            printf("%02d:%02d:%02d\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
        fflush(stdout);
    } else if (sig == SIGUSR2) {
        g_mode = g_mode == SECONDS ? MINUTES : SECONDS;
        setup();
    } else {
        err("unexpected signal: %d", sig);
    }
}

int setup()
{
    int ret = 0;
    struct timespec ts;
    struct itimerspec its = {0};

    ret = get_time(&ts);
    if (ret < 0) {
        err("failed to get current time");
        return ret;
    }

    its.it_interval.tv_sec = g_mode == MINUTES ? SEC_PER_MIN : 1;
    its.it_value.tv_sec = ts.tv_sec / SEC_PER_MIN * SEC_PER_MIN;

    ret = timer_settime(g_timer, TIMER_ABSTIME, &its, NULL);
    if (ret < 0) {
        perror("failed to set timer");
        return ret;
    }

    return ret;
}

int init()
{
    int ret = 0;
    struct sigevent evp = {
        .sigev_notify = SIGEV_SIGNAL,
        .sigev_signo = SIGUSR1,
    };
    struct sigaction sa = {
        .sa_handler = &sig_handler,
        .sa_flags = 0
    };
    sigemptyset(&sa.sa_mask);

    ret = sigaction(SIGUSR1, &sa, NULL);
    ret &= sigaction(SIGUSR2, &sa, NULL);
    if (ret < 0) {
        perror("failed to set signal handler");
        return ret;
    }

    ret = timer_create(CLOCK_REALTIME, &evp, &g_timer);
    if (ret < 0) {
        perror("failed to create timer");
        return -1;
    }

    return ret;
}

int main(int argc, char *argv[])
{
    int ret = 0;

    ret = parse_args(argc, argv);
    if (ret < 0)
        return ret;

    ret = init();
    if (ret < 0)
        return ret;

    ret = setup();
    if (ret < 0)
        return ret;

    while (true) {
        pause();
    }

    return ret;
}
