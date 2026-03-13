/*
  @file       array_basics.c
  @Author     R6bandito
  @brief      C数组的基础知识相关（定义，初始化，访问，数组名指针特性，多维数组等）.


  =========================================================================
    1.什么是数组？
      |__ 数组是一个存储固定大小的相同类型元素的**顺序集合**.(内部存储其一系列相同类型的变量).
      |__ 数组在内存中的布局是连续的.最低的地址对应第一个元素,最高的地址对应最后一个元素.


    2.数组的访问.
      |__ 数组可以使用下标进行访问.
        ------------------------------------
          example:
            int arr[10]; arr[2] = 5;
        ------------------------------------

      |__ 数组也支持指针访问. 指针访问与下标访问是等价的.
        ------------------------------------
          example:
            int arr[10]; *(arr+2) = 5;
        ------------------------------------

      |__ 注意 数组通过下标进行访问时，其下标索引的基地址为0(从 0 开始访问).int arrayInt[5]的最大访问索引为 arrayInt[4]. arrayInt[5]属于越界访问，危险！
      

    3.数组名的特性.
      |__ 数组名表示数组的地址,即数组首元素的地址.当我们在声明和定义一个数组时,该数组名就代表着该数组的地址.
      |__ 数组名本身是一个常量指针,如 int *const p 类型,可以修改地址的内容(相当于修改首元素值),但是不能修改指向!一旦确定，就不能再指向其他地方.
      |__ 大多数情况下，数组名会退化为指向数组首元素的指针. 例如将一维数组作为参数传递给函数时.
        ------------------------------------
          tips:
            数组名本身就是已经是指向首元素的地址. 以下两种方式都可以将指针指向首元素.
            ①int array[10];  int *start = array;
            ②int array[10];  int *start = &array[0];
            
            int array[10];   int (*start)[10] = &array; // 这里对array再取地址，得到的是指向整个数组的指针!(相当于对数组类型取地址).不要与上面两种搞混了.
         ------------------------------------ 

      |__ 数组名本身不是变量，而是一个固定地址的符号，编译器在编译时将其替换为数组首地址.
      |__ 字符串字面量（如 "hello"）本身也是数组,存储在只读数据区. 可以用来初始化字符数组，或者退化为指针.

      
    4.数组的定义与初始化.
      |__ 在 C 中要声明一个数组，需要指定元素的类型和元素的数量.
        ------------------------------------
          example:
            type arrayName [ arraySize ];   // 其中 type 为C中任意允许的数据类型. arraySize 为数组的长度（容量）.
            int arrayInt[10];               // 声明了arrayInt是一个整形数组, 其大小为10个int(最多能够连续线性存储10个int类型数据).
        ------------------------------------  

      |__ 初始化数组有两种方式. ①通过循环依次访问数组中每个成员，依次进行初始化.②声明数组后通过初始化语句就行初始化.
        ------------------------------------
          example:
            int arrayInt[5] = { 0, 1, 2, 3, 4 };      // 通过初始化语句，一句中初始化每个成员.arrayInt[0] = 0 ... arrayInt[4] = 4.
            int arrayInt[5] = { 0, 1, 2 };            // 部分初始化. 剩余元素(arrayInt[3], arrayInt[4]) 自动为0.
            int arrayInt[5] = { 0 };                  // 所有元素初始化为0.
            int arrayInt[] = { 1, 2, 3 };             // 声明之后紧接初始化时，可以省略元素的数量. 会根据初始化的元素数量进行自动推导(arrayInt的长度被自动推导为3).
            int arrayInt[5] = { [2] = 5, [4] = 10 };  // C99标准引入的指定初始化器语法，它允许在初始化数组时，通过 [下标] = 值 的形式显式指定要初始化的元素.其余未指定的元素自动初始化为 0.
            int arrayInt[5];                          // 未进行初始化. 其内容随机,可能引发未定义行为.
        ------------------------------------

      |__ 全局变量或 static 局部数组若未显式初始化，会被自动初始化为 0(对于整型)或 '\0'(对于字符型).
      |__ 局部数组(在函数内部定义的、未加 static 修饰的数组)不会自动初始化. 它的初始值是不确定的(即之前栈上遗留的随机值)，直接使用可能导致未定义行为.
      |__ 数组只能在定义时进行整体初始化. 定义结束后，后续不能够对数组进行整体赋值.只能逐个元素赋值或使用memcpy.


*/


