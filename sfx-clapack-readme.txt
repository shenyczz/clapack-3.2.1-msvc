
1. CLAPACK简介
 要了解CLAPACK，就要先知道什么是LAPACK。
 LAPACK(Linear Algebra PACKage)是一个高性能的线性代数计算库，以BLAS(Basic Linear Algebra Subprograms)为基础，用Fortran语言编写，
 可用于计算诸如求解线性代数方程、线性系统方程组的最小平方解、计算特征值和特征向量等问题。
 而CLAPACK则是LAPACK的C语言接口。

2. CLPACK的安装
 Download clapack-3.2.1-CMAKE.tgz and unzip.
 Download cmake and install it on your machine.
 Open CMAKE
 Point to your CLAPACK-3.2.1-CMAKE folder in the source code folder
 Point to a new folder where you want the build to be (not the same is better)
 Click configure, check the install path if you want to have the libraries and includes in a particular location.
 Choose Visual Studio Solution. You can also choose nmake or any other platform.
 You may have to click again configure until everything becomes white
 Click generate, that will create the Visual Studio for CLAPACK and you are done.
 Close CMAKE
 Look in your "build" folder, you have your CLAPACK Visual Studio Solution, just open it.
 Build the "ALL_BUILD" project, it will build the solution and create the librarires
 Build the "INSTALL". This will put the libraries and include in your install folder.
 Build the "RUN_TESTS". The BLAS and CLAPACK testings will be run.
       如果没修改CMAKE中的设置，那么搞定第6个步骤后，你会在C:\Program Files\CLAPACK发现你所需要的*.h和*.lib
 
 备注：在RUN_TESTS有部分不能通过，也不影响其余功能的正常使用

 站点链接：http://www.netlib.org/clapack/

3. CLAPACK的使用
 使用CLPACK前，要先清楚四点：

 [1]首先是Levels of Routines，即函数的层次。
    LAPACK将整个库分为三大块：driver, computional, auxiliary，具体哪块是做什么的，请自行看链接。

 [2]其次是Naming Scheme，即命名规则。
    LAPACK的 driver 和 computational 的函数名通常为XYYZZZ，其中X指使用的数据类型，YY指矩阵类型，ZZZ指函数的功能。
    例如SGEBRD的意思是单精度(S)的在一般的矩阵(GE)上执行bidiagonal reduction(BRD)操作，还是那句，具体的，请自行看链接。

 [3]再次是CLAPACK的函数不接收二维数组，即只能用一维数组代替二维数组。
    例如我想要个array[2][2] = { {1,2}, {3,4} }，那么正确的写法是array[2*2] = { 1, 2, 3, 4}。

 [4]最后是行主序与列主序的问题。
    CLAPACK看一维数组时会将其看成是按列存放的。
    所以习惯将一维数组看成是按行存放的要特别注意了。如果说就是不爽按列存放呢？那么可以在计算前，先将其转置。

4. CLAPACK 应用例子
 这里以使用dgemm_函数为例，对CLAPACK的使用方法进行说明。
 从dgemm_的命名可以看出，这是双精度(d)的在一般矩阵(GE)执行matrix-matrix操作的函数，更具体来说，是执行C = alpha* op(A)* op(B) + beta * C 操作。
 
 在clapack.h中的声明如下：
 int dgemm_(char *transa, char *transb, integer *m, integer *n, integer *k, doublereal *alpha,
            doublereal *a, integer *lda, doublereal *b, integer *ldb, doublereal *beta, doublereal *c__, integer *ldc);

 其中
 transa表示op(A)的操作，若transa = 'T'，则表示对A进行转置
 transb表示op(B)的操作
 m表示矩阵A的行数
 n表示矩阵B的列数
 k表示矩阵A的列数
 alpha就是alpha的值
 a就是矩阵A的一维存储，注意调用的函数会认为其是列主序的
 lda表示矩阵A的第一维(LDA specifies the first dimension of A)，其值根据transa的值而变
 b是矩阵B的一维存储
 beta就是alpha的值
 ldb表示矩阵B的第一维，其值根据transb的值而变
 c是矩阵C的一维存储
 ldc表示矩阵C的第一维
 

#include "iostream"
#include "f2c.h"        
#include "clapack.h"

using namespace std;

int main()
{
    char transa = 'T', transb = 'T';
    integer M = 2, N = 2, K = 2, LDA = K, LDB = N, LDC = M;
    double alpha = 1.0, A[4] = { 1, 2, 3, 4}, B[4] = { 5, 6, 7, 8 }, beta = 0.0, C[4];
    //下面的函数的意思是C = 1.0 * T(A) * T(B) + 0 * C，其中T()表示将某个矩阵转置
    //注意此时得到的C是按列存放的
    dgemm_(&transa, &transb, &M, &N, &K, &alpha, A, &LDA, B, &LDB, &beta, C, &LDC);
    
    cout<<C[0]<<" "<<C[2]<<endl;
    cout<<C[1]<<" "<<C[3]<<endl;

    return 0;
}

