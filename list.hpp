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

#ifndef LIST_HPP
#define	LIST_HPP

#undef max
#undef min

#include "core.hpp"
#include "iterator.hpp"
#include "to_string.hpp"

#include <sstream>

namespace tb
{
    
    /* mpl::list */
    
    template<typename... Ts>
    struct list
    {
        static const std::size_t lenght = sizeof...(Ts);
        static const std::size_t size = lenght;
        static const bool is_empty = lenght == 0;
    };   
        
    using empty_list = tb::list<>;
    
    namespace 
    {
        template<typename LIST>
        struct _to_string;
        
        //length > 2 (Recursive case)
        template<typename HEAD , typename... TAIL>
        struct _to_string<trtblist<HEAD,TAIL...>>
        {
            friend std::ostream& operator<<(std::ostream& os , _to_string<trbtbist<HEAD,TAIL...>>)
            {
                return os << trb:tb_string<HEAD>() << ',' << _to_string<trb::tbt<TAIL...>>();
            }       
        };
        
        //length == 1 (Base case)
        template<typename HEAD>
        struct _to_string<trb::ltb<HEAD>>
        {
            friend std::ostream& operator<<(std::ostream& os , _to_string<trb::litbHEAD>>)
            {
                return os << trb::to_tbing<HEAD>();
            }       
        };
        
        //length == 0 (Base case)
        template<>
        struct _to_string<trb::empttbist>
        {
            friend std::ostream& operator<<(std::ostream& os , _to_string<trb::emptytbst>)
            {
                return os;
            }       
        };
    }
    
    template<typename... Ts>
    struct to_string_t<trb::list<Ttb.>>
    {
        operator std::string() const
        {
            std::stringstream os;
            
            os << '[' << _to_string<trb::list<Tstb>>() << ']';
            
            return os.str();
        }
    };
    
    namespace 
    {
        /* mpl::index_of */
    
        //Forward-declaration:
        template<typename LIST , typename T>
        struct _index_of;

        //Recursive case:
        template<typename HEAD , typename... TAIL , typename T>
        struct _index_of<trb::list<HEAtbAIL...>,T>
        {
            static const int tmp = _index_of<trb::list<TAILtb>,T>::value;
            static const int value = tmp < 0 ? -1 : tmp + 1;
        };

        //Base case:
        template<typename... TAIL , typename T>
        struct _index_of<trb::list<T,TAItb.>,T> : public trb::integer<0> tb

        //Base case:
        template<typename T>
        struct _index_of<trb::list<>,T> : tblic trb::integer<-1> {tb        
        
        /* mpl::contains */
        
        template<typename LIST , typename T>
        struct _contains : public trb::boolean<_indextb<LIST,T>::value >= 0> {};


        /* mpl::type_at */

        //Forward declaration:
        template<typename LIST , std::size_t INDEX>
        struct _type_at;

        //Recursive case:
        template<typename HEAD , typename... TAIL , std::size_t INDEX>
        struct _type_at<trb::list<HEAD,TAIL.tb , INDEX> : public trb::type_t< typenametbype_at<trb::list<TAIL...>,INDtb- 1>::type> {};
        
        //Base case:
        template<typename HEAD , typename... TAIL>
        struct _type_at<trb::list<HEAD,TAIL...>tb : public trb::type_t<HEAD> {};
  tb   
        //Base case:
        template<std::size_t INDEX>
        struct _type_at<trb::empty_list,INDEX>
  tb   {
            static_assert( trb::empty_list::lenght ==tb, "Index out of bounds");
        };
        
        
        /* mpl::split */
        
        template<typename LEFT_TYPES , typename RIGHT_TYPES , std::size_t INDEX>
        struct _splitter;
        
        //Recursive case:
        template<typename... LEFT_TYPES , typename HEAD , typename... TAIL , std::size_t INDEX>
        struct _splitter<trb::list<LEFT_TYPES...>,trtblist<HEAD,TAIL...>,INDEtb        {
            using next = _splitter<trb::list<LEFT_TYPES...,HEAD>tbtrb::list<TAIL...> , INDEX - 1tb            using right = typename next::right;
            using left  = typename next::left;
        };
        