#include <stdio.h>
#include <locale.h>
#include <windows.h>

// -------------------------------------------------
  // 各个示例函数.
  void array_example_demo1( void );
  void array_example_demo2( void );
  void array_example_demo3( void );
  void array_example_demo4( void );
  void array_example_demo5( void );


  void utils_displayintArrayLength( int *array );
  char **get_str( void );
  char *get_str_array( void );
// -------------------------------------------------

int global_arrayInt[5];            // 全局数组. 自动初始化为0.

int main( void )
{
  // 设置程序 locale 为 UTF-8
  setlocale(LC_ALL, ".UTF-8");
  // 设置控制台输出代码页为 UTF-8
  SetConsoleOutputCP(CP_UTF8);

  array_example_demo1();
  array_example_demo2();
  array_example_demo3();
  array_example_demo4();
  array_example_demo5();

  return 0;
}


void array_example_demo1( void )
{
  printf("=========== 数组的定义与初始化 ===========\n");

  int arrayInt[5];                                // 定义了一个 int 类型，长度为5的整形数组.栈上未初始化的局部数组. 其值随机.
  for( int j = 0; j < sizeof(arrayInt)/sizeof(arrayInt[0]); j++ )
  {
    printf("arrayInt[%d] = %d\n", j, arrayInt[j]);
  }
  printf("\n");

  
  static int arrayStaticInt[5];                       // static修饰的数组，未初始化，其成员值自动初始化为0.


  char arrayChar[4] = { 'a', 'b', 'c', 'd' };     // 定义了一个 char 类型，长度为4数组.通过初始化语句分别进行初始化.
  for( int j = 0; j < sizeof(arrayChar)/sizeof(arrayChar[0]); j++ )
  {
    printf("arrayChar[%d] = %c\n", j, arrayChar[j]);
  }
  printf("\n");


  int arrayIntA[4] = { 1, 2 };                     // 部分初始化. 剩余未显示初始化的元素自动为0.
  for( int j = 0; j < sizeof(arrayIntA)/sizeof(arrayIntA[0]); j++ )
  {
    printf("arrayIntA[%d] = %d\n", j, arrayIntA[j]);
  }
  printf("\n");


  int arrayIntB[4] = { 0 };                         // arrayIntB所有元素全部初始化为0.


  int arrayIntC[4] = { [1] = 255, [3] = 255 };      // C99 指定初始化器对数组进行初始化. arrayIntC[1] 与 arrayIntC[3] 为255. 其余元素自动为0.
  for( int j = 0; j < sizeof(arrayIntC)/sizeof(arrayIntC[0]); j++ )
  {
    printf("arrayIntC[%d] = %d\n", j, arrayIntC[j]);
  }
  printf("\n");


  char arrayCharA[] = { 'a', 'b', 'c' };            // 省略数组长度(元素数量)的初始化. 其中数组长度被自动推导为3.
  printf("arrayCharA 的长度: %zu\n", sizeof(arrayCharA)/sizeof(arrayCharA[0])); 
  for( int j = 0; j < sizeof(arrayCharA)/sizeof(arrayCharA[0]); j++ )
  {
    printf("arrayCharA[%d] = %c\n", j, arrayCharA[j]);
  }
  printf("\n");


  // 可用字符串字面量来初始化数组(字符数组).编译器会自动添加结尾的 '\0'.
  // 注意：若数组大小小于字符串长度.则会因为缺失'\0'，从而导致编译错误或截断等问题(取决于编译器).
  char string1[] = "hello";                           // string1长度自动推导为 6（含'\0'）.
  char string2[10] = "hello";                         // 长度为10. 初始化后，空余的4个元素(string[6] ~ string[9])自动补'\0'.
  char string3[5] = "Mayuy";                          // ！数组过小，未能添加'\0'结束符!
  printf("string3 is: %s\n", string3);                // 读取越界！由于string3没有添加'\0'，printf打印时一路读取直到遇到一个'\0'为止(溢出读取).
  printf("\n");

  const int arrayIntConst[5] = { 11, 22, 33 };        // const修饰的数组必须在定义时进行初始化.之后不能修改任何元素.
  // arrayIntConst[2] = 44;                           // 非法操作.

  int arrayIntD[4] = { 0 };
  // arrayIntD = { 4, 5, 6, 7 };                      // 非法！数组只能定义时使用初始化语句整体进行初始化，定义后，不能再对其进行整体赋值!

  printf("===============================================\n\n");

  /*
    @result:
    arrayInt[0] = 0
    arrayInt[1] = 0
    arrayInt[2] = 0
    arrayInt[3] = 0
    arrayInt[4] = 0

    arrayChar[0] = a
    arrayChar[1] = b
    arrayChar[2] = c
    arrayChar[3] = d

    arrayIntA[0] = 1
    arrayIntA[1] = 2
    arrayIntA[2] = 0
    arrayIntA[3] = 0

    arrayIntC[0] = 0
    arrayIntC[1] = 255
    arrayIntC[2] = 0
    arrayIntC[3] = 255

    arrayCharA 的长度: 3
    arrayCharA[0] = a
    arrayCharA[1] = b
    arrayCharA[2] = c

    string3 is: Mayuyhello  // 理想情况应该打印:Mayuy   但此处打印了Mayuyhello.证明由于缺失'\0'，导致读取时读取到了栈上其它变量的位置.一直读取直到遇到第一个'\0'.
  */
}


