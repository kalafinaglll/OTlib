#include "Simple_OT.h"
#include "Random_OT.h"
#include "time.h"
#include <chrono>

using namespace std::chrono;




void Random_OT(size_t bitlen,int rand_amount,ROT_Receiver &rot_rec,ROT_Sender &rot_sen){
    // ROT_Receiver rot_rec;
    // ROT_Sender rot_sen;
    OT_Receiver otr;
    OT_Sender otsend;
    mpz_t g,p;
    gmplibfunc gm;
    size_t privsize = 15;

    std::string random_choicebit = random_bits_string(bitlen);
    //cout<<"randomchoice bits are:"<<random_choicebit<<endl;
    for(int i=0;i<rand_amount;i++){

        otsend.init(gm, privsize, 10, 128);
        otr.init(gm,otsend.g,otsend.prime,privsize);

        //cout<<"i is:"<<i<<endl;

        int cb = (int)random_choicebit[i]-48;
        rot_rec.choicestream<<cb;
        
        // if(cb != 0 && cb != 1){
        //     cout<<"cb error:"<<cb<<endl;
        //     break;
        // }
        otr.calchash(otsend.X,cb);

        std::string m0 = random_bits_string(128);
        std::string m1 = random_bits_string(128);

        rot_sen.m0.push_back(m0);
        rot_sen.m1.push_back(m1);


        otsend.enc(otr.Y,m0.c_str(),m1.c_str());
        if(cb == 0){
            otr.dec0(otsend.c0);
            //cout<<"m0 is:"<<otr.M0<<endl;
            rot_rec.mc.push_back(mpz_class(otr.M0).get_str(2));
        }
        if(cb == 1){
            otr.dec1(otsend.c1);
            //cout<<"m1 is:"<<otr.M1<<endl;
            rot_rec.mc.push_back(mpz_class(otr.M1).get_str(2));
        }
        //otr.dec(otsend.c0,otsend.c1);
    }

    //test

    // for(int i=0;i<rot_rec.choicestream.str().size();i++){
    //     cout<<"choice bit is:"<<rot_rec.choicestream.str()[i]<<endl;
    //     cout<<"mc is:"<<rot_rec.mc[i]<<endl;
    //     cout<<"m0 is:"<<rot_sen.m0[i]<<endl;
    //     cout<<"m1 is:"<<rot_sen.m1[i]<<endl;
    // }



}



void Random_OT_Fake(size_t bitlen,int rand_amount,ROT_Receiver &rot_rec,ROT_Sender &rot_sen){
    // ROT_Receiver rot_rec;
    // ROT_Sender rot_sen;
    OT_Receiver otr;
    OT_Sender otsend;
    mpz_t g,p;
    gmplibfunc gm;
    size_t privsize = 15;

    std::string random_choicebit = random_bits_string(bitlen);
    //cout<<"randomchoice bits are:"<<random_choicebit<<endl;
    for(int i=0;i<rand_amount;i++){

        int cb = (int)random_choicebit[i]-48;
        rot_rec.choicestream<<cb;
        

        std::string m0 = random_bits_string(128);
        std::string m1 = random_bits_string(128);

        rot_sen.m0.push_back(m0);
        rot_sen.m1.push_back(m1);


        if(cb == 0){
            rot_rec.mc.push_back(m0);
        }
        if(cb == 1){
            rot_rec.mc.push_back(m1);
        }

    }


}


void Random_OT_Fake_bitset(size_t bitlen,int rand_amount,ROT_Receiver &rot_rec,ROT_Sender &rot_sen){

    bitset<1000000> cb = random_bits_bitstream<1000000>(0.5);

    rot_rec.choice_bit_stream = cb;
    //cout<<"randomchoice bits are:"<<random_choicebit<<endl;
    for(int i=0;i<rand_amount;i++){


        bitset<128> m0 = random_bits_bitstream<128>(0.5);
        bitset<128> m1 = random_bits_bitstream<128>(0.5);

        rot_sen.m0_bitset.push_back(m0);
        rot_sen.m1_bitset.push_back(m1);


        if(cb == 0){
            rot_rec.mc_bitset.push_back(m0);
        }
        if(cb == 1){
            rot_rec.mc_bitset.push_back(m1);
        }

    }


}


