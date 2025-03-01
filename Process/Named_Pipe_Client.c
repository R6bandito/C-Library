#include <stdio.h>
#include <windows.h>

#define BUFFER_SIZE 1024

// 连接到命名管道
void ConnectToPipe(HANDLE* hPipe) {
  *hPipe = CreateFile(
    TEXT("\\\\.\\pipe\\SystemMonitor"),   // 管道名称
    GENERIC_READ | GENERIC_WRITE,         // 读写权限
    FILE_SHARE_READ,                      // 共享模式
    NULL,                                 // 默认安全属性
    OPEN_EXISTING,                        // 打开已存在的管道
    0,                                    // 默认属性
    NULL                                  // 默认模板
  );

  if (*hPipe == INVALID_HANDLE_VALUE) {
    fprintf(stderr , "CreateFile Failed\n");
    exit(EXIT_FAILURE);
  }
  printf("CreateFile Success\n");
}

void ReadFromPipe(HANDLE* hPipe , DWORD* dwRead , char* buffer) {
  WINBOOL Success;
  DWORD Data;
  Success = ReadFile(*hPipe , buffer , BUFFER_SIZE , dwRead , NULL);
  if (!Success) {
    fprintf(stderr , "Read from Pipe Failed\n");
    CloseHandle(*hPipe);
    exit(EXIT_FAILURE);
  }
  Data = *((DWORD*)buffer);
  printf("The System Runing Time is: %d h %d m\n" , Data / 60 , Data % 60);
}

int main(int argc , const char* argv[]) {
  HANDLE hPipe;
  DWORD dwRead;
  char buffer[BUFFER_SIZE];

  ConnectToPipe(&hPipe);

  while(1) {
    ReadFromPipe(&hPipe , &dwRead , buffer);
    Sleep(1000);
  }

  return 0;
}