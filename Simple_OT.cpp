#include <gmp.h>
#include <string>

#include <iostream>
#include "useGMP.h"
#include "Simple_OT.h"
#include "hash_mpz.h"
#include <unordered_map>
#include <time.h>
#include <chrono>

using namespace std::chrono;
using namespace std;




void OT_Sender::init(gmplibfunc gm,size_t privsize,size_t gsize,size_t primesize){
        mpz_init(this->M0);
        mpz_init(this->M1);
        mpz_init(this->g);
        mpz_init(this->x);
        mpz_init(this->X);
        mpz_init(this->prime);
        gm.generate_random(x,privsize);
        gm.generate_random(g,gsize);
        gm.generate_big_prime(prime,primesize);
        //gmp_printf("x = %Zd\n\n",x);
        //gmp_printf("g = %Zd\n\n",g);
        //gmp_printf("prime = %Zd\n\n",prime);
        mpz_powm(X,g,x,prime);
        //gmp_printf("X = %Zd\n\n",X);
        nbits = 32;
        //std::cout<<"Sender init finished\n"<<endl;

}

void OT_Sender::enc(mpz_t &Y,const char *m0,const char *m1){
    //std::cout<<"start enc"<<endl;
    mpz_class t0;
    mpz_class t1;
    mpz_powm(t0.get_mpz_t(),Y,x,prime);
    //std::cout<<"X is:"<<X<<endl;
    mpz_cdiv_q(Y,Y,X);
    mpz_powm(t1.get_mpz_t(),Y,x, prime);


    
    compute_mpz_hash(t0,nbits);
    compute_mpz_hash(t1,nbits);

    // std::cout<<"m0string0 is:"<<m0<<endl;
    // std::cout<<"m1string1 is:"<<m1<<endl;
    // std::cout<<"m0 mpzclass is:"<<mpz_class(m0,2).get_mpz_t()<<endl;
    // std::cout<<"m1 mpzclass is:"<<mpz_class(m1,2).get_mpz_t()<<endl;
    // std::cout<<"t1 is:"<<t1.get_mpz_t()<<endl;
    // mpz_xor(c0.get_mpz_t(),mpz_class(t0.get_str(),2).get_mpz_t(),mpz_class(m0,2).get_mpz_t());
    // mpz_xor(c1.get_mpz_t(),mpz_class(t1.get_str(),2).get_mpz_t(),mpz_class(m1,2).get_mpz_t());
    mpz_xor(c0.get_mpz_t(),t0.get_mpz_t(),mpz_class(m0,2).get_mpz_t());
    mpz_xor(c1.get_mpz_t(),t1.get_mpz_t(),mpz_class(m1,2).get_mpz_t());

}


void OT_Receiver::init(gmplibfunc gm, mpz_t &ginput,mpz_t &primeinput, size_t privsize){
        mpz_init(this->M0);
        mpz_init(this->M1);
        mpz_init(this->g);
        mpz_init(this->y);
        mpz_init(this->Y);
        mpz_init(this->prime);
        gm.generate_random(y,privsize);
        mpz_set(g,ginput);
        mpz_set(prime,primeinput);
        mpz_powm(Y,g, y, prime);
        nbits = 128;

        //std::cout<<"Receiver init finished\n"<<endl;

}

void OT_Receiver::calchash(mpz_t &X, int choicebit){

    
    if(choicebit == 1){ 
        //cout<<"choicebit is 1"<<endl;
        mpz_powm(Y,g,y,prime);
        mpz_mul(Y,Y,X);

    }
    if(choicebit == 0){
        //cout<<"choicebit is 0"<<endl;
        mpz_powm(Y,g,y,prime);
    }
    mpz_powm(key.get_mpz_t(),X,y,prime);

    compute_mpz_hash(key,nbits);
    //std::cout<<"****************"<<endl;
    //std::cout<<"key is:"<<key<<endl;

  
}

void OT_Receiver::dec(mpz_class &c0,mpz_class &c1){
    mpz_xor(M0,c0.get_mpz_t(),key.get_mpz_t());
    mpz_xor(M1,c1.get_mpz_t(),key.get_mpz_t());

}

void OT_Receiver::dec0(mpz_class &c0){
    mpz_xor(M0,c0.get_mpz_t(),key.get_mpz_t());

}
void OT_Receiver::dec1(mpz_class &c1){
    mpz_xor(M1,c1.get_mpz_t(),key.get_mpz_t());

}

void test_ot(){
    OT_Receiver otr;
    OT_Sender otsend;
    mpz_t g,p;
    gmplibfunc gm;
    size_t privsize = 15;
    otsend.init(gm, privsize, 10, 128);
    otr.init(gm,otsend.g,otsend.prime,privsize);
    otr.calchash(otsend.X,1);
    otsend.enc(otr.Y,"111111000000","000000111111");
    otr.dec(otsend.c0,otsend.c1);

    //std::cout<<"m0 is:"<<mpz_class(otr.M0).get_str(2)<<"\n";
    //std::cout<<"m1 is:"<<mpz_class(otr.M1).get_str(2)<<"\n";
    //otr.calchash();
}


void test_n_ot(int n){
    for(int i=0;i<n;i++){
        test_ot();
    }
}

void test_mpz_hash(mpz_class &a){
    //mpz_ui_pow_ui(a.get_mpz_t(), 168, 16);

    cout << "a       : " << a << "\n";
    cout << "hash( a): " << (hash<mpz_class> { }(a)) << "\n";
    cout << "hash(-a): " << (hash<mpz_class> { }(-a)) << "\n";

    unordered_map<mpz_class, int> map;
    map[a] = 2;
    cout << "map[a]  : " << map[a] << "\n";
}



// int main(){
//     // mpz_class big1;
//     // mpz_init_set_str(big1.get_mpz_t(),"173482764328734682784682764328734627384",10);
//     // gmp_printf("big1 = %Zd\n\n",big1.get_mpz_t());
//     int amount = 10000;
//     auto rstart = high_resolution_clock::now();
//     test_n_ot(amount);
//     auto rstop = high_resolution_clock::now(); 
//     auto rduration = duration_cast<microseconds>(rstop - rstart);
//     cout << "Time taken by SimpleOT: for "<<amount<<" times in "<< rduration.count() *1e-6<< " seconds" << endl;
//     // test_mpz_hash(big1);
//     // compute_mpz_hash(big1);
//     // cout << "hash( a): " << big1 << "\n";


//     return 0;



// }