// Boost.Units - A C++ library for zero-overhead dimensional analysis and 
// unit/quantity manipulation and conversion
//
// Copyright (C) 2003-2008 Matthias Christian Schabel
// Copyright (C) 2008 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_UNITS_SI_ILLUMINANCE_HPP
#define BOOST_UNITS_SI_ILLUMINANCE_HPP

#include "third_party/boost/boost/units/systems/si/base.hpp"
#include "third_party/boost/boost/units/physical_dimensions/illuminance.hpp"

namespace boost {

namespace units { 

namespace si {

typedef unit<illuminance_dimension,si::system>      illuminance;
    
BOOST_UNITS_STATIC_CONSTANT(lux,illuminance); 

} // namespace si

} // namespace units

} // namespace boost

#endif // BOOST_UNITS_SI_ILLUMINANCE_HPP
