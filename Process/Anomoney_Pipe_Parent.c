#include <stdio.h>
#include <windows.h>

// error handle
void ErrorHandle_CHILD(PROCESS_INFORMATION* pi) {
  fprintf(stderr , "Something Unexpected Happend,Error Code: %d\n" , GetLastError());
  CloseHandle(pi->hProcess) ; pi->hProcess = NULL;
  CloseHandle(pi->hThread); pi->hThread = NULL;
}

void ErrorHandle_PIPE(HANDLE* hRead , HANDLE* hWrite) {
  CloseHandle(*hRead);  *hRead = NULL;
  CloseHandle(*hWrite); *hWrite = NULL;
}

// create annonymous pipe
void CreateMyPipe(HANDLE* hRead, HANDLE* hWrite) {
  // security attributes setting for pipe
  SECURITY_ATTRIBUTES sa;  
  sa.bInheritHandle = TRUE;
  sa.nLength = sizeof(SECURITY_ATTRIBUTES);
  sa.lpSecurityDescriptor = NULL;

  if(!CreatePipe(hRead, hWrite, &sa, 0)) {
    fprintf(stderr, "Create Pipe Failed\n");
    ErrorHandle_PIPE(hRead , hWrite);
    exit(EXIT_FAILURE);
  }

  if(!SetHandleInformation(*hWrite , HANDLE_FLAG_INHERIT , 0)) {
    fprintf(stderr , "SetHandleInformation Failed\n");
    ErrorHandle_PIPE(hRead , hWrite);
    exit(EXIT_FAILURE);
  }

  printf("Create Pipe Success\n");
  // Print the fixed information of the pipe
  printf("hWrite = %p, hRead = %p\n\n", hWrite, hRead);
}

// write data to the pipe
void WriteToPipe(HANDLE* hWrite , HANDLE* hRead , const char* message , PROCESS_INFORMATION* pi) {
  // Print the fixed information of the pipe
  printf("WriteToPipe: hWrite = %p, hRead = %p, message = %s\n", hWrite, hRead, message);

  DWORD dwWritten;
  if(WriteFile(*hWrite , message , strlen(message) + 1 , &dwWritten , NULL) == FALSE) {
    fprintf(stderr , "Write to Pipe Failed\n");
    ErrorHandle_PIPE(hRead , hWrite);
    exit(EXIT_FAILURE);
  }
  printf("Write to Pipe Success\n");
  CloseHandle(*hWrite);  // close the write end of the pipe in the father process
  *hWrite = NULL;
  printf("The size of the data that is written to pipe is: %d\n\n" , dwWritten);
  WaitForSingleObject(pi->hProcess , INFINITE);
} 

// create child process
void CreateChildProcess(HANDLE* hRead , HANDLE* hWrite, PROCESS_INFORMATION* pi) {
  STARTUPINFO si;
  char cmdline[] = ".\\child.exe";
  ZeroMemory(&si , sizeof(STARTUPINFO));  // initialize the structure
  si.cb = sizeof(STARTUPINFO);

  // set the hStdInput, hStdOutput, hStdError members of the STARTUPINFO structure
  si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
  si.hStdInput = *hRead;
  si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
  si.dwFlags = STARTF_USESTDHANDLES;  

  if(!CreateProcess(NULL , cmdline , NULL , NULL , TRUE , 0 , NULL , NULL , &si , pi)) {
    fprintf(stderr , "Create Process Failed\n");
    ErrorHandle_CHILD(pi);
    ErrorHandle_PIPE(hRead , hWrite);
    exit(EXIT_FAILURE);
  }
  CloseHandle(*hRead);  // close the read end of the pipe in the father process
  *hRead = NULL;
  printf("Create Process Success. Start Write To Pipe\n");
}

int main(int argc, const char* argv[]) {
  HANDLE hWrite;  // pipe write handle

  HANDLE hRead;  // pipe read handle

  PROCESS_INFORMATION pi;  // process information

  const char* message = argv[1];  // message to be sent to the child process

  ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));  // initialize the structure

  CreateMyPipe(&hRead , &hWrite); 
  CreateChildProcess(&hRead , &hWrite , &pi);
  WriteToPipe(&hWrite , &hRead ,message , &pi);

  // close child process handle
  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);
  return 0;
}