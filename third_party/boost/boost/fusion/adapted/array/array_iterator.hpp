/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_FUSION_ARRAY_ITERATOR_26122005_2250)
#define BOOST_FUSION_ARRAY_ITERATOR_26122005_2250

#include <cstddef>
#include "third_party/boost/boost/config.hpp"
#include "third_party/boost/boost/mpl/int.hpp"
#include "third_party/boost/boost/mpl/assert.hpp"
#include "third_party/boost/boost/mpl/if.hpp"
#include "third_party/boost/boost/mpl/minus.hpp"
#include "third_party/boost/boost/type_traits/is_const.hpp"
#include "third_party/boost/boost/fusion/iterator/iterator_facade.hpp"

namespace boost { namespace fusion
{
    struct random_access_traversal_tag;

    template <typename Array, int Pos>
    struct array_iterator
        : iterator_facade<array_iterator<Array, Pos>, random_access_traversal_tag>
    {
        BOOST_MPL_ASSERT_RELATION(Pos, >=, 0);
        BOOST_MPL_ASSERT_RELATION(Pos, <=, Array::static_size);

        typedef mpl::int_<Pos> index;
        typedef Array array_type;

        array_iterator(Array& a)
            : array(a) {}

        Array& array;

        template <typename Iterator>
        struct value_of
        {
            typedef typename Iterator::array_type array_type;
            typedef typename array_type::value_type type;
        };

        template <typename Iterator>
        struct deref
        {
            typedef typename Iterator::array_type array_type;
            typedef typename 
                mpl::if_<
                    is_const<array_type>
                  , typename array_type::const_reference
                  , typename array_type::reference
                >::type 
            type;

            static type
            call(Iterator const & it)
            {
                return it.array[Iterator::index::value];
            }
        };

        template <typename Iterator, typename N>
        struct advance
        {
            typedef typename Iterator::index index;
            typedef typename Iterator::array_type array_type;
            typedef array_iterator<array_type, index::value + N::value> type;

            static type
            call(Iterator const& i)
            {
                return type(i.array);
            }
        };

        template <typename Iterator>
        struct next : advance<Iterator, mpl::int_<1> > {};

        template <typename Iterator>
        struct prior : advance<Iterator, mpl::int_<-1> > {};

        template <typename I1, typename I2>
        struct distance : mpl::minus<typename I2::index, typename I1::index>
        {
            typedef typename
                mpl::minus<
                    typename I2::index, typename I1::index
                >::type 
            type;

            static type
            call(I1 const&, I2 const&)
            {
                return type();
            }
        };

    private:

        array_iterator<Array, Pos>& operator=(array_iterator<Array, Pos> const&);
    };
}}

#endif
