//
//  opt_bc3revise.hpp
//  RapidLab
//
//  Created by Bertram Müller on 21.05.12.
//  Copyright (c) 2012 University of Stuttgart. All rights reserved.
//

#ifndef RapidLab_opt_bc3revise_hpp
#define RapidLab_opt_bc3revise_hpp

const double DOUBLE_MIN_VALUE = 0.000000001;

template<int _size_p>
int ioptimizer<_size_p>::sbc(box<_size_p> &b, int equation, int param) {
    interval I = b[param];
    
    int left_consistent = 0;
    int right_consistent = 0;
    
    interval I_left(I.lower(), mid(I));
    interval I_right(mid(I), I.upper());
    
    if (!left_consistent) {
        //array<interval, _size_p> g_eval = func_d(b);
        if (!zero_in(bc_func(b, interval(I_left.lower(), I_left.lower() + DOUBLE_MIN_VALUE), equation, param))) {
            I_left = interval(I_left.lower() + DOUBLE_MIN_VALUE, I_left.upper());
            
            if (!zero_in(bc_func(b, I_left, equation, param))) {
                I_left = interval(NAN, NAN);
                return 1;
            } else {
                interval numerator = bc_func(b, interval(I_left.lower(), I_left.lower()), equation, param); 
                interval denominator = bc_func_d(b, I_left, equation, param); 
                if (!zero_in(intersect(numerator, denominator))) {
                    interval x_prime;
                    x_prime = I_left.lower() - (numerator / denominator);
                    
                    x_prime = intersect(x_prime, I_left);
                    if (isnan(x_prime.lower())) {
                        return 1;
                    } else {
                        I_left = x_prime;
                    }
                }
            }
        } else {
            left_consistent = 1;
        }
    }
    if (!right_consistent) {
        //array<interval, _size_p> g_eval = func_d(b);
        if (!zero_in(bc_func(b, interval(I_right.upper() - DOUBLE_MIN_VALUE, I_right.upper()), equation, param))) {
            I_right = interval(I_right.lower(), I_right.upper() - DOUBLE_MIN_VALUE);
            
            if (!zero_in(bc_func(b, I_right, equation, param))) {
                I_right = interval(NAN, NAN);
            } else {
                interval numerator = bc_func(b, interval(I_right.upper(), I_right.upper()), equation, param); 
                interval denominator = bc_func_d(b, I_right, equation, param); 
                if (!zero_in(intersect(numerator, denominator))) {
                    interval x_prime;
                    x_prime = I_right.upper() - (numerator / denominator);
                    
                    x_prime = intersect(x_prime, I_right);
                    if (isnan(x_prime.lower())) {
                        return 1;
                    } else {
                        I_right = x_prime;
                    }
                }
            }
        } else {
            right_consistent = 1;
        }
    }
    
    if (isnan(I_left.lower())) {
        if (isnan(I_right.lower())) {
            return 1;
        } else {
            b[param] = interval(I_right.lower(), I_right.upper());
            return 0;
        }
    } 
    if (isnan(I_right.lower())) {
        b[param] = interval(I_left.lower(), I_left.upper());
        return 0;
    }
    b[param] = interval(min(I_left.lower(), I_right.lower()), max(I_left.upper(), I_right.upper()));
    return 0;
}

template<int _size_p>
int ioptimizer<_size_p>::bc3revise(box<_size_p> &b, int equation, int param) {
    interval I = b[param];
    interval I_left = left_narrow(b, I, equation, param);
    if (!isnan(I_left.lower())) {
        //update box
        b[param] = intersect(I, interval(I_left.lower(), b[param].upper()));
        //cout << b[param].lower() << endl;
        return 0;
    } else {
        //box rejected
        return 1;
    }
}

template<int _size_p>
interval ioptimizer<_size_p>::left_narrow(box<_size_p> &b, interval I, int equation, int param)  {
    interval box_restore = b[param];
//    b[param] = I;
//    array<interval, _size_p> g_eval = func_d(b);
//    if (!zero_in(g_eval[equation])) {
    if (!zero_in(bc_func(b, I, equation, param))) {
        return interval(NAN, NAN);
    }
    //check for canonical interval is missing (because of overestimation not necessary)
    interval I_before = I;
    I = newton(b, I, equation, param);
    
    if (isnan(I.lower())) {
        return I;
    }
//    b[param] = interval(I.lower(), I.lower() + 0.0001);
//    g_eval = func_d(b);
    b[param] = box_restore;
//    if (!zero_in(g_eval[equation])) {
    if (!zero_in(bc_func(b, interval(I.lower(), I.lower() + 0.001), equation, param))) {
        interval I1(I.lower(), mid(I));
        interval I2(mid(I), I.upper());
        I = left_narrow(b, I1, equation, param);
        if (isnan(I.lower())) {
            return left_narrow(b, I, equation, param);
        } else {
            return I;
        }
    }
    return I;
}

template<int _size_p>
interval ioptimizer<_size_p>::newton(box<_size_p> &b, interval I, int equation, int param) {
//    interval b_restore = b[param];
//    b[param] = mid(I);
//    array<interval, _size_p> f_d = func_d(b);
//    b[param] = I;
//    matrix<interval, _size_p, _size_p> f_dd = func_dd(b);
//        b[param] = b_restore;
    
    interval numerator = bc_func(b, mid(I), equation, param); 
    interval denominator = bc_func_d(b, I, equation, param); 
//    interval numerator = f_d[equation];
//    interval denominator = f_dd(equation, param);
    
    if (!zero_in(intersect(numerator, denominator))) {
        interval x_prime;
        x_prime = mid(I) - (numerator / denominator);
        
        I = intersect(x_prime, I);
//        if (isnan(x_prime.lower())) {
//            return I;
//        } else {
//            I = x_prime;
//        }
    }
    return I;
}

#endif
