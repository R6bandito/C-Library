#include <stdio.h>
#include <windows.h>

// create annonymous pipe
void CreateMyPipe(HANDLE* hRead, HANDLE* hWrite) {
  // security attributes setting for pipe
  SECURITY_ATTRIBUTES sa;  
  sa.bInheritHandle = TRUE;
  sa.nLength = sizeof(SECURITY_ATTRIBUTES);
  sa.lpSecurityDescriptor = NULL;

  if(!CreatePipe(hRead, hWrite, &sa, 0)) {
    fprintf(stderr, "Create Pipe Failed\n");
    exit(EXIT_FAILURE);
  }
  printf("Create Pipe Success\n");
  // Print the fixed information of the pipe
  printf("hWrite = %p, hRead = %p\n\n", hWrite, hRead);
}

// write data to the pipe
void WriteToPipe(HANDLE* hWrite , HANDLE* hRead , const char* message) {
  // Print the fixed information of the pipe
  printf("WriteToPipe: hWrite = %p, hRead = %p, message = %s\n", hWrite, hRead, message);

  DWORD dwWritten;
  if(WriteFile(*hWrite , message , strlen(message) + 1 , &dwWritten , NULL) == FALSE) {
    fprintf(stderr , "Write to Pipe Failed\n");
    exit(EXIT_FAILURE);
  }
  printf("Write to Pipe Success\n");
  printf("The size of the data that is written to pipe is: %d\n\n" , dwWritten);
  CloseHandle(*hWrite);  // close the write end of the pipe in the father process
} 

// create child process
void CreateChildProcess(HANDLE* hRead , PROCESS_INFORMATION* pi) {
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
    exit(EXIT_FAILURE);
  }
  printf("Create Process Success. Start Waiting\n");
  CloseHandle(*hRead);  // close the read end of the pipe in the father process
  WaitForSingleObject(pi->hProcess , INFINITE);
  printf("End Waiting\n");
}

int main(int argc, const char* argv[]) {
  HANDLE hWrite;  // pipe write handle

  HANDLE hRead;  // pipe read handle

  PROCESS_INFORMATION pi;  // process information

  const char* message = argv[1];  // message to be sent to the child process

  
  CreateMyPipe(&hRead , &hWrite); 
  WriteToPipe(&hWrite , &hRead ,message);
  CreateChildProcess(&hRead , &pi);

  // close child process handle
  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);
  return 0;
}