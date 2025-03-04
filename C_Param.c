/*
  C_Param.c
  C语言实现默认参数函数 及 可变参数函数 
*/
#include <stdio.h>
#include <stdarg.h> // 可变参数函数头文件
#include <assert.h>

typedef struct {
  size_t length;
  const char* message;
} Param;

#define DEFAULT {.length = 0 , .message = "Default Params Call"}

// 法一： 将参数封装到结构体中，并利用C99的指定初始化器设置默认值。
void function_A(Param param) {
  int length = param.length ? param.length : 0;
  const char* message = param.message ? param.message : "Hello, C!";
  printf("length: %d, message: %s\n", length, message);

}

void CallFunction_A() {
  function_A((Param)DEFAULT);  // 默认参数调用
  function_A((Param){.length = 512, .message = "All Custom Params Call."});  // 自定义参数调用
  function_A((Param){.message = "Part of Param Call."});  // 部分参数调用
}

// 法二：可变参数函数
void Function_B(size_t Var_Parcount , const char* message, ...) {
  assert(Var_Parcount == 0 || Var_Parcount == 1 || Var_Parcount == 2);
  va_list args;
  va_start(args, message);

  // 默认值
  int flag = 0;  
  size_t length = 0;

	if (Var_Parcount == 1) {
    length = va_arg(args, size_t);
  }
  else if (Var_Parcount == 2) {
    length = va_arg(args, size_t);
    flag = va_arg(args, int);
  }

  printf("length: %d, message: %s, flag: %d\n", length, message, flag);
  va_end(args);
}

void CallFunction_B() {
  Function_B(0, "Default Call");  // 默认参数调用
  Function_B(1, "Part of Param Call", 1024);  // 部分参数调用
  Function_B(2, "All Custom Params Call", 2048, 1);  // 自定义参数调用
  //Function_B(3, "Error Params Call", 4096, 1);  // 错误参数调用
}

// 法三： 不同名称模拟实现函数重载
// 完整参数函数
void Function_Full(const char* message, size_t length, int flag) {
  printf("length: %d, message: %s, flag: %d\n", length, message, flag);
}

// 部分参数函数
void Function_Two(const char* message, size_t length) {
  Function_Full(message, length, 0);
}

// 部分参数函数
void Function_One(const char* message) {
  Function_Full(message, 0, 0);
}

// 默认参数函数
void Function_Default() {
  Function_Full("Default Call", 0, 0);
}

void CallFunction_Full() {
  Function_Default();  // 默认参数调用
  Function_One("Part of Param Call");  // 部分参数调用
  Function_Full("All Custom Params Call", 2048, 1);  // 自定义参数调用
}

int main(int argc, const char* argv[]) {
  //CallFunction_A();
  //CallFunction_B();
  CallFunction_Full();
  return 0;
}