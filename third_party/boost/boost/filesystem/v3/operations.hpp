//  boost/filesystem/operations.hpp  ---------------------------------------------------//

//  Copyright Beman Dawes 2002-2009
//  Copyright Jan Langer 2002
//  Copyright Dietmar Kuehl 2001                                        
//  Copyright Vladimir Prus 2002
   
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  Library home page: http://www.boost.org/libs/filesystem

//--------------------------------------------------------------------------------------//

#ifndef BOOST_FILESYSTEM3_OPERATIONS_HPP
#define BOOST_FILESYSTEM3_OPERATIONS_HPP

#include "third_party/boost/boost/config.hpp"

# if defined( BOOST_NO_STD_WSTRING )
#   error Configuration not supported: Boost.Filesystem V3 and later requires std::wstring support
# endif

#include "third_party/boost/boost/filesystem/v3/config.hpp"
#include "third_party/boost/boost/filesystem/v3/path.hpp"

#include "third_party/boost/boost/detail/scoped_enum_emulation.hpp"
#include "third_party/boost/boost/detail/bitmask.hpp"
#include "third_party/boost/boost/system/error_code.hpp"
#include "third_party/boost/boost/system/system_error.hpp"
#include "third_party/boost/boost/shared_ptr.hpp"
#include "third_party/boost/boost/utility/enable_if.hpp"
#include "third_party/boost/boost/type_traits/is_same.hpp"
#include "third_party/boost/boost/iterator.hpp"
#include "third_party/boost/boost/cstdint.hpp"
#include "third_party/boost/boost/assert.hpp"

#include <string>
#include <utility> // for pair
#include <ctime>
#include <vector>
#include <stack>

#ifdef BOOST_WINDOWS_API
#  include <fstream>
#endif

#include "third_party/boost/boost/config/abi_prefix.hpp" // must be the last #include

//--------------------------------------------------------------------------------------//

namespace boost
{
  namespace filesystem3
  {

//--------------------------------------------------------------------------------------//
//                                     file_type                                        //
//--------------------------------------------------------------------------------------//

  enum file_type
  { 
    status_error,
#   ifndef BOOST_FILESYSTEM_NO_DEPRECATED
    status_unknown = status_error,
#   endif
    file_not_found,
    regular_file,
    directory_file,
    // the following may not apply to some operating systems or file systems
    symlink_file,
    block_file,
    character_file,
    fifo_file,
    socket_file,
    reparse_file,  // Windows: FILE_ATTRIBUTE_REPARSE_POINT that is not a symlink
    type_unknown,  // file does exist, but isn't one of the above types or
                   // we don't have strong enough permission to find its type

    _detail_directory_symlink  // internal use only; never exposed to users
  };

//--------------------------------------------------------------------------------------//
//                                       perms                                          //
//--------------------------------------------------------------------------------------//

  enum perms
  {
    no_perms = 0,       // file_not_found is no_perms rather than perms_not_known

    // POSIX equivalent macros given in comments.
    // Values are from POSIX and are given in octal per the POSIX standard.

    // permission bits
    
    owner_read = 0400,  // S_IRUSR, Read permission, owner
    owner_write = 0200, // S_IWUSR, Write permission, owner
    owner_exe = 0100,   // S_IXUSR, Execute/search permission, owner
    owner_all = 0700,   // S_IRWXU, Read, write, execute/search by owner

    group_read = 040,   // S_IRGRP, Read permission, group
    group_write = 020,  // S_IWGRP, Write permission, group
    group_exe = 010,    // S_IXGRP, Execute/search permission, group
    group_all = 070,    // S_IRWXG, Read, write, execute/search by group

    others_read = 04,   // S_IROTH, Read permission, others
    others_write = 02,  // S_IWOTH, Write permission, others
    others_exe = 01,    // S_IXOTH, Execute/search permission, others
    others_all = 07,    // S_IRWXO, Read, write, execute/search by others

    all_all = owner_all|group_all|others_all,  // 0777

    // other POSIX bits

    set_uid_on_exe = 04000, // S_ISUID, Set-user-ID on execution
    set_gid_on_exe = 02000, // S_ISGID, Set-group-ID on execution
    sticky_bit     = 01000, // S_ISVTX,
                            // (POSIX XSI) On directories, restricted deletion flag 
	                          // (V7) 'sticky bit': save swapped text even after use 
                            // (SunOS) On non-directories: don't cache this file
                            // (SVID-v4.2) On directories: restricted deletion flag
                            // Also see http://en.wikipedia.org/wiki/Sticky_bit

    perms_mask = all_all|set_uid_on_exe|set_gid_on_exe|sticky_bit,  // 07777

    perms_not_known = 0xFFFF, // present when directory_entry cache not loaded

    // options for permissions() function

    add_perms = 0x1000,     // adds the given permission bits to the current bits
    remove_perms = 0x2000,  // removes the given permission bits from the current bits;
                            // choose add_perms or remove_perms, not both; if neither add_perms
                            // nor remove_perms is given, replace the current bits with
                            // the given bits.

    symlink_perms = 0x4000  // on POSIX, don't resolve symlinks; implied on Windows
  };

  BOOST_BITMASK(perms)

//--------------------------------------------------------------------------------------//
//                                    file_status                                       //
//--------------------------------------------------------------------------------------//

