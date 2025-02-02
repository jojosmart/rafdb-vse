//  Copyright (c) 2008 Ben Hanson
//  Copyright (c) 2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(BOOST_SPIRIT_LEX_LEXERTL_GENERATE_CPP_FEB_10_2008_0855PM)
#define BOOST_SPIRIT_LEX_LEXERTL_GENERATE_CPP_FEB_10_2008_0855PM

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once      // MS compatible compilers support #pragma once
#endif

#include "third_party/boost/boost/spirit/home/support/detail/lexer/char_traits.hpp"
#include "third_party/boost/boost/spirit/home/support/detail/lexer/consts.hpp"
#include "third_party/boost/boost/spirit/home/support/detail/lexer/size_t.hpp"
#include "third_party/boost/boost/spirit/home/support/detail/lexer/state_machine.hpp"

///////////////////////////////////////////////////////////////////////////////
namespace boost { namespace spirit { namespace lex { namespace detail
{ 
    ///////////////////////////////////////////////////////////////////////////
    // Generate a table of the names of the used lexer states, which is a bit 
    // tricky, because the table stored with the rules is sorted based on the 
    // names, but we need it sorted using the state ids.
    template <typename Char>
    inline bool 
    generate_cpp_state_names (boost::lexer::basic_rules<Char> const& rules_,
        std::ostream &os_, char const* name_suffix = "")
    {
        // we need to re-sort the state names in ascending order if the state 
        // ids, filling possible gaps in between later
        typedef typename 
            boost::lexer::basic_rules<Char>::string_size_t_map::const_iterator
        state_iterator;
        typedef std::map<std::size_t, char const*> reverse_state_map_type;
        
        reverse_state_map_type reverse_state_map;
        state_iterator send = rules_.statemap().end();
        for (state_iterator sit = rules_.statemap().begin(); sit != send; ++sit)
        {
            typedef typename reverse_state_map_type::value_type value_type;
            reverse_state_map.insert(value_type((*sit).second, (*sit).first.c_str()));
        }
        
        os_ << "// this table defines the names of the lexer states\n";
        os_ << "char const* const lexer_state_names" << name_suffix 
            << "[" << rules_.statemap().size() << "] = \n{\n";
            
        typedef typename reverse_state_map_type::iterator iterator;
        iterator rend = reverse_state_map.end();
        std::size_t last_id = 0;
        for (iterator rit = reverse_state_map.begin(); rit != rend; ++rit)
        {
            for (/**/; last_id < (*rit).first; ++last_id)
            {
                os_ << "    0,  // \"<undefined state>\"\n";
            }
            os_ << "    \"" << (*rit).second << "\",\n";
        }
        os_ << "};\n\n";
        
        return true;
    }
    
