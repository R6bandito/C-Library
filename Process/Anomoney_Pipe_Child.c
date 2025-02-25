#include <stdio.h>
#include <windows.h>

#define BUFFER_SIZE 1024

int main(int argc , const char* argv[]) {
  char buffer[BUFFER_SIZE];
  DWORD dwRead;
  HANDLE hRead = GetStdHandle(STD_INPUT_HANDLE);
  while(ReadFile(hRead , buffer , sizeof(buffer) , &dwRead , NULL) && dwRead != 0) {
    buffer[min(dwRead , BUFFER_SIZE - 1)] = '\0';
    printf("Childs Process Read From Pipe: %s\n" , buffer);
    fflush(stdout);
  }
  printf("Child Process End\n");
  return 0;
}