
/*

  @file       pointer_basics.c
  @Author     R6bandito
  @brief      C指针的基础知识相关（定义，取址，解引用，赋值，空指针等）.


  =========================================================================
    1.什么是指针？
      |__ 指针是存储内存地址的变量，本质上是无符号整数（但是指针区分类型信息）.
      |__ 指针自身也占用内存空间.指针自身的内存占用大小取决于 **CPU架构和编译环境**. 64位系统中指针占用8字节，32位系统中指针占用4字节.
      |__ 指针的值是另一个变量的地址(也就是所谓的指向另一个变量).通过指针可以间接访问和修改数据.


    2.指针的类型
      |__ 指针是具有类型的. 
        ------------------------------------
          example:
            int *p;         // 指向整型
            char *c;        // 指向字符
            float *f;       // 指向浮点型
            void *v;        // 通用指针（也叫万能指针），无类型指针.
        ------------------------------------

      |__ 指针的类型并不会决定其自身内存空间的占用.但是指针的类型决定了： ①指针的运算步长  ②解引用的解释方式  ③编译器的类型检查
        ------------------------------------
          example:
            float *f 与 int *p 运算步长不同.(f+1) 与 (p+1) 的跨进步长不同！
            对于同一个地址 addr，倘若使 int *p 与 float *f同时指向addr，则解引用后的含义是不同的.
        ------------------------------------


      3.指针相关的核心操作
        |__ 指针可以取地址(&)，及解引用(*).
        |__ 指针之间可以进行赋值操作.


      4.指针可以进行算术运算.
        |__ 指针可以进行4种算术运算：①(++)自增运算  ②(--)自减运算  ③(+)加法运算  ④(-)减法运算
        |__ 指针可以进行比较运算.


      5.多级指针.
        |__ 二级指针 int **pp，存储指针变量的地址（存储指针的指针）.可用于维持动态二维数组.
        |__ 也存在且可使用 三级及以上的指针.


      6.指针与数组的关系.
        |__ 数组名不严格来讲其实就是指向数组首元素地址的指针.
        |__ 指针是变量，可以赋值与进行相关运算，但是数组名是常量，不能重新指向其它地址.
        |__ 数组有确定的存储空间，指针可能未初始化.


      7.函数指针.
        |__ 顾名思义，指向函数的指针.
        |__ 存储函数的入口地址，可以通过该指针调用函数.
        |__ 函数指针允许程序将函数作为参数传递、存储在数组中或从函数返回.
        |__ 函数名本身就是函数的地址（和数组名类似）.
        |__ 函数指针的类型由函数的返回值类型和参数类型决定.
        |__ 函数指针的典型用途就是 **回调函数**.
          ------------------------------------
            example:
              int (*p)(int int);  // p是指向返回int、带两个int参数的函数的指针.
          ------------------------------------


      8.void *指针的相关规则.
        |__ 所有指针都可以隐式转化为void *类型的指针，但是反之从void *类型转向其它类型指针则需要显式转换.
        |__ 不建议对void *类型的指针进行算术运算（尽管GCC编译器允许）.
        |__ 不能对void *类型的指针解引用，必须先显式转化为一个类型.


      9.指针与 const 关键字.
        |__ 指针 const 的4级保护机制:
          ------------------------------------
            example:
              const int *p;           // 保护指针p指向的内容
              int* const p;           // 保护指针p本身
              const int* const p;     // 双重保护
              int *p;                 // 无保护
          ------------------------------------

      
      10.空指针.
        |__ 空指针是一个特殊的指针值.表示不指向任何内存区域.
        |__ 常见的定义形式为  #define NULL ((void *)0).
        |__ 注意:空指针的0值指的是逻辑上的无效，而并非物理层面上的无效.部分嵌入式系统0地址可能是有效的，因此空指针在这些系统上也会定义为别的逻辑“无效”的值.
        |__ 在定义指针变量时，一定要紧跟着使用空指针对相关变量进行初始化.

  =========================================================================

*/

#include <stdio.h>
#include <locale.h>
#include <windows.h>

