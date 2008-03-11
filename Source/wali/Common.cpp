/*!
 * @author Nick Kidd
 */

#include "wali/Common.hpp"

namespace wali
{
    // std::cerr is the default err stream
    std::ostream* waliErr = &std::cerr;

    //! @return old err ostream
    std::ostream* set_wali_err( std::ostream* newErr )
    {
        std::ostream* tmp = waliErr;
        waliErr = newErr;
        return tmp;
    }

    //! @return old err ostream
    std::ostream* setWaliErr( std::ostream* newErr )
    {
        std::ostream* tmp = waliErr;
        waliErr = newErr;
        return tmp;
    }

    //! When running pre or post* query in FWPDS,
    //! setting this to true will also run the same
    //! query using regular WPDS and verify the results
    //! are the same.
    //!
    //! Default value is true
    static bool b_check_fwpds = true;

    /*!
     * @brief Turn FWPDS verification on and off.
     * @return void
     */
    void set_verify_fwpds( bool enable ) {
        b_check_fwpds = enable;
    }

    /*! 
     * @return true if FWPDS will verify the result
     * of each poststar or prestar query against EWPDS.
     */
    bool get_verify_fwpds() {
        return b_check_fwpds;
    }

    //! @brief the static variable moderating FWPDS laziness
    static bool b_lazy_fwpds = true;

    /*!
     * @brief Turn lazy weights on or off for FWPDS
     * @return void
     */
    void set_lazy_fwpds( bool enable ) {
        b_lazy_fwpds = enable;
    }

    /*! 
     * @return true if FWPDS outputs a WFA whose transitions have lazy
     * weights.returns a lazy automaton.
     */
    bool is_lazy_fwpds() {
        return b_lazy_fwpds;
    }

} // namespace wali

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
 */
