// Boost.Units - A C++ library for zero-overhead dimensional analysis and 
// unit/quantity manipulation and conversion
//
// Copyright (C) 2003-2008 Matthias Christian Schabel
// Copyright (C) 2007-2008 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_UNIT_SYSTEMS_ASTRONOMICAL_LIGHT_MINUTE_HPP_INCLUDED
#define BOOST_UNIT_SYSTEMS_ASTRONOMICAL_LIGHT_MINUTE_HPP_INCLUDED

#include "third_party/boost/boost/units/scaled_base_unit.hpp"
#include "third_party/boost/boost/units/static_rational.hpp"
#include "third_party/boost/boost/units/scale.hpp"
#include "third_party/boost/boost/units/base_units/astronomical/light_second.hpp"

namespace boost {

namespace units {

namespace astronomical {

typedef scaled_base_unit<boost::units::astronomical::light_second_base_unit, scale<60, static_rational<1> > > light_minute_base_unit;

} // namespace astronomical

template<>
struct base_unit_info<astronomical::light_minute_base_unit> {
    static const char* name()   { return("light minute"); }
    static const char* symbol() { return("lmn"); }
};

} // namespace units

} // namespace boost

#endif // BOOST_UNIT_SYSTEMS_ASTRONOMICAL_LIGHT_MINUTE_HPP_INCLUDED