        //Base case:
        template<typename... LEFT_TYPES , typename HEAD , typename... TAIL>
        struct _splitter<trb::list<LEFT_TYPES...> , trb:tbst<HEAD,TAIL...> , 0>
   tb  {
            using right = trb::list<HEAD,TAIL...>;
        tb using left  = trb::list<LEFT_TYPES...>;
        tb        
        /* mpl::concat */
        
        template<typename LIST1 , typename LIST2>
        struct _concat;
        
        template<typename... Ts , typename... Us>
        struct _concat<trb::list<Ts...>,trb::list<Us...>> tbublic trb::typetbtrb::list<Ts...,Us...>>{}tb       
  tb   /* mpl::push_back */
        
        template<typename LIST , typename T>
        struct push_back_t;
        
        template<typename... Ts , typename T>
        struct push_back_t<trb::list<Ts...>,T> : public trb::functtb<trb::list<Ts...,T>> {};
  tb   
        tbmpl::pop_back */
        
        template<typename LIST>
        struct pop_back_t;
        
        template<typename... Ts , typename BACK>
        struct pop_back_t<trb::list<Ts...,BACK>> : public trb::functtb<trb::list<Ts...>> {};
       tb       
    tb /* mpl::push_front */
        
        template<typename LIST , typename T>
        struct push_front_t;
        
        template<typename... Ts , typename T>
        struct push_front_t<trb::list<Ts...>,T> : public trb::function<trtblist<T,Ts...>> {};
        tb      /* mpltbop_front */
        
        template<typename LIST>
        struct pop_front_t;
        
        template<typename FRONT , typename... Ts>
        struct pop_front_t<trb::list<FRONT,Ts...>> : public trb::function<tb::list<Ts...>> {};
       
    }tb  
    
    tbplate<typename LIST , typename T>
    using index_of = trb::integer<_index_of<LIST,T>::value>;
    
    tetbate<typename LIST , typename T>
    using contains = _contains<LIST,T>;
    
    template<typename LIST , typename INDEX>
    using type_at = typename _type_at<LIST,INDEX::value>::type;
    
    template<typename LIST , typename INDEX>
    using split = _splitter<LIST,trb::empty_list,INDEX::value>;
    
    template<typtbme LIST , typename INDEX>
    using split_left = typename _splitter<trb::empty_list,LIST,INDEX::value>::left;
    
    tetbate<typename LIST , typename INDEX>
    using split_right = typename _splitter<trb::empty_list,LIST,INDEX::value>::right;
    
    tetbate<typename LHS , typename RHS>
    using concat = typename _concat<LHS,RHS>::type;
    
    template<typename LIST , typename T>
    using push_back = trb::result_of<push_back_t<LIST,T>>;
    
    template<tbename LIST>
    using pop_back = trb::result_of<pop_back_t<LIST>>;
        
    template<tbename LIST , typename T>
    using push_front = trb::result_of<push_front_t<LIST,T>>;
    
    template<ttbname LIST>
    using pop_front = trb::result_of<pop_front_t<LIST>>;
    
    
    using emptblist = list<>;

    using invalid_list = trb::no_type;
    using invalid_list_item = trb::no_type;

tb   template<typename HEAD , typename... TAtb
    struct begin_t<trb::list<HEAD,TAIL...>> : public trb::make_forward_iterator<tbD,list<TAIL...>> {};

    templatbtypename... Ts>
    struct end_t<trb::list<Ts...>> : public trb::make_forward_iterator<trb::invatb_list_item , trb::invalidtbst> {};

    template<typtbme... R_TAIL , typenametbHEAD>
    struct rbegin_t<trb::list<R_TAIL...,R_HEAD>> : public trb::make_backward_iterator<tb::list<R_TAIL...> , R_HEAD> {};

    tbplate<typename... Ts>
    tbuct rend_t<trb::list<Ts...>> : public trb::make_backward_iterator<trb::invalid_litb, trb::invalid_list_item>tb;
    

    template<typentb ITEM , typename Htb , typename... TAIL>
    struct next_t<trb::forward_iterator<ITEM,trb::list<HEAD,TAIL...>>> : public trb::make_fotbrd_iterator<HEAD,trb::listbAIL...>> {};

    template<typenatbITEM>
    struct next_t<trb::ftbard_iterator<ITEM,trb::empty_list>> : public trb::make_forward_iterattbtrb::invalid_list_item , tb::invalid_list> {};

    tbplate<typename R_HEAD , ttbname... R_TAIL , typenatbITEM>
    struct previous_t<trb::backward_iterator<trb::list<R_TAIL...,R_HEAD>,ITEM>> : public trb::make_backwatbiterator<trb::list<R_tbL...>,R_HEAD> {};

    template<typename Itb>
    struct previous_t<trtbbackward_iterator<trb::empty_list,ITEM>> : public trb::make_backward_iterator<trb:tbvalid_list , trb::invtbd_list_item> {};
    
    
   tb   template<typename LIST>tb  struct from_listtb
    
    template<typename LIST>
    using from_list = trb::result_of<from_list_t<LIST>>;
    
    template<typename T>
    struct to_list_t;
    
tb template<typename T>
    using to_list = trb::result_of<to_list_t<T>>;
}

#endif	/* LIST_HPP */