  class BOOST_FILESYSTEM_DECL file_status
  {
  public:
             file_status()            : m_value(status_error), m_perms(perms_not_known) {}
    explicit file_status(file_type v, perms prms = perms_not_known)
                                      : m_value(v), m_perms(prms) {}

    // observers
    file_type  type() const                       { return m_value; }
    perms      permissions() const                { return m_perms; } 

    // modifiers
    void       type(file_type v)                  { m_value = v; }
    void       permissions(perms prms)            { m_perms = prms; }

    bool operator==(const file_status& rhs) const { return type() == rhs.type() && 
                                                    permissions() == rhs.permissions(); }
    bool operator!=(const file_status& rhs) const { return !(*this == rhs); }

  private:
    file_type   m_value;
    enum perms  m_perms;
  };

  inline bool type_present(file_status f) { return f.type() != status_error; }
  inline bool permissions_present(file_status f)
                                          {return f.permissions() != perms_not_known;}
  inline bool status_known(file_status f) { return type_present(f) && permissions_present(f); }
  inline bool exists(file_status f)       { return f.type() != status_error
                                                && f.type() != file_not_found; }
  inline bool is_regular_file(file_status f){ return f.type() == regular_file; }
  inline bool is_directory(file_status f) { return f.type() == directory_file; }
  inline bool is_symlink(file_status f)   { return f.type() == symlink_file; }
  inline bool is_other(file_status f)     { return exists(f) && !is_regular_file(f)
                                                && !is_directory(f) && !is_symlink(f); }

# ifndef BOOST_FILESYSTEM_NO_DEPRECATED
  inline bool is_regular(file_status f)   { return f.type() == regular_file; }
# endif

  struct space_info
  {
    // all values are byte counts
    boost::uintmax_t capacity;
    boost::uintmax_t free;      // <= capacity
    boost::uintmax_t available; // <= free
  };

  BOOST_SCOPED_ENUM_START(copy_option)
    {none, fail_if_exists = none, overwrite_if_exists};
  BOOST_SCOPED_ENUM_END

//--------------------------------------------------------------------------------------//
//                             implementation details                                   //
//--------------------------------------------------------------------------------------//

  namespace detail
  {
    BOOST_FILESYSTEM_DECL
    file_status status(const path&p, system::error_code* ec=0);
    BOOST_FILESYSTEM_DECL
    file_status symlink_status(const path& p, system::error_code* ec=0);
    BOOST_FILESYSTEM_DECL
    bool is_empty(const path& p, system::error_code* ec=0);
    BOOST_FILESYSTEM_DECL
    path initial_path(system::error_code* ec=0);
    BOOST_FILESYSTEM_DECL
    path canonical(const path& p, const path& base, system::error_code* ec=0);
    BOOST_FILESYSTEM_DECL
    void copy(const path& from, const path& to, system::error_code* ec=0);
    BOOST_FILESYSTEM_DECL
    void copy_directory(const path& from, const path& to, system::error_code* ec=0);
    BOOST_FILESYSTEM_DECL
    void copy_file(const path& from, const path& to,
                    BOOST_SCOPED_ENUM(copy_option) option,  // See ticket #2925
                    system::error_code* ec=0);
    BOOST_FILESYSTEM_DECL
    void copy_symlink(const path& existing_symlink, const path& new_symlink, system::error_code* ec=0);
    BOOST_FILESYSTEM_DECL
    bool create_directories(const path& p, system::error_code* ec=0);
    BOOST_FILESYSTEM_DECL
    bool create_directory(const path& p, system::error_code* ec=0);
    BOOST_FILESYSTEM_DECL
    void create_directory_symlink(const path& to, const path& from,
                                  system::error_code* ec=0);
    BOOST_FILESYSTEM_DECL
    void create_hard_link(const path& to, const path& from, system::error_code* ec=0);
    BOOST_FILESYSTEM_DECL
    void create_symlink(const path& to, const path& from, system::error_code* ec=0);
    BOOST_FILESYSTEM_DECL
    path current_path(system::error_code* ec=0);
    BOOST_FILESYSTEM_DECL
    void current_path(const path& p, system::error_code* ec=0);
    BOOST_FILESYSTEM_DECL
    bool equivalent(const path& p1, const path& p2, system::error_code* ec=0);
    BOOST_FILESYSTEM_DECL
    boost::uintmax_t file_size(const path& p, system::error_code* ec=0);
    BOOST_FILESYSTEM_DECL
    boost::uintmax_t hard_link_count(const path& p, system::error_code* ec=0);
    BOOST_FILESYSTEM_DECL
    std::time_t last_write_time(const path& p, system::error_code* ec=0);
    BOOST_FILESYSTEM_DECL
    void last_write_time(const path& p, const std::time_t new_time,
                         system::error_code* ec=0);
    BOOST_FILESYSTEM_DECL
    void permissions(const path& p, perms prms, system::error_code* ec=0);
    BOOST_FILESYSTEM_DECL
    path read_symlink(const path& p, system::error_code* ec=0);
    BOOST_FILESYSTEM_DECL
      // For standardization, if the committee doesn't like "remove", consider "eliminate"
    bool remove(const path& p, system::error_code* ec=0);
    BOOST_FILESYSTEM_DECL
    boost::uintmax_t remove_all(const path& p, system::error_code* ec=0);
    BOOST_FILESYSTEM_DECL
    void rename(const path& old_p, const path& new_p, system::error_code* ec=0);
    BOOST_FILESYSTEM_DECL
    void resize_file(const path& p, uintmax_t size, system::error_code* ec=0);
    BOOST_FILESYSTEM_DECL
    space_info space(const path& p, system::error_code* ec=0); 
    BOOST_FILESYSTEM_DECL
    path system_complete(const path& p, system::error_code* ec=0);
    BOOST_FILESYSTEM_DECL
    path temp_directory_path(system::error_code* ec=0);
    BOOST_FILESYSTEM_DECL
    path unique_path(const path& p, system::error_code* ec=0);
  }  // namespace detail

//--------------------------------------------------------------------------------------//
//                                                                                      //
//                             status query functions                                   //
//                                                                                      //
//--------------------------------------------------------------------------------------//

