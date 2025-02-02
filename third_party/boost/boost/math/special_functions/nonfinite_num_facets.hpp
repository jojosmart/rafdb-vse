#ifndef BOOST_MATH_NONFINITE_NUM_FACETS_HPP
#define BOOST_MATH_NONFINITE_NUM_FACETS_HPP

// Copyright (c) 2006 Johan Rade
// Copyright 2011 Paul A. Bristow (comments)

// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

/*
\file

\brief non_finite_num facets for C99 standard output of infinity and NaN.

\details See fuller documentation at Boost.Math Facets
  for Floating-Point Infinities and NaNs.
*/

#include <cstring>
#include <ios>
#include <limits>
#include <locale>

#include "third_party/boost/boost/version.hpp"

#include "third_party/boost/boost/math/special_functions/fpclassify.hpp"
#include "third_party/boost/boost/math/special_functions/sign.hpp"

#ifdef _MSC_VER
#   pragma warning(push)
#   pragma warning(disable : 4127) // conditional expression is constant.
#   pragma warning(disable : 4706) // assignment within conditional expression.
#   pragma warning(disable : 4224) // formal parameter 'version' was previously defined as a type.
#endif

namespace boost {
  namespace math {

    // flags (enums can be ORed together)       -----------------------------------

    const int legacy = 0x1; //!< get facet will recognize most string representations of infinity and NaN.
    const int signed_zero = 0x2; //!< put facet will distinguish between positive and negative zero.
    const int trap_infinity = 0x4; /*!< put facet will throw an exception of type std::ios_base::failure
       when an attempt is made to format positive or negative infinity.
       get will set the fail bit of the stream when an attempt is made
       to parse a string that represents positive or negative sign infinity.
    */
    const int trap_nan = 0x8; /*!< put facet will throw an exception of type std::ios_base::failure
       when an attempt is made to format positive or negative NaN.
       get will set the fail bit of the stream when an attempt is made
       to parse a string that represents positive or negative sign infinity.
       */

    // class nonfinite_num_put -----------------------------------------------------

    template<
      class CharType,
      class OutputIterator = std::ostreambuf_iterator<CharType>
            >
    class nonfinite_num_put : public std::num_put<CharType, OutputIterator>
    {
    public:
      explicit nonfinite_num_put(int flags = 0) : flags_(flags) {}

    protected:
      virtual OutputIterator do_put(
        OutputIterator it, std::ios_base& iosb,
        CharType fill, double val) const
      {
        put_and_reset_width(it, iosb, fill, val);
        return it;
      }

      virtual OutputIterator do_put(
        OutputIterator it, std::ios_base& iosb,
        CharType fill, long double val) const
      {
        put_and_reset_width(it, iosb, fill, val);
        return it;
      }

    private:
      template<class ValType> void put_and_reset_width(
        OutputIterator& it, std::ios_base& iosb,
        CharType fill, ValType val) const
      {
        put_impl(it, iosb, fill, val);
        iosb.width(0);
      }

      template<class ValType> void put_impl(
        OutputIterator& it, std::ios_base& iosb,
        CharType fill, ValType val) const
      {
        switch((boost::math::fpclassify)(val)) {

        case FP_INFINITE:
          if(flags_ & trap_infinity)
            throw std::ios_base::failure("Infinity");
          else if((boost::math::signbit)(val))
            put_num_and_fill(it, iosb, "-", "inf", fill);
          else if(iosb.flags() & std::ios_base::showpos)
            put_num_and_fill(it, iosb, "+", "inf", fill);
          else
            put_num_and_fill(it, iosb, "", "inf", fill);
          break;

        case FP_NAN:
          if(flags_ & trap_nan)
            throw std::ios_base::failure("NaN");
          else if((boost::math::signbit)(val))
            put_num_and_fill(it, iosb, "-", "nan", fill);
          else if(iosb.flags() & std::ios_base::showpos)
            put_num_and_fill(it, iosb, "+", "nan", fill);
          else
            put_num_and_fill(it, iosb, "", "nan", fill);
          break;

        case FP_ZERO:
          if(flags_ & signed_zero) {
            if((boost::math::signbit)(val))
              put_num_and_fill(it, iosb, "-", "0", fill);
            else if(iosb.flags() & std::ios_base::showpos)
              put_num_and_fill(it, iosb, "+", "0", fill);
            else
              put_num_and_fill(it, iosb, "", "0", fill);
          }
          else
            put_num_and_fill(it, iosb, "", "0", fill);
          break;

        default:
          it = std::num_put<CharType, OutputIterator>::do_put(
            it, iosb, fill, val);
          break;
        }
      }

