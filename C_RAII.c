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
    printf("Auto CleanUP Has Been Called.\n");
  }
  return;
}

void auto_clean_up_int(int** iptr) {
  if (*iptr) {
    free(*iptr);
    printf("Int CleanUP Called.\n");
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
// 通过结构体封装资源及其清理逻辑，并显式调用析构函数。此处以文件句柄作为示例.
typedef struct FileHandle {
  FILE* fp;  // 封装的资源
  const char* mode;  // 附近信息（模式）
  const char* path;   // 附加信息（路径）
  void (*cleanup) (struct FileHandle* ); // 清理函数指针
} FileHandle ;

// 清理函数.
void cleanFileHandle(FileHandle* fh) {
  if (fh->fp) {
    fclose(fh->fp);
    printf("The File Has Been Closed.\n");
    fh->fp = NULL;
  }
  fh->path = NULL;
  fh->mode = NULL;
  return;
}

// 初始化.
int FileHandle_Init(FileHandle* fh , const char* path , const char* mode) {
  fh->fp = fopen(path , mode);
  if (!fh->fp) {
    fprintf(stderr , "Error Open The File.\n");
    return -1;
  }
  printf("SuccessFully Open.\n");

  fh->path = path;
  fh->mode = mode;
  fh->cleanup = cleanFileHandle;  // 注册清理函数.
  return 0;
}

// 以上使用了一个文件句柄简单封装的例子，来封装结构体简单手动模拟了RAII的操作.
// 尽管是手动调用的析构函数，但是代码生命周期清晰可见.

// 以上可抽象为泛型结构.
typedef void(* ResourceDestructor )(void* ) ;

typedef struct RAII_Handle {
  void* Resource ;
  ResourceDestructor cleanup;  // 清理函数.
  const char* message; // 附加信息.
} RAII_Handle ;

// 泛型清理函数.
void Resource_Clean(RAII_Handle* Resh) {
  if (Resh->Resource && Resh->cleanup) {
    Resh->cleanup(Resh->Resource) ; // 调用已注册的清理函数. 
    Resh->Resource = NULL;
  }
  return;
}

// 初始化.
void RAII_Init(
    RAII_Handle* Resh , 
    void* resource,
    ResourceDestructor dtor , 
    const char* message) {
  Resh->Resource = resource;
  Resh->message = message;
  Resh->cleanup = dtor;

  printf("Successfully Inite.\n");
}

// 针对不同资源的具体清理函数.
void File_Clean(void* ptr) {
  if (fclose((FILE*)ptr) == 0) {
    printf("FIle_Clean Called.\n");
  }
}

void Mem_Clean(void* ptr) {
  // 前面的泛型清理结构已经检查过非空了.
  free(ptr);
  printf("Mem_Clean Called.\n");
}

void Example_Func() {
  FILE* fp = fopen("C_Array.c" , "r");
  RAII_Handle file_handle;
  RAII_Init(&file_handle , fp , File_Clean , "NULL");

  int* arr = (int*)malloc(sizeof(int) * SIZE);
  if (!arr) {
    goto FAL;
  }
  RAII_Handle mem_handle;
  RAII_Init(&mem_handle , arr , Mem_Clean , "NULL");

FAL:  
  Resource_Clean(&mem_handle);
  Resource_Clean(&file_handle);
  return;
}

int main(int argc , const char* argv[]) {
  //Example_Function(20);
  
  FileHandle myfh;
  int result = FileHandle_Init(&myfh , "C_Array.c" , "r"); 
  if (result < 0) { goto Lable; }

  Example_Func();

Lable:
  myfh.cleanup(&myfh);
  return 0;
}