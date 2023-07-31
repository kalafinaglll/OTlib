#include <iostream>
#include "useGMP.h"
#include "hash_mpz.h"
#include <bitset>
#include "eigenutil.h"
using namespace std;




class ROT_Sender {
    public:
        std::vector<string> m0;
        std::vector<string> m1;
        std::vector<bitset<128>> m0_bitset;
        std::vector<bitset<128>> m1_bitset;


};

class ROT_Receiver {
    public:
        bitset<128> choicebits;
        std::stringstream choicestream;
        bitset<1000000> choice_bit_stream;
        std::vector<string> mc;
        std::vector<bitset<128>> mc_bitset;

};


static const std::string random_bits_string(size_t bitlen){
    std::stringstream s;
    for(int i=0;i<bitlen;i++){
        s<<generate_random_binary();
    }
    return s.str();

}

template<size_t size>
static const std::bitset<size> random_bits_bitstream(double p){
    typename std::bitset<size> bits;
    std::random_device rd;
    std::mt19937 gen( rd());
    std::bernoulli_distribution d(p);// double p = 0.5 

    for( int n = 0; n < size; ++n) {
        bits[n] = d(gen);
    }

    return bits;
}

static const std::bitset<128> to_bitset(std::string s)
{
    auto binary = [](char c) { return c == '0' || c == '1'; };
    auto not_binary = [binary](char c) { return !binary(c); };

    s.erase(std::remove_if(begin(s), end(s), not_binary), end(s));

    return std::bitset<128>(s);
}

static const std::string to_string(std::bitset<64> bs)
{
    return bs.to_string();
}

static const std::string to_string_128(std::bitset<128> bs)
{
    return bs.to_string();
}

static const std::bitset<128> to_bitset_128(std::string s)
{
    auto binary = [](char c) { return c == '0' || c == '1'; };
    auto not_binary = [binary](char c) { return !binary(c); };

    s.erase(std::remove_if(begin(s), end(s), not_binary), end(s));

    return std::bitset<128>(s);
}

