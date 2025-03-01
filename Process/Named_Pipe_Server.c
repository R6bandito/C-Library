/*
    由服务端持续发送系统运行的时间，多客户端实时接收并显示
  的简易监控系统。
*/

#include <stdio.h>
#include <windows.h>

// 检查客户端是否断开连接的参数包
typedef struct {
  HANDLE hPipe;
  HANDLE hEvent;
} CheckParam;

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

// 创建中断事件
HANDLE Interrupt() {
  HANDLE hEvent = CreateEvent(NULL , FALSE , FALSE , NULL);
  if (hEvent == NULL) {
    fprintf(stderr , "CreateEvent Failed\n");
    return NULL;
  }
  return hEvent;
}

// 检查客户端是否断开连接
// 通过DisconnectNamedPipe函数检查客户端是否断开连接，如果断开则触发事件。
void ConnectedCheck(CheckParam* param) {
  DWORD ByteVal;
  while(1) {
    if (PeekNamedPipe(param->hPipe , NULL , 0 , NULL , &ByteVal , NULL) == FALSE) {
      DWORD dwError = GetLastError();
      if (dwError == ERROR_BROKEN_PIPE) {
        printf("A Client Disconnected\n");
        SetEvent(param->hEvent);
        ExitThread(0);
      }
    }
    else {
      Sleep(1000);
      continue;
    }
  }
}

// 服务端向管道中写入数据
void WriteToPipe(HANDLE hPipe) {
  DWORD tickCount = 0;
  DWORD dwWritten;
  HANDLE hEvent = Interrupt();

  CheckParam param = {hPipe , hEvent};
  CreateThread(NULL , 128 , (LPTHREAD_START_ROUTINE)ConnectedCheck , &param , 0 , NULL);

  while(1) {
    tickCount = GetTickCount() / 60000;
    if (!WriteFile(hPipe , &tickCount , sizeof(DWORD) , &dwWritten , NULL)) {
      fprintf(stderr , "Write to Pipe Failed\n");
      CloseHandle(hPipe);
      exit(EXIT_FAILURE);
    }
    printf("Write to Pipe Success\n");
    // 每30s写一次。
    DWORD dwWait = WaitForSingleObject(hEvent , 30000);
    if (dwWait == WAIT_OBJECT_0) {
      printf("Triggered\n");
      CloseHandle(hEvent);
      CloseHandle(hPipe);
      hPipe = NULL;
      ExitThread(0);
    }
  }
}

// 等待客户端连接
void waitForClient(HANDLE hPipe) {
  DWORD ThreadID;
  while(1) {
    if (ConnectNamedPipe(hPipe , NULL) == FALSE) {}
    else {
      printf("A Client Successfully Connected\n");
      // 为每个客户端创建一个线程，可以处理多个客户端。
      if (!CreateThread(NULL , 128 , (LPTHREAD_START_ROUTINE)WriteToPipe , hPipe , 0 , &ThreadID)) {
        fprintf(stderr , "CreateThread Failed\n");
        CloseHandle(hPipe);
        hPipe = NULL;
        return;
      }
      CreateMyPipe(&hPipe);  //重新创建管道实例
    }
  }
}


int main(int argc , const char* argv[]) {
  HANDLE hPipe;

  CreateMyPipe(&hPipe);
  waitForClient(hPipe);
  
  return 0;
}