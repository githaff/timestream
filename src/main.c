#include <stdio.h>
#include <string.h>


void usage()
{
    puts("Usage: timestream [OPTION]");
    puts("tream time aligned to seconds/minutes");
    puts("");
    puts("ptions");
    puts("  -h, --help   show help");
    puts(" -s           output in seconds. Default: minutes");
}


int main(int argc, char *argv[])
{
    for (int i = 0; i < argc; i++) {
        if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
            usage();
            return 0;
        }
    }

    printf("U-u-u-u\n");

    return 0;
}