  inline
  file_status status(const path& p)    {return detail::status(p);}
  inline 
  file_status status(const path& p, system::error_code& ec)
                                       {return detail::status(p, &ec);}
  inline 
  file_status symlink_status(const path& p) {return detail::symlink_status(p);}
  inline
  file_status symlink_status(const path& p, system::error_code& ec)
                                       {return detail::symlink_status(p, &ec);}
  inline 
  bool exists(const path& p)           {return exists(detail::status(p));}
  inline 
  bool exists(const path& p, system::error_code& ec)
                                       {return exists(detail::status(p, &ec));}
  inline 
  bool is_directory(const path& p)     {return is_directory(detail::status(p));}
  inline 
  bool is_directory(const path& p, system::error_code& ec)
                                       {return is_directory(detail::status(p, &ec));}
  inline 
  bool is_regular_file(const path& p)  {return is_regular_file(detail::status(p));}
  inline 
  bool is_regular_file(const path& p, system::error_code& ec)
                                       {return is_regular_file(detail::status(p, &ec));}
  inline 
  bool is_other(const path& p)         {return is_other(detail::status(p));}
  inline 
  bool is_other(const path& p, system::error_code& ec)
                                       {return is_other(detail::status(p, &ec));}
  inline
  bool is_symlink(const path& p)       {return is_symlink(detail::symlink_status(p));}
  inline 
  bool is_symlink(const path& p, system::error_code& ec)
                                       {return is_symlink(detail::symlink_status(p, &ec));}
# ifndef BOOST_FILESYSTEM_NO_DEPRECATED
  inline
  bool is_regular(const path& p)       {return is_regular(detail::status(p));}
  inline
  bool is_regular(const path& p, system::error_code& ec)
                                       {return is_regular(detail::status(p, &ec));}
# endif

  inline
  bool is_empty(const path& p)         {return detail::is_empty(p);}
  inline
  bool is_empty(const path& p, system::error_code& ec)
                                       {return detail::is_empty(p, &ec);}

//--------------------------------------------------------------------------------------//
//                                                                                      //
//                             operational functions                                    //
//                  in alphabetical order, unless otherwise noted                       //
//                                                                                      //
//--------------------------------------------------------------------------------------//
 
  //  forward declarations
  path current_path();  // fwd declaration
  path initial_path();

  BOOST_FILESYSTEM_DECL
  path absolute(const path& p, const path& base=current_path());
  //  If base.is_absolute(), throws nothing. Thus no need for ec argument

  inline
  path canonical(const path& p, const path& base=current_path())
                                       {return detail::canonical(p, base);}
  inline
  path canonical(const path& p, system::error_code& ec)
                                       {return detail::canonical(p, current_path(), &ec);}
  inline
  path canonical(const path& p, const path& base, system::error_code& ec)
                                       {return detail::canonical(p, base, &ec);}

# ifndef BOOST_FILESYSTEM_NO_DEPRECATED
  inline
  path complete(const path& p)
  {
    return absolute(p, initial_path());
  }

  inline
  path complete(const path& p, const path& base)
  {
    return absolute(p, base);
  }
# endif

  inline
  void copy(const path& from, const path& to) {detail::copy(from, to);}

  inline
  void copy(const path& from, const path& to, system::error_code& ec) 
                                       {detail::copy(from, to, &ec);}
  inline
  void copy_directory(const path& from, const path& to)
                                       {detail::copy_directory(from, to);}
  inline
  void copy_directory(const path& from, const path& to, system::error_code& ec)
                                       {detail::copy_directory(from, to, &ec);}
  inline
  void copy_file(const path& from, const path& to,   // See ticket #2925
                 BOOST_SCOPED_ENUM(copy_option) option)
                                       {detail::copy_file(from, to, option);}
  inline
  void copy_file(const path& from, const path& to)
                                       {detail::copy_file(from, to, copy_option::fail_if_exists);}
  inline
  void copy_file(const path& from, const path& to,   // See ticket #2925
                 BOOST_SCOPED_ENUM(copy_option) option, system::error_code& ec)
                                       {detail::copy_file(from, to, option, &ec);}
  inline
  void copy_file(const path& from, const path& to, system::error_code& ec)
                                       {detail::copy_file(from, to, copy_option::fail_if_exists, &ec);}
  inline
  void copy_symlink(const path& existing_symlink, const path& new_symlink) {detail::copy_symlink(existing_symlink, new_symlink);}

