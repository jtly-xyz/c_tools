#ifndef LITTLEFS_SHELL_COM_H_
#define LITTLEFS_SHELL_COM_H_

typedef int (*exec_fun)(int argc, char *argv[]);

typedef struct _cmd_t
{
    const char * const cmd;
    exec_fun func;
    const char * const help;

} cmd_t;

extern cmd_t ls;
extern cmd_t cd;
extern cmd_t cp;
extern cmd_t rm;
extern cmd_t mv;
extern cmd_t mkdir;
extern cmd_t help;
extern cmd_t exit_cmd;

extern char *NOW_PATH_ARRY[64];
extern char NOW_PATH_BUF[1024];
extern char oper_path[1024];
//extern char oper_path2[1024];

void exec_cmd(char * input_str);

#endif
