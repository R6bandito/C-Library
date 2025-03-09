# C-Library

## C RAII思想模拟实现

### **什么是RAII?** 

​	RAII（Resource Acquisition Is Initializing），也就是**资源获取即初始化**。其核心思想是：**将资源的获取与对象的初始化绑定，并在对象生命周期结束时自动释放资源**。虽然在C语言中并没有像C++一样的离开作用域自动调用析构函数，但可以通过一些途径来模拟其实现。

### 依赖于gcc/clang工具的特性实现：

​	对于 `GCC/Clang` 编译器，其提供了关键工具：

```c
__attribute__((cleanup))
```

用于为每个资源定义专用的清理函数。**并且实现当离开作用域时，自动调用清理函数释放资源的功能。**

​	其中值得注意的是：**清理函数必须遵循特定的函数签名。**

```c
void cleanup_function(void *ptr);
```

```c
// 1：可以利于GCC/Clang编译器的 __attribute__((cleanup)) 的拓展工具来实现RAII
// 清理函数.
void auto_clean_up(char** cptr) {
  if (*cptr) {
    free(*cptr);
    printf("Auto CleanUP Has Been Called.\n");
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
}
```



​         如上述示例，使用`__attribute__`工具为局部变量注册了清理函数之后，当程序流程离开变量的作用域时，会自动调用所注册的`auto_clean_up`方法。**被管理的变量`Message`的地址将会被传入清理函数之中**。由于`Message`为指针变量且传递的是地址，因此清理函数在接受参数时应该接受的是双指针（即指向指针变量的指针）。



```c
// 使用宏进一步封装代码.
#define RAII_VARIBLE(type , name , cleanup_function , init_Command) \
                  type name __attribute__((cleanup(cleanup_function))) = init_Command 
void Example_Function(size_t length) {
  assert (length > 0);
  RAII_VARIBLE(char* , myMessage , auto_clean_up , (char*)malloc(length + 1));
  if (!myMessage) {
        ....
  }
  // 多个RAII可以嵌套使用.
  RAII_VARIBLE(int* , myInt , auto_clean_up_int , (int*)malloc(sizeof(int) * SIZE));
  		....
  }
}
```

​	如上，多个`__attribute__`可以嵌套使用，

应值得注意的是：

​	1.**该工具所实现的RAII所针对的是局部变量**，若有变量的作用域是全局的，则清理函数不会在程序结束时自动调用。  

​	2.`__attribute__`工具所具有的特性是依赖于gcc/Clang的实现的，因此对运行的环境有要求，跨平台能力不好。

### 基于对象方式的手动封装：

​	**通过结构体和函数指针模拟“类”，在结构体中保存资源指针和清理函数，显式或隐式调用清理**。由于并不具备c++的特性，因此所谓的析构函数（清理函数）需手动调用。这种实现会使代码更为复杂，但是相对来说也能更加直观地观察资源生命周期，并且得益于结构体，可拓展性也强。

```c
// 2：不依赖编译器拓展，封装结构体手动实现RAII.
// 通过结构体封装资源及其清理逻辑，并显式调用析构函数。此处以文件句柄作为示例.
typedef struct FileHandle {
  FILE* fp;  // 封装的资源
  const char* mode;  // 附近信息（模式）
  const char* path;   // 附加信息（路径）
  void (*cleanup) (struct FileHandle* ); // 清理函数指针
} FileHandle ;
```



## 柔性数组与可变长数组

​	柔性数组是 C99 标准中引入的一种特性，**主要用于在结构体的末尾声明一个长度未定的数组**。它允许**更高效地管理动态内存**，尤其是在需要将**结构体和变长数据连续存储的场景**中。

### **基本定义：**

​	在结构体的**最后一个成员位置**，声明一个未指定长度的数组（在C99标准下，【】内不需要写0。但是对于部分编译器为了兼容性可以写0）。

```c
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
```

​	注意：一个结构体只能包含一个柔性数组！像下列的写法是错误的。

```c
typedef struct {
  int length_int;
  int length_char;
  int data_int[];
  char data_char[];
} Array_Int;
//这种写法是完全错误的！
```

- **柔性数组本身不占用结构体的内存空间**（`sizeof(Array_Int)` 不包含 `data` 的大小）。
- **需要手动分配内存来容纳结构体本身和柔性数组所需的空间。**

### **使用：**

​	计算好内存之后直接通过`malloc`一次分配进行使用。**计算总内存大小：结构体本身 + 柔性数组所需空间。**

```c
Array_Int* function_NewArray(int length) {
  assert(length > 0);
  Array_Int* myArr = (Array_Int*)malloc(sizeof(Array_Int) + sizeof(int) * length);
  if (!myArr) {
    fprintf(stderr , "Error on Allocate Array.");
    return 0;
  }
  myArr->length = length;
  return myArr;
}
```

​	访问时直接通过柔性数组名操作数据：

```c
  for(int i = 0 ; i < Arr->length ; ++i) {
    printf("%d  ", Arr->data[i]);
  }
```

​	**内存释放时只需要释放一次。**（传统的变长数组需要两次释放）。

```c
free(s); // 同时释放结构体和柔性数组的内存
```



### **传统可变长数组：**

```c
typedef struct {
  int length;
  char* data;
} Old_Arr;
```

​	传统可变长数组相比起柔性数组而言：

1：**需要两次内存分配**，结构体和 `data` 指向的内存块需要分别分配。

```c
// 第一次内存分配（结构体）
  Old_Arr* old_arr = (Old_Arr*)malloc(sizeof(Old_Arr));
  if (!old_arr) {
	...
  }
  old_arr->length = length;

  // 第二次内存分配（分配数组空间）
  old_arr->data = (char*)malloc(length + 1);
  if (!old_arr->data) {
 	...
  }
```

2：**传统可变长数组内部数据是不连续的**，结构体和数据分离，可能影响缓存效率。

3：多次分配可能产生内存碎片。

### **实际应用场景示例：**

​	**网络通信协议包**

```c
// 自定义协议包：头部 + 变长负载
struct Packet {
    int type;
    int payload_length;
    unsigned char payload[]; // 柔性数组存储负载数据
};

// 创建数据包
struct Packet *packet = malloc(sizeof(struct Packet) + data_length);
packet->type = 1;
packet->payload_length = data_length;
memcpy(packet->payload, raw_data, data_length);
```



## C模拟实现默认函数参数特性

​	**方法一：结构体封装参数 + 默认初始化**

​	将参数打包封装到一个结构体中，以结构体作为整体进行传参。

```c
#define DEFAULT {.length = 0 , .message = "Default Params Call"}

typedef struct {
  size_t length;
  const char* message;
} Param;

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
```

该方法尽管实现较为复杂，但优点是直观易读。

​	**方法二：使用可变参数函数**

​	通过`<stdarg.h>`处理可变参数，手动设置默认值。

```c
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
```

尽管没有使用到复杂的结构体与宏，但是逻辑设计容易出错，不是很喜欢这种写法。

​	**方法三：函数重载模拟**

​	通过不同名称的函数模拟重载，调用时自动补全参数。

```c
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
```

通过函数之间的嵌套调用来设置默认参数，很直观，设计逻辑也很简单，但是多个函数也增加了维护的复杂度。这种写法个人也十分喜欢。