// -------------------------------------------------
  // 各个示例函数.
  void pointer_example_demo_1( void );    // 指针的基本操作（定义、取址、解引用、赋值）
  void pointer_example_demo_2( void );    // 指针类型、大小、步长、解引用含义
  void pointer_example_demo_3( void );    // 指针算术运算（自增/减、加减、比较、求距离）
  void pointer_example_demo_4( void );    // 空指针的定义、初始化、判断及注意事项
  void pointer_example_demo_5( void );    // const 与指针的四种保护机制
  void pointer_example_demo_6( void );    // 数组与指针的关系（数组名作为常量指针、下标与指针等价、sizeof 差异、数组指针的步长）
  void pointer_example_demo_7( void );    // 多级指针（一级到三级）及其与 const 的混合规则
  void pointer_example_demo_8( void );    // 函数指针的声明、赋值、调用、函数指针数组、空函数指针检查
  void pointer_example_demo_9( void );    // void* 指针的隐式转换、不能直接解引用和算术运算（GCC 扩展的说明）、强制转换后使用

  static void utils_display( int array[] );
  void function_A( void );
  int function_B_Sub( int x, int y );
  int function_C_Add( int x, int y );
  int function_D_Mult( int x, int y );
  int function_E_Divide( int x, int y );
// -------------------------------------------------


int main( void )
{
  // 设置程序 locale 为 UTF-8
  setlocale(LC_ALL, ".UTF-8");
  // 设置控制台输出代码页为 UTF-8
  SetConsoleOutputCP(CP_UTF8);

  pointer_example_demo_1();   
  pointer_example_demo_2();   
  pointer_example_demo_3();   
  pointer_example_demo_4();
  pointer_example_demo_5();
  pointer_example_demo_6();
  pointer_example_demo_7();
  pointer_example_demo_8();
  pointer_example_demo_9();
  

  return 0;
}


/*
  @brief  指针的基本操作：定义，取址，解引用，赋值.
*/
void pointer_example_demo_1( void )
{
  printf("=========== 指针的基本操作 ===========\n");
  int ver_a = 50;
  float ver_b = 80.0f;
  int *ptr_a = &ver_a;                  // 定义指针变量ptr，并取ver的地址作为初始化.

  float *ptr_b = NULL;                  // 也可以先初始化为空指针，而后再对其进行赋值.
  ptr_b = &ver_b;

  printf("ver_a的地址: %p \nptr_a的地址: %p \nptr_a所指向的地址: %p \nptr_a的值: %d\n", &ver_a, &ptr_a, ptr_a, *ptr_a);
  printf("\nver_b的地址: %p \nptr_b的地址: %p \nptr_b所指向的地址: %p \nptr_b的值: %.2f\n", &ver_b, &ptr_b, ptr_b, *ptr_b);

  *ptr_a = 35;  *ptr_b = 12.5;          // 通过指针间接修改变量的值.
  printf("\n修改后 ptr_a的地址: %p \t修改后 ptr_a的值: %d\n", ptr_a, *ptr_a);
  printf("修改后 ptr_b的地址: %p \t修改后 ptr_a的值: %.2f\n", ptr_b, *ptr_b);
  printf("===============================================\n\n");

  /*
    @result : 
      =========== 指针的基本操作 ===========
        ver_a的地址: 000000000061FE1C 
        ptr_a的地址: 000000000061FE10
        ptr_a所指向的地址: 000000000061FE1C
        ptr_a的值: 50

        ver_b的地址: 000000000061FE18
        ptr_b的地址: 000000000061FE08
        ptr_b所指向的地址: 000000000061FE18
        ptr_b的值: 80.00

        修改后 ptr_a的地址: 000000000061FE1C    修改后 ptr_a的值: 35
        修改后 ptr_b的地址: 000000000061FE18    修改后 ptr_a的值: 12.50
  */
  /*
      上述结果可以看到：
        指针变量自身在内存占有一定的空间，有自身的内存地址.指针变量的值为所指向变量的地址.
        通过 解引用(*)可以对指向内存地址的数据进行修改.
  */
}


