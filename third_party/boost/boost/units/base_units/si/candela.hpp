// Boost.Units - A C++ library for zero-overhead dimensional analysis and 
// unit/quantity manipulation and conversion
//
// Copyright (C) 2003-2008 Matthias Christian Schabel
// Copyright (C) 2007-2008 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_UNITS_SI_CANDELA_BASE_UNIT_HPP
#define BOOST_UNITS_SI_CANDELA_BASE_UNIT_HPP

#include <string>

#include "third_party/boost/boost/units/config.hpp"
#include "third_party/boost/boost/units/base_unit.hpp"
#include "third_party/boost/boost/units/physical_dimensions/luminous_intensity.hpp"

namespace boost {

namespace units {

namespace si {

struct candela_base_unit : public base_unit<candela_base_unit, luminous_intensity_dimension, -3>
{
    static std::string name()   { return("candela"); }
    static std::string symbol() { return("cd"); }
};

} // namespace si

} // namespace units

} // namespace boost

#if BOOST_UNITS_HAS_BOOST_TYPEOF

#include BOOST_TYPEOF_INCREMENT_REGISTRATION_GROUP()

BOOST_TYPEOF_REGISTER_TYPE(boost::units::si::candela_base_unit)

#endif

//#include "third_party/boost/boost/units/base_units/detail/conversions.hpp"

#endif // BOOST_UNITS_SI_CANDELA_BASE_UNIT_HPP
