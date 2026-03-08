
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
              int const *p;           // 保护指针p本身
              const int const *p;     // 双重保护
              int *p;                 // 无保护
          ------------------------------------


  =========================================================================

*/

#include <stdio.h>
#include <locale.h>
#include <windows.h>

// -------------------------------------------------
  // 各个示例函数.
  void pointer_example_demo_1( void );
  void pointer_example_demo_2( void );
  void pointer_example_demo_3( void );
  void pointer_example_demo_4( void );


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
      ptr_int += 2 地址为: 000000000061FDDC, 值为: 3
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


}
