#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Eigen>
#include <ctime>
#include <random>

using namespace std;
using namespace Eigen;
using Eigen::MatrixXd;
using Eigen::MatrixXi;
using Eigen::Matrix;




const int generate_random_binary(){
    static default_random_engine e(time(0));
    static uniform_int_distribution<unsigned> u(0, 1);

    return u(e);

}

void Matrix_xor(Matrix<int,Dynamic,Dynamic> &matrixO,Matrix<int,Dynamic,Dynamic> &matrixA, Matrix<int,Dynamic,Dynamic> &matrixB){
  matrixO = matrixA + matrixB;
  matrixO = matrixO.unaryExpr([](const int x) { return x%2; });
}

void Generate_random_Binary_Matrix(Matrix<int,Dynamic,Dynamic> &output,int row, int column){
  output.resize(row,column);
  output = output.unaryExpr([](const int x){return generate_random_binary();});
  // cout<<output<<endl;

}


// int main()
// {
//   MatrixXd m(2,2);
//   MatrixXi B(4,4);
//   Matrix<int,4,4> A;
//   A.Random();
//   B.setZero();
//   for(int i=0;i<10;i++){
//     cout<<generate_random_binary()<<endl;
//   }
// //   A = A.unaryExpr([](int x){
// //     srand((unsigned) time(0));
// //     x = rand() % 2;
// //     return x;});
// //   A.setRandom();
// //   B.setRandom();

//   A = A.unaryExpr([](const int x){return generate_random_binary();});
//   cout<<"A is:"<<A<<endl;
//   cout<<"B is:"<<B<<endl;
//   B = A + B;
//   B = B.unaryExpr([](const int x) { return x%2; });
//   cout << B << endl;

//   return 0;
// }