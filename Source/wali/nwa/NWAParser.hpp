#ifndef NWA_PARSER_HPP
#define NWA_PARSER_HPP

#include <iosfwd>
#include "NWA.hpp"

namespace wali {
    namespace nwa {
        extern NWARefPtr read_nwa(std::istream & is);
        extern void test_all();
    }
}

#endif