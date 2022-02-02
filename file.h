#ifndef FILE_H_
#define FILE_H_

#define MAX_FILE_NAME_LEN 8
#define MAX_FILE_NUM 10
#define MAX_FILE_BUF 1024

struct FILE {
    unsigned short name[MAX_FILE_NAME_LEN];
};

extern struct FILE file_list[MAX_FILE_NUM];

#endif // FILE_H_