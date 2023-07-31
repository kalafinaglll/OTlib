#include "OTExtension.h"
#include <bitset>
#include <Eigen/Eigen>
#include <string>
#include <iostream>
#include "time.h"
#include <chrono>
using namespace std;
using namespace Eigen;
using namespace std::chrono;



void OTE_Sender::init(int rows,int cols){

    generate_rand_bitstream_bitset_128(random_str,128);
    //S = random_str.to_string();
    M.resize(rows,cols);


}

void OTE_Sender::col_OT_Receiver_init(mpz_t &g,mpz_t &prime){
    size_t privsize = 15;
    ot_rev.init(gm,g,prime,privsize);
    mpz_init(this->Y);
    mpz_set(Y,ot_rev.Y);
    // mpz_init(this->plaintext0);
    // mpz_init(this->plaintext1);
}


void OTE_Sender::col_OT_Receiver_calchash(mpz_t &X, int choice_bit){
    ot_rev.calchash(X,choice_bit);
    mpz_set(Y,ot_rev.Y);

}

void OTE_Sender::col_OT_Receiver_dec(mpz_class &c0,mpz_class &c1){
    ot_rev.dec(c0,c1);
    // mpz_set(plaintext0,ot_rev.M0);
    // mpz_set(plaintext1,ot_rev.M1);
    mpz_set(plaintext0.get_mpz_t(),ot_rev.M0);
    mpz_set(plaintext1.get_mpz_t(),ot_rev.M1);

}

void OTE_Sender::final_OTE_m0m1_load(){
    std::stringstream sq;
    for(int i=0;i<random_str.size();i++){sq<<random_str[i];}
    for(int i=0;i<col_length;i++){

        // m0.push_back(to_string_128(to_bitset_128(Row_to_String(M,i))));
        // m1.push_back(to_string_128(to_bitset_128(sq.str()) ^ to_bitset_128(Row_to_String(M,i))));

        // cout<<"m0 string is:"<<m0[i]<<endl;
        // cout<<"m1 string is:"<<m1[i]<<endl;
        
        m0_bitset.push_back(Row_to_bitset(M,i));
        m1_bitset.push_back(to_bitset_128(sq.str()) ^ Row_to_bitset(M,i));

        // cout<<"m0 bitset is:"<<m0_bitset[i]<<endl;
        // cout<<"m1 bitset is:"<<m1_bitset[i]<<endl;
        
    }

}


void OTE_Receiver::init(int row,int column){

    bitset<1000000> bits;
    generate_rand_bitstream_bitset_1million(bits,1000000);

    rand_oneMillion_bitsteam = bits;

    Matrix<int,Dynamic,Dynamic> M;
    Generate_random_Binary_Matrix(M,col_length, 128);
    M0.resize(col_length,128);
    M1.resize(col_length,128);

    //cout<<"M0 M1"<<endl;

    for(int i=0;i<col_length;i++){
        
        if((bits[i] && 1) == 1){
            M0.row(i) = M.row(i);
            M1.row(i) = M.row(i).unaryExpr([](const int x){return x ^ 1;});
            continue;
        }
        if((bits[i] && 1) == 0){
            M0.row(i) = M.row(i);
            M1.row(i) = M.row(i);
            continue;
        }
        else{
            cout<<"error:"<<bits[i]<<endl;
            break;
        }
        
    }


    // cout<<bits[1]<<endl;
    // cout<<M0.row(1)<<endl;
    // cout<<M1.row(1)<<endl;
    // std::stringstream ss;
    // ss<<M0.row(1);
    // cout<<ss.str()<<endl;
    // std::stringstream ss2;
    // ss2<<ss.str();
    // MatrixXd test;
    // test = FromString(ss.str());
    // cout<<test<<endl;
    // M0.row(1) = test.row(1);
    // cout<<M0.row(1)<<endl;



}

void OTE_Receiver::col_OT_Sender_init(){

    size_t privsize = 15;
    otsend.init(gm, privsize, 10, 128);
    mpz_init(this->g);
    mpz_init(this->prime);
    mpz_set(g,otsend.g);
    mpz_set(prime,otsend.prime);
    mpz_init(this->X);
    mpz_set(X,otsend.X);
}

void OTE_Receiver::col_OT_Sender_enc(mpz_t &Y,char const *mm0,char const *mm1){

    otsend.enc(Y,mm0,mm1);
    c0 = otsend.c0;
    c1 = otsend.c1;
}


