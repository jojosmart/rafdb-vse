// Boost.Units - A C++ library for zero-overhead dimensional analysis and 
// unit/quantity manipulation and conversion
//
// Copyright (C) 2003-2008 Matthias Christian Schabel
// Copyright (C) 2008 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_UNITS_ELECTRIC_POTENTIAL_DERIVED_DIMENSION_HPP
#define BOOST_UNITS_ELECTRIC_POTENTIAL_DERIVED_DIMENSION_HPP

#include "third_party/boost/boost/units/derived_dimension.hpp"
#include "third_party/boost/boost/units/physical_dimensions/length.hpp"
#include "third_party/boost/boost/units/physical_dimensions/mass.hpp"
#include "third_party/boost/boost/units/physical_dimensions/time.hpp"
#include "third_party/boost/boost/units/physical_dimensions/current.hpp"

namespace boost {

namespace units {

/// derived dimension for electric potential : L^2 M T^-3 I^-1
typedef derived_dimension<length_base_dimension,2,
                          mass_base_dimension,1,
                          time_base_dimension,-3,
                          current_base_dimension,-1>::type electric_potential_dimension;        

} // namespace units

} // namespace boost

#endif // BOOST_UNITS_ELECTRIC_POTENTIAL_DERIVED_DIMENSION_HPP
