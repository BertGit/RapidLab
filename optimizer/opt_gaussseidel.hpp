//
//  gaussseidel.hpp
//  RapidLab
//
//  Created by Bertram Müller on 09.05.12.
//  Copyright (c) 2012 University of Stuttgart. All rights reserved.
//

#ifndef RapidLab_opt_gaussseidel_hpp
#define RapidLab_opt_gaussseidel_hpp

#include <Eigen/Core>
#include <Eigen/LU>

template <int _size_p>
int ioptimizer<_size_p>::gauss_seidel(matrix<interval, _size_p, _size_p> const &A, array<interval, _size_p> const &b, array<interval, _size_p> &x, array<interval, _size_p> const &x_tilda) const {
    
    int dim = _size_p;
    
    matrix<double, _size_p, _size_p> midMatrix = mid<_size_p, _size_p>(A);
    array<double, _size_p * _size_p> midMatrix_data = midMatrix.data();
    
    Eigen::Matrix3d eig_matrix(midMatrix_data.data());
    Eigen::Matrix3d C = eig_matrix.inverse();

    if (isinf(C(0,0))) {
        cout << "inf";
        return 0;
    }

    for (int k = 0; k < dim; k++) {
        //Compute preconditioning row
//        double max_mag = 0.0;
//        int i0 = 0;
//        for (int l = 0; l < _size_p; l++) {
//            //take the column with largest magnitude
//            double mag_A = mag(A(k,l));
//            if (mag_A > max_mag) {
//                max_mag = mag_A;
//                i0 = l;
//            }
//        }
//
//        //EDIT LINE
//        //i0 = k;
//        //*********
//        
//        //cout << A;
//        
//        interval sum(0);
//        for (int j = 0; j < _size_p; j++) {
//            if (j != k) {
//                sum = sum + A(i0,j) * (x[j] - x_tilda[j]);
//            }
//        }
//        interval numerator = (-b[i0] + sum);
//        interval denominator = A(i0,k);

        interval sum(0);
        for (int j = 0; j < _size_p; j++) {
            if (j != k) {
                interval A_aux(0);
                for (int i = 0; i < _size_p; i++) {
                    A_aux = A_aux + C(k,i) * A(i,j);
                }
                sum = sum + A_aux * (x[j] - x_tilda[j]);
            }
        }
        interval b_aux(0);
        for (int i = 0; i < _size_p; i++) {
            b_aux = b_aux + C(k,i) * b[i];
        }
        interval numerator = (-b_aux + sum);
        interval A_aux(0);
        for (int i = 0; i < _size_p; i++) {
            A_aux = A_aux + C(k,i) * A(i,k);
        }
        interval denominator = A_aux;

        
        if (!zero_in(intersect(numerator, denominator))) {
            if (zero_in(denominator)) {
                interval div1, div2;
                if (numerator.upper() < 0) {
                    div1 = interval(-INFINITY, numerator.upper() / denominator.upper());
                    div2 = interval(numerator.upper() / denominator.lower(), INFINITY);
                } else {
                    //numerator.lower() > 0
                    div1 = interval(-INFINITY, numerator.lower() / denominator.lower());
                    div2 = interval(numerator.lower() / denominator.upper(), INFINITY);
                }
                interval x_prime1 = x_tilda[k] - div1;
                interval x_prime2 = x_tilda[k] - div2;
                if (x[k].lower() > x_prime1.upper() && x[k].upper() < x_prime2.lower()) {
                    //no solution
                    return 1;
                }
                interval x_aux1 = intersect(x_prime1, x[k]);
                interval x_aux2 = intersect(x_prime2, x[k]);
//                if (denominator.upper() != 0.0 && denominator.lower() != 0.0) {
//                    x[k] = interval(min(x_aux1.lower(), x_aux2.lower()), max(x_aux1.upper(), x_aux2.upper()));
//                }
            } else {
                interval x_prime;
                x_prime = x_tilda[k] - (numerator / denominator);
                
                x[k] = intersect(x_prime, x[k]);
                if (isnan(x[k].lower())) {
                    return 1;
                }
            }
        }
    }
    
    return 0;
}

#endif