void pointer_example_demo_2( void )
{
  printf("=========== 指针内存大小 步长与指针类型 ===========\n");

  {
    int *p = NULL;
    double *q = NULL;
    printf("p 所占空间大小: %zu Bytes\nq 所占空间大小: %zu Bytes\n", sizeof(p), sizeof(q));
    /*
      @result :
        p 所占空间大小: 8Bytes
        q 所占空间大小: 8Bytes

      指针所占内存空间大小与指针类型无关. 在64位机上运行得到指针大小为8字节. 当拿到32位机（如STM32）上运行时，则会输出4Bytes.
    */
  }

  {
    double example_ver = 66.60;
    int *ptr_A = NULL;
    char *ptr_B = NULL;
    double *ptr_C = NULL;

    // 让 ptr_A ptr_B ptr_C先指向同一个地址.
    // 注意：此处出现不同类型指针赋值操作，编译器会报出警告.
    ptr_A = &example_ver;
    ptr_B = &example_ver;
    ptr_C = &example_ver;

    printf("\nptr_A所指向地址: %p\n",ptr_A);
    printf("ptr_B所指向地址: %p\n",ptr_B);
    printf("ptr_C所指向地址: %p\n\n",ptr_C);

    int *temp_A = ptr_A + 1;
    char *temp_B = ptr_B + 1;
    double *temp_C = ptr_C + 1;

    printf("temp_A所指向地址: %p\n", temp_A);
    printf("temp_B所指向地址: %p\n", temp_B);
    printf("temp_C所指向地址: %p\n\n", temp_C);



    // 注意：temp_A,ptr_A与 temp_C,ptr_C必须要进行指针类型转换！三者均转化为char *类型指针后才能准确计算步长差距.
    // 若不进行类型转换，则得出的步长均为1（这并非计算错误，而是指针算术的本质:**以指针指向的数据类型大小为单位进行地址偏移和差值计算，因此运算结果表示的是元素的个数，而非字节数.**）
    printf("步长(int): %d\n", ((char *)temp_A - (char *)ptr_A));
    printf("步长(char): %d\n", (temp_B - ptr_B));
    printf("步长(double): %d\n\n", ((char *)temp_C - (char *)ptr_C));
    /*
      @result :
        ptr_A所指向地址: 000000000061FDD8
        ptr_B所指向地址: 000000000061FDD8
        ptr_C所指向地址: 000000000061FDD8

        temp_A所指向地址: 000000000061FDDC
        temp_B所指向地址: 000000000061FDD9
        temp_C所指向地址: 000000000061FDE0

        步长(int): 4
        步长(char): 1
        步长(double): 8
    */
    /*
      可见指针的类型与指针运算中计算步长是有关联的.不同类型指针在进行指针运算时步长跨度是不同的.
    */
  }

  {
    unsigned int example_ver = 0x0BCDEFFF;        // 0x0BCDEFFF(198045695)
    unsigned short *ptr_short = (unsigned short *)&example_ver;
    unsigned int *ptr_int = &example_ver;

    printf("\nint变量的值: %d\n", example_ver);
    printf("unsigned int *解引用的值: %d\n", *ptr_int);
    printf("unsigned short *解引用的值: %d\n", *ptr_short);
    printf("unsigned int *解引用的值(HEX): %x\n", *ptr_int);
    printf("unsigned short *解引用的值(HEX): %x\n", *ptr_short);
    printf("===============================================\n\n");

    /*
      @result : 
        int变量的值: 198045695
        unsigned int *解引用的值: 198045695
        unsigned short *解引用的值: 61439
        unsigned int *解引用的值(HEX): bcdefff
        unsigned short *解引用的值(HEX): efff
    */
    /*
      上述example_ver内存布局如下:
        0xFF 0xEF 0xCD 0x0B (自左向右，0xFF为最低地址 0x0B为最高地址).
        unsigned int *ptr_int为4字节，解引用时读取从起始地址开始的连续 4 个字节，按小端序组合为 0x0BCDEFFF，输出198045695.
        unsigned short *ptr_short 为2字节，解引用时只读取起始地址开始的连续 2 个字节，按小端序组合为0xEFFF，输出61439.
      因此，针对同一地址不同的指针类型进行解引用，所代表的含义是不同的.
    */
  }
}



