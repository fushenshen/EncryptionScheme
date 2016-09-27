#include <boost/numeric/ublas/operation.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <cstdlib>
#include <cassert>
#include <random>
#include <cmath>
#include "Edec.hpp"
static int  n =2;
static int  N =12;
static int q =11158;
//四舍五入取整数
inline int mod(double num)
{
    int dev=(int)std::round(num/q);
    return num-q*dev;
}
boost::numeric::ublas::matrix<int > FinalS(n+1,1);
inline int RandomNum()
{
    std::random_device rd;
    
    std::mt19937 rng(rd());
    
    //在这里取到一个范围在0～2q的随机 数字
    //在取一个矩阵的时候mod q 取四舍五入
    std::uniform_int_distribution<int> uni(0,2*q);
    
    auto random_integer = uni(rng);
   
    return random_integer;
}
boost::numeric::ublas::matrix<int > GetRandomMatrixS()
{
    using matrix=boost::numeric::ublas::matrix<int>;
    
    matrix m1(n+1,1);
    
    m1(0,0)=1;
    
    for(unsigned int i=1;i<n+1;++i)
    {
      m1(i,0)=mod(RandomNum());
    }
    
    return m1;
}
boost::numeric::ublas::matrix<int > GetRandomMatrixA()
{
    using matrix=boost::numeric::ublas::matrix<int>;
    
    matrix m1(N,n);
    
    for(unsigned int x=0;x<N;++x)
    {
        for(unsigned int y=0;y<n;++y)
        {
            m1(x,y)=mod(RandomNum());
        }
    }

    return m1;
}

boost::numeric::ublas::matrix<int >  GetMatrixB()
{
    using matrix=boost::numeric::ublas::matrix<int>;
    //先取 e == N χ 。
    matrix e(N,1);
   
    for(int i=0;i<N;++i)
    {
        e(i,0)=RandomNum()%2;

    }
//     std::cout<<e<<"\n";
    matrix b(N,1);
    
    auto MatrixA=GetRandomMatrixA();
    
    auto MatrixS=GetRandomMatrixS();
    
    FinalS=MatrixS;
    
    matrix MatrixSp(n,1);
    
    for(unsigned int i=1;i<=n;++i)
    {
        MatrixSp(i-1,0)=MatrixS(i,0);
    }

    auto MatrixB=boost::numeric::ublas::prod(MatrixA, MatrixSp);

    b=( MatrixB+e);
    
    matrix MatrixAfinal(N,n+1);
    
    auto col1=boost::numeric::ublas::column(b, 0);
    
    MatrixA=-MatrixA;
    
    for(unsigned int i=0;i<N;++i)
    {
        for(unsigned int j=0;j<n+1;++j)
        {
            if(j==0){
                MatrixAfinal(i,0)=b(i,0);
            
            }
            
            else
            {
                MatrixAfinal(i,j)=MatrixA(i,j-1);
                
            }
        }
   
    }
    
    return MatrixAfinal;

}

//对消息的加密
//secret为要加密的数字
boost::numeric::ublas::matrix<int > Enc(int secret)
{
    using matrix=boost::numeric::ublas::matrix<int>;
    //密文 c
    matrix c(n+1,1);

    matrix r (N,1);
    
    auto MatrixA=GetMatrixB();
    
    
    
    for(unsigned int i=0;i<N;++i)
    {
        r(i,0)=RandomNum()%2;
    }

    //倒置矩阵
    MatrixA= boost::numeric::ublas::trans(MatrixA);
    
    //A倒置T * r
    matrix tmp(n+1,1);
    
   tmp= boost::numeric::ublas::prod(MatrixA, r);
    
    //构建加密消息
    matrix m(n+1,1);
    
    m(0,0)=secret;
    m(1,0)=0;
    m(2,0)=0;
    for(unsigned i=1;i<n+1;++i)
    {
        m(i,0)=m(i,0)*(q/2);
    }
    m(0,0)=m(0,0)*(q/2);

    c=m+tmp;
    
    return c;
}
inline int InnerProduct(int (&arr1)[3],int (&arr2)[3])
{
    int res=0;
    for(unsigned int i=0;i<n+1;++i)
    {
        res+=arr1[i]*arr2[i];
    }
    return res;
}

int  Dec(int val)
{
    //c为密文
    auto c=Enc(val);

    
    int arr1[3];
    int arr2[3];
    
    auto res1=boost::numeric::ublas::column(c, 0);
    
    auto res2=boost::numeric::ublas::column(FinalS, 0);
    
    for(int i=0;i<n+1;++i)
    {
        arr1[i]=res1(i);
        arr2[i]=res2(i);
    }
  int   res=InnerProduct(arr1, arr2);
    
    double qtwo=(double)2/q;
    
    res=res*qtwo;
    
    return res%2;
}

