#include <iostream>
#include <complex>
#include <random>
#include <cassert>
#include "NtruGsw.hpp"
#include <NTL/ZZX.h>
#include <NTL/ZZ_pXFactoring.h>
#include <NTL/ZZ_pEX.h>
#include <NTL/ZZ_p.h>

NTL::ZZX ntru::keyGen()
{
    assert(ring!=NULL);
    assert(Inequality!=NULL);
    NTL::ZZX f,tmpres,g;
    while(1)
    {
        int random=std::abs(GetRandom());
        unsigned int idx=random%(n);
        NTL::ZZ num= ring[idx];
        if(num*2>q||num*2<0)continue;
        SetCoeff(f, idx, num*2);
        SetCoeff(f, 0, 1);
        GCD(tmpres, ring, Inequality);
        if(tmpres[0]==1)break;
    }
    //随机选取 g<-X
    int ng=std::abs(GetRandom());
    ng%=n;
    /*
     ********************************
     */
    Genf=f;
    //此处得到了一个f
    NTL::ZZ gtmp= ring[ng];
    SetCoeff(g,ng,gtmp);
    //保存一份f
    NTL::ZZX prevf(f);
    prevf=ReversePoly(f);
    
    prevf=2*prevf*g;
    ZZXmod(prevf);
    
    
    //已经存在了 f g
    //对f取mod
    
    return prevf;
}


 NTL::ZZX ntru::ReversePoly( NTL::ZZX &f)
{
    NTL::ZZ r(1);//比较结果
    NTL::ZZX eq1,eq2;
    
    
    int random=std::abs(GetRandom());
    unsigned int idx=random%(n);
    NTL::ZZ num= ring[idx];
    SetCoeff(eq1, idx, num);
    
    
    int random1=std::abs(GetRandom());
    unsigned int idx1=random1%(n);
    NTL::ZZ num1= ring[idx1];
    SetCoeff(eq2, idx1, num1);
    XGCD(r, eq2, f, eq1, Inequality);
    return eq2;

}
void ntru::RandomRing()
{
    // 0次到 n-1的一个多项式环
    for(unsigned int i=0;i<n;++i)
    {
        
        int ranum=GetRandom();
        int pow=mod(ranum);
        if(!pow)pow+=1;
        SetCoeff(ring, i, pow);
        
    }
}

