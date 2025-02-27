/*
    由服务端持续发送系统运行的时间，多客户端实时接收并显示
  的简易监控系统。
*/

#include <stdio.h>
#include <windows.h>

// 创建命名管道
void CreateMyPipe(HANDLE* hPipe) {
  SECURITY_ATTRIBUTES sa;
  sa.bInheritHandle = TRUE;
  sa.nLength = sizeof(SECURITY_ATTRIBUTES);
  sa.lpSecurityDescriptor = NULL;

  *hPipe = CreateNamedPipe(
    TEXT("\\\\.\\pipe\\SystemMonitor"),  // 管道名称
    PIPE_ACCESS_DUPLEX,                  // 打开模式：双向管道
    PIPE_TYPE_MESSAGE |                  // 管道模式：消息管道
    PIPE_READMODE_MESSAGE |              // 读取模式：消息读取模式
    PIPE_WAIT,                           // 阻塞模式
    PIPE_UNLIMITED_INSTANCES,            // 最大实例数
    1024,                                // 输出缓冲区大小
    1024,                                // 输入缓冲区大小
    0,                                   // 默认超时时间
    &sa                                  // 默认安全属性
  );

  if (*hPipe == INVALID_HANDLE_VALUE) {
    fprintf(stderr , "CreateNamedPipe Failed\n");
    exit(EXIT_FAILURE);
  }
  printf("CreateNamedPipe Success\n");
}

// 服务端向管道中写入数据
void WriteToPipe(HANDLE hPipe) {
  DWORD tickCount = 0;
  DWORD dwWritten;
  tickCount = GetTickCount() / 60000;
  if (!WriteFile(hPipe , &tickCount , sizeof(DWORD) , &dwWritten , NULL)) {
    fprintf(stderr , "Write to Pipe Failed\n");
    CloseHandle(hPipe);
    exit(EXIT_FAILURE);
  }
  printf("Write to Pipe Success\n");
}

// 等待客户端连接
void waitForClient(HANDLE hPipe) {
  while(1) {
    if (ConnectNamedPipe(hPipe , NULL) == FALSE) {}
    else {
      printf("Client Successfully Connected\n");
      WriteToPipe(hPipe);
      Sleep(10000);  //每 10s 写一次
    }
  }
}

int main(int argc , const char* argv[]) {
  HANDLE hPipe;

  CreateMyPipe(&hPipe);
  waitForClient(hPipe);
  
  return 0;
}