#include <stdio.h>
#include <gmp.h>
#include <time.h>
#include "useGMP.h"



void gmplibfunc::generate_random(mpz_t &rand, size_t nbits){
        //clock_t time = clock();
        gmp_randstate_t grt;
        gmp_randinit_mt(grt);
        //gmp_randseed_ui(grt, time);
        mpz_init(rand);
        mpz_urandomb(rand, grt, nbits);
        mpz_nextprime(rand, rand);
    }

void gmplibfunc::generate_big_prime(mpz_t &bigprime, size_t nbits){
        //clock_t time = clock();
        gmp_randstate_t grt;
        gmp_randinit_default(grt);
        gmp_randinit_mt(grt);
        //gmp_randseed_ui(grt, time);
        mpz_init(bigprime);
        mpz_urandomb(bigprime, grt, nbits);
        mpz_nextprime(bigprime, bigprime);

    }

void gmplibfunc::powm(mpz_t &result, mpz_t &base, mpz_t &exp, mpz_t &mod){
        mpz_powm(result,base,exp,mod);
    }


// int main()
// {
//     // clock_t time = clock();
//     // gmp_randstate_t grt;
//     // gmp_randinit_default(grt);
//     // gmp_randseed_ui(grt, time);

//     // mpz_t p;
//     // mpz_init(p);
//     // mpz_urandomb(p, grt, 1024);
//     // mpz_nextprime(p, p);

//     // mpz_t q;
//     // mpz_init(q);
//     // mpz_urandomb(q, grt, 1024);
//     // mpz_nextprime(q, q);

//     // mpz_t n;
//     // mpz_init(n);
//     // mpz_mul(n, p, q);

//     // mpz_t euler;//euler=n-p-q+1
//     // mpz_init(euler);
//     // mpz_sub(euler, n, p);
//     // mpz_sub(euler, euler,q);
//     // mpz_add_ui(euler, euler, 1);
//     // mpz_powm(p,p,q,n);

//     // gmp_printf("p = %Zd\n\n", p);
//     // gmp_printf("q = %Zd\n\n", q);
//     // gmp_printf("n = %Zd\n\n", n);
//     // gmp_printf("euler = %Zd\n\n", euler);
//     gmplibfunc gn;
//     mpz_t bigmodprime;
//     mpz_t rand;
//     mpz_t bigprime;
//     mpz_t result;
//     mpz_init(result);
//     gn.generate_random(rand,10);
//     gn.generate_big_prime(bigprime,10);
//     gn.generate_big_prime(bigmodprime,20);
//     gn.powm(result,bigprime,rand,bigmodprime);
//     gmp_printf("rand = %Zd\n\n", rand);
//     gmp_printf("bigprime = %Zd\n\n", bigprime);
//     gmp_printf("powm = %Zd\n\n",result);

    
  
//     return 0;

// }