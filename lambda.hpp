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

#ifndef LAMBDA_HPP
#define	LAMBDA_HPP

#include "functional.hpp"
#include "let_expressions.hpp"

namespace tml
{
    /*
     * Represents a lambda expression.
     * 
     * A lambda expression is just a template that holds a variable and a functional
     * expression (The lambda body) where the variable is used. 
     * The lambda expression is dessigned to act as a functional expression to be evaluated lazily,
     * when the value of the parameter is specified. Thats why the 'result'  member of function entities
     * is parametrized.
     * 
     * Because the 'result' member doesn't work like in other function entities (In other function entities returns
     * the result, in a lambda computes the result), a lambda cannot be considered a classic function entity.
     * The implementation trait 'tml::impl::is_function' and the 'tml::eval' implementation are overrided to
     * cover this special behaviour of lambda expressions. 
     */
    template<typename X , typename BODY>
    struct lambda
    {
        /*
         * Lambda body evaluation: Substitute the variable of the lambda in the body with
         * the argumment passed to the lambda and evaluate the resulting expression.
         */
        template<typename ARG>
        using result = tml::eval<tml::let<X,ARG,BODY>>;
    };
    
    namespace impl
    {
        /*
         * A lambda expression is not considered a function entity (See documentation above)
         */
        template<typename X , typename BODY>
        struct is_function<lambda<X,BODY>>
        {
            static constexpr bool result = false;
        };
        
        /*
         * tml::let is overrided to compute the result of the lambda when the lambda itselft is evaluated.
         * Of course a lambda have to be evaluated with its calling parameter, like:
         * 
         *     using result = tml::eval<lambda,param>;
         * 
         * so tml::eval acts as a calling high-level metafunction (See the tml::eval documentation) always when a 
         * lambda is passed.
         */
        template<typename X , typename BODY , typename ARG , bool is_function>
        struct evaluate_impl<is_function,lambda<X,BODY>,ARG> : 
            public tml::function<typename lambda<X,BODY>::result<tml::eval<ARG>>> 
        {};
    }
}

#endif	/* LAMBDA_HPP */
