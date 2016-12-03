#ifndef RapidLab_opt_gaussseidel_hpp
#define RapidLab_opt_gaussseidel_hpp

template <size_t _size_p>
int optimizer<_size_p>::gauss_seidel(
    const Eigen::Matrix<interval, _size_p, _size_p>& A,
    const std::array<double, _size_p>& b,
    box<_size_p>& x,
    const std::array<double, _size_p>& x_tilda) const {

    Eigen::Matrix<double, _size_p, _size_p> mid_matrix;
    for (size_t k = 0; k < _size_p; ++k) {
        for (size_t j = 0; j < _size_p; ++j) {
            mid_matrix(j,k) = mid(A(j,k));
        }
    }

    Eigen::Matrix<double, _size_p, _size_p> C = mid_matrix.inverse();

    for (size_t k = 0; k < _size_p; ++k) {
        interval sum(0);
        for (size_t j = 0; j < _size_p; j++) {
            if (j != k) {
                interval A_aux(0);
                for (size_t i = 0; i < _size_p; i++) {
                    A_aux += C(k,i) * A(i,j);
                }
                sum += A_aux * (x[j] - x_tilda[j]);
            }
        }
        interval b_aux(0);
        for (size_t i = 0; i < _size_p; i++) {
            b_aux += C(k,i) * b[i];
        }
        interval numerator = (-b_aux + sum);
        interval A_aux(0);
        for (size_t i = 0; i < _size_p; i++) {
            A_aux += C(k,i) * A(i,k);
        }
        interval denominator = A_aux;

        if (!zero_in(intersect(numerator, denominator))) {
            if (!zero_in(denominator)) {
                interval x_prime;
                x_prime = x_tilda[k] - (numerator / denominator);

                x[k] = intersect(x_prime, x[k]);
                if (std::isnan(x[k].lower())) {
                    return 1;
                }
            }
        }
    }

    return 0;
}

#endif
