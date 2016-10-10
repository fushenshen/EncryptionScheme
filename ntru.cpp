#include <iostream>
#include <NTL/ZZX.h>
#include <random>
#include <cmath>
using namespace std;
using namespace NTL;
ZZX final;
//在范围n之间生存生成随机数
inline int RandomNum()
{
    std::random_device rd;
    
    std::mt19937 rng(rd());
    
    //在这里取到一个范围在0～2q的随机 数字
    //在取一个矩阵的时候mod q 取四舍五入
    std::uniform_int_distribution<int> uni(INT_MIN,INT_MAX);
    
    auto random_integer = uni(rng);
    
    return random_integer;

}

inline int Mymod(double num,int n)
{
    int dev=(int)std::round(num/n);
    return num-n*dev;
}
//分圆不等式
// x`n+1
ZZX cyclotomic(int N)
{
//    const int  N =5;
    
    ZZX Inequality;
    
    //设置常数项为1
    SetCoeff(Inequality, 0, 1);
    
    SetCoeff(Inequality, N, N);
    
//    cout<<Inequality<<endl;
    
    return  Inequality;
}
//多项式环
//设置一个n项的不等式环 使得常数落在范围(-q/2,q/2)
ZZX PolyRing(int n,int q)
{
    
    ZZX tmp;
    
    for(unsigned int i=0;i<n;++i)
    {
       
        int pow=Mymod(RandomNum(), q);
        if(!pow)pow+=1;
        SetCoeff(tmp, i, pow);
        
    }
//    cout<<tmp<<endl;

    return tmp;
}
//得到密钥
ZZX  PrivateKey(int n,int q)
{
    ZZX f;
    //得到分圆不等式
    ZZX aN=cyclotomic(n);
    
    ZZX ring=PolyRing(n,q);
    
    ZZX tmpres;
    while(1)
    {
        int random=std::abs(RandomNum());
        
        unsigned int idx=random%(n);
    
//        if(ring[idx]!=NULL)continue;
        ZZ num= ring[idx];
        
        SetCoeff(f, idx, num*2);
        SetCoeff(f, 0, 1);
        
        GCD(tmpres, ring, aN);

        //找到   f `
        if(tmpres[0]==1)break;
    }
    final=f;
    return f;
}
//h=2g *(f`-1) -1∈Rq
ZZX  PublicKey(int n,int q)
{
    ZZX ring=PolyRing(n,q);
//    cout<<ring<<endl;
    ZZX f=PrivateKey(n, q);
    ZZ r(1);//比较结果
    ZZX cys=cyclotomic(n); //分圆式 1+x`n
    ZZX eq1;//不等式1，从ring中得到
    ZZX eq2;//不等式2 ,  从ring中得到
    ZZX g;
    ZZX h;//最终结果
    
    int random2=std::abs(RandomNum());
    unsigned int idx2=random2%(n);
    ZZ num2= ring[idx2];
    SetCoeff(g, idx2, num2);
    
    int random=std::abs(RandomNum());
    unsigned int idx=random%(n);
    ZZ num= ring[idx];
    SetCoeff(eq1, idx, num);
    
    int random1=std::abs(RandomNum());
    unsigned int idx1=random1%(n);
    ZZ num1= ring[idx1];
    SetCoeff(eq2, idx1, num1);
    XGCD(r, eq2, f, eq1, cys);
//    ZZ &get=eq2[eq2.rep.length()-1];
    return 2*g*eq2;

}
//加密过程
ZZX Enc(int n,int q)
{
    //需要加密的消息
    ZZX s;
    ZZX e;
    ZZX ring=PolyRing(n,q);
    ZZX m(1);
    
    int random=std::abs(RandomNum());
    unsigned int idx2=random%(n);
    ZZ num2= ring[idx2];
    SetCoeff(s, idx2, num2);
    
    
    int random1=std::abs(RandomNum());
    unsigned int idx1=random1%(n);
    ZZ num1= ring[idx1];
    SetCoeff(e, idx1, num1);
    
    //private Key
    ZZX h=PublicKey(n, q);
    //密文
    ZZX c=m+h*2+2*e;
    return c;
}
ZZ Dec(int n,int q)
{
    //密文
    ZZX c=   Enc(n,q);
    ZZX dec=c*final;
    return dec[0];
}


int main()
{

    
    
}