  inline
  void copy_symlink(const path& existing_symlink, const path& new_symlink, system::error_code& ec)
                                       {detail::copy_symlink(existing_symlink, new_symlink, &ec);}
  inline
  bool create_directories(const path& p) {return detail::create_directories(p);}

  inline
  bool create_directories(const path& p, system::error_code& ec)
                                       {return detail::create_directories(p, &ec);}
  inline
  bool create_directory(const path& p) {return detail::create_directory(p);}

  inline
  bool create_directory(const path& p, system::error_code& ec)
                                       {return detail::create_directory(p, &ec);}
  inline
  void create_directory_symlink(const path& to, const path& from)
                                       {detail::create_directory_symlink(to, from);}
  inline
  void create_directory_symlink(const path& to, const path& from, system::error_code& ec)
                                       {detail::create_directory_symlink(to, from, &ec);}
  inline
  void create_hard_link(const path& to, const path& new_hard_link) {detail::create_hard_link(to, new_hard_link);}

  inline
  void create_hard_link(const path& to, const path& new_hard_link, system::error_code& ec)
                                       {detail::create_hard_link(to, new_hard_link, &ec);}
  inline
  void create_symlink(const path& to, const path& new_symlink) {detail::create_symlink(to, new_symlink);}

  inline
  void create_symlink(const path& to, const path& new_symlink, system::error_code& ec)
                                       {detail::create_symlink(to, new_symlink, &ec);}
  inline
  path current_path()                  {return detail::current_path();}

  inline
  path current_path(system::error_code& ec) {return detail::current_path(&ec);}

  inline
  void current_path(const path& p)     {detail::current_path(p);}

  inline
  void current_path(const path& p, system::error_code& ec) {detail::current_path(p, &ec);}

  inline
  bool equivalent(const path& p1, const path& p2) {return detail::equivalent(p1, p2);}

  inline
  bool equivalent(const path& p1, const path& p2, system::error_code& ec)
                                       {return detail::equivalent(p1, p2, &ec);}
  inline
  boost::uintmax_t file_size(const path& p) {return detail::file_size(p);}

  inline
  boost::uintmax_t file_size(const path& p, system::error_code& ec)
                                       {return detail::file_size(p, &ec);}
  inline
  boost::uintmax_t hard_link_count(const path& p) {return detail::hard_link_count(p);}

  inline
  boost::uintmax_t hard_link_count(const path& p, system::error_code& ec)
                                       {return detail::hard_link_count(p, &ec);}
  inline
  path initial_path()                  {return detail::initial_path();}

  inline
  path initial_path(system::error_code& ec) {return detail::initial_path(&ec);}

  template <class Path>
  path initial_path() {return initial_path();}
  template <class Path>
  path initial_path(system::error_code& ec) {return detail::initial_path(&ec);}

  inline
  std::time_t last_write_time(const path& p) {return detail::last_write_time(p);}

  inline
  std::time_t last_write_time(const path& p, system::error_code& ec)
                                       {return detail::last_write_time(p, &ec);}
  inline
  void last_write_time(const path& p, const std::time_t new_time)
                                       {detail::last_write_time(p, new_time);}
  inline
  void last_write_time(const path& p, const std::time_t new_time, system::error_code& ec)
                                       {detail::last_write_time(p, new_time, &ec);}
  inline
  void permissions(const path& p, perms prms)
                                       {detail::permissions(p, prms);}
  inline
  void permissions(const path& p, perms prms, system::error_code& ec)
                                       {detail::permissions(p, prms, &ec);}

  inline
  path read_symlink(const path& p)     {return detail::read_symlink(p);}

  inline
  path read_symlink(const path& p, system::error_code& ec)
                                       {return detail::read_symlink(p, &ec);}
  inline
    // For standardization, if the committee doesn't like "remove", consider "eliminate"
  bool remove(const path& p)           {return detail::remove(p);}

  inline
  bool remove(const path& p, system::error_code& ec) {return detail::remove(p, &ec);}

  inline
  boost::uintmax_t remove_all(const path& p) {return detail::remove_all(p);}
    
  inline
  boost::uintmax_t remove_all(const path& p, system::error_code& ec)
                                       {return detail::remove_all(p, &ec);}
  inline
  void rename(const path& old_p, const path& new_p) {detail::rename(old_p, new_p);}

  inline
  void rename(const path& old_p, const path& new_p, system::error_code& ec)
                                       {detail::rename(old_p, new_p, &ec);}
  inline  // name suggested by Scott McMurray
  void resize_file(const path& p, uintmax_t size) {detail::resize_file(p, size);}

  inline
  void resize_file(const path& p, uintmax_t size, system::error_code& ec)
                                       {detail::resize_file(p, size, &ec);}
  inline
  space_info space(const path& p)      {return detail::space(p);} 

  inline
  space_info space(const path& p, system::error_code& ec) {return detail::space(p, &ec);} 

# ifndef BOOST_FILESYSTEM_NO_DEPRECATED
  inline bool symbolic_link_exists(const path& p)
                                       { return is_symlink(symlink_status(p)); }
# endif

  inline
  path system_complete(const path& p)  {return detail::system_complete(p);}

  inline
  path system_complete(const path& p, system::error_code& ec)
                                       {return detail::system_complete(p, &ec);}
  inline
  path temp_directory_path()           {return detail::temp_directory_path();}