void pointer_example_demo_3( void )
{
  printf("=========== 指针运算 ===========\n");
  int array[5] = { 0, 1, 2, 3, 4 };
  int *ptr_int = array;                  // 数组名就是指向数组中首元素的指针，且是常量 不能更改其指向.

  for( int j = 0; j < sizeof(array)/sizeof(array[0]); j++ )
  {
    printf("ptr_int[%d] 地址: %p, ptr_int[%d] 地址对应值: %d\n", j, ptr_int, j, *ptr_int);
    (ptr_int++);                          // 采用自增运算进行遍历.
  }

  // printf("ptr_int[5] 地址: %p, ptr_int[5]的值: %d\n", ptr_int, *ptr_int);    // 此时指针ptr_int已经指向数组之后(越界),对其解引用危险！

  for( int j = sizeof(array)/sizeof(array[0]) - 1; j >= 0; j-- )
  {
    printf("自减运算反向遍历  ");
    (ptr_int--);                          // 采用自减运算进行反向遍历.
    printf("ptr_int[%d] 地址: %p, ptr_int[%d] 地址对应值: %d\n", j, ptr_int, j, *ptr_int);    
  }

  ptr_int = &array[2];
  ptr_int += 2;                           // 指针加法运算.(往后移动两个元素位置)
  printf("ptr_int += 2 地址为: %p, 值为: %d\n", ptr_int, *ptr_int);
  ptr_int -= 1;                           // 指针减法运算.(往前移动一个元素位置)
  printf("ptr_int -= 1 地址为: %p, 值为: %d\n", ptr_int, *ptr_int);

  int *ptr_start = &array[0];
  int *ptr_end = &array[4];
  // 注意：指针运算的实质是**以指针指向的数据类型大小为单位**进行算术运算的. 因此ptr_end-ptr_start返回的并非是二者之间的所差的字节数，而是元素数目.
  printf("ptr_end - ptr_start = %td (相差的元素个数)\n", ptr_end - ptr_start);  // 指针运算.（求距离）

  {
    // 指针之间的比较运算.
    // 可用于两指针前后位置的判断.
    int *p = &array[1];
    int *q = &array[3];
    if ( p > q ) 
    {
      printf("p指向的元素在q之后.\n");
    }
    else 
    {
      printf("p指向的元素在q之前.\n");
    }
  }
  printf("===============================================\n\n");

  /*
    @result : 
      ptr_int[0] 地址: 000000000061FDD0, ptr_int[0] 地址对应值: 0
      ptr_int[1] 地址: 000000000061FDD4, ptr_int[1] 地址对应值: 1
      ptr_int[2] 地址: 000000000061FDD8, ptr_int[2] 地址对应值: 2
      ptr_int[3] 地址: 000000000061FDDC, ptr_int[3] 地址对应值: 3
      ptr_int[4] 地址: 000000000061FDE0, ptr_int[4] 地址对应值: 4
      ptr_int[5] 地址: 000000000061FDE4, ptr_int[5]的值: 0
      自减运算反向遍历  ptr_int[4] 地址: 000000000061FDE0, ptr_int[4] 地址对应值: 4
      自减运算反向遍历  ptr_int[3] 地址: 000000000061FDDC, ptr_int[3] 地址对应值: 3
      自减运算反向遍历  ptr_int[2] 地址: 000000000061FDD8, ptr_int[2] 地址对应值: 2
      自减运算反向遍历  ptr_int[1] 地址: 000000000061FDD4, ptr_int[1] 地址对应值: 1
      自减运算反向遍历  ptr_int[0] 地址: 000000000061FDD0, ptr_int[0] 地址对应值: 0
      ptr_int += 2 地址为: 000000000061FDE0, 值为: 4
      ptr_int -= 1 地址为: 000000000061FDDC, 值为: 3
      ptr_end - ptr_start = 4 (相差的元素个数)
      p指向的元素在q之前.
  */
  /*
    指针的算术运算的跨进步长与指针所指向的元素类型有关，指针所指向的数据类型决定了加减运算中指针递进的距离.
    即： int *p指针指向类型为int，(p + 1)代表指针向后移动4字节（一个int类型的大小），从而指向下一个int元素.
    而 char *p指针指向类型为char, (p + 1)则代表指针向后移动1个字节（一个char类型大小），从而指向下一个char元素.
  */
}


void pointer_example_demo_4( void )
{
  printf("=========== 空指针 ===========\n");
  // #define NULL ((void *)0);                   // C语言中空指针最常见的定义形式.是一个特殊值，表示不指向任何有效内存地址.
    /* 不同编译器/平台的NULL定义可能不同 */
  // 典型情况：
  // 1. 大多数现代系统：NULL = (void*)0
  // 2. 早期C++兼容：NULL = 0  （纯整数0）
  // 3. 某些嵌入式系统：NULL = (void*)0xFFFFFFFF  （如果0地址有效）

  // 空指针可以赋值给任意类型指针（包括通用指针(void *)）.
  int *p = NULL;
  float *f = NULL;
  double *db = NULL;
  char *ch = NULL;

  // 最好在声明一个指针后立马使用NULL进行初始化.以明确表示一个确定的状态.
  int *ptr_ex = NULL;
  printf("ptr_ex的地址: %p\n", ptr_ex);
  // printf("ptr_ex的值: %p\n", *ptr_ex);         // 注意，对空指针解引用会导致段错误!日常使用中要防止对空指针进行解引用.

  int exam = 2;
  ptr_ex = &exam;
  
  // 指针判断.
  if ( ptr_ex == NULL )                           // 显示判断 if ( pointer == NULL ){ ... }
  {
    printf("ptr_ex 为空指针(显示判断).\n");
  }
  else 
  {
    printf("ptr_ex 非空指针(显示判断).\n");
  }


  if ( !ptr_ex )                                  // 隐式比较(简洁写法.) if ( !pointer ) { ... }  
  {
    printf("ptr_ex 为空指针(隐式布尔转换判断).\n");
  }
  else 
  {
    printf("ptr_ex 非空指针(隐式布尔转换判断).\n");
  }

  // if ( ptr_ex == 0 ) { ... }                    // 易引发误解，不宜使用.


  // 指针的初始化.
  int *ptr_noInitial;                             // 未初始化指针. 危险！值不确定，可能指向任何地方.
  int *ptr_Initial = NULL;                        // 已初始化的指针，有明确状态（地址00000000）. 因此声明指针后要紧跟初始化.
  printf("ptr_noInitial 地址: %p, 值: %d\n", ptr_noInitial, *ptr_noInitial);
  printf("ptr_Initial 地址: %p\n", ptr_Initial);     // 注意：这里再次提醒一下，不能对空指针解引用！
  printf("===============================================\n\n");
  // printf("ptr_Initial 地址: %p, 值: %d\n", ptr_Initial, *ptr_Initial);     // Segmentation fault!对空指针解引用造成段错误.

  /*
    @result : 
      ptr_ex的地址: 0000000000000000
      ptr_ex 非空指针(显示判断).
      ptr_ex 非空指针(隐式布尔转换判断).
      ptr_noInitial 地址: 000000000061FDC9, 值: 0
      ptr_Initial 地址: 0000000000000000
  */
  /*
    可以看到. 声明但是未初始化的指针是随机指向一个地址的. 而这个地址的值也是不确定的. 
    因此声明指针后一定要初始化!
  */
}