void array_example_demo2( void )
{
  printf("=========== 数组的访问 与 数组名特性 ===========\n");

  int array_Int[5] = { 1, 2, 3, 4, 5 };
  for( int j = 0; j < sizeof(array_Int)/sizeof(array_Int[0]); j++ )
  {
    printf("(下标访问) array_Int[%d] = %d\n", j, array_Int[j]);   // 通过下标进行访问.
  }

  array_Int[2] = 66;                                             // 通过下标修改指定元素.
  array_Int[4] = 99;
  // array_Int[5] = 100;                                         // 注意！数组的下标访问索引是从0开始的！array_Int[5]访问越界！ 
  for( int j = 0; j < sizeof(array_Int)/sizeof(array_Int[0]); j++ )
  {
    printf("(下标访问) 修改后array_Int[%d] = %d\n", j, array_Int[j]);   
  }
  printf("\n");


  int array_IntA[4] = { 1, 2, 3, 4 };
  for( int j = 0; j < sizeof(array_IntA)/sizeof(array_IntA[0]); j++ )
  {
    printf("(指针访问) array_IntA[%d] = %d\n", j, *(array_IntA + j)); // 通过指针进行访问.(数组名表示数组首元素的地址,此处直接将数组名作为指针使用)
    // printf("(指针访问) array_IntA[%d] = %d\n", j, *(array_IntA++)); // Ps:  *(array_IntA++)是无法通过编译的.原因在于数组名是常量指针,不能够改变其指向!
  }
  *(array_IntA + 2) = 30;                                           // 通过指针运算的方式访问数组元素.并进行修改.((array_IntA + 2)指向数组中第三个元素)
  *(array_IntA + 3) = 40;                                           // (array_IntA + 3)指向数组中最后一个元素.
  for( int j = 0; j < sizeof(array_IntA)/sizeof(array_IntA[0]); j++ )
  {
    printf("(指针访问) 修改后array_IntA[%d] = %d\n", j, *(array_IntA + j)); 
  }
  printf("\n");


  // 下标访问与数组名访问是等价的.
  int array[5] = { 1, 2, 3, 4, 5 };
  array[1] = 22;
  *(array + 1) = 222;               // 等价于 array[1].
  printf("修改后 array[1] = %d\n", array[1]);

  array[3] = 44;                    // 下标访问,等价于 *(array + 3),合理.
  4[array] = 55;                    // 下标访问,等价于 *(4 + array),同样合理，但是写法诡异，不推荐.
  for( int j = 0; j < sizeof(array)/sizeof(array[0]); j++ )
  {
    printf("array[%d] = %d\n", j, *(array + j)); 
  }


  // 数组下标的特性.
  // 数组名 与 指针 十分类似. 但是也有不同的地方.
  // ① 数组名是符号，不占用内存空间. 在编译时会替换成数组首元素的地址 / 而指针变量需要占用内存空间.
  // ② 数组名是常量地址，不可以修改其指向！/ 而（非const修饰的常量指针）指针可以修改指向其它对象.
  int myarrayA[] = { 1, 2 };
  int myarrayB[] = { 3, 4 };
  // myarrayA = myarrayB;             // 通过数组名赋值将myarrayA重新指向myarrayB. 非法操作.
  int *ptr_to_array = myarrayA;
  ptr_to_array = myarrayB;              // 通过指针的方式. 将指针从myarrayA 重新指向到 myarrayB. 合法.

  // ③ 数组名解引用获得数组首元素的值.在此基础上进行偏移可以访问到数组中其余元素（偏移时不能改变原先数组名指向！）.
  int myarrayC[] = { 1, 2, 3 };
  printf("myarrayC[2] = %d\n", *(myarrayC + 2));  // 合法. 通过偏移方式访问myarrayC中最后一个元素值.
  // printf("myarrayC[2] = %d\n", *(myarrayC++)); // 不合法!自增运算改变了数组名的指向！

  // ④ sizeof(数组名) 得到整个数组的大小 / 而 sizeof(指针) 得到指针的所占空间大小.
  printf("sizeof(myarrayC) = %zu\n", sizeof(myarrayC));           // 输出 12.(Bytes)
  printf("sizeof(ptr_to_array) = %zu\n", sizeof(ptr_to_array));   // 64位系统输出 8;32位系统输出 4. Ps: 64位系统指针所占空间为8字节, 32位系统指针所占为4字节.

  // ⑤ 对数组名取地址(&)将得到指向整个数组的指针 / 对指针取地址得到指针变量自身的地址.
  int (*__ptr)[3] = &myarrayC;              // __ptr是一个指向整个数组的指针.
  int **pp1 = &ptr_to_array;                // pp1是一个指向ptr_to_array变量的指针.
  // 指针 p1 与 __ptr 步长不同！
  int *p1 = myarrayC;
  printf("myarrayC 首地址: %p\n", myarrayC);
  printf("__ptr + 1: %p\n", (__ptr + 1));   // __ptr为指向一个int[3]数组的指针. __ptr + 1前进12个字节（一个数组）.
  printf("pp1 + 1: %p\n", (p1 + 1));       // p1为一个指向int类型的指针. p1 + 1前进4个字节（一个int）.


  // ⑦ 数组名在大部分情况下会发生退化.在大多数使用数组名的表达式中，数组名会隐式转换为指向其第一个元素的指针.这种转换称为数组到指针的退化.
  // 退化后的指针类型是指向数组元素类型的指针.
  int myarrayD[4] = { 1, 2, 3, 4 };
  int *ptr_to_int = myarrayD;             // 数组名发生退化. myarrayD退化为指向myarrayD[0]的指针. 即 int *ptr_to_int = &myarrayD[0];
  
  // 数组名作为参数传递时，也会发生数组名的退化.
  printf("未传参时 array 数组的长度: %zu Bytes\n", sizeof(myarrayD));
  utils_displayintArrayLength(myarrayD);  // 数组名作为函数参数传入，退化为指针. 相当于传入 int *p;

  // 也有部分情况不会发生数组名的退化.
  size_t myarrayDLength = sizeof(myarrayD); // sizeof操作数. 不会发生数组名的退化（除开因为传参导致的数组名退化）.
  int (*ptr_to_D)[4] = &myarrayD;   // 对数组名取地址时，不会发生退化，而是得到一个指向对应数组的指针.
  int **pdouble = &ptr_to_int;      // 对指针变量本身取地址，得到一个二级指针.二者是有区别的，要仔细分辨.

  // ⑧ 字符串字面量本身也是一个数组.可用于初始化指针或初始化数组.
  char str[] = "Hello,World";     // str为一个字符数组.其内拷贝字符串字面量"Hello,World"的数据（包含'\0'）.
  printf("str 大小: %zu\n", sizeof(str));
  printf("str + 4 = %c\n", *(str + 4));
  // printf("str + 1 = %c\n", *str++);    // 非法！数组名不能修改指向.

  char *ptr_toChar = "Hello,World"; // ptr_toChar 指向字符串字面量的首字符.
  printf("ptr_toChar + 4 = %c\n", *(ptr_toChar + 4));
  printf("ptr_toChar + 1 = %c\n", *ptr_toChar++);

  printf("===============================================\n\n");
  /*
    @result: 
      (下标访问) array_Int[0] = 1
      (下标访问) array_Int[1] = 2
      (下标访问) array_Int[2] = 3
      (下标访问) array_Int[3] = 4
      (下标访问) array_Int[4] = 5
      (下标访问) 修改后array_Int[0] = 1
      (下标访问) 修改后array_Int[1] = 2
      (下标访问) 修改后array_Int[2] = 66
      (下标访问) 修改后array_Int[3] = 4
      (下标访问) 修改后array_Int[4] = 99

      (指针访问) array_IntA[0] = 1
      (指针访问) array_IntA[1] = 2
      (指针访问) array_IntA[2] = 3
      (指针访问) array_IntA[3] = 4
      (指针访问) 修改后array_IntA[0] = 1
      (指针访问) 修改后array_IntA[1] = 2
      (指针访问) 修改后array_IntA[2] = 30
      (指针访问) 修改后array_IntA[3] = 40

      修改后 array[1] = 222
      array[0] = 1
      array[1] = 222
      array[2] = 3
      array[3] = 44
      array[4] = 55
      myarrayC[2] = 3
      sizeof(myarrayC) = 12
      sizeof(ptr_to_array) = 8
      myarrayC 首地址: 000000000061FD5C
      __ptr + 1: 000000000061FD68
      pp1 + 1: 000000000061FD60
      未传参时 array 数组的长度: 16 Bytes
      传入参数时 array 数组的长度: 8 Bytes
      str 大小: 12
      str + 4 = o
      ptr_toChar + 4 = o
      ptr_toChar + 1 = H
  */
}

