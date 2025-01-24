//  A thread-safe version of Boost.Signals.

// Copyright Frank Mori Hess 2007-2009
//
// Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#ifndef BOOST_SIGNALS2_SIGNAL_HPP
#define BOOST_SIGNALS2_SIGNAL_HPP

#include <algorithm>
#include "third_party/boost/boost/assert.hpp"
#include "third_party/boost/boost/config.hpp"
#include "third_party/boost/boost/function.hpp"
#include "third_party/boost/boost/mpl/identity.hpp"
#include "third_party/boost/boost/shared_ptr.hpp"
#include "third_party/boost/boost/signals2/connection.hpp"
#include "third_party/boost/boost/signals2/detail/unique_lock.hpp"
#include "third_party/boost/boost/signals2/detail/replace_slot_function.hpp"
#include "third_party/boost/boost/signals2/detail/result_type_wrapper.hpp"
#include "third_party/boost/boost/signals2/detail/signals_common.hpp"
#include "third_party/boost/boost/signals2/detail/signals_common_macros.hpp"
#include "third_party/boost/boost/signals2/detail/slot_groups.hpp"
#include "third_party/boost/boost/signals2/detail/slot_call_iterator.hpp"
#include "third_party/boost/boost/signals2/optional_last_value.hpp"
#include "third_party/boost/boost/signals2/mutex.hpp"
#include "third_party/boost/boost/signals2/slot.hpp"
#include "third_party/boost/boost/throw_exception.hpp"
#include <functional>

#ifdef BOOST_NO_VARIADIC_TEMPLATES
#include "third_party/boost/boost/signals2/preprocessed_signal.hpp"
#else
#include "third_party/boost/boost/signals2/variadic_signal.hpp"
#endif

#endif // BOOST_SIGNALS2_SIGNAL_HPP
