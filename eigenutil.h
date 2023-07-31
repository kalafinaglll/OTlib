
#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Eigen>
#include <ctime>
#include <random>
#include <bitset>
#include <regex>
using namespace std;
using namespace Eigen;
using Eigen::Matrix;
using std::bitset;




int generate_random_binary();
void Matrix_xor(Matrix<int,Dynamic,Dynamic> &matrixO,Matrix<int,Dynamic,Dynamic> &matrixA, Matrix<int,Dynamic,Dynamic> &matrixB);
void Generate_random_Binary_Matrix(Matrix<int,Dynamic,Dynamic> &output,int row, int column);



static void const generate_rand_bitstream(std::string &S,size_t stringlen){

    for(int i = 0;i<stringlen;i++){
        S += std::to_string(generate_random_binary());
    }

    cout<< S<< endl;

}

static void const generate_rand_bitstream_bitset_20(bitset<20> &bits,size_t stringlen){

    std::string S;
    for(int i = 0;i<stringlen;i++){
        S += std::to_string(generate_random_binary());
    }

    bits = bitset<20>(S);

    cout<< S<< endl;

}

static void const xor_bit_stream_20(bitset<20> &bO, bitset<20> &b1,bitset<20> &b2){
    bO = (b1 ^= b2);
}

static void const generate_rand_bitstream_bitset_128(bitset<128> &bits,size_t stringlen){

    std::string S;
    for(int i = 0;i<stringlen;i++){
        S += std::to_string(generate_random_binary());
    }

    bits = bitset<128>(S);

    cout<< S<< endl;

}

static void const xor_bit_stream_128(bitset<128> &bO, bitset<128> &b1,bitset<128> &b2){
    bO = (b1 ^= b2);
}


static void const generate_rand_bitstream_bitset_1million(bitset<1000000> &bits,size_t stringlen){

    std::string S;
    for(int i = 0;i<stringlen;i++){
        S += std::to_string(generate_random_binary());
    }

    bits = bitset<1000000>(S);

    //cout<< S<< endl;

}

static void const xor_bit_stream_1million(bitset<1000000> &bO, bitset<1000000> &b1,bitset<1000000> &b2){
    bO = (b1 ^= b2);
}


static std::string ToString(const Eigen::MatrixXd &mat) {
    std::stringstream ss;
    ss << mat;
    return ss.str();
}

static std::string Strip(std::string &in) {
    in.erase(std::remove_if(in.begin(), in.end(), [](unsigned char x) {
        return std::isspace(x);
    }), in.end());
    return in;
}

static std::vector<std::string> Split(const std::string &content, const char &delim) {
    std::istringstream iss(content);
    std::vector<std::string> vStrs;
    std::string str;
    while (getline(iss, str, delim)) {
        vStrs.push_back(Strip(str));
    }
    return vStrs;
}

static std::string RemoveExtraSpaces(const std::string &str) {
    bool previous_item_empty = true;
    std::string res;
    for (auto &current_item : str) {
        if (' ' == current_item && !previous_item_empty) {
            res.push_back(current_item);
            previous_item_empty = true;
        } else if (' ' == current_item && previous_item_empty) {
            continue;
        } else {
            res.push_back(current_item);
            previous_item_empty = false;
        }
    }
    return res;
}

static std::string RegexReplaceAll(const std::string &text, const std::string &reg_ex, std::string new_value) {
    std::regex r(reg_ex);
    return std::regex_replace(text, r, new_value);
}

static Eigen::MatrixXd FromString(const std::string &str) {
    std::stringstream stream(str); // stream << str
    std::vector<std::vector<double>> data_list;
    std::string res;
    while(!stream.eof()) {
        getline(stream, res, '\n');
        std::vector<double> row_list;
        auto list_str = Split(RemoveExtraSpaces(res), ' ');
        if (!data_list.empty()) {
            if (data_list.back().size() != list_str.size()) {
                std::cerr << "invalid input: " << data_list.size() + 1 << " row data is not valid\n";
                return Eigen::MatrixXd();
            }
        }
        for (auto &elem : list_str) {
            row_list.push_back(atof(elem.c_str()));
        }
        data_list.push_back(row_list);
    }
    size_t rows = data_list.size();
    size_t cols = data_list.front().size();
    Eigen::MatrixXd mat(rows, cols);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            mat(i, j) = data_list[i][j];
        }
    }
    return mat;
}

static std::string Column_to_String(const Eigen::Matrix<int,Dynamic,Dynamic> &m, int colnumber){
    //cout<<m.col(colnumber)<<endl;
    std::stringstream ss;
    ss<<m.col(colnumber);
    std::string result;
    result = ss.str();

    result.erase(std::remove(result.begin(), result.end(), '\n'), result.cend());
    //cout<<"here is the string:"<<result<<"\n";
    return result;

}

static std::string Row_to_String(const Eigen::Matrix<int,Dynamic,Dynamic> &m, int rownumber){
    // cout<<"******************************************\n"<<endl;
    // cout<<m.row(rownumber)<<endl;
    std::stringstream ss;
    ss<<m.row(rownumber);
    std::string result;
    result = ss.str();

    result.erase(std::remove(result.begin(), result.end(), '\n'), result.cend());
    //cout<<"here is the string:"<<result<<"\n";
    return result;

}


static std::bitset<128> Row_to_bitset(const Eigen::Matrix<int,Dynamic,Dynamic> &m, int rownumber){
    // cout<<"******************************************\n"<<endl;
    // cout<<m.row(rownumber)<<endl;
    //m.row(rownumber);
    bitset<128> result;
    for (int i = 0; i < m.row(rownumber).size() ; i++) {
    // if (m.row(rownumber)[i] > 0) {
    //     result.set(i, true);
    // } else {
    //     result.set(i, false);
    // }
    result.set(m.row(rownumber).size()-i-1,m.row(rownumber)[i]);
    }
    return result;

}