  inline
  path temp_directory_path(system::error_code& ec) 
                                       {return detail::temp_directory_path(&ec);}
  inline
  path unique_path(const path& p="%%%%-%%%%-%%%%-%%%%")
                                       { return detail::unique_path(p); }
  inline
  path unique_path(const path& p, system::error_code& ec)
                                       { return detail::unique_path(p, &ec); }

//--------------------------------------------------------------------------------------//
//                                                                                      //
//                                 directory_entry                                      //
//                                                                                      //
//--------------------------------------------------------------------------------------//

//  GCC has a problem with a member function named path within a namespace or 
//  sub-namespace that also has a class named path. The workaround is to always
//  fully qualify the name path when it refers to the class name.

class BOOST_FILESYSTEM_DECL directory_entry
{
public:

  // compiler generated copy constructor, copy assignment, and destructor apply

  directory_entry() {}
  explicit directory_entry(const boost::filesystem::path& p,
    file_status st = file_status(), file_status symlink_st=file_status())
    : m_path(p), m_status(st), m_symlink_status(symlink_st)
    {}

  void assign(const boost::filesystem::path& p,
    file_status st = file_status(), file_status symlink_st = file_status())
    { m_path = p; m_status = st; m_symlink_status = symlink_st; }

  void replace_filename(const boost::filesystem::path& p,
    file_status st = file_status(), file_status symlink_st = file_status())
  {
    m_path.remove_filename();
    m_path /= p;
    m_status = st;
    m_symlink_status = symlink_st;
  }

# ifndef BOOST_FILESYSTEM_NO_DEPRECATED
  void replace_leaf(const boost::filesystem::path& p,
    file_status st, file_status symlink_st)
      { replace_filename(p, st, symlink_st); }
# endif

  const boost::filesystem::path&  path() const               {return m_path;}
  file_status   status() const                               {return m_get_status();}
  file_status   status(system::error_code& ec) const         {return m_get_status(&ec);}
  file_status   symlink_status() const                       {return m_get_symlink_status();}
  file_status   symlink_status(system::error_code& ec) const {return m_get_symlink_status(&ec);}

  bool operator==(const directory_entry& rhs) {return m_path == rhs.m_path;} 
  bool operator!=(const directory_entry& rhs) {return m_path != rhs.m_path;} 
  bool operator< (const directory_entry& rhs) {return m_path < rhs.m_path;} 
  bool operator<=(const directory_entry& rhs) {return m_path <= rhs.m_path;} 
  bool operator> (const directory_entry& rhs) {return m_path > rhs.m_path;} 
  bool operator>=(const directory_entry& rhs) {return m_path >= rhs.m_path;} 

private:
  boost::filesystem::path   m_path;
  mutable file_status       m_status;           // stat()-like
  mutable file_status       m_symlink_status;   // lstat()-like

  file_status m_get_status(system::error_code* ec=0) const;
  file_status m_get_symlink_status(system::error_code* ec=0) const;
}; // directory_entry

//--------------------------------------------------------------------------------------//
//                                                                                      //
//                            directory_iterator helpers                                //
//                                                                                      //
//--------------------------------------------------------------------------------------//

class directory_iterator;

namespace detail
{
  BOOST_FILESYSTEM_DECL
    system::error_code dir_itr_close(// never throws()
    void *& handle
#   if     defined(BOOST_POSIX_API)
    , void *& buffer
#   endif
  ); 

  struct dir_itr_imp
  {
    directory_entry  dir_entry;
    void*            handle;

#   ifdef BOOST_POSIX_API
    void*            buffer;  // see dir_itr_increment implementation
#   endif

    dir_itr_imp() : handle(0)
#   ifdef BOOST_POSIX_API
      , buffer(0)
#   endif
    {}

    ~dir_itr_imp() // never throws
    {
      dir_itr_close(handle
#       if defined(BOOST_POSIX_API)
         , buffer
#       endif
    );
    }
  };

  // see path::iterator: comment below
  BOOST_FILESYSTEM_DECL void directory_iterator_construct(directory_iterator& it,
    const path& p, system::error_code* ec);
  BOOST_FILESYSTEM_DECL void directory_iterator_increment(directory_iterator& it,
    system::error_code* ec);

}  // namespace detail

//--------------------------------------------------------------------------------------//
//                                                                                      //
//                                directory_iterator                                    //
//                                                                                      //
//--------------------------------------------------------------------------------------//