void test_OTE_single(){
    OTE_Sender oteS;
    OTE_Receiver oteR;
    oteR.init(1000000,128);
    oteS.init(1000000,128);

    oteR.col_OT_Sender_init();
    oteS.col_OT_Receiver_init(oteR.g,oteR.prime);
    
    oteS.col_OT_Receiver_calchash(oteR.X,1);
    oteR.col_OT_Sender_enc(oteS.Y,"1234567","7654321");
    oteS.col_OT_Receiver_dec(oteR.c0,oteR.c1);

    std::cout<<"m0 is:"<<oteS.plaintext0<<"\n";
    std::cout<<"m1 is:"<<oteS.plaintext1<<"\n";

    /*
    otsend.init(gm, privsize, 10, 128);
    otr.init(gm,otsend.g,otsend.prime,privsize);
    otr.calchash(otsend.X,0);
    otsend.enc(otr.Y,"123321","111111");
    otr.dec(otsend.c0,otsend.c1);
    */

}

void test_OTE(){
    OTE_Sender oteS;
    OTE_Receiver oteR;
    oteR.init(1000000,128);
    oteS.init(1000000,128);

    for(int i= 0;i<128;i++){
        //init
        oteR.col_OT_Sender_init();
        oteS.col_OT_Receiver_init(oteR.g,oteR.prime);
        //S to choice
        
        oteS.col_OT_Receiver_calchash(oteR.X,oteS.random_str[i]);
        //m0,m1
        std::string m0,m1;
        m0 = Column_to_String(oteR.M0,i);

        m1 = Column_to_String(oteR.M1,i);

        // cout<<"m0 should be:"<<m0<<endl;
        // cout<<"m0 len should be:"<<strlen(m0.c_str())<<endl;
        // cout<<"m1 should be:"<<m1<<endl;
        // cout<<"m1 len should be:"<<strlen(m1.c_str())<<endl;

        

        oteR.col_OT_Sender_enc(oteS.Y,m0.c_str(),m1.c_str());
        //oteR.col_OT_Sender_enc(oteS.Y,"111111000000111111111111000000111111111111000000111111","000000111111000000000000111111000000000000111111000000");
        // //dec
        oteS.col_OT_Receiver_dec(oteR.c0,oteR.c1);
        //put mc on one column
        // std::cout<<"m0        is:"<<oteS.plaintext0.get_str(2)<<"\n";
        // std::cout<<"m1        is:"<<oteS.plaintext1.get_str(2)<<"\n";

        cout<<"M0 M1 "<<i<<endl;
        if(oteS.random_str[i] == 0){
            int zeropadding = 0;
            std::string p0Bin = oteS.plaintext0.get_str(2);
            int lenofp0 = p0Bin.length();
            zeropadding = col_length - lenofp0;
            for(int j =0;j<zeropadding;j++){
                oteS.M.col(i)[j] = 0 ;
            }
            for(int k = 0;k<lenofp0;k++)
                oteS.M.col(i)[k+zeropadding] = (int)p0Bin[k]-48;
        }
        if(oteS.random_str[i] == 1){
            int zeropadding = 0;
            std::string p1Bin = oteS.plaintext1.get_str(2);
            int lenofp1 = p1Bin.length();
            zeropadding = col_length - lenofp1;
            for(int j =0;j<zeropadding;j++){
                oteS.M.col(i)[j] = 0 ;
            }
            for(int k = 0;k<lenofp1;k++)
                oteS.M.col(i)[k+zeropadding] = (int)p1Bin[k]-48;
        }

    }


    // for(int i=0;i<10;i++){
    //     cout<<"*******************************************"<<endl;

    //     cout<<"first rd bit is:"<<oteS.random_str[i]<<endl;
    //     cout<<"Matrix 0 col is:"<<Column_to_String(oteR.M0,i).substr(0,100)<<endl;
    //     cout<<"Matrix 1 col is:"<<Column_to_String(oteR.M1,i).substr(0,100)<<endl;

    //     // cout<<"M0    row    is:"<<to_string_128(to_bitset_128(Row_to_String(oteR.M0,0))).substr(0,100)<<endl;

    //     string rs = Column_to_String(oteS.M,i);
    //     cout<<"Mathe matrix is:"<<rs.substr(0,100)<<endl;
    //     cout<<"*******************************************"<<endl;
    // }



    // if(rs.substr(0,100) != Column_to_String(oteR.M0,1) || rs.substr(0,100) != Column_to_String(oteR.M1,1) ){

    //     cout<<"error"<<endl;
    // }
    oteS.final_OTE_m0m1_load();
    
    cout<<"OTReceiver choice bit is:"<<oteR.rand_oneMillion_bitsteam[1]<<endl;
    cout<<"Matrix 0 row is:"<<oteS.m0[1]<<endl;

    cout<<"Matrix 1 row is:"<<oteS.m1[1]<<endl;
    

    string rs2 = Row_to_String(oteR.M0,1);
    cout<<"Mathe matri0 is:"<<to_string_128(to_bitset_128(rs2)).substr(0,128)<<endl;

    // cout<<"OTReceiver choice bit is:"<<oteR.rand_oneMillion_bitsteam[1]<<endl;

    // cout<<"OTSenderM:"<<endl;
    // for(int i=0;i<3;i++){

    //     cout<<oteS.M.row(i)<<endl;
    // }
    
    // cout<<"Randomstr:"<<oteS.random_str<<endl;

    // cout<<"OTReceiM0:"<<endl;
    // for(int i=0;i<3;i++){
    //     cout<<oteR.M0.row(i)<<endl;
    // }
    



    

}


