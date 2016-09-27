#ifndef Edec_hpp
#define Edec_hpp
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#define TEST()\
double lnum=0;\
double rnum=0;\
for(int i=0;i<200;++i)\
{\
    if(Dec(0)==0)lnum++;\
}\
for(int i=0;i<200;++i)\
{\
    if(Dec(1)==1)rnum++;\
}\
std::cout<<"解密成功率"<<std::endl;\
std::cout<<"1  的成功率"<<rnum/200*100<<" % "<<std::endl;\
std::cout<<"0  的成功率"<<lnum/200*100<<" % "<<std::endl;\

boost::numeric::ublas::matrix<int > GetRandomMatrixS();
boost::numeric::ublas::matrix<int > GetRandomMatrixA();
boost::numeric::ublas::matrix<int >  GetMatrixB();
boost::numeric::ublas::matrix<int > Enc(int secret);

int Dec(int);
#endif /* Edec_hpp */
