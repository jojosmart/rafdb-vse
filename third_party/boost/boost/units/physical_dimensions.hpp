// Boost.Units - A C++ library for zero-overhead dimensional analysis and 
// unit/quantity manipulation and conversion
//
// Copyright (C) 2003-2008 Matthias Christian Schabel
// Copyright (C) 2007-2010 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_UNITS_PHYSICAL_UNITS_HPP
#define BOOST_UNITS_PHYSICAL_UNITS_HPP

///
/// \file
/// \brief Physical dimensions according to the SI system.
/// \details This header includes all physical dimension headers for both base
///   and derived dimensions.
///

// Include all of the physical_dimension headers.

// SI seven fundamental dimensions.
#include "third_party/boost/boost/units/physical_dimensions/amount.hpp"
#include "third_party/boost/boost/units/physical_dimensions/current.hpp"
#include "third_party/boost/boost/units/physical_dimensions/length.hpp"
#include "third_party/boost/boost/units/physical_dimensions/luminous_intensity.hpp"
#include "third_party/boost/boost/units/physical_dimensions/mass.hpp"
#include "third_party/boost/boost/units/physical_dimensions/temperature.hpp"
#include "third_party/boost/boost/units/physical_dimensions/time.hpp"

// Base dimensions are extended to include plane and solid angle for convenience.
#include "third_party/boost/boost/units/physical_dimensions/plane_angle.hpp"
#include "third_party/boost/boost/units/physical_dimensions/solid_angle.hpp"

// Derived dimensions.
#include "third_party/boost/boost/units/physical_dimensions/absorbed_dose.hpp"
#include "third_party/boost/boost/units/physical_dimensions/acceleration.hpp"
#include "third_party/boost/boost/units/physical_dimensions/action.hpp"
#include "third_party/boost/boost/units/physical_dimensions/activity.hpp"
#include "third_party/boost/boost/units/physical_dimensions/angular_acceleration.hpp"
#include "third_party/boost/boost/units/physical_dimensions/angular_momentum.hpp"
#include "third_party/boost/boost/units/physical_dimensions/angular_velocity.hpp"
#include "third_party/boost/boost/units/physical_dimensions/area.hpp"
#include "third_party/boost/boost/units/physical_dimensions/capacitance.hpp"
#include "third_party/boost/boost/units/physical_dimensions/conductance.hpp"
#include "third_party/boost/boost/units/physical_dimensions/conductivity.hpp"
#include "third_party/boost/boost/units/physical_dimensions/dose_equivalent.hpp"
#include "third_party/boost/boost/units/physical_dimensions/dynamic_viscosity.hpp"
#include "third_party/boost/boost/units/physical_dimensions/electric_charge.hpp"
#include "third_party/boost/boost/units/physical_dimensions/electric_potential.hpp"
#include "third_party/boost/boost/units/physical_dimensions/energy.hpp"
#include "third_party/boost/boost/units/physical_dimensions/energy_density.hpp"
#include "third_party/boost/boost/units/physical_dimensions/force.hpp"
#include "third_party/boost/boost/units/physical_dimensions/frequency.hpp"
#include "third_party/boost/boost/units/physical_dimensions/heat_capacity.hpp"
#include "third_party/boost/boost/units/physical_dimensions/illuminance.hpp"
#include "third_party/boost/boost/units/physical_dimensions/impedance.hpp"
#include "third_party/boost/boost/units/physical_dimensions/inductance.hpp"
#include "third_party/boost/boost/units/physical_dimensions/kinematic_viscosity.hpp"
#include "third_party/boost/boost/units/physical_dimensions/luminance.hpp"
#include "third_party/boost/boost/units/physical_dimensions/luminous_flux.hpp"
#include "third_party/boost/boost/units/physical_dimensions/magnetic_field_intensity.hpp"
#include "third_party/boost/boost/units/physical_dimensions/magnetic_flux.hpp"
#include "third_party/boost/boost/units/physical_dimensions/magnetic_flux_density.hpp"
#include "third_party/boost/boost/units/physical_dimensions/mass_density.hpp"
#include "third_party/boost/boost/units/physical_dimensions/molar_energy.hpp"
#include "third_party/boost/boost/units/physical_dimensions/molar_heat_capacity.hpp"
#include "third_party/boost/boost/units/physical_dimensions/moment_of_inertia.hpp"
#include "third_party/boost/boost/units/physical_dimensions/momentum.hpp"
#include "third_party/boost/boost/units/physical_dimensions/permeability.hpp"
#include "third_party/boost/boost/units/physical_dimensions/permittivity.hpp"
#include "third_party/boost/boost/units/physical_dimensions/power.hpp"
#include "third_party/boost/boost/units/physical_dimensions/pressure.hpp"
#include "third_party/boost/boost/units/physical_dimensions/reluctance.hpp"
#include "third_party/boost/boost/units/physical_dimensions/resistance.hpp"
#include "third_party/boost/boost/units/physical_dimensions/resistivity.hpp"
#include "third_party/boost/boost/units/physical_dimensions/specific_energy.hpp"
#include "third_party/boost/boost/units/physical_dimensions/specific_heat_capacity.hpp"
#include "third_party/boost/boost/units/physical_dimensions/specific_volume.hpp"
#include "third_party/boost/boost/units/physical_dimensions/stress.hpp"
#include "third_party/boost/boost/units/physical_dimensions/surface_density.hpp"
#include "third_party/boost/boost/units/physical_dimensions/surface_tension.hpp"
#include "third_party/boost/boost/units/physical_dimensions/thermal_conductivity.hpp"
#include "third_party/boost/boost/units/physical_dimensions/torque.hpp"
#include "third_party/boost/boost/units/physical_dimensions/velocity.hpp"
#include "third_party/boost/boost/units/physical_dimensions/volume.hpp"
#include "third_party/boost/boost/units/physical_dimensions/wavenumber.hpp"

#endif // BOOST_UNITS_PHYSICAL_UNITS_HPP