void pointer_example_demo_5( void )
{
  printf("=========== 指针与const关键字 ===========\n");
  int example_ver_alpha = 50;               // 无const保护.
  int example_ver_bravo = 99;
  int *ptr_int = &example_ver_alpha;
  printf("修改前 ptr_int 所指向地址: %p, 值: %d\n", ptr_int, *ptr_int);

  *ptr_int = 25;                            // 无 const 保护，可修改指针所指地址的内容.
  printf("内容修改后 ptr_int 所指向地址: %p, 值: %d\n", ptr_int, *ptr_int);

  ptr_int = &example_ver_bravo;             // 无 const 保护，可修改指针本身指向的地址.
  printf("指针指向地址修改后 ptr_int 所指向地址: %p, 值: %d\n", ptr_int, *ptr_int);


  const int *ptr_content_protect = &example_ver_alpha;      // const int *p, p所指向的内容受保护,不能修改p所指向地址的内容.
  // int const *ptr_content_protect = &example_ver_alpha;   // 同上
  printf("修改前 const ptr_content_protect 所指向地址: %p, 值: %d\n", ptr_content_protect, *ptr_content_protect);
  // *ptr_content_protect = 66;                             // 不能修改内容.
  ptr_content_protect = &example_ver_bravo;                 // 可以修改所指向的地址.(可以指向不同地址)
  printf("修改后 const ptr_content_protect 所指向地址: %p, 值: %d\n", ptr_content_protect, *ptr_content_protect);

  int* const ptr_dir_protect = &example_ver_alpha;          // int* const p,p所指向的地址受保护，地址内容不受保护.
  printf("修改前 const ptr_dir_protect 所指向地址: %p, 值: %d\n", ptr_dir_protect, *ptr_dir_protect);
  *ptr_dir_protect = 66;                                    // 可以修改内容.
  // ptr_dir_protect = &example_ver_bravo;                  // 不可以修改指向.
  printf("修改后 const ptr_dir_protect 所指向地址: %p, 值: %d\n", ptr_dir_protect, *ptr_dir_protect);


  const int* const ptr_double_protect = &example_ver_alpha; // const int* const p, p所指向的内容及其所指向的地址都受保护,不能修改p指向的地址以及地址对应的内容.
  // *ptr_double_protect = 66;                              // 不能修改内容.
  // ptr_double_protect = &example_ver_bravo;               // 所指向的地址也不能修改.
  printf("===============================================\n\n");

  /*
    @result : 
      修改前 ptr_int 所指向地址: 000000000061FDFC, 值: 50
      内容修改后 ptr_int 所指向地址: 000000000061FDFC, 值: 25
      指针指向地址修改后 ptr_int 所指向地址: 000000000061FDF8, 值: 99
      修改前 const ptr_content_protect 所指向地址: 000000000061FDFC, 值: 25
      修改后 const ptr_content_protect 所指向地址: 000000000061FDF8, 值: 99
      修改前 const ptr_dir_protect 所指向地址: 000000000061FDFC, 值: 25
      修改后 const ptr_dir_protect 所指向地址: 000000000061FDFC, 值: 66
  */
}


