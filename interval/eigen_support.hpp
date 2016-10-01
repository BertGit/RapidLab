#ifndef RapidLab_eigen_support_hpp
#define RapidLab_eigen_support_hpp

#include "interval/core.hpp"
#include <Eigen/Core>
#include <Eigen/Dense>

namespace Eigen {
template<> struct NumTraits<rapidlab::interval>
 : NumTraits<double> // permits to get the epsilon, dummy_precision, lowest, highest functions
{
  typedef rapidlab::interval Real;
  typedef rapidlab::interval NonInteger;
  typedef rapidlab::interval Nested;
  enum {
    IsComplex = 0,
    IsInteger = 0,
    IsSigned = 1,
    RequireInitialization = 1,
    ReadCost = 2,
    AddCost = 6,
    MulCost = 20
  };
};
}

#endif
