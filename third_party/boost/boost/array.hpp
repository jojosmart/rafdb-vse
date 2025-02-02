/* The following code declares class array,
 * an STL container (as wrapper) for arrays of constant size.
 *
 * See
 *      http://www.boost.org/libs/array/
 * for documentation.
 *
 * The original author site is at: http://www.josuttis.com/
 *
 * (C) Copyright Nicolai M. Josuttis 2001.
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * 28 Dec 2010 - (mtc) Added cbegin and cend (and crbegin and crend) for C++Ox compatibility.
 * 10 Mar 2010 - (mtc) fill method added, matching resolution of the standard library working group.
 *      See <http://www.open-std.org/jtc1/sc22/wg21/docs/lwg-defects.html#776> or Trac issue #3168
 *      Eventually, we should remove "assign" which is now a synonym for "fill" (Marshall Clow)
 * 10 Mar 2010 - added workaround for SUNCC and !STLPort [trac #3893] (Marshall Clow)
 * 29 Jan 2004 - c_array() added, BOOST_NO_PRIVATE_IN_AGGREGATE removed (Nico Josuttis)
 * 23 Aug 2002 - fix for Non-MSVC compilers combined with MSVC libraries.
 * 05 Aug 2001 - minor update (Nico Josuttis)
 * 20 Jan 2001 - STLport fix (Beman Dawes)
 * 29 Sep 2000 - Initial Revision (Nico Josuttis)
 *
 * Jan 29, 2004
 */
#ifndef BOOST_ARRAY_HPP
#define BOOST_ARRAY_HPP

#include "third_party/boost/boost/detail/workaround.hpp"

#if BOOST_WORKAROUND(BOOST_MSVC, >= 1400)  
# pragma warning(push)  
# pragma warning(disable:4996) // 'std::equal': Function call with parameters that may be unsafe
# pragma warning(disable:4510) // boost::array<T,N>' : default constructor could not be generated 
# pragma warning(disable:4610) // warning C4610: class 'boost::array<T,N>' can never be instantiated - user defined constructor required 
#endif

#include <cstddef>
#include <stdexcept>
#include "third_party/boost/boost/assert.hpp"
#include "third_party/boost/boost/swap.hpp"

// Handles broken standard libraries better than <iterator>
#include "third_party/boost/boost/detail/iterator.hpp"
#include "third_party/boost/boost/throw_exception.hpp"
#include <algorithm>

// FIXES for broken compilers
#include "third_party/boost/boost/config.hpp"


namespace boost {

    template<class T, std::size_t N>
    class array {
      public:
        T elems[N];    // fixed-size array of elements of type T

      public:
        // type definitions
        typedef T              value_type;
        typedef T*             iterator;
        typedef const T*       const_iterator;
        typedef T&             reference;
        typedef const T&       const_reference;
        typedef std::size_t    size_type;
        typedef std::ptrdiff_t difference_type;

        // iterator support
        iterator        begin()       { return elems; }
        const_iterator  begin() const { return elems; }
        const_iterator cbegin() const { return elems; }
        
        iterator        end()       { return elems+N; }
        const_iterator  end() const { return elems+N; }
        const_iterator cend() const { return elems+N; }

        // reverse iterator support
#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION) && !defined(BOOST_MSVC_STD_ITERATOR) && !defined(BOOST_NO_STD_ITERATOR_TRAITS)
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
#elif defined(_MSC_VER) && (_MSC_VER == 1300) && defined(BOOST_DINKUMWARE_STDLIB) && (BOOST_DINKUMWARE_STDLIB == 310)
        // workaround for broken reverse_iterator in VC7
        typedef std::reverse_iterator<std::_Ptrit<value_type, difference_type, iterator,
                                      reference, iterator, reference> > reverse_iterator;
        typedef std::reverse_iterator<std::_Ptrit<value_type, difference_type, const_iterator,
                                      const_reference, iterator, reference> > const_reverse_iterator;
#elif defined(_RWSTD_NO_CLASS_PARTIAL_SPEC) 
        typedef std::reverse_iterator<iterator, std::random_access_iterator_tag, 
              value_type, reference, iterator, difference_type> reverse_iterator; 
        typedef std::reverse_iterator<const_iterator, std::random_access_iterator_tag,
              value_type, const_reference, const_iterator, difference_type> const_reverse_iterator;
#else
        // workaround for broken reverse_iterator implementations
        typedef std::reverse_iterator<iterator,T> reverse_iterator;
        typedef std::reverse_iterator<const_iterator,T> const_reverse_iterator;
#endif

        reverse_iterator rbegin() { return reverse_iterator(end()); }
        const_reverse_iterator rbegin() const {
            return const_reverse_iterator(end());
        }
        const_reverse_iterator crbegin() const {
            return const_reverse_iterator(end());
        }

