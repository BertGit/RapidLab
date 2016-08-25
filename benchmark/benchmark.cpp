#include <iostream>
#include <interval/arithmetic.hpp>
#include <interval/box.hpp>
#include <interval/io.hpp>

#include <boost/numeric/interval.hpp>
#include <boost/timer.hpp>

int main() {
    using namespace rapidlab;


    boost::timer t;
    _MM_SET_ROUNDING_MODE(_MM_ROUND_UP);
    interval m(1,2);
    for (size_t i = 0; i < 10000000; ++i) {
        m = m - 3;
    }
    std::cout << "mine " << t.elapsed() << "\n";
    // boost::timer t2;
    // boost::numeric::interval<double> d(1,2);
    // for (size_t i = 0; i < 10000000; ++i) {
    //     d = d / boost::numeric::interval<double>(3,5);
    // }
    // std::cout << "boost " << t2.elapsed() << "\n";
    // std::cout << d.lower() << "," << d.upper() << "\n";
    std::cout << m << "\n";
}
