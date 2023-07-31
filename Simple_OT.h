#ifndef  SIMPLEot_H
#define  SIMPLEot_H
#include <gmp.h>
#include <gmpxx.h>
#include <string>
#include <sstream>

#include <iostream>
#include "useGMP.h"
#include "hash_mpz.h"
using namespace std;




class OT_Sender {
    public:
        mpz_t M0,M1;
        mpz_t g; //generator
        mpz_t x; //random number
        mpz_t X; //public exp
        mpz_t Y; //public exp
        mpz_t prime;
        int nbits;
        mpz_class c0;
        mpz_class c1;
        void init(gmplibfunc gm,size_t privsize,size_t gsize,size_t primesize);
        void enc(mpz_t &Y,const char *m0,const char *m1);



};

class OT_Receiver {
    public:
        mpz_t M0,M1;
        mpz_t g; //generator
        mpz_t y; //random number
        mpz_t Y; // g^y
        mpz_t prime;// big prime
        mpz_t X;// g^x from sender
        bool bit_choice; // bit choice
        int nbits;
        mpz_class key;
        void init(gmplibfunc gm, mpz_t &ginput,mpz_t &primeinput, size_t privsize);
        void calchash(mpz_t &X, int choicebit);
        void dec(mpz_class &c0,mpz_class &c1);
        void dec0(mpz_class &c0);
        void dec1(mpz_class &c1);


};

static void const compute_mpz_hash(mpz_class &input , int size){
    input = hash<mpz_class>{}(input);
    std::stringstream ss;
    ss<<1;
    for(int i=0;i<size;i++){
        ss<<0;
    }
    mpz_class v = mpz_class(ss.str());
    mpz_mod(input.get_mpz_t(),input.get_mpz_t(),v.get_mpz_t());
}

#endif