        reverse_iterator rend() { return reverse_iterator(begin()); }
        const_reverse_iterator rend() const {
            return const_reverse_iterator(begin());
        }
        const_reverse_iterator crend() const {
            return const_reverse_iterator(begin());
        }

        // operator[]
        reference operator[](size_type i) 
        { 
            BOOST_ASSERT( i < N && "out of range" ); 
            return elems[i];
        }
        
        const_reference operator[](size_type i) const 
        {     
            BOOST_ASSERT( i < N && "out of range" ); 
            return elems[i]; 
        }

        // at() with range check
        reference at(size_type i) { rangecheck(i); return elems[i]; }
        const_reference at(size_type i) const { rangecheck(i); return elems[i]; }
    
        // front() and back()
        reference front() 
        { 
            return elems[0]; 
        }
        
        const_reference front() const 
        {
            return elems[0];
        }
        
        reference back() 
        { 
            return elems[N-1]; 
        }
        
        const_reference back() const 
        { 
            return elems[N-1]; 
        }

        // size is constant
        static size_type size() { return N; }
        static bool empty() { return false; }
        static size_type max_size() { return N; }
        enum { static_size = N };

        // swap (note: linear complexity)
        void swap (array<T,N>& y) {
            for (size_type i = 0; i < N; ++i)
                boost::swap(elems[i],y.elems[i]);
        }

        // direct access to data (read-only)
        const T* data() const { return elems; }
        T* data() { return elems; }

        // use array as C array (direct read/write access to data)
        T* c_array() { return elems; }

        // assignment with type conversion
        template <typename T2>
        array<T,N>& operator= (const array<T2,N>& rhs) {
            std::copy(rhs.begin(),rhs.end(), begin());
            return *this;
        }

        // assign one value to all elements
        void assign (const T& value) { fill ( value ); }    // A synonym for fill
        void fill   (const T& value)
        {
            std::fill_n(begin(),size(),value);
        }

        // check range (may be private because it is static)
        static void rangecheck (size_type i) {
            if (i >= size()) {
                std::out_of_range e("array<>: index out of range");
                boost::throw_exception(e);
            }
        }

    };

#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
    template< class T >
    class array< T, 0 > {

      public:
        // type definitions
        typedef T              value_type;
        typedef T*             iterator;
        typedef const T*       const_iterator;
        typedef T&             reference;
        typedef const T&       const_reference;
        typedef std::size_t    size_type;
        typedef std::ptrdiff_t difference_type;

        // iterator support
        iterator        begin()       { return       iterator( reinterpret_cast<       T * >( this ) ); }
        const_iterator  begin() const { return const_iterator( reinterpret_cast< const T * >( this ) ); }
        const_iterator cbegin() const { return const_iterator( reinterpret_cast< const T * >( this ) ); }

        iterator        end()       { return  begin(); }
        const_iterator  end() const { return  begin(); }
        const_iterator cend() const { return cbegin(); }

        // reverse iterator support
#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION) && !defined(BOOST_MSVC_STD_ITERATOR) && !defined(BOOST_NO_STD_ITERATOR_TRAITS)
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
#elif defined(_MSC_VER) && (_MSC_VER == 1300) && defined(BOOST_DINKUMWARE_STDLIB) && (BOOST_DINKUMWARE_STDLIB == 310)
        // workaround for broken reverse_iterator in VC7
        typedef std::reverse_iterator<std::_Ptrit<value_type, difference_type, iterator,
                                      reference, iterator, reference> > reverse_iterator;
        typedef std::reverse_iterator<std::_Ptrit<value_type, difference_type, const_iterator,
                                      const_reference, iterator, reference> > const_reverse_iterator;
#elif defined(_RWSTD_NO_CLASS_PARTIAL_SPEC) 
        typedef std::reverse_iterator<iterator, std::random_access_iterator_tag, 
              value_type, reference, iterator, difference_type> reverse_iterator; 
        typedef std::reverse_iterator<const_iterator, std::random_access_iterator_tag,
              value_type, const_reference, const_iterator, difference_type> const_reverse_iterator;
#else
        // workaround for broken reverse_iterator implementations
        typedef std::reverse_iterator<iterator,T> reverse_iterator;
        typedef std::reverse_iterator<const_iterator,T> const_reverse_iterator;
#endif

        reverse_iterator rbegin() { return reverse_iterator(end()); }
        const_reverse_iterator rbegin() const {
            return const_reverse_iterator(end());
        }
        const_reverse_iterator crbegin() const {
            return const_reverse_iterator(end());
        }

        reverse_iterator rend() { return reverse_iterator(begin()); }
        const_reverse_iterator rend() const {
            return const_reverse_iterator(begin());
        }
        const_reverse_iterator crend() const {
            return const_reverse_iterator(begin());
        }

        // operator[]
        reference operator[](size_type /*i*/)
        {
            return failed_rangecheck();
        }

