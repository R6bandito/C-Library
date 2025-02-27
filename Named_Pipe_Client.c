#include <stdio.h>
#include <windows.h>

// 连接到命名管道
void ConnectToPipe(HANDLE* hPipe , DWORD* dwRead) {
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

int main(int argc , const char* argv[]) {
  HANDLE hPipe;
  DWORD dwRead;
  DWORD minutes;
  ConnectToPipe(&hPipe , &dwRead);

  while(1) {}

  return 0;
}