#ifndef  OTExtension_H
#define  OTExtension_H
#include <gmp.h>
#include <gmpxx.h>
#include <string>
#include <bitset>
#include "Simple_OT.h"
#include "Random_OT.h"

#include <iostream>
#include "useGMP.h"
#include "hash_mpz.h"
#include <Eigen/Eigen>
using namespace std;
using namespace Eigen;

#define _one_OTE_length 128

#define col_length 1000000 //number of OTs 


class OTE_Sender {
    public:
        std::string S;
        bitset<128> random_str;
        Matrix<int,Dynamic,Dynamic> M;
        OT_Receiver ot_rev;
        gmplibfunc gm;
        mpz_t g,prime;
        mpz_t X;
        mpz_t Y;
        void init(int rows,int cols);
        void col_OT_Receiver_init(mpz_t &g,mpz_t &prime);
        void col_OT_Receiver_calchash(mpz_t &X, int choice_bit);
        void col_OT_Receiver_dec(mpz_class &c0,mpz_class &c1);
        void final_OTE_m0m1_load();
        mpz_class plaintext0;
        mpz_class plaintext1;

        vector<string> m0;
        vector<string> m1;

        vector<bitset<128>> m0_bitset;
        vector<bitset<128>> m1_bitset;


        


};

class OTE_Receiver {
    public:
        Matrix<int,Dynamic,Dynamic> M0,M1;
        bitset<_one_OTE_length> rand_bitstream; // first col, bit choice
        bitset<1000000> rand_oneMillion_bitsteam;
        OT_Sender otsend;
        gmplibfunc gm;
        mpz_t g;
        mpz_t prime;
        mpz_t X;
        mpz_class c0;
        mpz_class c1;



        void init(int Mrow,int Mcolumn);
        void col_OT_Sender_init();
        void col_OT_Sender_enc(mpz_t &Y,char const *mm0,char const *mm1);


};

#endif