  class directory_iterator
    : public boost::iterator_facade< directory_iterator,
                                     directory_entry,
                                     boost::single_pass_traversal_tag >
  {
  public:

    directory_iterator(){}  // creates the "end" iterator

    // iterator_facade derived classes don't seem to like implementations in
    // separate translation unit dll's, so forward to detail functions
    explicit directory_iterator(const path& p)
        : m_imp(new detail::dir_itr_imp)
          { detail::directory_iterator_construct(*this, p, 0); }

    directory_iterator(const path& p, system::error_code& ec)
        : m_imp(new detail::dir_itr_imp)
          { detail::directory_iterator_construct(*this, p, &ec); }

   ~directory_iterator() {} // never throws

    directory_iterator& increment(system::error_code& ec)
    { 
      detail::directory_iterator_increment(*this, &ec);
      return *this;
    }

  private:
    friend struct detail::dir_itr_imp;
    friend BOOST_FILESYSTEM_DECL void detail::directory_iterator_construct(directory_iterator& it,
      const path& p, system::error_code* ec);
    friend BOOST_FILESYSTEM_DECL void detail::directory_iterator_increment(directory_iterator& it,
      system::error_code* ec);

    // shared_ptr provides shallow-copy semantics required for InputIterators.
    // m_imp.get()==0 indicates the end iterator.
    boost::shared_ptr< detail::dir_itr_imp >  m_imp;

    friend class boost::iterator_core_access;

    boost::iterator_facade<
      directory_iterator,
      directory_entry,
      boost::single_pass_traversal_tag >::reference dereference() const 
    {
      BOOST_ASSERT_MSG(m_imp.get(), "attempt to dereference end iterator");
      return m_imp->dir_entry;
    }

    void increment() { detail::directory_iterator_increment(*this, 0); }

    bool equal(const directory_iterator& rhs) const
      { return m_imp == rhs.m_imp; }
  };

//--------------------------------------------------------------------------------------//
//                                                                                      //
//                      recursive_directory_iterator helpers                            //
//                                                                                      //
//--------------------------------------------------------------------------------------//

  BOOST_SCOPED_ENUM_START(symlink_option)
  {
    none,
    no_recurse = none,         // don't follow directory symlinks (default behavior)
    recurse,                   // follow directory symlinks
    _detail_no_push = recurse << 1  // internal use only
  };
  BOOST_SCOPED_ENUM_END

  BOOST_BITMASK(BOOST_SCOPED_ENUM(symlink_option))

  namespace detail
  {
    struct recur_dir_itr_imp
    {
      typedef directory_iterator element_type;
      std::stack< element_type, std::vector< element_type > > m_stack;
      int  m_level;
      BOOST_SCOPED_ENUM(symlink_option) m_options;

      recur_dir_itr_imp() : m_level(0), m_options(symlink_option::none) {}

      void increment(system::error_code* ec);  // ec == 0 means throw on error

      void pop();

    };

    //  Implementation is inline to avoid dynamic linking difficulties with m_stack:
    //  Microsoft warning C4251, m_stack needs to have dll-interface to be used by
    //  clients of struct 'boost::filesystem::detail::recur_dir_itr_imp'

    inline
    void recur_dir_itr_imp::increment(system::error_code* ec)
    // ec == 0 means throw on error
    {
      if ((m_options & symlink_option::_detail_no_push) == symlink_option::_detail_no_push)
        m_options &= ~symlink_option::_detail_no_push;

      else
      {
        // Logic for following predicate was contributed by Daniel Aarno to handle cyclic
        // symlinks correctly and efficiently, fixing ticket #5652.
        //   if (((m_options & symlink_option::recurse) == symlink_option::recurse
        //         || !is_symlink(m_stack.top()->symlink_status()))
        //       && is_directory(m_stack.top()->status())) ...
        // The predicate code has since been rewritten to pass error_code arguments,
        // per ticket #5653.
        bool or_pred = (m_options & symlink_option::recurse) == symlink_option::recurse
                       || (ec == 0 ? !is_symlink(m_stack.top()->symlink_status())
                                   : !is_symlink(m_stack.top()->symlink_status(*ec)));
        if (ec != 0 && *ec)
          return;
        bool and_pred = or_pred && (ec == 0 ? is_directory(m_stack.top()->status())
                                            : is_directory(m_stack.top()->status(*ec)));
        if (ec != 0 && *ec)
          return;

        if (and_pred)
        {
          if (ec == 0)
            m_stack.push(directory_iterator(m_stack.top()->path()));
          else
          {
            m_stack.push(directory_iterator(m_stack.top()->path(), *ec));
            if (*ec)
              return;
          }
          if (m_stack.top() != directory_iterator())
          {
            ++m_level;
            return;
          }
          m_stack.pop();
        }
      }

      while (!m_stack.empty() && ++m_stack.top() == directory_iterator())
      {
        m_stack.pop();
        --m_level;
      }
    }

    inline
    void recur_dir_itr_imp::pop()
    {
      BOOST_ASSERT_MSG(m_level > 0,
        "pop() on recursive_directory_iterator with level < 1");

      do
      {
        m_stack.pop();
        --m_level;
      }
      while (!m_stack.empty() && ++m_stack.top() == directory_iterator());
    }
  } // namespace detail

//--------------------------------------------------------------------------------------//
//                                                                                      //
//                           recursive_directory_iterator                               //
//                                                                                      //
//--------------------------------------------------------------------------------------//