      void put_num_and_fill(
        OutputIterator& it, std::ios_base& iosb, const char* prefix,
        const char* body, CharType fill) const
      {
        int width = (int)std::strlen(prefix) + (int)std::strlen(body);
        std::ios_base::fmtflags adjust
          = iosb.flags() & std::ios_base::adjustfield;
        const std::ctype<CharType>& ct
          = std::use_facet<std::ctype<CharType> >(iosb.getloc());

        if(adjust != std::ios_base::internal && adjust != std::ios_base::left)
          put_fill(it, iosb, fill, width);

        while(*prefix)
          *it = ct.widen(*(prefix++));

        if(adjust == std::ios_base::internal)
          put_fill(it, iosb, fill, width);

        if(iosb.flags() & std::ios_base::uppercase) {
          while(*body)
            *it = ct.toupper(ct.widen(*(body++)));
        }
        else {
          while(*body)
            *it = ct.widen(*(body++));
        }

        if(adjust == std::ios_base::left)
          put_fill(it, iosb, fill, width);
      }

      void put_fill(
        OutputIterator& it, std::ios_base& iosb,
        CharType fill, int width) const
      {
        for(std::streamsize i = iosb.width() - static_cast<std::streamsize>(width); i > 0; --i)
          *it = fill;
      }

    private:
      const int flags_;
    };


    // class nonfinite_num_get ------------------------------------------------------

    template<
      class CharType,
      class InputIterator = std::istreambuf_iterator<CharType>
    >
    class nonfinite_num_get : public std::num_get<CharType, InputIterator>
    {

    public:
      explicit nonfinite_num_get(int flags = 0) : flags_(flags)
      {}

    protected:  // float, double and long double versions of do_get.
      virtual InputIterator do_get(
        InputIterator it, InputIterator end, std::ios_base& iosb,
        std::ios_base::iostate& state, float& val) const
      {
        get_and_check_eof(it, end, iosb, state, val);
        return it;
      }

      virtual InputIterator do_get(
        InputIterator it, InputIterator end, std::ios_base& iosb,
        std::ios_base::iostate& state, double& val) const
      {
        get_and_check_eof(it, end, iosb, state, val);
        return it;
      }

      virtual InputIterator do_get(
        InputIterator it, InputIterator end, std::ios_base& iosb,
        std::ios_base::iostate& state, long double& val) const
      {
        get_and_check_eof(it, end, iosb, state, val);
        return it;
      }

      //..............................................................................

    private:
      template<class ValType> static ValType positive_nan()
      {
        // On some platforms quiet_NaN() may be negative.
        return (boost::math::copysign)(
          std::numeric_limits<ValType>::quiet_NaN(), static_cast<ValType>(1)
          );
        // static_cast<ValType>(1) added Paul A. Bristow 5 Apr 11
      }

      template<class ValType> void get_and_check_eof
      (
        InputIterator& it, InputIterator end, std::ios_base& iosb,
        std::ios_base::iostate& state, ValType& val
      ) const
      {
        get_signed(it, end, iosb, state, val);
        if(it == end)
          state |= std::ios_base::eofbit;
      }