    ///////////////////////////////////////////////////////////////////////////
    // Generate a tokenizer for the given state machine.
    template <typename Char>
    inline bool 
    generate_cpp (boost::lexer::basic_state_machine<Char> const& sm_, 
        boost::lexer::basic_rules<Char> const& rules_, 
        std::ostream &os_, char const* name_suffix = "", 
        bool skip_on_nomatch = true, bool optimize_parameters = true)
    {
        if (sm_._lookup->empty())
            return false;
            
        std::size_t const dfas_ = sm_._dfa->size();
        std::size_t const lookups_ = sm_._lookup->front()->size();

        os_ << "// Copyright (c) 2008 Ben Hanson\n";
        os_ << "//\n";
        os_ << "// Distributed under the Boost Software License, "
            "Version 1.0. (See accompanying\n";
        os_ << "// file licence_1_0.txt or copy at "
            "http://www.boost.org/LICENSE_1_0.txt)\n\n";
        os_ << "// Auto-generated by boost::lexer\n";

        std::string guard(__DATE__ "_" __TIME__);
        std::string::size_type p = guard.find_first_of(": ");
        while (std::string::npos != p) 
        {
            guard.replace(p, 1, "_");
            p = guard.find_first_of(": ", p);
        }
      
        os_ << "#if !defined(BOOST_SPIRIT_LEXER_NEXT_TOKEN_" << guard << ")\n";
        os_ << "#define BOOST_SPIRIT_LEXER_NEXT_TOKEN_" << guard << "\n\n";
      
        os_ << "#include "third_party/boost/boost/detail/iterator.hpp"\n";
        os_ << "#include "third_party/boost/boost/spirit/home/support/detail/lexer/char_traits.hpp"\n\n";

        os_ << "// the generated table of state names and the tokenizer have to be\n"
               "// defined in the boost::spirit::lex::static namespace\n";
        os_ << "namespace boost { namespace spirit { namespace lex { "
            "namespace static_ {\n\n";

        // generate the table containing state names 
        if (!generate_cpp_state_names(rules_, os_, name_suffix))
            return false;

        os_ << "template<typename Iterator>\n";
        os_ << "std::size_t next_token" << name_suffix  << " (";

        if (dfas_ > 1 || !optimize_parameters)
        {
            os_ << "std::size_t &start_state_, ";
        }

        if (sm_._seen_BOL_assertion || !optimize_parameters)
        {
            os_ << "Iterator const& start_, ";
        }

        if (dfas_ > 1 || sm_._seen_BOL_assertion || !optimize_parameters)
        {
            os_ << "\n    ";
        }
      
        os_ << "Iterator &start_token_, Iterator const& end_)\n";
        os_ << "{\n";
        os_ << "    enum {end_state_index, id_index, state_index, bol_index, "
          "eol_index,\n";
        os_ << "        dead_state_index, dfa_offset};\n";
        os_ << "    static const std::size_t npos = static_cast"
          "<std::size_t>(~0);\n";

        if (dfas_ > 1)
        {
            for (std::size_t state_ = 0; state_ < dfas_; ++state_)
            {
                std::size_t i_ = 0;
                std::size_t j_ = 1;
                std::size_t count_ = lookups_ / 8;
                std::size_t const* lookup_ = &sm_._lookup[state_]->front ();
                std::size_t const* dfa_ = &sm_._dfa[state_]->front ();

                os_ << "    static const std::size_t lookup" << state_ 
                    << "_[" << lookups_ << "] = {";

                for (/**/; i_ < count_; ++i_)
                {
                    const std::size_t index_ = i_ * 8;

                    os_ << lookup_[index_];

                    for (; j_ < 8; ++j_)
                    {
                        os_ << ", " << lookup_[index_ + j_];
                    }

                    if (i_ < count_ - 1)
                    {
                        os_ << "," << std::endl << "        ";
                    }

                    j_ = 1;
                }

                os_ << "};\n";
                count_ = sm_._dfa[state_]->size ();
                os_ << "    static const std::size_t dfa" << state_ << "_[" <<
                    count_ << "] = {";
                count_ /= 8;

                for (i_ = 0; i_ < count_; ++i_)
                {
                    const std::size_t index_ = i_ * 8;

                    os_ << dfa_[index_];

                    for (j_ = 1; j_ < 8; ++j_)
                    {
                        os_ << ", " << dfa_[index_ + j_];
                    }

                    if (i_ < count_ - 1)
                    {
                        os_ << "," << std::endl << "        ";
                    }
                }

                std::size_t const mod_ = sm_._dfa[state_]->size () % 8;
                if (mod_)
                {
                    const std::size_t index_ = count_ * 8;

                    if (count_)
                    {
                        os_ << ",\n        ";
                    }

                    os_ << dfa_[index_];

                    for (j_ = 1; j_ < mod_; ++j_)
                    {
                        os_ << ", " << dfa_[index_ + j_];
                    }
                }

                os_ << "};\n";
            }

            std::size_t count_ = sm_._dfa_alphabet.size();
            std::size_t i_ = 1;

            os_ << "    static const std::size_t *lookup_arr_[" << count_ <<
                "] = {";

            os_ << "lookup0_";

            for (i_ = 1; i_ < count_; ++i_)
            {
                os_ << ", " << "lookup" << i_ << "_";
            }

            os_ << "};\n";
            os_ << "    static const std::size_t dfa_alphabet_arr_[" << 
                count_ << "] = {";

            os_ << sm_._dfa_alphabet.front ();

            for (i_ = 1; i_ < count_; ++i_)
            {
                os_ << ", " << sm_._dfa_alphabet[i_];
            }

            os_ << "};\n";
            os_ << "    static const std::size_t *dfa_arr_[" << count_ <<
                "] = {";
            os_ << "dfa0_";

            for (i_ = 1; i_ < count_; ++i_)
            {
                os_ << ", " << "dfa" << i_ << "_";
            }

            os_ << "};\n";
        }
        else
        {
            std::size_t const* lookup_ = &sm_._lookup[0]->front();
            std::size_t const* dfa_ = &sm_._dfa[0]->front();
            std::size_t i_ = 0;
            std::size_t j_ = 1;
            std::size_t count_ = lookups_ / 8;

            os_ << "    static const std::size_t lookup_[";
            os_ << sm_._lookup[0]->size() << "] = {";

            for (; i_ < count_; ++i_)
            {
                const std::size_t index_ = i_ * 8;

                os_ << lookup_[index_];

                for (; j_ < 8; ++j_)
                {
                    os_ << ", " << lookup_[index_ + j_];
                }

                if (i_ < count_ - 1)
                {
                    os_ << "," << std::endl << "        ";
                }

                j_ = 1;
            }

            os_ << "};\n";
            os_ << "    static const std::size_t dfa_alphabet_ = " <<
              sm_._dfa_alphabet.front () << ";\n";
            os_ << "    static const std::size_t dfa_[" <<
              sm_._dfa[0]->size () << "] = {";
                count_ = sm_._dfa[0]->size () / 8;

            for (i_ = 0; i_ < count_; ++i_)
            {
                const std::size_t index_ = i_ * 8;

                os_ << dfa_[index_];

                for (j_ = 1; j_ < 8; ++j_)
                {
                    os_ << ", " << dfa_[index_ + j_];
                }

                if (i_ < count_ - 1)
                {
                    os_ << "," << std::endl << "        ";
                }
            }

            const std::size_t mod_ = sm_._dfa[0]->size () % 8;

            if (mod_)
            {
                const std::size_t index_ = count_ * 8;

                if (count_)
                {
                    os_ << ",\n        ";
                }

                os_ << dfa_[index_];

                for (j_ = 1; j_ < mod_; ++j_)
                {
                    os_ << ", " << dfa_[index_ + j_];
                }
            }

            os_ << "};\n";
        }

        os_ << "\n    if (start_token_ == end_) return 0;\n\n";

        if (dfas_ > 1)
        {
            os_ << "again:\n";
            os_ << "    const std::size_t * lookup_ = lookup_arr_[start_state_];\n";
            os_ << "    std::size_t dfa_alphabet_ = dfa_alphabet_arr_[start_state_];\n";
            os_ << "    const std::size_t *dfa_ = dfa_arr_[start_state_];\n";
        }

        os_ << "    const std::size_t *ptr_ = dfa_ + dfa_alphabet_;\n";
        os_ << "    Iterator curr_ = start_token_;\n";
        os_ << "    bool end_state_ = *ptr_ != 0;\n";
        os_ << "    std::size_t id_ = *(ptr_ + id_index);\n";
        os_ << "    Iterator end_token_ = start_token_;\n";
        os_ << '\n';
        os_ << "    while (curr_ != end_)\n";
        os_ << "    {\n";

        if (sm_._seen_BOL_assertion)
        {
            os_ << "        std::size_t const BOL_state_ = ptr_[bol_index];\n";
        }

        if (sm_._seen_EOL_assertion)
        {
            os_ << "        std::size_t const EOL_state_ = ptr_[eol_index];\n";
        }

        if (sm_._seen_BOL_assertion || sm_._seen_EOL_assertion)
        {
            os_ << '\n';
        }

        if (sm_._seen_BOL_assertion && sm_._seen_EOL_assertion)
        {
            os_ << "        if (BOL_state_ && (start_token_ == start_ ||\n";
            os_ << "            *(start_token_ - 1) == '\\n'))\n";
            os_ << "        {\n";
            os_ << "            ptr_ = &dfa_[BOL_state_ * dfa_alphabet_];\n";
            os_ << "        }\n";
            os_ << "        else if (EOL_state_ && *curr_ == '\\n')\n";
            os_ << "        {\n";
            os_ << "            ptr_ = &dfa_[EOL_state_ * dfa_alphabet_];\n";
            os_ << "        }\n";
            os_ << "        else\n";
            os_ << "        {\n";
            os_ << "            std::size_t const state_ =\n";

            if (lookups_ == 256)
            {
                os_ << "                ptr_[lookup_[<typename Traits::index_type>"
                    "(*curr_++)]];\n";
            }
            else
            {
                os_ << "                ptr_[lookup_[*curr_++]];\n";
            }

            os_ << '\n';
            os_ << "            if (state_ == 0) break;\n";
            os_ << '\n';
            os_ << "            ptr_ = &dfa_[state_ * dfa_alphabet_];\n";
            os_ << "        }\n";
        }
        else if (sm_._seen_BOL_assertion)
        {
            os_ << "        if (BOL_state_ && (start_token_ == start_ ||\n";
            os_ << "            *(start_token_ - 1) == '\\n'))\n";
            os_ << "        {\n";
            os_ << "            ptr_ = &dfa_[BOL_state_ * dfa_alphabet_];\n";
            os_ << "        }\n";
            os_ << "        else\n";
            os_ << "        {\n";
            os_ << "            std::size_t const state_ =\n";

            if (lookups_ == 256)
            {
                os_ << "                ptr_[lookup_[static_cast<unsigned char>\n";
                os_ << "                (*curr_++)]];\n";
            }
            else
            {
                os_ << "                ptr_[lookup_[*curr_++]];\n";
            }

            os_ << '\n';
            os_ << "            if (state_ == 0) break;\n";
            os_ << '\n';
            os_ << "            ptr_ = &dfa_[state_ * dfa_alphabet_];\n";
            os_ << "        }\n";
        }
        else if (sm_._seen_EOL_assertion)
        {
            os_ << "        if (EOL_state_ && *curr_ == '\\n')\n";
            os_ << "        {\n";
            os_ << "            ptr_ = &dfa_[EOL_state_ * dfa_alphabet_];\n";
            os_ << "        }\n";
            os_ << "        else\n";
            os_ << "        {\n";
            os_ << "            std::size_t const state_ =\n";

            if (lookups_ == 256)
            {
                os_ << "                ptr_[lookup_[static_cast<unsigned char>\n";
                os_ << "                (*curr_++)]];\n";
            }
            else
            {
                os_ << "                ptr_[lookup_[*curr_++]];\n";
            }

            os_ << '\n';
            os_ << "            if (state_ == 0) break;\n";
            os_ << '\n';
            os_ << "            ptr_ = &dfa_[state_ * dfa_alphabet_];\n";
            os_ << "        }\n";
        }
        else
        {
            os_ << "        std::size_t const state_ =\n";

            if (lookups_ == 256)
            {
                os_ << "            ptr_[lookup_[static_cast<unsigned char>\n";
                os_ << "            (*curr_++)]];\n";
            }
            else
            {
                os_ << "            ptr_[lookup_[*curr_++]];\n";
            }

            os_ << '\n';
            os_ << "        if (state_ == 0) break;\n";
            os_ << '\n';
            os_ << "        ptr_ = &dfa_[state_ * dfa_alphabet_];\n";
        }

        os_ << '\n';
        os_ << "        if (*ptr_)\n";
        os_ << "        {\n";
        os_ << "            end_state_ = true;\n";
        os_ << "            id_ = *(ptr_ + id_index);\n";

        if (dfas_ > 1)
        {
            os_ << "            start_state_ = *(ptr_ + state_index);\n";
        }

        os_ << "            end_token_ = curr_;\n";
        os_ << "        }\n";
        os_ << "    }\n";
        os_ << '\n';

        if (sm_._seen_EOL_assertion)
        {
            os_ << "    const std::size_t EOL_state_ = ptr_[eol_index];\n";
            os_ << '\n';
            os_ << "    if (EOL_state_ && curr_ == end_)\n";
            os_ << "    {\n";
            os_ << "        ptr_ = &dfa_[EOL_state_ * dfa_alphabet_];\n";
            os_ << '\n';
            os_ << "        if (*ptr_)\n";
            os_ << "        {\n";
            os_ << "            end_state_ = true;\n";
            os_ << "            id_ = *(ptr_ + id_index);\n";

            if (dfas_ > 1)
            {
                os_ << "            start_state_ = *(ptr_ + state_index);\n";
            }

            os_ << "            end_token_ = curr_;\n";
            os_ << "        }\n";
            os_ << "    }\n";
            os_ << '\n';
        }

        os_ << "    if (end_state_)\n";
        os_ << "    {\n";
        os_ << "        // return longest match\n";
        os_ << "        start_token_ = end_token_;\n";

        if (dfas_ > 1)
        {
            os_ << '\n';
            os_ << "        if (id_ == 0) goto again;\n";
        }

        os_ << "    }\n";
        os_ << "    else\n";
        os_ << "    {\n";
      
        if (skip_on_nomatch)
        {
            os_ << "        // No match causes char to be skipped\n";
            os_ << "        ++start_token_;\n";
        }
      
        os_ << "        id_ = npos;\n";
        os_ << "    }\n";
        os_ << '\n';
        os_ << "    return id_;\n";
        os_ << "}\n\n";

        os_ << "}}}}  // namespace boost::spirit::lex::static_\n\n";
        
        os_ << "#endif\n";
        
        return os_.good();
    }
    
///////////////////////////////////////////////////////////////////////////////
}}}}   // namespace boost::spirit::lex::detail

///////////////////////////////////////////////////////////////////////////////
namespace boost { namespace spirit { namespace lex 
{ 
    ///////////////////////////////////////////////////////////////////////////
    template <typename Lexer>
    inline bool 
    generate_static(Lexer const& lexer, std::ostream& os, 
        char const* name_suffix = "")
    {
        if (!lexer.get_definition().init_dfa())
            return false;
        return detail::generate_cpp(lexer.get_definition().state_machine, 
            lexer.get_definition().rules, os, name_suffix, false, false);
    }

///////////////////////////////////////////////////////////////////////////////
}}}

#endif
