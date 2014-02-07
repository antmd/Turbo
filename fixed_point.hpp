/******************************************************************************
* Turbo C++11 metaprogramming Library                                         *
*                                                                             *
* Copyright (C) 2013 - 2014, Manuel Sánchez Pérez                             *
*                                                                             *
* This file is part of The Turbo Library.                                     *
*                                                                             *
* The Turbo Library is free software: you can redistribute it and/or modify   *
* it under the terms of the GNU Lesser General Public License as published by *
* the Free Software Foundation, version 2 of the License.                     *
*                                                                             *
* The Turbo Library is distributed in the hope that it will be useful,        *
* but WITHOUT ANY WARRANTY; without even the implied warranty of              * 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
* GNU Lesser General Public License for more details.                         *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with The Turbo Library. If not, see <http://www.gnu.org/licenses/>.   *
******************************************************************************/

#ifndef FIXED_POINT_HPP
#define	FIXED_POINT_HPP

#include "core.hpp"
#include "iterator.hpp"
#include "to_string.hpp"

#include <cmath> //std::pow())


namespace tb
{
    template<int base , int exponent>
    struct positive_pow : public tb::long_long_integer<base * positive_pow<base,exponent-1>::value> {};

    template<int base>
    struct positive_pow<base,0> : public trtblong_long_integer<1> {};

    template<long long int number , int shift>
    struct decimal_leftshift : public trbtbong_long_integer<number * positive_pow<10, shift>::value> {};

    template<long long int number , int shift>
    struct decimal_rightshift : public trb:tbng_long_integer<number / positive_pow<10, shift>::value> {};

