#include "shell.h"

#include "common.h"
#include "efi.h"
#include "file.h"
#include "graphics.h"
#include "gui.h"
#include "string.h"

#define MAX_CMDLINE 32

void pstat(void) {
    unsigned long long status;
    struct EFI_SIMPLE_POINTER_STATE s;
    unsigned long long waitidx;

    SPP->Reset(SPP, /*ExtendedVerification=*/FALSE);
    while (1) {
        ST->BootServices->WaitForEvent(1, &(SPP->WaitForInput), &waitidx);
        status = SPP->GetState(SPP, &s);
        if (!status) {
            puth(s.RelativeMovementX, 8);
            put(L" ");
            puth(s.RelativeMovementY, 8);
            put(L" ");
            puth(s.RelativeMovementZ, 8);
            put(L" ");
            puth(s.LeftButton, 1);
            put(L" ");
            puth(s.RightButton, 1);
            put(L" ");
        }
    }
}

int ls(void) {
    unsigned long long status;
    struct EFI_FILE_PROTOCOL *root;
    unsigned long long buf_size;
    unsigned char file_buf[MAX_FILE_BUF];
    struct EFI_FILE_INFO *file_info;
    int idx = 0;
    int file_num;

    status = SFSP->OpenVolume(SFSP, &root);

    while (1) {
        buf_size = MAX_FILE_BUF;
        // Read() to directory's EFI_FILE_PROTOCOL will get file list
        status = root->Read(root, &buf_size, (void *) file_buf);
        // buf_size = 0 when all the files and directories have been read.
        if (!buf_size) {
            break;
        }
        file_info = (struct EFI_FILE_INFO *) file_buf;
        strncpy(file_list[idx].name, file_info->FileName, MAX_FILE_NAME_LEN - 1);
        file_list[idx].name[MAX_FILE_NAME_LEN - 1] = L'\0';
        put(file_list[idx].name);
        put(L" ");
        idx++;
    }
    put(L"\r\n");
    file_num = idx;

    root->Close(root);
    return file_num;
}

void cat(unsigned short *file_name) {
    unsigned long long status;
    struct EFI_FILE_PROTOCOL *root;
    struct EFI_FILE_PROTOCOL *file;
    unsigned long long buf_size = MAX_FILE_BUF;
    unsigned short file_buf[MAX_FILE_BUF / 2];

    // Get root dir's EFI_FILE_PROTOCOL
    status = SFSP->OpenVolume(SFSP, &root);

    // Get file's EFI_FILE_PROTOCOL
    status = root->Open(root, &file, file_name, EFI_FILE_MODE_READ, /*Attributes=*/0);

    status = file->Read(file, &buf_size, (void *) file_buf);

    put(file_buf);

    file->Close(file);
    root->Close(root);
}

void shell(void) {
    unsigned short cmdline[MAX_CMDLINE];

    while (1) {
        put(L"inos> ");

        wait_and_get_line(cmdline, MAX_CMDLINE);
        if (strncmp(cmdline, L"hello", 6) == 0) {
            put(L"hello\r\n");
        } else if (strncmp(cmdline, L"gui", 4) == 0) {
            gui();
        } else if (strncmp(cmdline, L"pstat", 6) == 0) {
            pstat();
        } else if (strncmp(cmdline, L"ls", 3) == 0) {
            ls();
        } else if (strncmp(cmdline, L"cat", 4) == 0) {
            cat(L"a.txt");
        } else {
            put(L"command not found.\r\n");
        }
    }
}