  class recursive_directory_iterator
    : public boost::iterator_facade<
        recursive_directory_iterator,
        directory_entry,
        boost::single_pass_traversal_tag >
  {
  public:

    recursive_directory_iterator(){}  // creates the "end" iterator

    explicit recursive_directory_iterator(const path& dir_path,
      BOOST_SCOPED_ENUM(symlink_option) opt = symlink_option::none)
      : m_imp(new detail::recur_dir_itr_imp)
    {
      m_imp->m_options = opt;
      m_imp->m_stack.push(directory_iterator(dir_path));
      if (m_imp->m_stack.top() == directory_iterator())
        { m_imp.reset (); }
    }

    recursive_directory_iterator(const path& dir_path,
      BOOST_SCOPED_ENUM(symlink_option) opt,
      system::error_code & ec)
    : m_imp(new detail::recur_dir_itr_imp)
    {
      m_imp->m_options = opt;
      m_imp->m_stack.push(directory_iterator(dir_path, ec));
      if (m_imp->m_stack.top() == directory_iterator())
        { m_imp.reset (); }
    }

    recursive_directory_iterator(const path& dir_path,
      system::error_code & ec)
    : m_imp(new detail::recur_dir_itr_imp)
    {
      m_imp->m_options = symlink_option::none;
      m_imp->m_stack.push(directory_iterator(dir_path, ec));
      if (m_imp->m_stack.top() == directory_iterator())
        { m_imp.reset (); }
    }

    recursive_directory_iterator& increment(system::error_code& ec)
    {
      BOOST_ASSERT_MSG(m_imp.get(),
        "increment() on end recursive_directory_iterator");
      m_imp->increment(&ec);
      if (m_imp->m_stack.empty())
        m_imp.reset(); // done, so make end iterator
      return *this;
    }

    int level() const
    { 
      BOOST_ASSERT_MSG(m_imp.get(),
        "level() on end recursive_directory_iterator");
      return m_imp->m_level;
    }

    bool no_push_pending() const
    {
      BOOST_ASSERT_MSG(m_imp.get(),
        "is_no_push_requested() on end recursive_directory_iterator");
      return (m_imp->m_options & symlink_option::_detail_no_push)
        == symlink_option::_detail_no_push;
    }

#   ifndef BOOST_FILESYSTEM_NO_DEPRECATED
    bool no_push_request() const { return no_push_pending(); }
#   endif

    void pop()
    { 
      BOOST_ASSERT_MSG(m_imp.get(),
        "pop() on end recursive_directory_iterator");
      m_imp->pop();
      if (m_imp->m_stack.empty()) m_imp.reset(); // done, so make end iterator
    }

    void no_push(bool value=true)
    {
      BOOST_ASSERT_MSG(m_imp.get(),
        "no_push() on end recursive_directory_iterator");
      if (value)
        m_imp->m_options |= symlink_option::_detail_no_push;
      else
        m_imp->m_options &= ~symlink_option::_detail_no_push;
    }

    file_status status() const
    {
      BOOST_ASSERT_MSG(m_imp.get(),
        "status() on end recursive_directory_iterator");
      return m_imp->m_stack.top()->status();
    }

    file_status symlink_status() const
    {
      BOOST_ASSERT_MSG(m_imp.get(),
        "symlink_status() on end recursive_directory_iterator");
      return m_imp->m_stack.top()->symlink_status();
    }

  private:

    // shared_ptr provides shallow-copy semantics required for InputIterators.
    // m_imp.get()==0 indicates the end iterator.
    boost::shared_ptr< detail::recur_dir_itr_imp >  m_imp;

    friend class boost::iterator_core_access;

    boost::iterator_facade< 
      recursive_directory_iterator,
      directory_entry,
      boost::single_pass_traversal_tag >::reference
    dereference() const 
    {
      BOOST_ASSERT_MSG(m_imp.get(),
        "dereference of end recursive_directory_iterator");
      return *m_imp->m_stack.top();
    }

    void increment()
    { 
      BOOST_ASSERT_MSG(m_imp.get(),
        "increment of end recursive_directory_iterator");
      m_imp->increment(0);
      if (m_imp->m_stack.empty())
        m_imp.reset(); // done, so make end iterator
    }

    bool equal(const recursive_directory_iterator& rhs) const
      { return m_imp == rhs.m_imp; }

  };

# if !defined(BOOST_FILESYSTEM_NO_DEPRECATED)
  typedef recursive_directory_iterator wrecursive_directory_iterator;
# endif

//--------------------------------------------------------------------------------------//
//                                                                                      //
//                            class filesystem_error                                    //
//                                                                                      //
//--------------------------------------------------------------------------------------//
  
  class BOOST_SYMBOL_VISIBLE filesystem_error : public system::system_error
  {
  // see http://www.boost.org/more/error_handling.html for design rationale

  // all functions are inline to avoid issues with crossing dll boundaries

  public:
    // compiler generates copy constructor and copy assignment

    filesystem_error(
      const std::string & what_arg, system::error_code ec)
      : system::system_error(ec, what_arg)
    {
      try
      {
        m_imp_ptr.reset(new m_imp);
      }
      catch (...) { m_imp_ptr.reset(); }
    }

    filesystem_error(
      const std::string & what_arg, const path& path1_arg,
      system::error_code ec)
      : system::system_error(ec, what_arg)
    {
      try
      {
        m_imp_ptr.reset(new m_imp);
        m_imp_ptr->m_path1 = path1_arg;
      }
      catch (...) { m_imp_ptr.reset(); }
    }
    
    filesystem_error(
      const std::string & what_arg, const path& path1_arg,
      const path& path2_arg, system::error_code ec)
      : system::system_error(ec, what_arg)
    {
      try
      {
        m_imp_ptr.reset(new m_imp);
        m_imp_ptr->m_path1 = path1_arg;
        m_imp_ptr->m_path2 = path2_arg;
      }
      catch (...) { m_imp_ptr.reset(); }
    }

    ~filesystem_error() throw() {}

