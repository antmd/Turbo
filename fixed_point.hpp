/* 
 * File:   fixed_point.hpp
 * Author: paradise
 *
 * Created on 5 de septiembre de 2013, 13:31
 */

#ifndef FIXED_POINT_HPP
#define	FIXED_POINT_HPP

#include "basic_types.hpp"
#include "operators.hpp"
#include "iterator.hpp"
#include "to_string.hpp"

#include <cmath> //std::pow())


namespace mpl
{
    template<long long base , int exponent>
    struct positive_pow : public mpl::long_long_integer<base * positive_pow<base,exponent-1>::value> {};

    template<long long base>
    struct positive_pow<base,0> : public mpl::long_long_integer<1> {};

    template<long long int number , int shift>
    struct decimal_leftshift : public mpl::long_long_integer<number * positive_pow<10, shift>::value> {};

    template<long long int number , int shift>
    struct decimal_rightshift : public mpl::long_long_integer<number / positive_pow<10, shift>::value> {};

    template<bool CONDITION , long long NUMBER , int SHIFT>
    struct decimal_shift_chooser
    {
        using shifter = decimal_leftshift<NUMBER,SHIFT>;
    };

    template<long long NUMBER , int SHIFT>
    struct decimal_shift_chooser<false,NUMBER,SHIFT>
    {
        using shifter = decimal_rightshift<NUMBER,-SHIFT>;
    };

    template<long long number , int shift>
    struct decimal_shift
    {
        using shifter = typename decimal_shift_chooser<( shift >= 0 ) , number , shift>::shifter;
        static const long long int value = shifter::value;
    };

    /* Fixed-point implementation: */

    using fixed_point_bits      = long long int;
    using fixed_point_precision = unsigned int;

#ifndef MPL_CUSTOM_FIXED_POINT_DEFAULT_PRECISION
    const fixed_point_precision DEFAULT_FRACTIONAL_PRECISION = 8; 
#else
    const fixed_point_precision DEFAULT_FRACTIONAL_PRECISION = MPL_CUSTOM_FIXED_POINT_DEFAULT_PRECISION; 
#endif

    template<fixed_point_bits BITS , fixed_point_precision PRECISION = DEFAULT_FRACTIONAL_PRECISION>
    struct fixed_point
    {
        static const fixed_point_bits bits = BITS;
        static const fixed_point_precision precision = PRECISION;
        
        operator float()
        {
            return (float)BITS * std::pow(10.0f,-(float)PRECISION);
        };
    };
    
    template<typename NUMBER>
    struct normalize_t;
    
    template<typename NUMBER>
    using normalize = typename normalize_t<NUMBER>::result;
    
    namespace
    {
        template<bool is_negative , int mantissa , int exponent> 
        struct make_decimal;
        
        template<int mantissa , int exponent>
        struct make_decimal<true , mantissa , exponent>
        {
            using result = mpl::fixed_point<mantissa , -exponent>;
        };
        
        template<int mantissa , int exponent>
        struct make_decimal<false , mantissa , exponent>
        {
            using result = mpl::fixed_point<mpl::decimal_leftshift<mantissa , exponent>::value,0>;
        };
    }

    template<int mantissa , int exponent = 0 , fixed_point_precision PRECISION = mpl::DEFAULT_FRACTIONAL_PRECISION> // MANTISSA x 10^EXPONENT
    using decimal = typename make_decimal<(exponent < 0) , mantissa,exponent>::result;


    template<fixed_point_bits BITS , fixed_point_precision PRECISION>
    struct zero_t<mpl::fixed_point<BITS,PRECISION>> : public mpl::function<mpl::fixed_point<0,0>> {};

    template<fixed_point_bits BITS , fixed_point_precision PRECISION>
    struct one_t<mpl::fixed_point<BITS,PRECISION>> : public mpl::function<fixed_point<1,0>> {};
    

    
    template<fixed_point_bits NUMBER_BITS , fixed_point_precision NUMBER_PRECISION>
    struct normalize_t<mpl::fixed_point<NUMBER_BITS,NUMBER_PRECISION>>
    {
        template<bool IS_MULTIPLE , typename NUMBER>
        struct normalizer;
        
