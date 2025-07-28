#include "error.h"
#include <stdio.h>
#include <string.h>


static ErrorCode_t error = ERR_NONE;
static char error_message[128];
static const char *error_file = NULL;
static int error_line = 0;

void error_raise(ErrorCode_t err_code, const char *err_msg, const char *err_file, int err_line)
{
    error = err_code;
    strncpy(error_message, err_msg, sizeof(error_message));
    error_file = err_file;
    error_line = err_line;

    fprintf(stderr, "ERROR[%d] at %s:%d: %s\n", err_code, err_file, err_line, err_msg);
}

void error_clear(void)
{
    error = ERR_NONE;
    error_message[0] = 0;
    error_file = NULL;
    error_line = 0;
}