/*=============================================================================
    Boost.Wave: A Standard compliant C++ preprocessor library

    http://www.boost.org/

    Copyright (c) 2001-2011 Hartmut Kaiser. Distributed under the Boost
    Software License, Version 1.0. (See accompanying file
    LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_WAVE_FILESYSTEM_COMPATIBILITY_MAR_09_2009_0142PM)
#define BOOST_WAVE_FILESYSTEM_COMPATIBILITY_MAR_09_2009_0142PM

#include <string>

#include "third_party/boost/boost/version.hpp"
#include "third_party/boost/boost/filesystem/path.hpp"
#include "third_party/boost/boost/filesystem/operations.hpp"

namespace boost { namespace wave { namespace util
{
///////////////////////////////////////////////////////////////////////////////
// filesystem wrappers allowing to handle different Boost versions
#if !defined(BOOST_FILESYSTEM_NO_DEPRECATED)
// interface wrappers for older Boost versions
    inline boost::filesystem::path initial_path()
    {
        return boost::filesystem::initial_path(); 
    }

    inline boost::filesystem::path current_path()
    {
        return boost::filesystem::current_path(); 
    }

    template <typename String>
    inline boost::filesystem::path create_path(String const& p)
    {
#if BOOST_FILESYSTEM_VERSION >= 3
        return boost::filesystem::path(p);
#else
        return boost::filesystem::path(p, boost::filesystem::native);
#endif
    }

    inline std::string leaf(boost::filesystem::path const& p) 
    { 
#if BOOST_FILESYSTEM_VERSION >= 3
        return p.leaf().string(); 
#else
        return p.leaf(); 
#endif
    }

    inline boost::filesystem::path branch_path(boost::filesystem::path const& p) 
    { 
        return p.branch_path(); 
    }

    inline boost::filesystem::path normalize(boost::filesystem::path& p)
    {
        return p.normalize();
    }

    inline std::string native_file_string(boost::filesystem::path const& p) 
    { 
#if BOOST_FILESYSTEM_VERSION >= 3
        return p.string(); 
#else
        return p.native_file_string(); 
#endif
    }

    inline boost::filesystem::path complete_path(
        boost::filesystem::path const& p)
    {
#if BOOST_FILESYSTEM_VERSION >= 3
        return boost::filesystem3::complete(p, initial_path());
#else
        return boost::filesystem::complete(p, initial_path());
#endif
    }

    inline boost::filesystem::path complete_path(
        boost::filesystem::path const& p, boost::filesystem::path const& base)
    {
#if BOOST_FILESYSTEM_VERSION >= 3
        return boost::filesystem3::complete(p, base);
#else
        return boost::filesystem::complete(p, base);
#endif
    }

#else

// interface wrappers if deprecated functions do not exist
    inline boost::filesystem::path initial_path()
    { 
#if BOOST_FILESYSTEM_VERSION >= 3
        return boost::filesystem3::detail::initial_path();
#else
        return boost::filesystem::initial_path<boost::filesystem::path>();
#endif
    }

    inline boost::filesystem::path current_path()
    { 
#if BOOST_FILESYSTEM_VERSION >= 3
        return boost::filesystem3::current_path();
#else
        return boost::filesystem::current_path<boost::filesystem::path>();
#endif
    }

    template <typename String>
    inline boost::filesystem::path create_path(String const& p)
    {
        return boost::filesystem::path(p);
    }

    inline std::string leaf(boost::filesystem::path const& p) 
    { 
#if BOOST_VERSION >= 104600 && BOOST_FILESYSTEM_VERSION >= 3
        return p.filename().string();
#else
        return p.filename(); 
#endif
    }

    inline boost::filesystem::path branch_path(boost::filesystem::path const& p) 
    { 
        return p.parent_path(); 
    }

    inline boost::filesystem::path normalize(boost::filesystem::path& p)
    {
        return p; // function doesn't exist anymore
    }

    inline std::string native_file_string(boost::filesystem::path const& p) 
    { 
#if BOOST_VERSION >= 104600
        return p.string(); 
#else
        return p.file_string(); 
#endif
    }

    inline boost::filesystem::path complete_path(
        boost::filesystem::path const& p)
    {
#if BOOST_VERSION >= 104600 && BOOST_FILESYSTEM_VERSION >= 3
        return boost::filesystem::absolute(p, initial_path());
#else
        return boost::filesystem::complete(p, initial_path());
#endif
    }

    inline boost::filesystem::path complete_path(
        boost::filesystem::path const& p, boost::filesystem::path const& base)
    {
#if BOOST_VERSION >= 104600 && BOOST_FILESYSTEM_VERSION >= 3
        return boost::filesystem::absolute(p, base);
#else
        return boost::filesystem::complete(p, base);
#endif
    }
#endif

}}}

#endif