      template<class ValType> void get_signed
      (
        InputIterator& it, InputIterator end, std::ios_base& iosb,
        std::ios_base::iostate& state, ValType& val
      ) const
      {
        const std::ctype<CharType>& ct
          = std::use_facet<std::ctype<CharType> >(iosb.getloc());

        char c = peek_char(it, end, ct);

        bool negative = (c == '-');

        if(negative || c == '+')
        {
          ++it;
          c = peek_char(it, end, ct);
          if(c == '-' || c == '+')
          { // Without this check, "++5" etc would be accepted.
            state |= std::ios_base::failbit;
            return;
          }
        }

        get_unsigned(it, end, iosb, ct, state, val);

        if(negative)
        {
          val = (boost::math::changesign)(val);
        }
      } // void get_signed

      template<class ValType> void get_unsigned
      ( //! Get an unsigned floating-point value into val,
        //! but checking for letters indicating non-finites.
        InputIterator& it, InputIterator end, std::ios_base& iosb,
        const std::ctype<CharType>& ct,
        std::ios_base::iostate& state, ValType& val
      ) const
      {
        switch(peek_char(it, end, ct))
        {
        case 'i':
          get_i(it, end, ct, state, val);
          break;

        case 'n':
          get_n(it, end, ct, state, val);
          break;

        case 'q':
        case 's':
          get_q(it, end, ct, state, val);
          break;

        default: // Got a normal floating-point value into val.
          it = std::num_get<CharType, InputIterator>::do_get(
            it, end, iosb, state, val);
          if((flags_ & legacy) && val == static_cast<ValType>(1)
            && peek_char(it, end, ct) == '#')
            get_one_hash(it, end, ct, state, val);
          break;
        }
      } //  get_unsigned

      //..........................................................................

      template<class ValType> void get_i
      ( // Get the rest of all strings starting with 'i', expect "inf", "infinity".
        InputIterator& it, InputIterator end, const std::ctype<CharType>& ct,
        std::ios_base::iostate& state, ValType& val
      ) const
      {
        if(!std::numeric_limits<ValType>::has_infinity
          || (flags_ & trap_infinity))
        {
            state |= std::ios_base::failbit;
            return;
        }

        ++it;
        if(!match_string(it, end, ct, "nf"))
        {
          state |= std::ios_base::failbit;
          return;
        }

        if(peek_char(it, end, ct) != 'i')
        {
          val = std::numeric_limits<ValType>::infinity();  // "inf"
          return;
        }

        ++it;
        if(!match_string(it, end, ct, "nity"))
        { // Expected "infinity"
          state |= std::ios_base::failbit;
          return;
        }

        val = std::numeric_limits<ValType>::infinity(); // "infinity"
      } // void get_i

      template<class ValType> void get_n
      ( // Get expected strings after 'n', "nan", "nanq", "nans", "nan(...)"
        InputIterator& it, InputIterator end, const std::ctype<CharType>& ct,
        std::ios_base::iostate& state, ValType& val
      ) const
      {
        if(!std::numeric_limits<ValType>::has_quiet_NaN
          || (flags_ & trap_nan)) {
            state |= std::ios_base::failbit;
            return;
        }

        ++it;
        if(!match_string(it, end, ct, "an"))
        {
          state |= std::ios_base::failbit;
          return;
        }

        switch(peek_char(it, end, ct)) {
        case 'q':
        case 's':
          if(flags_ && legacy)
            ++it;
          break;  // "nanq", "nans"

        case '(':   // Optional payload field in (...) follows.
         {
            ++it;
            char c;
            while((c = peek_char(it, end, ct))
              && c != ')' && c != ' ' && c != '\n' && c != '\t')
              ++it;
            if(c != ')')
            { // Optional payload field terminator missing!
              state |= std::ios_base::failbit;
              return;
            }
            ++it;
            break;  // "nan(...)"
          }

        default:
          break;  // "nan"
        }

        val = positive_nan<ValType>();
      } // void get_n

