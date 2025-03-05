/*
  C 柔性数组
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// 柔性数组 C99标准写法
typedef struct {
  int length;
  int data[];
} Array_Int;

// 旧式兼容性写法。
// typedef struct {
//   int length;
//   int data[0];
// } Array_Int;

typedef struct {
  int length;
  char* data;
} Old_Arr;

void Print_Arr_New(Array_Int* Arr) {
  printf("The Array That you Got is: ");
  for(int i = 0 ; i < Arr->length ; ++i) {
    printf("%d  ", Arr->data[i]);
  }
  printf("\n");
}

void Print_Arr_Old(Old_Arr* Arr) {
  printf("The Array That you Got is: ");
  for(int i = 0 ; i < Arr->length ; ++i) {
    printf("%c  ", Arr->data[i]);
  }
  printf("\n");
}

Array_Int* function_NewArray(int length) {
  assert(length > 0);
  Array_Int* myArr = (Array_Int*)malloc(sizeof(Array_Int) + sizeof(int) * length);
  if (!myArr) {
    fprintf(stderr , "Error on Allocate Array.");
    return 0;
  }
  myArr->length = length;
  for(int i = 0 ; i < length ; ++i) {
    myArr->data[i] = i * 2;
  }
  return myArr;
}


// 传统可变长数组写法。
Old_Arr* function_OldArray(int length) {
  assert(length > 0);
  // 第一次内存分配（结构体）
  Old_Arr* old_arr = (Old_Arr*)malloc(sizeof(Old_Arr));
  if (!old_arr) {
    fprintf(stderr , "Error Allocation.");
    return 0;
  }
  old_arr->length = length;

  // 第二次内存分配（分配数组空间）
  old_arr->data = (char*)malloc(length + 1);
  if (!old_arr->data) {
    free(old_arr);
    fprintf(stderr , "Error Allocation.");
    return 0;
  }

  for (int i = 0 ; i < length ; ++i) {
    old_arr->data[i] = (char)('a' + i);
  }
  return old_arr;
}

int main() {
  Array_Int* myArr = function_NewArray(20);
  if (!myArr) {
    exit(EXIT_FAILURE);
  }
  Print_Arr_New(myArr);

  Old_Arr* oldarr = function_OldArray(10);
  if (!oldarr) {
    if (!myArr) {
      free(myArr);
    }
    exit(EXIT_FAILURE);
  }
  Print_Arr_Old(oldarr);

  free(myArr);
  // 同样的，两次内存分配意味着两次内存释放。
  free(oldarr->data);
  free(oldarr);
  return 0;
}


