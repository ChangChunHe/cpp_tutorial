#include<iostream>
#include<Eigen/Dense>
#include <math.h>

using namespace Eigen;



int modify_idx(int idx,int n){
if (idx<0){
  return idx +n;
}
if (idx>=n){
  return idx-n;
}
return idx;
}

float get_energy(ArrayXXf model,int n){
ArrayXXf nearest(4,2);
nearest << 1,0,
           0,1,
           -1,0,
           0,-1;
float E = 0;
for (int i=0;i<n;i++){
  for (int j=0;j<n;j++){
    for (int k=0;k<4;k++){
      int ix = i+nearest(k,0);
      int iy = j+nearest(k,1);
       ix = modify_idx(ix,n);
       iy = modify_idx(iy,n);
      E -= model(i,j)*model(ix,iy);
    }
  }
}
// E = E / (n*n);
return E/(n*n);
}


float  get_mag_under_temp(float T) {
  int n = 40;
  srand((unsigned) time(NULL));
  ArrayXXf model = ArrayXXf::Random(n,n);
  model = model.sign();
  float E = get_energy(model,n);
for (int iter=0;iter<5e6;iter++){
  int flip_idx = (rand() %(n*n));
  int flip_col = flip_idx % n;
  int flip_row = flip_idx / n;
  ArrayXXf new_model = model;
  new_model(flip_row,flip_col) = -new_model(flip_row,flip_col);
  float new_E = get_energy(new_model,n);
  float ratio = (float)rand() / RAND_MAX;
  if ((new_E < E) || (ratio < exp((E-new_E)/T))) {
    model = new_model;
    E = new_E;
  }
}
float mean_mag = model.sum();
return mean_mag;
}

int main() {
  VectorXd T = VectorXd::LinSpaced(10,0.00000001,3);
  for (int i=0;i<10;i++){
    float mean_mag = get_mag_under_temp(T(i));
    std::cout <<  abs(mean_mag) << '\n';
  }
  return 0;
}