        template<fixed_point_bits BITS , fixed_point_precision PRECISION>
        struct normalizer<true,mpl::fixed_point<BITS,PRECISION>>
        {
            static const fixed_point_bits new_bits = BITS / 10;
            static const fixed_point_precision new_precision = PRECISION - 1;
            
            using result = typename normalizer<(new_precision > 0) && (new_bits % 10 == 0) , mpl::fixed_point<new_bits,new_precision>>::result;
        };
        
        template<fixed_point_bits BITS , fixed_point_precision PRECISION>
        struct normalizer<false,mpl::fixed_point<BITS,PRECISION>>
        {
            using result = mpl::fixed_point<BITS,PRECISION>;
        };
        
        using result = typename normalizer<(NUMBER_PRECISION > 0) && (NUMBER_BITS % 10 == 0) , mpl::fixed_point<NUMBER_BITS,NUMBER_PRECISION>>::result;
    };
    
    
    /* Comparison operator */
    
    template<fixed_point_bits BITS1 , fixed_point_bits BITS2 , fixed_point_precision P1 , fixed_point_precision P2>
    struct equal_t<mpl::fixed_point<BITS1,P1> , mpl::fixed_point<BITS2,P2>> : public mpl::function<mpl::boolean<BITS1 == BITS2>> {}; //Supposing the numbers are normalized, the precission must be the same, so the bits are equal if the numbers are equal

    /* fixed vs fixed operations: */

    
    //fixed vs fixed of same precision:
    
    template<fixed_point_bits BITS1 , fixed_point_bits BITS2 , fixed_point_precision PRECISION>
    struct add_t<mpl::fixed_point<BITS1,PRECISION> , mpl::fixed_point<BITS2,PRECISION>> : public mpl::function<mpl::normalize<fixed_point<BITS1+BITS2 , PRECISION>>> {};

    template<fixed_point_bits BITS1 , fixed_point_bits BITS2 , fixed_point_precision PRECISION>
    struct sub_t<mpl::fixed_point<BITS1,PRECISION> , mpl::fixed_point<BITS2,PRECISION>> : public mpl::function<mpl::normalize<fixed_point<BITS1-BITS2 , PRECISION>>> {};

    template<fixed_point_bits BITS1 , fixed_point_bits BITS2 , fixed_point_precision PRECISION>
    struct mul_t<mpl::fixed_point<BITS1,PRECISION> , mpl::fixed_point<BITS2,PRECISION>> : public mpl::function<mpl::normalize<fixed_point<BITS1*BITS2 , PRECISION + PRECISION>>> {};

    template<fixed_point_bits BITS1 , fixed_point_bits BITS2 , fixed_point_precision PRECISION>
    struct div_t<mpl::fixed_point<BITS1,PRECISION> , mpl::fixed_point<BITS2,PRECISION>> : public mpl::function<mpl::normalize<fixed_point<decimal_leftshift<BITS1,PRECISION>::value/BITS2 , PRECISION>>> {};
    
    //fixed vs fixed with different precision:
    
    template<fixed_point_bits BITS1 , fixed_point_bits BITS2 , fixed_point_precision P1 , fixed_point_precision P2>
    struct add_t<mpl::fixed_point<BITS1,P1> , mpl::fixed_point<BITS2,P2>>
    {
        static const fixed_point_precision max = P1 >= P2 ? P1 : P2;
        
        using result = mpl::normalize<mpl::add<mpl::fixed_point<mpl::decimal_shift<BITS1 , max - P1>::value , max> , mpl::fixed_point<mpl::decimal_shift<BITS2 , max - P2>::value , max>>>; 
    };

    template<fixed_point_bits BITS1 , fixed_point_bits BITS2 , fixed_point_precision P1 , fixed_point_precision P2>
    struct sub_t<mpl::fixed_point<BITS1,P1> , mpl::fixed_point<BITS2,P2>>
    {
        static const fixed_point_precision max = P1 >= P2 ? P1 : P2;
        
        using result = mpl::normalize<mpl::sub<mpl::fixed_point<mpl::decimal_shift<BITS1 , max - P1>::value , max> , mpl::fixed_point<mpl::decimal_shift<BITS2 , max - P2>::value , max>>>; 
    };