void pointer_example_demo_6( void )
{
  printf("=========== 指针与数组关系 ===========\n");
  
  int array[4] = { 1, 2, 3, 4 };
  int *ptr_int = array;                   // 数组名即首元素地址.

  ptr_int = &array[2];                     // 指针可以修改指向.

  // 数组名不能修改指向,但是可以修改指向地址的内容. 其类型等价于 int* const p.
  // array = &array[2];                    // 编译错误. 数组名是常量指针，不能修改指向.
  *array = 99;                             // 数组名可以指向的内容. 等价于 array[0] = 99; 

  // 数组的下标引用 与 指针遍历是等价的. 即 array[j] 等价于 *(array + j).
  for( int j = 0; j < sizeof(array)/sizeof(array[0]); j++ )
  {
    printf("(下标遍历)array[%d] 值为 %d\n", j, array[j]);
  }

  printf("\n");

  for( int j = 0; j < sizeof(array)/sizeof(array[0]); j++ )
  {
    // 前面说过数组名作为常量指针不能够修改其指向，而此处*(array + j)却通过编译. 是因为此处借助数组名array进行偏移所得的结果是临时变量，并没有修改array的指向.
    // *(array + j)可以通过编译, 而 array = (array + j);  *array; 这两句是无法通过编译的，因为修改了常量指针.
    printf("(指针遍历)array[%d] 值为 %d\n", j, *(array + j)); 
  }


  // 数组名在大多数表达式中会“退化”为指向首元素的指针，但在两种情况下保持数组类型：
  // 1.
  {
    // 在使用sizeof(数组名)时，会得出整个数组的长度;当使用sizeof(指针)时，则固定输出指针大小(64位系统指针占8字节).这是数组名与指针的区别之一.
    printf("sizeof(array) = %zu\n", sizeof(array));     // 输出:16 (若int占4字节).返回整个数组的字节数，而非指针大小.
    printf("sizeof(ptr_int) = %zu\n", sizeof(ptr_int)); // 输出:8 (指针大小).
  }

  // 2.
  {
    int (*p)[4] = &array;                      // 注意:array是指向首元素的地址.&array是整个数组的指针(相当于对数组类型取地址),其值与数组首元素地址相同，但步长不同.
    printf("array首元素地址: %p\n", array);
    printf("array数组地址: %p\n", p);

    // 指向数组地址的指针与数组首元素指针所跨越的步长是不同的！
    printf("(首元素地址)array + 1 地址: %p, 步长: %td\n",(array + 1), (char *)(array + 1) - (char *)(array));
    printf("(数组地址)p + 1 地址: %p, 步长: %td\n", (p + 1), (char *)(p + 1) - (char *)(p));
    /*
      @result : 
        array首元素地址: 000000000061FDF0
        array数组地址: 000000000061FDF0
        (首元素地址)array + 1 地址: 000000000061FDF4, 步长: 4
        (数组地址)p + 1 地址: 000000000061FE00, 步长: 16
    */
    /*
      p为指向整个数组的指针 步长为整个数组的大小.
      array为指向元素首地址的指针. 步长为一个 sizeof(int);
    */
  }

  // 当数组名传递给函数时，它退化为指针，sizeof 无法获得数组长度.
  utils_display(array);
}

static void utils_display( int array[] )
{
  // 数组名作为函数参数传入时,发生退化. 等价于 int *array. 调用sizeof(array)返回指针的大小，而不是数组的大小！ 
  printf("传入的数组大小: %d\n", sizeof(array));
}


