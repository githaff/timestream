#include "aux.h"
#include "config.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


typedef enum Mode { SECONDS, MINUTES } Mode;

Mode g_mode = MINUTES;


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

void parse_args(int argc, char *argv[])
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
            err("unknown option '%s'.\nTry --help for more information.",
                argv[i]);
            exit(-1);
        }
    }
}

int main(int argc, char *argv[])
{
    int ret = 0;

    parse_args(argc, argv);

    return ret;
}
