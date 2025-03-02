/*
  C语言实现的最大游程长度计算
*/

#include <stdio.h>
#include <windows.h>

// 将最大游程对应的字符串截取出来。
char* GetStr(const char* str , void* pPos , size_t length) {
  // 一头一尾双指针定位。
  void* pStrHead = pPos;
  void* pStrTail = pPos + length - 1;
  char* buffer = (char*) malloc(length + 1);

  for( ; pStrHead <= pStrTail ; ++pStrHead) {
    buffer[pStrHead - pPos] = *(char *)pStrHead;
  }
  buffer[length] = '\0';
  return buffer;
}

size_t GetMaxRunLength(char* str) {
  char NextReadValue;  
  char CurrentValue = str[0];
  size_t CurrentLength = 1;
  size_t MaxLength = 1;
  void* pPos = str;
  void* pMaxPos;

  for (size_t i = 1 ; str[i] != '\0' ; ++i) {
    NextReadValue = str[i];  // 读取下一个字符
    pPos++;  // 记录位置。
    if (NextReadValue == CurrentValue) {  // 读取的下一个字符与当前字符比较
      CurrentLength++;  // 相同则表示为连续序列，长度加1
      if (CurrentLength > MaxLength) {  // 更新最大长度
        MaxLength = CurrentLength;
        pMaxPos = pPos-CurrentLength+1;
      }
    }
    else {
      CurrentValue = NextReadValue;  // 不相同则更新当前字符。
      CurrentLength = 1;
    }
  }
  char* buffer = GetStr(str , pMaxPos , MaxLength);
  printf("The MaxRunLength String is %s \n" , buffer);
  free(buffer);
  return MaxLength;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <String>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  size_t MaxRunLength = GetMaxRunLength(argv[1]);
  printf("The MaxRunLength of %s is %zu \n", argv[1] , MaxRunLength);
  return 0;
}