/* 
 * File:   main.cpp
 * Author: manu343726
 *
 * Created on 1 de septiembre de 2013, 13:16
 */

//#define TURBO_CUSTOM_SQRT_DEFAULT_ITERATIONS 20

#include "basic_types.hpp"
#include "operators.hpp"
#include "expressions.hpp"
#include "list.hpp"
#include "for_loops.hpp"
#include "numeric_iterators.hpp"
#include "sort.hpp"
#include "fixed_point.hpp"
#include "vector.hpp"
#include "trigonometry.hpp"
#include "matrix3x3.hpp"
#include "matrix4x4.hpp"
#include "prime_filter.hpp"
#include "color.hpp"
#include "sqrt.hpp"

#include <iostream>




using a = mpl::normalize<mpl::decimal<12345,-4>>;
using b = mpl::decimal<25>;
using c = mpl::mul<b,b>;

template<typename FP_NUMBER>
void print_bits()
{
    std::cout << FP_NUMBER::bits << " : " << FP_NUMBER::precision << std::endl;
}

int main()
{  
    
    std::cout << mpl::to_string<c>() << std::endl;
    print_bits<math::pi>();
    print_bits<a>();
    print_bits<b>();
    print_bits<c>();
    
    print_bits<math::sin<math::pi>>();
}