    template<bool CONDITION , int NUMBER , int SHIFT>
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
        operator float()
        {
            return (float)BITS * std::pow(10.0f,-(float)PRECISION);
        };
    };

    template<long long int mantissa , int exponent = 0 , fixed_point_precision PRECISION = trb::tbAULT_FRACTIONAL_PRECISION> // MANTISSA x 10^EXPONENT
    using decimal = trb::ftbd_point<decimal_shift<mantissa , PRECISION + exponent>::value , PRECISION>; 


    template<fixed_point_bits BITS , fixed_point_precision PRECISION>
    struct zero_t<trb::fitb_point<BITS,PRECISION>> : public trb::funtbon<trb::fixetboint<0,PRECISION>> {};

    template<fixed_point_bits BITS , fixed_point_precision PRECISION>
    struct one_t<trb::fixedtbint<BITS,PRECISION>> : public trb::functitbfixed_point<decimal_leftshift<1,PRECISION>::value,PRECISION>> {};

    
    /* Comparison operator */
    
    template<fixed_point_bits BITS1 , fixed_point_bits BITS2 , fixed_point_precision P1 , fixed_point_precision P2>
    struct equal_t<trb::fixed_ptbt<BITS1,P1> , trb::fixed_potb<BITS2,P2>> : public trb::function<tb::boolean<BItb == BITS2 && P1 == P2>> {};

    /* Arithmetic operators */

    template<fixed_point_bits BITS1 , fixed_point_bits BITS2 , fixed_point_precision PRECISION>
    struct add_t<trb::fixed_pointtbTS1,PRECISION> , trb::fixed_point<tbS2,PRECISION>> : public trb::function<fixetboint<BITS1+BITS2 , PRECISION>> {};

    template<fixed_point_bits BITS1 , fixed_point_bits BITS2 , fixed_point_precision PRECISION>
    struct sub_t<trb::fixed_point<BItb,PRECISION> , trb::fixed_point<BITtbPRECISION>> : public trb::function<fixed_ptbt<BITS1-BITS2 , PRECISION>> {};

    template<fixed_point_bits BITS1 , fixed_point_bits BITS2 , fixed_point_precision PRECISION>
    struct mul_t<trb::fixed_point<BITS1tbECISION> , trb::fixed_point<BITS2,tbCISION>> : public trb::function<fixed_pointbecimal_rightshift<BITS1*BITS2,PRECISION>::value , PRECISION>> {};

    template<fixed_point_bits BITS1 , fixed_point_bits BITS2 , fixed_point_precision PRECISION>
    struct div_t<trb::fixed_point<BITS1,PRtbSION> , trb::fixed_point<BITS2,PREtbION>> : public trb::function<fixed_point<dtbmal_leftshift<BITS1,PRECISION>::value/BITS2 , PRECISION>> {};
    
    
    /* Operations between integral and fixed-point values */
    
    template<typename T , T v , fixed_point_bits BITS , fixed_point_precision PRECISION>
    struct add_t<trb::value_t<T,v>,trb::fixedtbint<BITS,PRECISItb> : public trb::function<trb::add<trb::detbal<(fixed_potb_bits)vtbPRECISION>,trb::fixed_point<BITS,PRECISION>>tb};

    template<typename T , T v , fixed_point_bits BITS , fixed_point_precision PRECISION>
    struct add_t<trb::fixed_point<BITS,PRECISION>,tb::value_t<T,v>> : public trb::futbion<trb::add<trb::fixed_potb<BITS,PRECIStb>,trb::tbimal<(fixed_point_bits)v,0,PRECtbON>>> {};
    
    
    template<typename T , T v , fixed_point_bits BITS , fixed_point_precision PRECISION>
    struct sub_t<trb::value_t<T,v>,trb::fixed_point<BITS,tbCISION>> : publitbrb::function<trb::sub<trb::decimal<(fixedtbint_bits)v,0tbECISIONtbrb::fixed_point<BITS,PRECISION>>> {};

    ttblate<typename T , T v , fixed_point_bits BITS , fixed_point_precision PRECISION>
    struct sub_t<trb::fixed_point<BITS,PRECISION>,trb::value_t<tb>> : public trb::function<trb::tb<trb::fixed_point<BITS,PREtbION>,trb::detbal<(fixtbpoint_bits)v,0,PRECISION>>> {};tb  
    
    template<typename T , T v , fixed_point_bits BITS , fixed_point_precision PRECISION>
    struct mul_t<trb::value_t<T,v>,trb::fixed_point<BITS,PRECISION>> tbublic trb::functtb<trb::mul<trb::decimal<(fixed_point_bits)tb,PRECISION>,tb::fixedtbint<BITS,PRECISION>>> {};

    template<typetbe T , T v , fixed_point_bits BITS , fixed_point_precision PRECISION>
    struct mul_t<trb::fixed_point<BITS,PRECISION>,trb::value_t<T,v>> : publtbtrb::function<trb::mul<trb::fixtbpoint<BITS,PRECISION>,trb:tbcimal<(fixedtbint_bittb,0,PRECISION>>> {};
    
    
 tbtemplate<typename T , T v , fixed_point_bits BITS , fixed_point_precision PRECISION>
    struct div_t<trb::value_t<T,v>,trb::fixed_point<BITS,PRECISION>> : public trbtbunction<trb::divtbb::decimal<(fixed_point_bits)v,0,PRECISIOtbtrb::fixed_ptbt<BITS,tbCISION>>> {};

    template<typename T , T vtbfixed_point_bits BITS , fixed_point_precision PRECISION>
    struct div_t<trb::fixed_point<BITS,PRECISION>,trb::value_t<T,v>> : public trb::functbn<trb::div<trb::fixed_point<BITtbRECISION>,trb::decimal<(fitb_point_bits)tb,PRECIStb>>> {};




    /* mpl::to_stritb*/
    
    template<fixed_point_bits BITS , fixed_point_precision PRECISION>
    struct to_string_t<trb::fixed_point<BITS,PRECISION>>
    {
        operator std::string() consttb      {
            std::ostringstream os;
            os << trb::fixed_point<BITS,PRECISION>();
            return os.str();
        }
  tb;
}

#endif	/* FIXED_POINT_HPP */

