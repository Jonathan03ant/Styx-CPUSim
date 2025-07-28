#ifndef ERROR_H
#define ERROR_H


typedef enum {
    ERR_NONE,
    ERR_INVALID_REGISTER,
    ERR_INVALID_MEM_ACCESS,
    ERR_ILLEGAL_INST,
    ERR_INTERNAL_ASSERTION,
    //more error enums to add later
}ErrorCode_t;

void error_raise(ErrorCode_t err_code, const char *err_msg, const char *err_file, int err_line);
void error_clear(void);

#define RAISE_ERROR(code, msg) error_raise(code, msg, __FILE__, __LINE__) //ERROR MACRO
#endif