void pointer_example_demo_7( void )
{
  printf("=========== 多级指针 ===========\n");

  int value = 30;
  int *ptr_p1 = &value;                    // 一级指针,指向int的指针.
  printf("示例变量 value 的地址: %p, 值: %d\n", &value, value);
  printf("ptr_p1 自身地址: %p, ptr_p1 所指向的地址: %p, 地址所对应的值: %d\n", &ptr_p1, ptr_p1, *ptr_p1);

  int **ptr_p2 = &ptr_p1;   // 二级指针. 指向指针的指针.p2有一个自身的地址，p2指向的地址为p1自身的地址.
  // 注意!p2指向的为p1，*(p2) 对p2解一次引用得到的是p1的地址. *(*p2) 对p2解两次引用才能拿到底层p1所指向的内存的值.
  printf("ptr_p2 自身地址: %p, ptr_p2 所指向的地址: %p, 地址所对应的值: %d\n", &ptr_p2, ptr_p2,*(*(ptr_p2)));  

  
  int ***ptr_p3 = &ptr_p2;  // 三级指针. 指向指针的指针的指针. p3有一个自身地址，p3指向的地址为p2自身的地址.
  // 注意！p3指向为p2,*(p3) 对p3解一次引用得到p2, *(*(p3)) 对p3解两次引用得到p1, *(*(*(p3))) 对p3解三次引用才能拿到p1所指向内存的值.
  printf("ptr_p3 自身地址: %p, ptr_p3 所指向的地址: %p, 地址所对应的值: %d\n", &ptr_p3, ptr_p3,*(*(*(ptr_p3))));  

  *ptr_p1 = 40;
  printf("一级指针ptr_p1修改后. value的值: %d\n", value);

  *(*ptr_p2) = 50;
  printf("二级指针ptr_p2修改后. value的值: %d\n", value);

  *(*(*ptr_p3)) = 60;
  printf("三级指针ptr_p3修改后. value的值: %d\n", value);
  /* 
    多级指针是链式结构. 上述三级指针调用链如下： p3 -> p2 -> p1 -> value. 
    更高级(4级 5级 ...)指针以此类推.每增加一级指针，就需要多一次解引用才能获取实际数据.
    每一级指针都是独立的变量，都有自己的内存地址.
    可以通过任意级别的指针修改到最终数据.
  */

  // 多级指针 与 const 关键字.
  // 对于多级指针的const保护，其规律是:从变量名开始从右向左逐层解析,每遇到一个 * 就表示一层指针,const 修饰它左边最近的那个 *.
  // 被修饰的指针本身不可修改指向,最左边的 const 修饰最终的数据，决定其读写权限.
  int value_2 = 10;
  const int *p1 = &value_2;   // 根据规律. p1可修改指向. 但*p1不能进行修改.
  // *p1 = 50;                // 不能通过编译 *p1不能修改.
  //p1 = &value;                // 通过编译  p1指向可修改.

  const int **pp1 = &p1;      // 同理. pp1未被const修饰, *pp1未被修饰, 因此均可以更改指向和内容.被修饰的是**pp1，因此**pp1不能修改.
  // pp1 = (const int *)&ptr_p1;    // 通过编译. pp1指向可改.
  // *pp1 = &value;                 // 通过编译. *pp1可改.
  // **pp1 = 50;                    // 不通过编译. **pp1不能改.

  const int ***ppp1 = &pp1;   // ppp1 *ppp1 **ppp1 均未被修饰，均可改. ***ppp1被const修饰，不可更改.
  // ***ppp1 = 50;            // 不通过编译. ***ppp1不可更改.

  // 从变量名ppp2开始, ppp2未被修饰，可以修改;*ppp2被const修饰，为常量指针,不可修改;**ppp2也被const修饰,不可修改;***ppp2未被修饰，可以修改.
  int *const *const *ppp2 = &pp1;         // ps:此处赋值操作存在类型不匹配问题,但是不影响最终编译,只做示例使用.
  // ppp2 = &ptr_p2;          // 通过编译. ppp2指向可变.
  // *ppp2 = &ptr_p1;         // 不通过编译. *ppp2不可修改.
  // **ppp2 = &value;         // 不通过编译. **ppp2不可修改.
  // ***ppp2 = 60;            // 通过编译. ***ppp2可以修改.

  /* 4级及以上指针与const的关系 以此类推 */
  printf("===============================================\n\n");

  /*
    @result:
      示例变量 value 的地址: 000000000061FE0C, 值: 30
      ptr_p1 自身地址: 000000000061FE00, ptr_p1 所指向的地址: 000000000061FE0C, 地址所对应的值: 30
      ptr_p2 自身地址: 000000000061FDF8, ptr_p2 所指向的地址: 000000000061FE00, 地址所对应的值: 30
      ptr_p3 自身地址: 000000000061FDF0, ptr_p3 所指向的地址: 000000000061FDF8, 地址所对应的值: 30
      一级指针ptr_p1修改后. value的值: 40
      二级指针ptr_p2修改后. value的值: 50
      三级指针ptr_p3修改后. value的值: 60
  */
}