        const_reference operator[](size_type /*i*/) const
        {
            return failed_rangecheck();
        }

        // at() with range check
        reference at(size_type /*i*/)               {   return failed_rangecheck(); }
        const_reference at(size_type /*i*/) const   {   return failed_rangecheck(); }

        // front() and back()
        reference front()
        {
            return failed_rangecheck();
        }

        const_reference front() const
        {
            return failed_rangecheck();
        }

        reference back()
        {
            return failed_rangecheck();
        }

        const_reference back() const
        {
            return failed_rangecheck();
        }

        // size is constant
        static size_type size() { return 0; }
        static bool empty() { return true; }
        static size_type max_size() { return 0; }
        enum { static_size = 0 };

        void swap (array<T,0>& /*y*/) {
        }

        // direct access to data (read-only)
        const T* data() const { return 0; }
        T* data() { return 0; }

        // use array as C array (direct read/write access to data)
        T* c_array() { return 0; }

        // assignment with type conversion
        template <typename T2>
        array<T,0>& operator= (const array<T2,0>& ) {
            return *this;
        }

        // assign one value to all elements
        void assign (const T& value) { fill ( value ); }
        void fill   (const T& ) {}
        
        // check range (may be private because it is static)
        static reference failed_rangecheck () {
                std::out_of_range e("attempt to access element of an empty array");
                boost::throw_exception(e);
#if defined(BOOST_NO_EXCEPTIONS) || (!defined(BOOST_MSVC) && !defined(__PATHSCALE__))
                //
                // We need to return something here to keep
                // some compilers happy: however we will never
                // actually get here....
                //
                static T placeholder;
                return placeholder;
#endif
            }
    };
#endif

    // comparisons
    template<class T, std::size_t N>
    bool operator== (const array<T,N>& x, const array<T,N>& y) {
        return std::equal(x.begin(), x.end(), y.begin());
    }
    template<class T, std::size_t N>
    bool operator< (const array<T,N>& x, const array<T,N>& y) {
        return std::lexicographical_compare(x.begin(),x.end(),y.begin(),y.end());
    }
    template<class T, std::size_t N>
    bool operator!= (const array<T,N>& x, const array<T,N>& y) {
        return !(x==y);
    }
    template<class T, std::size_t N>
    bool operator> (const array<T,N>& x, const array<T,N>& y) {
        return y<x;
    }
    template<class T, std::size_t N>
    bool operator<= (const array<T,N>& x, const array<T,N>& y) {
        return !(y<x);
    }
    template<class T, std::size_t N>
    bool operator>= (const array<T,N>& x, const array<T,N>& y) {
        return !(x<y);
    }

    // global swap()
    template<class T, std::size_t N>
    inline void swap (array<T,N>& x, array<T,N>& y) {
        x.swap(y);
    }

#if defined(__SUNPRO_CC)
//  Trac ticket #4757; the Sun Solaris compiler can't handle
//  syntax like 'T(&get_c_array(boost::array<T,N>& arg))[N]'
//  
//  We can't just use this for all compilers, because the 
//      borland compilers can't handle this form. 
    namespace detail {
       template <typename T, std::size_t N> struct c_array
       {
           typedef T type[N];
       };
    }
    
   // Specific for boost::array: simply returns its elems data member.
   template <typename T, std::size_t N>
   typename detail::c_array<T,N>::type& get_c_array(boost::array<T,N>& arg)
   {
       return arg.elems;
   }

   // Specific for boost::array: simply returns its elems data member.
   template <typename T, std::size_t N>
   typename const detail::c_array<T,N>::type& get_c_array(const boost::array<T,N>& arg)
   {
       return arg.elems;
   }
#else
// Specific for boost::array: simply returns its elems data member.
    template <typename T, std::size_t N>
    T(&get_c_array(boost::array<T,N>& arg))[N]
    {
        return arg.elems;
    }
    
    // Const version.
    template <typename T, std::size_t N>
    const T(&get_c_array(const boost::array<T,N>& arg))[N]
    {
        return arg.elems;
    }
#endif
    
#if 0
    // Overload for std::array, assuming that std::array will have
    // explicit conversion functions as discussed at the WG21 meeting
    // in Summit, March 2009.
    template <typename T, std::size_t N>
    T(&get_c_array(std::array<T,N>& arg))[N]
    {
        return static_cast<T(&)[N]>(arg);
    }
    
    // Const version.
    template <typename T, std::size_t N>
    const T(&get_c_array(const std::array<T,N>& arg))[N]
    {
        return static_cast<T(&)[N]>(arg);
    }
#endif

} /* namespace boost */


#if BOOST_WORKAROUND(BOOST_MSVC, >= 1400)  
# pragma warning(pop)  
#endif 

#endif /*BOOST_ARRAY_HPP*/