    const path& path1() const
    {
      static const path empty_path;
      return m_imp_ptr.get() ? m_imp_ptr->m_path1 : empty_path ;
    }
    const path& path2() const
    {
      static const path empty_path;
      return m_imp_ptr.get() ? m_imp_ptr->m_path2 : empty_path ;
    }

    const char* what() const throw()
    {
      if (!m_imp_ptr.get())
        return system::system_error::what();

      try
      {
        if (m_imp_ptr->m_what.empty())
        {
          m_imp_ptr->m_what = system::system_error::what();
          if (!m_imp_ptr->m_path1.empty())
          {
            m_imp_ptr->m_what += ": \"";
            m_imp_ptr->m_what += m_imp_ptr->m_path1.string();
            m_imp_ptr->m_what += "\"";
          }
          if (!m_imp_ptr->m_path2.empty())
          {
            m_imp_ptr->m_what += ", \"";
            m_imp_ptr->m_what += m_imp_ptr->m_path2.string();
            m_imp_ptr->m_what += "\"";
          }
        }
        return m_imp_ptr->m_what.c_str();
      }
      catch (...)
      {
        return system::system_error::what();
      }
    }

  private:
    struct m_imp
    {
      path         m_path1; // may be empty()
      path         m_path2; // may be empty()
      std::string  m_what;  // not built until needed
    };
    boost::shared_ptr<m_imp> m_imp_ptr;
  };

//  test helper  -----------------------------------------------------------------------//

//  Not part of the documented interface since false positives are possible;
//  there is no law that says that an OS that has large stat.st_size
//  actually supports large file sizes.

  namespace detail
  {
    BOOST_FILESYSTEM_DECL bool possible_large_file_size_support();
  }

  } // namespace filesystem3
} // namespace boost

//----------------------------------------------------------------------------//

namespace boost
{
  namespace filesystem
  {
    // permissions
    using filesystem3::no_perms;
    using filesystem3::owner_read;
    using filesystem3::owner_write;
    using filesystem3::owner_exe;
    using filesystem3::owner_all;
    using filesystem3::group_read;
    using filesystem3::group_write;
    using filesystem3::group_exe;
    using filesystem3::group_all;
    using filesystem3::others_read;
    using filesystem3::others_write;
    using filesystem3::others_exe;
    using filesystem3::others_all;
    using filesystem3::all_all;
    using filesystem3::set_uid_on_exe;
    using filesystem3::set_gid_on_exe;
    using filesystem3::sticky_bit;
    using filesystem3::perms_mask;
    using filesystem3::perms_not_known;
    using filesystem3::add_perms;
    using filesystem3::remove_perms;
    using filesystem3::symlink_perms;

    using filesystem3::absolute;
    using filesystem3::block_file;
    using filesystem3::canonical;
    using filesystem3::character_file;
//    using filesystem3::copy;
    using filesystem3::copy_file;
    using filesystem3::copy_option;
    using filesystem3::copy_symlink;
    using filesystem3::create_directories;
    using filesystem3::create_directory;
    using filesystem3::create_hard_link;
    using filesystem3::create_symlink;
    using filesystem3::create_directory_symlink;
    using filesystem3::current_path;
    using filesystem3::directory_entry;
    using filesystem3::directory_file;
    using filesystem3::directory_iterator;
    using filesystem3::equivalent;
    using filesystem3::exists;
    using filesystem3::fifo_file;
    using filesystem3::file_not_found;
    using filesystem3::file_size;
    using filesystem3::file_status;
    using filesystem3::file_type;
    using filesystem3::filesystem_error;
    using filesystem3::hard_link_count;
    using filesystem3::initial_path;
    using filesystem3::is_directory;
    using filesystem3::is_directory;
    using filesystem3::is_empty;
    using filesystem3::is_other;
    using filesystem3::is_regular_file;
    using filesystem3::is_symlink;
    using filesystem3::last_write_time;
    using filesystem3::permissions;
    using filesystem3::permissions_present;
    using filesystem3::perms;
    using filesystem3::read_symlink;
    using filesystem3::recursive_directory_iterator;
    using filesystem3::regular_file;
    using filesystem3::reparse_file;
    using filesystem3::remove;
    using filesystem3::remove_all;
    using filesystem3::rename;
    using filesystem3::resize_file;
    using filesystem3::socket_file;
    using filesystem3::space;
    using filesystem3::space_info;
    using filesystem3::status;
    using filesystem3::status_error;
    using filesystem3::status_known;
    using filesystem3::symlink_file;
    using filesystem3::symlink_option;
    using filesystem3::symlink_status;
    using filesystem3::system_complete;
    using filesystem3::temp_directory_path;
    using filesystem3::type_present;
    using filesystem3::type_unknown;
    using filesystem3::unique_path;
# ifndef BOOST_FILESYSTEM_NO_DEPRECATED
    using filesystem3::is_regular;
    using filesystem3::status_unknown;
    using filesystem3::symbolic_link_exists;
    //using filesystem3::wdirectory_iterator;
    //using filesystem3::wdirectory_entry;
# endif
    namespace detail
    {
      using filesystem3::detail::possible_large_file_size_support;
    }
  }
}

#include "third_party/boost/boost/config/abi_suffix.hpp" // pops abi_prefix.hpp pragmas
#endif // BOOST_FILESYSTEM3_OPERATIONS_HPP
