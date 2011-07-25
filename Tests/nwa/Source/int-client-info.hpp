#ifndef WALI_TESTS_NWA_int_client_info_HPP
#define WALI_TESTS_NWA_int_client_info_HPP

#include "wali/nwa/ClientInfo.hpp"

namespace wali {
    namespace nwa {

        struct IntClientInfo : ClientInfo
        {
            IntClientInfo * clone() {
                return new IntClientInfo(*this);
            }

            IntClientInfo(int n_)
                : n(n_)
            {}

            IntClientInfo(IntClientInfo const & other)
                : ClientInfo(other)
                , n(other.n)
            {}
            
            int n;
        };
        
    }
}

#endif
