#ifndef _USEGMP_H
#define _USEGMP_H

#include <stdio.h>
#include <gmp.h>

class gmplibfunc
{
    public:
    mpz_t rand;
    mpz_t bigprime;
    
    void generate_random(mpz_t &rand, size_t nbits);
    void generate_big_prime(mpz_t &bigprime, size_t nbits);
    void powm(mpz_t &result, mpz_t &base, mpz_t &exp, mpz_t &mod);

};



















#endif