void utils_displayintArrayLength( int *array )
{
  // 注意：虽然调用处传入的是数组名， 但是退化为了 int *指针类型. 因此sizeof(array)固定为8字节(64位系统).
  // 为了解决这个问题,通常需要重新设计接口，额外传递数组长度.
  printf("传入参数时 array 数组的长度: %zu Bytes\n", sizeof(array));
}


void array_example_demo3( void )
{
  printf("=========== 字符数组 与 字符指针 区别 ===========\n");

  // 字符数组 char str[] = "C programing"; 是在内存中（通常在栈上或静态存储区）分配一块连续存储空间.并且将字符串"C programing"的内容拷贝到这块空间中.
  // 数组名 str 是这块空间的起始地址. str 是常量，不能修改其指向（也就是不能作为左值进行赋值操作）.
  char str[] = "C programing";
  // str = "hello,world";       // 非法操作! str 为常量 不能进行修改.

  // 字符串字面量"C programing"存储在只读数据段,指针变量p自身存储在栈上（若定义在局部）.p保存了该字符串的首地址.p本身可以修改指向其它地址.
  char *p = "C programing";
  p = "Hello,World";            // p可以指向其它地址.

  printf("修改前 str: %s\n", str);
  printf("修改前 p: %s\n", p);
  printf("\n");


  // 字符数组 str 的内容是可以修改的.(除非使用 const 限定)
  str[2] = 'P';
  printf("修改后 str: %s\n", str);

  // 不能通过指针修改字符串字面量.
  //*(p + 6) = 'w';   // 危险操作! 试图通过指针修改只读数据区的字符串字面量. 编译通过，运行出现段错误.
  //printf("修改后 p: %s\n", p);

  // 字符串字面量存储在常量数据区. 因此生命周期在整个程序运行阶段都存在. 但是不能依赖此特性返回局部指针！
  // char **pptr_to_str = get_str();
  // printf("get_str : %s\n", *pptr_to_str);     // 危险!! 访问已经销毁的指针变量.

  char *ptr_to_array = get_str_array();
  printf("ptr_to_array : %s\n", ptr_to_array);  // 危险！返回的局部数组. 尽管地址已经被调用者拿到，但是在作用域结束后 str 数组已被释放，指针访问到被销毁的数据. 行为未定义.

  printf("===============================================\n\n");
  /*
    @result:
      修改前 str: C programing
      修改前 p: Hello,World

      修改后 str: C Programing
      ptr_to_array : (null)
  */
}