void test_OTE_bit_set(){
    OTE_Sender oteS;
    OTE_Receiver oteR;
    oteR.init(1000000,128);
    oteS.init(1000000,128);

    for(int i= 0;i<128;i++){
        //init
        oteR.col_OT_Sender_init();
        oteS.col_OT_Receiver_init(oteR.g,oteR.prime);
        //S to choice
        
        oteS.col_OT_Receiver_calchash(oteR.X,oteS.random_str[i]);
        //m0,m1
        std::string m0,m1;
        m0 = Column_to_String(oteR.M0,i);

        m1 = Column_to_String(oteR.M1,i);

        oteR.col_OT_Sender_enc(oteS.Y,m0.c_str(),m1.c_str());
        // //dec
        oteS.col_OT_Receiver_dec(oteR.c0,oteR.c1);
        //put mc on one column
        cout<<"M0 M1 "<<i<<endl;
        if(oteS.random_str[i] == 0){
            int zeropadding = 0;
            std::string p0Bin = oteS.plaintext0.get_str(2);
            int lenofp0 = p0Bin.length();
            zeropadding = col_length - lenofp0;
            for(int j =0;j<zeropadding;j++){
                oteS.M.col(i)[j] = 0 ;
            }
            for(int k = 0;k<lenofp0;k++)
                oteS.M.col(i)[k+zeropadding] = (int)p0Bin[k]-48;
        }
        if(oteS.random_str[i] == 1){
            int zeropadding = 0;
            std::string p1Bin = oteS.plaintext1.get_str(2);
            int lenofp1 = p1Bin.length();
            zeropadding = col_length - lenofp1;
            for(int j =0;j<zeropadding;j++){
                oteS.M.col(i)[j] = 0 ;
            }
            for(int k = 0;k<lenofp1;k++)
                oteS.M.col(i)[k+zeropadding] = (int)p1Bin[k]-48;
        }

    }

    oteS.final_OTE_m0m1_load();
    
    cout<<"OTReceiver choice bit is:"<<oteR.rand_oneMillion_bitsteam[1]<<endl;
    cout<<"Matrix 0 row is:"<<oteS.m0_bitset[1]<<endl;

    cout<<"Matrix 1 row is:"<<oteS.m1_bitset[1]<<endl;
    

    string rs2 = Row_to_String(oteR.M0,1);
    cout<<"Mathe matri0 is:"<<to_string_128(to_bitset_128(rs2)).substr(0,128)<<endl;

}




// int main(){
//     // std::string S;
//     // bitset<20> bits;
//     // bitset<20> bit1;
//     // bitset<20> bit2;
//     // generate_rand_bitstream_bitset_20(bit1,20);
//     // generate_rand_bitstream_bitset_20(bit2,20);
//     // xor_bit_stream_20(bits,bit1,bit2);
//     // cout<<bits.to_string()<<endl;
//     auto start = high_resolution_clock::now();
//     test_OTE_bit_set();
//     auto stop = high_resolution_clock::now();
//     auto duration = duration_cast<microseconds>(stop - start);
//     cout << "Time taken by OTExtension: for "<<col_length<<" times in "<<duration.count() *1e-6<< " seconds" << endl;
// }