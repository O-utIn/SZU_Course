#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

// 带时间戳的打印函数
int tprintf(const char *fmt, ...) {
    va_list args;
    struct tm *tstruct;
    time_t tsec;

    tsec = time(NULL);
    tstruct = localtime(&tsec);
    printf("%02d:%02d:%02d: %5d ", tstruct->tm_hour, tstruct->tm_min, tstruct->tm_sec, getpid());

    va_start(args, fmt);
    return vprintf(fmt, args);
}
