//
//  NtruGsw.hpp
//  NTL_test
//
//  Created by 傅莘莘 on 2016/10/10.
//  Copyright © 2016年 傅莘莘. All rights reserved.
//

#ifndef NtruGsw_hpp
#define NtruGsw_hpp
#include <NTL/ZZX.h>
#include <cmath>
class ntru
{
    
public:
    //init ntru
    ntru()
    {
        init();
    }
    //
    void init()
    {
        //构建多项式环
        RandomRing();
        //分圆不等式
        SetCoeff(Inequality, 0, 1);
        SetCoeff(Inequality, n, n);
        h=keyGen();
        
    }
    NTL::ZZX keyGen();
    
    //return f^-1
    NTL::ZZX ReversePoly( NTL::ZZX &f);
    
    
    //这里有一个大坑,原本是 num-n*dev导致环的取值不对
    inline int mod(double num)
    {
        int dev=(int)std::round(num/q);
        return num-q*dev;
    }
    //获取随机数字
    inline int GetRandom()
    {
        std::random_device rd;
        std::mt19937 rng(rd());
        std::uniform_int_distribution<int> uni(-q,q);
        auto random_integer = uni(rng);
        return random_integer;
    }
    ~ntru()
    {
        
    }
    
    
private:
    
    //多项式取模
    
   inline void ZZXmod(NTL::ZZX &mo)
    {
        
        for(int  zp=0;zp<=mo.rep.length()-1;++zp){
        NTL::ZZ ModNum=mo.rep[zp];
        NTL::ZZ left;
        left= ModNum- (ModNum/q)*q;
        //进位
        if(2*left>=q)
        {
            left=ModNum-(ModNum/q+1)*q;
        }
        
        mo.rep[zp]=left;
    }
    
    }
    
    //得到一个多项式环
    void  RandomRing();
private:
    //n是以2为幂次方的数字
    int n{8};
    //素数模 q
    int q{17};
    int l{static_cast<int>(std::log2(q))-1};
    NTL::ZZX ring;
    //分圆不等式
    NTL::ZZX Inequality;
    
    //privekeyGen
    NTL::ZZX h,Genf;
    
    
public:
    friend
    class NtruEdc;
    
};

//加密和解密类
class NtruEdc
{
public:
    
    NtruEdc():nt(std::make_shared<ntru>())
    {
        InitDec();
    }
    NTL::ZZ RandomZzx()
    {
        int random=std::abs(nt->GetRandom());
        unsigned int idx=random%(nt->n);
        NTL::ZZ num= nt->ring[idx];
        return num;
    }
    void Enc()
    {

        //C[l-1]*f 
        NTL::ZZX tmp=dec[nt->l-1]*(nt->Genf);
        std::cout<<"C[l-1] *f "<<tmp<<std::endl;
        nt->ZZXmod(tmp);
        std::cout<<tmp<<std::endl;
        double ppow=std::pow(2, (-(nt->l-1)));
        std::cout<<" ppow "<<ppow<<"--- "<<(-(nt->l-1))<<"res"<<(double)ppow*tmp[nt->l-1]<<std::endl;
    }
   
private:
    void InitDec()
    {
        //列向量的长度
        int Nl=nt->l;
        //init zzx e
        for(int i=0;i<Nl;++i)
        {
            NTL::ZZ num=RandomZzx();
            SetCoeff(e,i,num);
            NTL::ZZ num1=RandomZzx();
            SetCoeff(s,i,num1);
        }
        
        NTL::ZZ hval=nt->h.rep[nt->h.rep.length()-1];
        for(int i=0;i<Nl;++i)
        {
            
            NTL::ZZ tmp;
            m*std::pow(2, i)+nt->h*s[i]+e[i];
            //            SetCoeff(e,i,num);
            tmp=(m*std::pow(2, i)+hval*s[i]+e[i])[0];
            SetCoeff(dec,i,tmp);
        }
        nt->ZZXmod(dec);
        
    }
    
   
private:
    std::shared_ptr<ntru> nt;
    
    NTL::ZZX e;
    NTL::ZZX s;
    
    //密文组
    NTL::ZZX dec;
    //明文组
    NTL::ZZX enc;
    
    //密文 ==1
    NTL::ZZX m{1};
};


#endif /* NtruGsw_hpp */