char **get_str( void )
{
  char *ptr = "hello";

  // 危险！ptr为局部变量,尽管 "hello" 生命周期在整个程序运行阶段，但是ptr本身存储在栈空间中，作用域结束时被销毁.
  // 此处返回的ptr的地址 实际上一但离开作用域就被销毁了.
  return &ptr;                                          // 编译报出警告: 返回局部变量的地址.
}

char *get_str_array( void )
{
  // 同样的，不能返回局部的数组. 一但离开作用域，局部数组str就被销毁.
  // 而返回的地址就算被调用方正确拿到，但是由于地址指向的内存空间已经销毁，因此再访问时就会出现未定义行为.
  char str[] = "Hello, C";

  return str;
}


void array_example_demo4( void )
{
  printf("=========== 多维数组 ===========\n");

  // 多维数组最简单的形式是二维数组,二维数组的定义与一维数组类似.
  // 二维数组在本质上是一维数组的一个列表，声明一个 x 行 y 列的二维整型数组，形式如下：
  //          typename arrayName[x][y] = { 初始化语句... };
  // 其中. typename 可以是任意C语言的有效类型；x与y 分别是 二维数组的两个维度.

  int array[3][4];          // 声明一个3行4列的int类型数组.这是一个包含3个数组的数组，每个子数组有4个int.
  int (*p)[3][4] = &array;   // 注意：二维数组的类型为 int[x][y]，而不是 int**.

  // 多维数组的初始化.
  // ① 使用初始化语句进行完全初始化.
  int array_A[2][3] = { {0, 1, 2}, {3, 4, 5} };
  for( int i = 0; i < sizeof(array_A)/sizeof(array_A[0]); i++ )
  {
    for( int j = 0; j < sizeof(array_A[0])/sizeof(array_A[0][0]); j++ )
    {
      printf("array_A[%d][%d] = %d\t", i, j, array_A[i][j]);
    }
    printf("\n");
  }

  // ② 连续初始化.(从第一行开始，按照顺序逐行进行填充)
  printf("\n");
  int array_B[2][3] = { 0, 1, 2, 3, 4, 5 };
  for( int i = 0; i < sizeof(array_B)/sizeof(array_B[0]); i++ )
  {
    for( int j = 0; j < sizeof(array_B[0])/sizeof(array_B[0][0]); j++ )
    {
      printf("array_B[%d][%d] = %d\t", i, j, array_B[i][j]);
    }
    printf("\n");
  }

  // ③ 同样支持部分初始化.
  printf("\n");
  int array_C[2][3] = { {1}, {3, 4} };  // 等价于 { {1, 0, 0}, {3, 4, 0} }.
  for( int i = 0; i < sizeof(array_C)/sizeof(array_C[0]); i++ )
  {
    for( int j = 0; j < sizeof(array_C[0])/sizeof(array_C[0][0]); j++ )
    {
      printf("array_C[%d][%d] = %d\t", i, j, array_C[i][j]);
    }
    printf("\n");
  }

  // ④ C99特性引入的指定初始化方式.
  printf("\n");
  int array_D[2][3] = { [0][1] = 9, [1][2] = 8 }; // array_D[0][1] = 9, array_D[1][2] = 8.其余元素初始化为0.
  for( int i = 0; i < sizeof(array_D)/sizeof(array_D[0]); i++ )
  {
    for( int j = 0; j < sizeof(array_D[0])/sizeof(array_D[0][0]); j++ )
    {
      printf("array_D[%d][%d] = %d\t", i, j, array_D[i][j]);
    }
    printf("\n");
  }
  printf("\n");


  // 同样，编译器可推导第一维的大小，但是必须知道其它维度.
  int array_E[][3] = {{0, 1}, {2, 4}, {3, 6}};    // 自动推导为 int array_E[3][3];
  // int array_F[][] = { {1}, {2} };              // 错误! 编译器不知道子数组(第二维)大小.

  // 多维数组同一维数组一样，可以采用 下标访问 与 指针访问.
  int array_G[2][3] = { {1, 2, 3}, {4, 5, 6} };
  printf("array_G[1][2] = %d\n", array_G[1][2]);  // 第二行最后一个元素(6).
  printf("array_G[0][2] = %d\n", *(*array_G + 2)); // 第一行最后一个元素(3).

  // 二维数组的指针运算.
  int array_H[2][3] = { {1, 2, 3}, {4, 5, 6} };
  int (*p1)[3] = array_H;                          // 指向第一个子数组（第一行）.
  printf("array_H 的地址: %p\n", array_H);
  printf("p1 的地址: %p\n", p1);
  printf("p1 + 1的地址: %p\n", (p1 + 1));          // 由于p1是指向int[3]类型的指针，因此移动 3 * sizeof(int) = 12个字节，指向第二个子数组.
  printf("(p1 + 1) - p1 = %d\n", (char *)(p1 + 1) - (char *)p1);

  int *p2 = array_H[1];                           // 指向第二行(第二个子数组)首元素(4).
  printf("p2 的地址: %p\tp2 的值: %d\n", p2, *p2);
  printf("p2+1 的地址: %p\tp2+1 的值: %d\n", (p2 + 1), *(p2 + 1));  // p2是指向int类型的指针，因此移动 sizeof(int) = 4个字节. 指向该行中下一个int元素.
  printf("(p2 + 1) - p2 = %d\n", (char *)(p2 + 1) - (char *)p2);

  // 二维数组数组名代表整个二维数组对象，它在内存中是一块连续的存储空间，按行优先顺序存放所有元素.
  // 在大多数情况下 二维数组数组名会退化为指向第一行(第一个子数组)的指针.
  // 注意：数组的数组名都是常量！！
  int array_I[2][3] = { {1, 2, 3}, {4, 5, 6} };

  // array_I是指向第一行子数组的指针(指针退化);(array_I + 1)将指针指向第二行子数组; *(array_I + 1)表示将指针指向第二行子数组第一个元素(4);
  // (*(array_I + 1) + 2) 表示将指针移动到第二行的最后一个元素(6).
  // *((*(array_I + 1)) + 2) 最后解引用表示取第二行最后一个元素的值6，并将其修改为 99.
  *((*(array_I + 1)) + 2) = 99; 
  printf("array_I[1][2] = %d\n", array_I[1][2]); 

  // 对数组名取地址(&) 得到指向整个二维数组的指针.它与退化后的指针值相同，但步长是整个数组的大小.
  int (*p_A)[2][3] = &array_I;

  // sizeof(二维数组名)得到的是整个数组的大小！ 无论几维数组，使用sizeof关键字得到的都是总大小.
  printf("二维数组 array_I 的大小: %zu\n", sizeof(array_I));  // 大小 = 3 * sizeof(int) * 2 = 24字节.

  // 三维数组定义:
  int RGB[3][16][16]; // 以 RGB数组为例.可以理解为有三个平面，每个平面为16*16的表格即可.

  // 三维数组的初始化.
  int cube[2][3][3] = {
    {{0,1,2}, 
     {3,4,5},
     {6,7,8}}, 

    {{9,10,11},
     {12,13,14},
     {15,16,17}}
    };

  // 三维数组指针运算与数组名性质与二维数组类似.
  // (cube + 1): 指向cube三维数组的第二面.
  // (*(cube + 1)): 解引用使指针从指向面 变为 指向 行.(目前指针指向第二面的第一行{9,10,11}).
  // ((*(cube + 1)) + 1): 将当前指针指向当前面(第二面)的第二行({12,13,14}).
  // (*((*(cube + 1)) + 1)): 再解一次引用，使指针从指向行 变为 指向 元素.(目前指针指向第二面第二行第一个元素(12)).
  // ((*((*(cube + 1)) + 1)) + 2): 将指针指向当前面(第二面)，当前行(第二行)的第三个元素(14).
  // *((*((*(cube + 1)) + 1)) + 2): 最后再解一次引用得到 第二面第二行第三个元素的值，并将其修改为 99.
  // 可以看到高维数组的指针操作十分复杂，对于高维数组尽量使用下标访问.
  *((*((*(cube + 1)) + 1)) + 2) = 99;
  printf("cube[1][1][2] = %d\n", cube[1][1][2]);

  // 更高维的数组以此类推.

  printf("===============================================\n\n");
  /*
    array_A[0][0] = 0       array_A[0][1] = 1       array_A[0][2] = 2
    array_A[1][0] = 3       array_A[1][1] = 4       array_A[1][2] = 5

    array_B[0][0] = 0       array_B[0][1] = 1       array_B[0][2] = 2
    array_B[1][0] = 3       array_B[1][1] = 4       array_B[1][2] = 5

    array_C[0][0] = 1       array_C[0][1] = 0       array_C[0][2] = 0
    array_C[1][0] = 3       array_C[1][1] = 4       array_C[1][2] = 0

    array_D[0][0] = 0       array_D[0][1] = 9       array_D[0][2] = 0
    array_D[1][0] = 0       array_D[1][1] = 0       array_D[1][2] = 8

    array_G[1][2] = 6
    array_G[0][2] = 3
    array_H 的地址: 000000000061FCC0
    p1 的地址: 000000000061FCC0
    p1 + 1的地址: 000000000061FCCC
    (p1 + 1) - p1 = 12
    p2 的地址: 000000000061FCCC     p2 的值: 4
    p2+1 的地址: 000000000061FCD0   p2+1 的值: 5
    (p2 + 1) - p2 = 4
    array_I[1][2] = 99
    二维数组 array_I 的大小: 24
    cube[1][1][2] = 99
  */
}


void array_example_demo5( void )
{
  printf("=========== 多维数组的函数参数传递 ===========\n");


}

