/*
  C RAII思想的模拟实现.
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define SIZE 5

// 使用宏进一步封装代码.
#define RAII_VARIBLE(type , name , cleanup_function , init_Command) \
                  type name __attribute__((cleanup(cleanup_function))) = init_Command 

// 1：可以利于GCC/Clang编译器的 __attribute__((cleanup)) 的拓展工具来实现RAII
// 清理函数.
void auto_clean_up(char** cptr) {
  if (*cptr) {
    free(*cptr);
    printf("Auto CleanUP Has Been Called.");
  }
  return;
}

void auto_clean_up_int(int** iptr) {
  if (*iptr) {
    free(*iptr);
    printf("Int CleanUP Called.");
  }
  return;
}

void Example_Function(size_t length) {
  assert (length > 0);
  // 使用 __attribute__((cleanup)) 注册清理函数.
  //char* myMessage __attribute__((cleanup(auto_clean_up))) = (char*)malloc(length + 1);

  RAII_VARIBLE(char* , myMessage , auto_clean_up , (char*)malloc(length + 1));
  if (!myMessage) {
    fprintf(stderr , "Allocation Failed.");
    return;
  }
  // 多个RAII可以嵌套使用.
  RAII_VARIBLE(int* , myInt , auto_clean_up_int , (int*)malloc(sizeof(int) * SIZE));
  for(int i = 0 ; i < SIZE ; ++i) {
    myInt[i] = i * 4;
    printf("%d  " , myInt[i]);
  }

  memset(myMessage , 0 , length + 1);
  for( int i = 0 ; i < length ; ++i) {
    myMessage[i] = 'a' + i;
    }
  myMessage[length] = '\0';
  printf("The String is : %s \n" , myMessage);
  // 直接离开作用域，不需要显式free释放.
  // 函数返回时，auto_clean_up 自动调用
}

// 2：不依赖编译器拓展，封装结构体手动实现RAII.

int main(int argc , const char* argv[]) {
  Example_Function(20);

  return 0;
}