void derandomize(ROT_Receiver &rot_rec,ROT_Sender &rot_sen, int amount){

    std::string m0 = random_bits_string(128);
    std::string m1 = random_bits_string(128);


    for(int i=0;i<amount;i++){
        //cout<<"loop in "<<i<<" times"<<endl;
        int randchoice = generate_random_binary();

        //cout<<"choice is:"<<randchoice<<endl;
        int d = randchoice ^ ((int)rot_rec.choicestream.str()[i]-48);

        

        bitset<128> e0 ;
        bitset<128> e1;
        // if(d != 0 && d != 1){
        //      cout<<"d error!"<<endl;
        // }
        if(d == 0){
            //e0 = rot_sen.m0[i] ^ m0
            //cout<<"d is:"<<d<<endl;
            e0 = to_bitset(rot_sen.m0[i]) ^ to_bitset(m0);
            e1 = to_bitset(rot_sen.m1[i]) ^ to_bitset(m1);
        }
        if(d == 1){
            //cout<<"d is:"<<d<<endl;
            e0 = to_bitset(rot_sen.m1[i]) ^ to_bitset(m0);
            e1 = to_bitset(rot_sen.m0[i]) ^ to_bitset(m1);
        }

        // if(e0 == ""|| e1 ==""){
        //     cout<<"xor error!"<<endl;
        //     break;
        // }

        if(randchoice == 0){
            bitset<128> mc = e0 ^ to_bitset(rot_rec.mc[i]);
            // string mc = to_string_128(e0 ^ to_bitset(rot_rec.mc[i]));
            // cout<<"*m0 is:"<<rot_sen.m0[i]<<endl;
            // cout<<"*m1 is:"<<rot_sen.m1[i]<<endl;
            // cout<<" mc is:"<<mc<<endl;
            // cout<<" m0 is:"<<m0<<endl;
            // cout<<" m1 is:"<<m1<<endl;
            // if(mc != m0){
            //     cout<<"mc error 0"<<endl;
            //     break;
            // }
        }
        if(randchoice == 1){
            bitset<128> mc = e1 ^ to_bitset(rot_rec.mc[i]);
            //string mc = to_string_128(e1 ^ to_bitset(rot_rec.mc[i]));
            // cout<<"*m0 is:"<<rot_sen.m0[i]<<endl;
            // cout<<"*m1 is:"<<rot_sen.m1[i]<<endl;
            // cout<<" mc is:"<<mc<<endl;
            // cout<<" m0 is:"<<m0<<endl;
            // cout<<" m1 is:"<<m1<<endl;
            // if(mc != m1){
            //     cout<<"mc error 1"<<endl;
            //     break;
            // }
        }
    }
    

}


void derandomize_bitset(ROT_Receiver &rot_rec,ROT_Sender &rot_sen, int amount){

    bitset<128> m0 = random_bits_bitstream<128>(0.5);
    bitset<128> m1 = random_bits_bitstream<128>(0.5);




    for(int i=0;i<amount;i++){
        //cout<<"loop in "<<i<<" times"<<endl;
        int randchoice = generate_random_binary();

        //cout<<"choice is:"<<randchoice<<endl;
        int d = randchoice ^ rot_rec.choice_bit_stream[i];

        

        bitset<128> e0 ;
        bitset<128> e1;
        // if(d != 0 && d != 1){
        //      cout<<"d error!"<<endl;
        // }
        if(d == 0){
            //e0 = rot_sen.m0[i] ^ m0
            //cout<<"d is:"<<d<<endl;
            e0 = rot_sen.m0_bitset[i] ^ m0;
            e1 = rot_sen.m1_bitset[i] ^ m1;
        }
        if(d == 1){

            e0 = rot_sen.m1_bitset[i] ^ m0;
            e1 = rot_sen.m0_bitset[i] ^ m1;
        }

        if(randchoice == 0){
            bitset<128> mc = e0 ^ rot_rec.mc_bitset[i];
        }
        if(randchoice == 1){
            bitset<128> mc = e1 ^ rot_rec.mc_bitset[i];
        }
    }
    

}



void derandomize_OTE(ROT_Receiver &rot_rec,ROT_Sender &rot_sen, int amount){


}






int main(){
    // std::string S;
    // bitset<20> bits;
    // bitset<20> bit1;
    // bitset<20> bit2;
    // generate_rand_bitstream_bitset_20(bit1,20);
    // generate_rand_bitstream_bitset_20(bit2,20);
    // xor_bit_stream_20(bits,bit1,bit2);
    // cout<<bits.to_string()<<endl;
    ROT_Receiver rot_rec;
    ROT_Sender rot_sen;
    auto rstart = high_resolution_clock::now();
    int rand_amount = 1000000;
    //Random_OT(100000,rand_amount,rot_rec,rot_sen);
    Random_OT_Fake_bitset(1000000,rand_amount,rot_rec,rot_sen);
    auto rstop = high_resolution_clock::now();
    auto rduration = duration_cast<microseconds>(rstop - rstart);
    cout << "Time taken by RandomOT: for "<<rand_amount<<" times in "<< rduration.count() *1e-6<< " seconds" << endl;


    int onlineOT = 1000000;
    auto start = high_resolution_clock::now();
    derandomize_bitset(rot_rec,rot_sen,onlineOT);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by DeRand_onlineOT: for "<<onlineOT<<" times in "<<duration.count() *1e-6<< " seconds" << endl;
 
    return 0;

}