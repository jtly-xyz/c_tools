#include <stdio.h>
#include <stdlib.h>
#include "com.h"

char *NOW_PATH_ARRY[64] = {"/", NULL};
char NOW_PATH_BUF[1024] = "/";
char oper_path[1024] = {0};
//char oper_path2[1024] = {0};

static cmd_t *cmd_arr[] = {
    &ls,
    &cd,
    &cp,
    &rm,
    &mv,
    &mkdir,
    &help,
    &exit_cmd,
};

static  int find_cmd(const char * input_str)
{
    int i = 0;
    int cmd_num = sizeof(cmd_arr) / sizeof(cmd_t *);

    while(i < cmd_num)
    {
        if(strcmp(cmd_arr[i]->cmd, input_str) == 0)
            return i;
        i++;
    }

    return -1;
}

static char * cmd_tok(char * str)
{
    char * res = NULL;
    static char * tmp = NULL;
    if(str != NULL)
        tmp = str;


    if(*tmp == 0)
        return NULL;

    while((*tmp) && (*(tmp++) == ' '));
    if(*tmp == 0)
        return NULL;
    res = tmp - 1;

    while((*tmp) && (*(tmp++) != ' '));
    if(*tmp)
        *(tmp-1) = 0;

    return res;
}

void exec_cmd(char * input_str)
{
    int i = 0;
    int serial = -1;

    char *cmd_str = NULL;
    char *argv_str[32] = {NULL};
    char *tmp = NULL;

    if(input_str == NULL || *input_str == 0)
    {
        return;
    }

    cmd_str = cmd_tok(input_str);

    serial = find_cmd(cmd_str);

    if(serial >= 0)
    {
        //获取参数
        while(1)
        {
            tmp = cmd_tok(NULL);

            if(tmp == NULL)
                break;

            argv_str[i] = tmp;
            i++;
        }

        cmd_arr[serial]->func(i, argv_str);
    }
    else
    {
        printf("%s Command not found\n", cmd_str);
    }

}