      template<class ValType> void get_q
      ( // Get expected rest of string starting with 'q': "qnan".
        InputIterator& it, InputIterator end, const std::ctype<CharType>& ct,
        std::ios_base::iostate& state, ValType& val
      ) const
      {
        if(!std::numeric_limits<ValType>::has_quiet_NaN
          || (flags_ & trap_nan) || !(flags_ & legacy))
        {
          state |= std::ios_base::failbit;
          return;
        }

        ++it;
        if(!match_string(it, end, ct, "nan"))
        {
          state |= std::ios_base::failbit;
          return;
        }

        val = positive_nan<ValType>(); // "QNAN"
      } //  void get_q

      template<class ValType> void get_one_hash
      ( // Get expected string after having read "1.#": "1.#IND", "1.#QNAN", "1.#SNAN".
        InputIterator& it, InputIterator end, const std::ctype<CharType>& ct,
        std::ios_base::iostate& state, ValType& val
      ) const
      {

        ++it;
        switch(peek_char(it, end, ct))
        {
        case 'i': // from IND (indeterminate), considered same a QNAN.
          get_one_hash_i(it, end, ct, state, val); // "1.#IND"
          return;

        case 'q': // from QNAN
        case 's': // from SNAN - treated the same as QNAN.
          if(std::numeric_limits<ValType>::has_quiet_NaN
            && !(flags_ & trap_nan))
          {
            ++it;
            if(match_string(it, end, ct, "nan"))
            { // "1.#QNAN", "1.#SNAN"
 //             ++it; // removed as caused assert() cannot increment iterator).
// (match_string consumes string, so not needed?).
// https://svn.boost.org/trac/boost/ticket/5467
// Change in nonfinite_num_facet.hpp Paul A. Bristow 11 Apr 11 makes legacy_test.cpp work OK.
              val = positive_nan<ValType>(); // "1.#QNAN"
              return;
            }
          }
          break;

        default:
          break;
        }

        state |= std::ios_base::failbit;
      } //  void get_one_hash

      template<class ValType> void get_one_hash_i
      ( // Get expected strings after 'i', "1.#INF", 1.#IND".
        InputIterator& it, InputIterator end, const std::ctype<CharType>& ct,
        std::ios_base::iostate& state, ValType& val
      ) const
      {
        ++it;

        if(peek_char(it, end, ct) == 'n')
        {
          ++it;
          switch(peek_char(it, end, ct))
          {
          case 'f':  // "1.#INF"
            if(std::numeric_limits<ValType>::has_infinity
              && !(flags_ & trap_infinity))
            {
                ++it;
                val = std::numeric_limits<ValType>::infinity();
                return;
            }
            break;

          case 'd':   // 1.#IND"
            if(std::numeric_limits<ValType>::has_quiet_NaN
              && !(flags_ & trap_nan))
            {
                ++it;
                val = positive_nan<ValType>();
                return;
            }
            break;

          default:
            break;
          }
        }

        state |= std::ios_base::failbit;
      } //  void get_one_hash_i

      //..........................................................................

      char peek_char
      ( //! \return next char in the input buffer, ensuring lowercase (but do not 'consume' char).
        InputIterator& it, InputIterator end,
        const std::ctype<CharType>& ct
      ) const
      {
        if(it == end) return 0;
        return ct.narrow(ct.tolower(*it), 0); // Always tolower to ensure case insensitive.
      }

      bool match_string
      ( //! Match remaining chars to expected string (case insensitive),
        //! consuming chars that match OK.
        //! \return true if matched expected string, else false.
        InputIterator& it, InputIterator end,
        const std::ctype<CharType>& ct,
        const char* s
      ) const
      {
        while(it != end && *s && *s == ct.narrow(ct.tolower(*it), 0))
        {
          ++s;
          ++it; //
        }
        return !*s;
      } // bool match_string

    private:
      const int flags_;
    }; //

    //------------------------------------------------------------------------------

  }   // namespace math
}   // namespace boost

#ifdef _MSC_VER
#   pragma warning(pop)
#endif

#endif