    template<fixed_point_bits BITS1 , fixed_point_bits BITS2 , fixed_point_precision P1 , fixed_point_precision P2>
    struct mul_t<mpl::fixed_point<BITS1,P1> , mpl::fixed_point<BITS2,P2>>
    {
        static const fixed_point_precision max = P1 >= P2 ? P1 : P2;
        
        using result = mpl::normalize<mpl::fixed_point<BITS1 * BITS2 , P1 + P2>>; 
    };
    
    template<fixed_point_bits BITS1 , fixed_point_bits BITS2 , fixed_point_precision P1 , fixed_point_precision P2>
    struct div_t<mpl::fixed_point<BITS1,P1> , mpl::fixed_point<BITS2,P2>> 
    {
        static const fixed_point_precision max = P1 >= P2 ? P1 : P2;
        
        using result = mpl::normalize<mpl::fixed_point<decimal_leftshift<BITS1,P2>::value / BITS2 , P1 - P2>>; 
    };
    
    
    /* Operations between integral and fixed-point values */
    
    template<typename T , T v , fixed_point_bits BITS , fixed_point_precision PRECISION>
    struct add_t<mpl::value_t<T,v>,mpl::fixed_point<BITS,PRECISION>> : public mpl::function<mpl::add<mpl::decimal<(fixed_point_bits)v,0,PRECISION>,mpl::fixed_point<BITS,PRECISION>>> {};

    template<typename T , T v , fixed_point_bits BITS , fixed_point_precision PRECISION>
    struct add_t<mpl::fixed_point<BITS,PRECISION>,mpl::value_t<T,v>> : public mpl::function<mpl::add<mpl::fixed_point<BITS,PRECISION>,mpl::decimal<(fixed_point_bits)v,0,PRECISION>>> {};
    
    
    template<typename T , T v , fixed_point_bits BITS , fixed_point_precision PRECISION>
    struct sub_t<mpl::value_t<T,v>,mpl::fixed_point<BITS,PRECISION>> : public mpl::function<mpl::sub<mpl::decimal<(fixed_point_bits)v,0,PRECISION>,mpl::fixed_point<BITS,PRECISION>>> {};

    template<typename T , T v , fixed_point_bits BITS , fixed_point_precision PRECISION>
    struct sub_t<mpl::fixed_point<BITS,PRECISION>,mpl::value_t<T,v>> : public mpl::function<mpl::sub<mpl::fixed_point<BITS,PRECISION>,mpl::decimal<(fixed_point_bits)v,0,PRECISION>>> {};
    
    
    template<typename T , T v , fixed_point_bits BITS , fixed_point_precision PRECISION>
    struct mul_t<mpl::value_t<T,v>,mpl::fixed_point<BITS,PRECISION>> : public mpl::function<mpl::mul<mpl::decimal<(fixed_point_bits)v,0,PRECISION>,mpl::fixed_point<BITS,PRECISION>>> {};

    template<typename T , T v , fixed_point_bits BITS , fixed_point_precision PRECISION>
    struct mul_t<mpl::fixed_point<BITS,PRECISION>,mpl::value_t<T,v>> : public mpl::function<mpl::mul<mpl::fixed_point<BITS,PRECISION>,mpl::decimal<(fixed_point_bits)v,0,PRECISION>>> {};
    
    
    template<typename T , T v , fixed_point_bits BITS , fixed_point_precision PRECISION>
    struct div_t<mpl::value_t<T,v>,mpl::fixed_point<BITS,PRECISION>> : public mpl::function<mpl::div<mpl::decimal<(fixed_point_bits)v,0,PRECISION>,mpl::fixed_point<BITS,PRECISION>>> {};

    template<typename T , T v , fixed_point_bits BITS , fixed_point_precision PRECISION>
    struct div_t<mpl::fixed_point<BITS,PRECISION>,mpl::value_t<T,v>> : public mpl::function<mpl::div<mpl::fixed_point<BITS,PRECISION>,mpl::decimal<(fixed_point_bits)v,0,PRECISION>>> {};




    /* mpl::to_string */
    
    template<fixed_point_bits BITS , fixed_point_precision PRECISION>
    struct to_string_t<mpl::fixed_point<BITS,PRECISION>>
    {
        operator std::string() const
        {
            std::ostringstream os;
            os << mpl::fixed_point<BITS,PRECISION>();
            return os.str();
        }
    };
}

#endif	/* FIXED_POINT_HPP */