void pointer_example_demo_8( void )
{
  printf("=========== 函数指针 ===========\n");

  void (*__ptr_to_voidfunc)( void );    // 声明一个函数指针. 返回 void 类型，传入参数为 void.

  __ptr_to_voidfunc = function_A;       // 将function_A地址赋值给__ptr_to_voidfunc.
  // __ptr_to_voidfunc = &function_A;   // 和上面完全等价,因为函数名本身就会隐式地转换为指向该函数的指针.

  __ptr_to_voidfunc();                  // 指针形式函数调用.

  int (*__ptr_to_intfunc)( int, int ) = function_B_Sub; // 定义一个int (*p)( int, int )类型函数指针.
  printf("function_B_Sub has been called. x - y = %d\n", __ptr_to_intfunc(10, 5));

  // 函数指针数组.
  int (*operator[4])( int, int ) = { function_B_Sub, function_C_Add, function_D_Mult, function_E_Divide };
  char *operations_names[] = { "减法", "加法", "乘法", "除法" };

  for( int j = 0; j < sizeof(operator)/sizeof(operator[0]); j++ )
  {
    printf("%s: %d\n", operations_names[j], operator[j](10, 5));
  }

  // 注意! 声明函数指针时必须加括号(). int *p(int,int)这是一个返回int *指针的函数.
  // int (*p)(int, int) 这才是一个指向函数的指针.
  // int *__ptr_to_func( int, int );        // 声明一个返回int *指针的函数.
  // __ptr_to_func = function_B_Sub;  // 不通过编译.


  // 函数指针类型与函数必须完全匹配（返回类型匹配,参数匹配）.
  // int (*__ptr_to_func)(int);
  // __ptr_to_func = function_C_Add;     // 编译报警！类型不匹配.

  // void (*__ptr_to_funca)(int, int);
  // __ptr_to_funca = function_D_Mult;   // 编译报警! 类型不匹配.


  int (*__ptr_to_func)(int, int) = NULL;  // 函数指针也可使用空指针进行初始化.
  // __ptr_to_func(5 ,10);                // 危险！调用空函数指针.
  if ( !__ptr_to_func ) printf("函数指针为空\n");   // 在调用函数指针前先进行检查.
  else  __ptr_to_func(5, 10);

  printf("===============================================\n\n");
}


void pointer_example_demo_9( void )
{
  printf("=========== void 万能指针 ===========\n");

  int a = 123;
  float b = 456.7;
  char c = 'v';

  // 隐式转换. C语言中,任意类型指针隐式转换为void *类型.
  void *ptr_void1 = &a;
  void *ptr_void2 = &b;
  void *ptr_void3 = &c;

  //  不能对void *指针解引用！
  // *ptr_void1 = 789;      // 无法通过编译. 编译器不知道要读写多少字节，也不知道如何解释这些字节。

  // 同样，不能对void *指针进行指针运算. 
  // 在标准C语言中是不允许使用void *进行算术运算的，因为 void 是一个不完整的类型，其大小未知，编译器无法确定指针移动的步长. 
  // 此处能够通过编译,并且sizeof(void)值为1，是因为所使用的编译器(GCC)提供了语言拓展，将void *的算术运算视为了char *的算术运算.这不是标准行为，容易导致错误逻辑。
  int array[3] = { 0, 1, 2 };
  void *vp = array;
  printf("sizeof(void) 值: %d\n", sizeof(void));
  printf("array[0] 地址: %p\n", array);
  printf("array[1] 地址: %p\n", ++vp);

  // 必须先经过强制类型转换才能解引用.
  *(int *)ptr_void1 = 789;
  printf("ptr_void1 值: %d\n", *(int *)ptr_void1);

  *(char *)ptr_void3 = 'b';
  printf("ptr_void3 值: %c\n", *(char *)ptr_void3);

  // 转换的类型决定了编译器对该指针所指变量的解释方式.
  int ver = 0xABCDEFFF;
  void *cvp = &ver;
  printf("转换成char: %02X\n", *(unsigned char *)cvp);    // 读取一个字节. 解释为 char 变量.
  printf("转换成short: %04X\n", *(unsigned short *)cvp);  // 读取两个字节. 解释为 short 变量.
  printf("转换成int: %08X\n", *(unsigned int *)cvp);      // 读取4个字节. 解释为 int 变量.
  /*
    @result:
      sizeof(void) 值: 1
      array[0] 地址: 000000000061FDE0
      array[1] 地址: 000000000061FDE1
      ptr_void1 值: 789
      ptr_void3 值: b
      转换成char: FF
      转换成short: EFFF
      转换成int: ABCDEFFF
  */

  printf("===============================================\n\n");
}


void function_A( void )
{
  printf("function_A has been called.\n");
}

int function_B_Sub( int x, int y )
{
  return x - y;
}

int function_C_Add( int x, int y )
{
  return x + y;
}

int function_D_Mult( int x, int y )
{
  return x * y;
}

int function_E_Divide( int x, int y )
{
  return x / y;
}
