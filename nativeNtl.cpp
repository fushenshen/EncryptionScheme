#include <NTL/ZZ_pXFactoring.h>
#include <NTL/ZZ_pEX.h>
#include <NTL/ZZ.h>
#include <iostream>
#include <random>
#include <vector>
#include "nativeNtl.hpp"


inline int GetRandom()
{
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uni(0,8);
    auto random_integer = uni(rng);
    return random_integer;
}
class Ntru_native
{

public:
    //分圆不等式 X^( n - 1 )
    Ntru_native():n(8),q(1117),l()
    {
        l={static_cast<int>(std::log2(q))-1};
        init();
        
    }
    void init()
    {
        NTL::ZZ_p::init(NTL::ZZ(q));
        NTL::ZZ_pX  tmp;
        
        SetCoeff(ring, 0, -1);
        SetCoeff(ring,n, 1);
        NTL::ZZ_p::init(NTL::ZZ(2));

        //fuck1 represent  f`
        long idx=GetRandom()%n;
        if(!idx)idx+=1;
        NTL::SetCoeff(fuck1, idx, 1);
        NTL::SetCoeff(fuck1, 0, 1);
        
        long idx1=GetRandom()%n;
        if(!idx1)idx1+=1;
        NTL::SetCoeff(gfuck, idx1, 1);
        NTL::SetCoeff(gfuck, 0, 0);
    
        KeyGen();
    }
    
    //compute KeyGen
    void   KeyGen()
    {
    
        //store result represent f^{-1}
        NTL::ZZ_pX x;
        
        NTL::ZZ_pX t;
        
        //prime poly reprensent 1
        NTL::ZZ_pX d;
        
        //random init three zz_px
        long idx=GetRandom()%n;
        if(!idx)idx+=1;
        NTL::SetCoeff(t, idx, 1);
        
        NTL::SetCoeff(d, 0, 1);
        
        /***
            copy f
         ****/
        tmpfuck=fuck1;
        
        NTL::XGCD(d, x, t, fuck1, ring);
        
        keygen= NTL::MulMod( x, gfuck,ring);
        
        Enc();
    }
    
//    inline void  handle_pex(NTL::ZZ_pEX &s,NTL::ZZ_pEX &e)
//    {
//      
//    }
    
    void set_vec(std::vector<NTL::ZZ_pX> &s,std::vector<NTL::ZZ_pX>& e)
    {
        NTL::ZZ_pX m1,m2;
    
        NTL::ZZ_p::init(NTL::ZZ(2));
        
        for(int i=0;i<l+1;++i)
        {
        
            BuildIrred(m1, n-1);
            s.push_back(m1);
            BuildIrred(m2, n-1);
            e.push_back(m2);
        }
        
    }
    void Enc()
    {
        
        //密文
        NTL::ZZ_pX message;
        
        
        NTL::SetCoeff(message, 0, 1);
        
        NTL::ZZ_p::init(NTL::ZZ(2));
        
        NTL::ZZ_pX PP;
        BuildIrred(PP, n);

        NTL::ZZ_pE::init(PP);
        
//        NTL::ZZ_pEX s;
//        NTL::ZZ_pEX e;
//        random(s, l+1);
//        random(e,l+1);
        
        set_vec(s, e);

        NTL::ZZ_p::init(NTL::ZZ(q));
        NTL::ZZ_pX tmp;
      
        for(int i=0;i<l;++i)
        {
        
            NTL::SetCoeff(tmp, 0, std::pow(2, i));
            Encc.push_back(NTL::MulMod(message,tmp,ring)+NTL::MulMod(keygen,s[i],ring)+e[i]);
        }
        add();
        Dec();
    }
    
    void add()
    {
        NTL::ZZ_pX add_res;
        NTL::add(add_res, Encc.at(0), Encc.at(1));
        std::cout<<"C1+C2 = "<<add_res<<std::endl;
    }
    
    void Dec()
    {
        
        NTL::ZZ_p::init(NTL::ZZ(q));
        NTL::ZZ_pX tmp;
        NTL::SetCoeff(tmp, 0, std::pow(2, (-l-1)));
        
        
        /**
             test
         **/
//        std::cout<<"-l -1 "<<-(l-1)<<std::endl;
//        std::cout<<std::pow(2, -(l-1))<<std::endl;
        
        NTL::ZZ_p::init(NTL::ZZ(q));
        std::cout<< NTL::MulMod(Encc.at(l-1), tmpfuck, ring)<<"\n";
        auto &item=Encc.at(l-1);
        std::cout<< item[0]<<std::endl;
        NTL::SetCoeff(tmp, 0, 1);
//        std::cout<<"tmp"<<tmp<<std::endl;
        
        
    }
    
    
private:
    //环基于这个质数
    //环上多项式的系数

    //最高项次为4次
    int n,q,l;
    //标准偏移
    NTL::ZZ_pX fuck1,gfuck,tmpfuck ;
    //keygen
    NTL::ZZ_pX keygen;
    //分圆式
    NTL::ZZ_pX ring;
    
    //密文向量
    std::vector<NTL::ZZ_pX> Encc;
    
    
    std::vector<NTL::ZZ_pX> s;
    std::vector<NTL::ZZ_pX> e;
    

};
int main()
{
    for(int i=0;i<1000;++i)
    Ntru_native Nn;
    return 0;
}
