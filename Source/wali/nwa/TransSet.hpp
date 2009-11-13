#ifndef wali_nwa_TransSet_GUARD
#define wali_nwa_TransSet_GUARD 1

/**
 * @author Amanda Burton
 */

// ::wali
#include "wali/Printable.hpp"
#include "wali/Key.hpp"
#include "wali/KeyContainer.hpp"
#include "wali/nwa/StateSet.hpp"
#include "wali/nwa/TransInfo.hpp"

// std::c++
#include <iostream>
#include <set>

namespace wali
{
  namespace nwa
  {

    template <typename St, typename Sym, typename Call, typename Internal, typename Return>
    class TransSet : public Printable
    {
      //TODO: update comments
      public:        
        typedef std::set< Call > Calls;
        typedef std::set< Internal > Internals;
        typedef std::set< Return > Returns;
        typedef typename Calls::iterator callIterator;
        typedef typename Internals::iterator internalIterator;
        typedef typename Returns::iterator returnIterator;
        typedef TransInfo<St,Call,Internal,Return> Info;
      
      //
      // Methods
      //

      public:
        //Constructors and Destructor
        TransSet( );
        TransSet( const TransSet & other );
        TransSet & operator=( const TransSet & other );

        ~TransSet( );

        //Transition Accessors

        /**
         * TODO
         */
        bool getSymbol(St from, St to, Sym & sym);

        /**
        * TODO
        */
        bool findTrans(St from, const Sym & sym, St to) const;

        /**
         * TODO
         */
        std::set<St*> getReturnSites(St callSite);

        /**
         * TODO
         */
        std::set<St*> getCallSites(St exitSite, St returnSite);

        /**
         * TODO
         */
        void dupTrans(St orig,St dup);

        /**
         *
         * @brief removes all transitions from this collection of transitions
         *
         * This method removes all transitions from this collection of
         * transitions by redirecting all existing transitions to point to the 
         * error state.
         *
         */
        void clear( );
      
        /**
         *
         * @brief get all call transitions in the collection of transitions associated 
         * with the NWA
         *
         * This method provides access to all call transitions in the collection of
         * transitions associated with the NWA.  
         *
         * @return all call transitions in the collection of transitions associated 
         * with the NWA
         *
         */
        const Calls & getCalls();
        
        /**
         *
         * @brief get all internal transitions in the collection of transitions 
         * associated with the NWA
         *
         * This method provides access to all internal transitions in the collection 
         * of transitions associated with the NWA.  
         *
         * @return all internal transitions in the collection of transitions associated 
         * with the NWA
         *
         */
        const Internals & getInternals();
        
        /**
         *
         * @brief get all return transitions in the collection of transitions 
         * associated with the NWA
         *
         * This method provides access to all return transitions in the collection 
         * of transitions associated with the NWA.  
         *
         * @return all return transitions in the collection of transitions associated 
         * with the NWA
         *
         */
        const Returns & getReturns();
      
        /**
         *
         * @brief add the given call transition to the collection of transitions
         *
         * This method adds the given call transition to the collection of 
         * transitions associated with the NWA.  If the call transition with the
         * same from state and symbol but the error state as the to state exists 
         * in the collection of transitions, it is removed.  If this exact call 
         * transition already exists, false is returned. Otherwise, true is returned.
         *
         * @parm the call transition to add to the collection of transitions
         * @return false if the call transition already exists in the collection
         *
         */
        bool addCall(Call addTrans);
        
        /**
         *
         * @brief add the given internal transition to the collection of transitions
         *
         * This method adds the given internal transition to the collection of 
         * transitions associated with the NWA.  If the internal transition with the
         * same from state and symbol but the error state as the to state exists in 
         * the collection of transitions, it is removed.  If this exact internal 
         * transition already exists, false is returned. Otherwise, true is returned.
         *
         * @parm the internal transition to add to the collection of transitions
         * @return false if the internal transition already exists in the collection
         *
         */
        bool addInternal(Internal addTrans);
        
        /**
         *
         * @brief add the given return transition to the collection of transitions
         *
         * This method adds the given return transition to the collection of 
         * transitions associated with the NWA.  If the return transition with the 
         * same from state, pred state, and symbol but the error state as the to 
         * state exists in the collection of transitions, it is removed.  If this 
         * exact return transition already exists, false is returned. Otherwise, 
         * true is returned.
         *
         * @parm the return transition to add to the collection of transitions
         * @return false if the return transition already exists in the collection
         *
         */
        bool addReturn(Return addTrans);
      
        /**
         *
         * @brief add all transitions in the given collection to this
         * collection of transitions
         *
         * This method adds all of the transitions in the given collection
         * of transitions to this collection of transitions.
         *
         * @parm the collection of transitions to add to this collection
         * of transitions
         *
         */
        void addAllTrans(TransSet addTransSet);
      
        /**
         *
         * @brief remove the given call transition from the collection of 
         * transitions
         *
         * This method removes the given call transition from the collection 
         * of transitions.  If removing this transition leaves the NWA with no
         * call transitions corresponding to the from state and symbol of the 
         * removed transition, a call transition with the same from state and 
         * symbol as the removed call transition and the error state as the to 
         * state is added to the NWA.  If the given call transition does not 
         * exist in the collection of transitions false is returned.  Otherwise,
         * true is returned.
         *
         * @parm the call transition to remove from the collection
         * @return false if the given call transition does not exist in the 
         * collection, true otherwise.
         *
         */
        bool removeCall(Call removeTrans);
        
        /**
         *
         * @brief remove the given internal transition from the collection of 
         * transitions
         *
         * This method removes the given internal transition from the collection 
         * of transitions.  If removing this transition leaves the NWA with no 
         * internal transitions corresponding to the from state and symbol of the
         * removed transition, an internal transition with the same from state and
         * symbol as the removed internal transition and the error state as the to
         * state is added to the NWA.  If the given internal transition does not 
         * exist in the collection of transitions false is returned.  Otherwise,
         * true is returned.
         *
         * @parm the internal transition to remove from the collection
         * @return false if the given internal transition does not exist in the 
         * collection, true otherwise.
         *
         */
        bool removeInternal(Internal removeTrans);
        
        /**
         *
         * @brief remove the given return transition from the collection of 
         * transitions
         *
         * This method removes the given return transition from the collection 
         * of transitions.  If removing this transition leaves the NWA with no 
         * return transitions corresponding to the from state, pred state, and 
         * symbol of the removed transition, a return transition with the same 
         * from state, pred state, and symbol as the removed return transition 
         * and the error state as the to state is added to the NWA.  If the 
         * given return transition does not exist in the collection of transitions 
         * false is returned.  Otherwise, true is returned.
         *
         * @parm the return transition to remove from the collection
         * @return false if the given return transition does not exist in the 
         * collection, true otherwise.
         *
         */
        bool removeReturn(Return removeTrans);
                      
        /**
         *
         * @brief test if the given call transition is in the collection of 
         * transitions associated with the NWA
         *
         * This method tests whether the given call transition is in the collection
         * of transitions associated with the NWA.
         *
         * @param the call transition to check
         * @return true if the given call transition is in the collection of 
         * transitions associated with the NWA
         *
         */
        bool isCall(Call trans);
        
        /**
         *
         * @brief test if the given internal transition is in the collection of 
         * transitions associated with the NWA
         *
         * This method tests whether the given internal transition is in the 
         * collection of transitions associated with the NWA.
         *
         * @param the internal transition to check
         * @return true if the given internal transition is in the collection of 
         * transitions associated with the NWA
         *
         */
        bool isInternal(Internal trans);
        
        /**
         *
         * @brief test if the given return transition is in the collection of 
         * transitions associated with the NWA
         *
         * This method tests whether the given return transition is in the 
         * collection of transitions associated with the NWA.
         *
         * @param the return transition to check
         * @return true if the given return transition is in the collection of 
         * transitions associated with the NWA
         *
         */
        bool isReturn(Return trans);              
                      
        //Utilities	

        /**
         *
         * @brief print the collection of transitions
         *
         * This method prints out the transition set to the output stream 
         * provided.
         *
         * @param the output stream to print to
         * @return the output stream that was printed to
         *
         */
        std::ostream & print( std::ostream & o ) const;

        /**
         *
         * @brief tests whether this collection of transitions is equivalent 
         * to the collection of transitions 'other'
         *
         * This method tests the equivalence of this set of transitions and 
         * the set of transitions 'other'.
         *
         * @param the TransSet to compare this TransSet to
         * @return true if this TransSet is equivalent to the TransSet 'other'
         *
         */
        bool operator==( TransSet & other );

        /**
         *
         * @brief provides access to the call transitions in the collection 
         * through an iterator
         *
         * This method provides access to the call transitions in the collection
         * of transitions through an iterator.
         *
         * @return the starting point of an iterator through the call transitions
         * in the collection of transitions
         *
         */
        callIterator beginCall( );
        
        /**
         *
         * @brief provides access to the internal transitions in the collection 
         * through an iterator
         *
         * This method provides access to the internal transitions in the collection
         * of transitions through an iterator.
         *
         * @return the starting point of an iterator through the internal transitions
         * in the collection of transitions
         *
         */
        internalIterator beginInternal( );
        
        /**
         *
         * @brief provides access to the return transitions in the collection 
         * through an iterator
         *
         * This method provides access to the return transitions in the collection
         * of transitions through an iterator.
         *
         * @return the starting point of an iterator through the return transitions
         * in the collection of transitions
         *
         */
        returnIterator beginReturn( );
      
        /**
         *
         * @brief provides access to the call transitions in the collection 
         * through an iterator
         *
         * This method provides access to the call transitions in the collection
         * of transitions through an iterator.
         *
         * @return the exit point of an iterator through the call transitions in
         * the collection of transitions
         *
         */
         callIterator endCall( );
         
         /**
         *
         * @brief provides access to the internal transitions in the collection 
         * through an iterator
         *
         * This method provides access to the internal transitions in the collection
         * of transitions through an iterator.
         *
         * @return the exit point of an iterator through the internal transitions in
         * the collection of transitions
         *
         */
         internalIterator endInternal( );
         
         /**
         *
         * @brief provides access to the return transitions in the collection 
         * through an iterator
         *
         * This method provides access to the return transitions in the collection
         * of transitions through an iterator.
         *
         * @return the exit point of an iterator through the return transitions in
         * the collection of transitions
         *
         */
         returnIterator endReturn( );

        /**
         *
         * @brief returns the number of call transitions in the collection of
         * transitions associated with the NWA
         *
         * This method returns the number of call transitions in the collection
         * of transitions associated with the NWA.  Note: This count does not 
         * include any transition to or from the error state. 
         *
         * @return the number of call transitions in the collection of transitions
         * assoicated with the NWA
         *
         */
        size_t sizeCall( );
        
        /**
         *
         * @brief returns the number of internal transitions in the collection of
         * transitions associated with the NWA
         *
         * This method returns the number of internal transitions in the collection
         * of transitions associated with the NWA.  Note: This count does not include
         * any transition to or from the error state.
         *
         * @return the number of internal transitions in the collection of transitions
         * assoicated with the NWA
         *
         */
        size_t sizeInternal( );
        
        /**
         *
         * @brief returns the number of return transitions in the collection of
         * transitions associated with the NWA
         *
         * This method returns the number of return transitions in the collection
         * of transitions associated with the NWA.  Note: This count does not include
         * any transition to or from the error state or with the error state as the
         * pred state.
         *
         * @return the number of return transitions in the collection of transitions
         * assoicated with the NWA
         *
         */
        size_t sizeReturn( );
        
        /**
         *
         * @brief returns the total number of transitions in the collection of
         * transitions associated with the NWA
         *
         * This method returns the total number of transitions in the collection
         * of transitions associated with the NWA.  Note: This count does not 
         * include any transitions involving the error state.  This is equivalent 
         * to counting all transitions explicitly added to the NWA.
         *
         * @return the total number of transitions in the collection of transitions
         * assoicated with the NWA
         *
         */
        size_t size( );
        
        //TODO
        const std::set<Internal*> getTransFrom( St name );
        const std::set<Internal*> getTransTo( St name );
        const std::set<Call*> getTransCall( St name );
        const std::set<Call*> getTransEntry( St name );
        const std::set<Return*> getTransExit( St name );
        const std::set<Return*> getTransPred( St name );
        const std::set<Return*> getTransRet( St name );
        
        bool isFrom( St name );
        bool isTo( St name );
        bool isCall( St name );
        bool isEntry( St name );
        bool isExit( St name );
        bool isPred( St name );
        bool isRet( St name );

        /** 
         *
         * @brief removes all call transitions to or from the state with 
         * the given name  
         *
         * This method removes all call transitions to or from the state 
         * with the given name. If no call transitions exist to or from 
         * this state false is returned.  Otherwise, true is returned.
         *
         * @parm the name of the state whose transitions to remove
         * @return false if no transitions were removed
         *
         */
        bool removeCallTransWith( St name );
        
      
        /** 
         *
         * @brief removes all internal transitions to or from the state
         * with the given name
         *
         * This method removes all internal transitions to or from the
         * state with the given name.  If no internal transitions exist
         * to or from this state false is returned.  Otherwise, true is 
         * returned. 
         *
         * @parm the name of the state whose transitions to remove
         * @return false if no transitions were removed
         *
         */
        bool removeInternalTransWith( St name );
      
        /** 
         *
         * @brief removes all return transitions to or from the state with
         * the given name as well as return transitions corresponding to 
         * calls from the state with the given name
         *
         * This method removes all return transitions to or from the state
         * with the given name as well as return transitions corresponding
         * to calls from the state with the given name.  If no return 
         * transitions exist to or from this state false is returned.  
         * Otherwise, true is returned.
         *
         * @parm the name of the state whose transitions to remove
         * @return false if no transitions were removed
         *
         */
        bool removeReturnTransWith( St name );
      
      
        /** 
         *
         * @brief removes all call transitions with the given symbol 
         *
         * This method removes all call transitions with the given symbol. 
         * If no call transitions exist with the given symbol false is 
         * returned.  Otherwise, true is returned.
         *
         * @parm the name of the symbol whose transitions to remove
         * @return false if no transitions were removed
         *
         */
        bool removeCallTransSym(Sym sym);
        
        /** 
         *
         * @brief removes all internal transitions with the given symbol 
         *
         * This method removes all internal transitions with the given symbol. 
         * If no internal transitions exist with the given symbol false is 
         * returned.  Otherwise, true is returned.
         *
         * @parm the name of the symbol whose transitions to remove
         * @return false if no transitions were removed
         *
         */
        bool removeInternalTransSym(Sym sym);
        
        /** 
         *
         * @brief removes all return transitions with the given symbol 
         *
         * This method removes all return transitions with the given symbol. 
         * If no return transitions exist with the given symbol false is 
         * returned.  Otherwise, true is returned.
         *
         * @parm the name of the symbol whose transitions to remove
         * @return false if no transitions were removed
         *
         */
        bool removeReturnTransSym(Sym sym);
      
        /**
         *
         * @brief test if there exists a call transition with the given from state 
         * and symbol in the collection of transitions associated with the NWA
         *
         * This method tests whether there exists a call transition with the given 
         * from state and symbol but not the error state as the to state in the 
         * collection of transitions associated with the NWA.
         *
         * @param from: the desired from state for the call transition
         * @param sym: the desired symbol for the call transition
         * @return true if there exists a call transition with the given from state and
         * symbol in the collection of transitions associated with the NWA
         *
         */
        bool callExists(St from,Sym sym);
        
        const Calls getCalls(St from,Sym sym);
        
        /**
         *
         * @brief test if there exists an internal transition with the given from state 
         * and symbol in the collection of transitions associated with the NWA
         *
         * This method tests whether there exists an internal transition with the given 
         * from state and symbol but not the error state as the to state in the collection 
         * of transitions associated with the NWA.
         *
         * @param from: the desired from state for the internal transition
         * @param sym: the desired symbol for the internal transition
         * @return true if there exists an internal transition with the given from state and
         * symbol in the collection of transitions associated with the NWA
         *
         */
        bool internalExists(St from,Sym sym);
        
        const Internals getInternals(St from,Sym sym);
        
        /**
         *
         * @brief test if there exists a return transition with the given from state, 
         * predecessor state, and symbol in the collection of transitions associated 
         * with the NWA
         *
         * This method tests whether there exists a return transition with the given 
         * from state, predecessor state, and symbol but not the error state as the to
         * state in the collection of transitions associated with the NWA.
         *
         * @param from: the desired from state for the return transition
         * @param pred: the desired predecessor state for the return transition
         * @param sym: the desired symbol for the return transition
         * @return true if there exists a return transition with the given from state and
         * symbol in the collection of transitions associated with the NWA
         *
         */
        bool returnExists(St from, St pred, Sym sym);
        
        const Returns getReturns(St from, Sym sym);

      //
      // Variables
      //
      
      protected: 
        Calls callTrans;
        Internals internalTrans;
        Returns returnTrans;
        
        Info T_info;
    };
    
    //
    // Methods
    //

    //Constructors and Destructor
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    TransSet<St,Sym,Call,Internal,Return>::TransSet( )
    {
      callTrans = Calls();
      internalTrans = Internals();
      returnTrans = Returns();  
      
      T_info = Info();
    }
    
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    TransSet<St,Sym,Call,Internal,Return>::TransSet( const TransSet<St,Sym,Call,Internal,Return> & other )
    {
      callTrans = other.callTrans;
      internalTrans = other.internalTrans;
      returnTrans = other.returnTrans;
      
      T_info = other.T_info;
    }
    
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    TransSet<St,Sym,Call,Internal,Return> & TransSet<St,Sym,Call,Internal,Return>::operator=( const TransSet<St,Sym,Call,Internal,Return> & other )
    {
      callTrans = other.callTrans;
      internalTrans = other.internalTrans;
      returnTrans = other.returnTrans;
      
      T_info = other.T_info;
      return *this;
    }

   
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    TransSet<St,Sym,Call,Internal,Return>::~TransSet( ) 
    {
      clear();
      T_info.clearMaps(); 
    }

    //Transition Accessors

    /**
     *
     * @brief removes all transitions from this collection of transitions
     *
     */
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    void TransSet<St,Sym,Call,Internal,Return>::clear( )
    {
      callTrans.clear();
      internalTrans.clear();
      returnTrans.clear();
      
      T_info.clearMaps();
    }

    /**
     * TODO
     */
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    bool TransSet<St,Sym,Call,Internal,Return>::getSymbol(St fromSt, St toSt, Sym & sym)
    {
      const Info::Internals from = T_info.from(fromSt);
      for( Info::Internals::const_iterator it = from.begin(); it != from.end(); it++ )
      {
        if( toSt == (*it)->third )
        {
          sym = (*it)->second;
          return true;
        }
      }
      
      const Info::Calls call = T_info.call(fromSt);
      for( Info::Calls::const_iterator it = call.begin(); it != call.end(); it++ )
      {
        if( toSt == (*it)->third )
        {
          sym = (*it)->second;
          return true;
        }
      }
            
      const Info::Returns exit = T_info.exit(fromSt);
      for( Info::Returns::const_iterator it = exit.begin(); it != exit.end(); it++ )
      {
        if( toSt == (*it)->fourth )
        {
          sym = (*it)->third;
          return true;
        }
      }
      //TODO: Q: does this count as a symbol we would like to have?
      /*const Info::Returns pred = T_info.pred(fromSt);
      for( Info::Returns::const_iterator it = pred.begin(); it != pred.end(); it++ )
      {
        if( toSt == (*it)->fourth )
        {
          sym = (*it)->third;
          return true;
        }
      }*/
      
      return false;
    }


    /**
     * TODO
     */
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    bool TransSet<St,Sym,Call,Internal,Return>::findTrans(St fromSt, const Sym & sym, St toSt) const
    {
      const Info::Internals from = T_info.from(fromSt);
      for( Info::Internals::const_iterator it = from.begin(); it != from.end(); it++ )
      {
        if( toSt == (*it)->third && sym==(*it).second)
          return true;
      }
      
      const Info::Calls call = T_info.call(fromSt);
      for( Info::Calls::const_iterator it = call.begin(); it != call.end(); it++ )
      {
        if( toSt == (*it)->third && sym == (*it)->second)
        {
          return true;
        }
      }
            
      const Info::Returns exit = T_info.exit(fromSt);
      for( Info::Returns::const_iterator it = exit.begin(); it != exit.end(); it++ )
      {
        if( toSt == (*it)->fourth && sym == (*it)->third)
        {
          return true;
        }
      }
      //TODO: Q: does this count as a symbol we would like to have?
      /*const Info::Returns pred = T_info.pred(fromSt);
      for( Info::Returns::const_iterator it = pred.begin(); it != pred.end(); it++ )
      {
        if( toSt == (*it)->fourth )
        {
          sym = (*it)->third;
          return true;
        }
      }*/
      
      return false;
    }

    /**
     * TODO
     */
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    std::set<St*> TransSet<St,Sym,Call,Internal,Return>::getReturnSites(St callSite)
    {
      std::set<St*> returns;
      const Info::Returns pred = T_info.pred(callSite);
      for( Info::Returns::const_iterator it = pred.begin(); it != pred.end(); it++ )
      {
        returns.insert(&(*it)->fourth);
      }
      return returns;
    }

    /**
     * TODO
     */
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    std::set<St*> TransSet<St,Sym,Call,Internal,Return>::getCallSites(St exitSite, St returnSite)
    {
      std::set<St*> calls;
      const Info::Returns exit = T_info.exit(exitSite);
      for( Info::Returns::const_iterator it = exit.begin(); it != exit.end(); it++ )
      {
        if( (*it)->fourth == returnSite )
          calls.insert(&(*it)->first);
      }
      return calls;
    }
    
    /**
     * TODO
     */
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    void TransSet<St,Sym,Call,Internal,Return>::dupTrans(St orig,St dup)
    { 
      const Info::Internals from = T_info.from(orig);
      for( Info::Internals::const_iterator it = from.begin(); it != from.end(); it++ )
      {
        Internal iTrans = Internal(dup,(*it)->second,(*it)->third);
        addInternal(iTrans);
        if( orig == (*it)->third )
        {
          iTrans = Internal(dup,(*it)->second,dup);
          addInternal(iTrans);
        }
      }

      const Info::Internals to = T_info.to(orig);
      for( Info::Internals::const_iterator it = to.begin(); it != to.end(); it++ )
      {
        Internal iTrans = Internal((*it)->first,(*it)->second,dup);
        addInternal(iTrans);
      }
      
      const Info::Calls call = T_info.call(orig);
      for( Info::Calls::const_iterator it = call.begin(); it != call.end(); it++ )
      {
        Call cTrans = Call(dup,(*it)->second,(*it)->third);
        addCall(cTrans);
        if( orig == (*it)->third )
        {
          cTrans = Call(dup,(*it)->second,dup);
          addCall(cTrans);
        }
      }
      
      const Info::Calls entry = T_info.entry(orig);
      for( Info::Calls::const_iterator it = entry.begin(); it != entry.end(); it++ )
      {
        Call cTrans = Call((*it)->first,(*it)->second,dup);
        addCall(cTrans);
      }
      
      const Info::Returns exit = T_info.exit(orig);
      for( Info::Returns::const_iterator it = exit.begin(); it != exit.end(); it++ )
      {
        Return rTrans = Return(dup,(*it)->second,(*it)->third,(*it)->fourth);
        addReturn(rTrans);
        if( orig == (*it)->second )
        {
          rTrans = Return(dup,dup,(*it)->third,(*it)->fourth);
          addReturn(rTrans);
        }
        if( orig == (*it)->fourth )
        {
          rTrans = Return(dup,(*it)->second,(*it)->third,dup);
          addReturn(rTrans);
        }
        if( orig == (*it)->second && orig == (*it)->fourth )
        {
          rTrans = Return(dup,dup,(*it)->third,dup);
          addReturn(rTrans);
        }
      }
      
      const Info::Returns pred = T_info.pred(orig);
      for( Info::Returns::const_iterator it = pred.begin(); it != pred.end(); it++ )
      {
        Return rTrans = Return((*it)->first,dup,(*it)->third,(*it)->fourth);
        addReturn(rTrans);
        if( orig == (*it)->fourth )
        {
          rTrans = Return((*it)->first,dup,(*it)->third,dup);
          addReturn(rTrans);
        }
      }
      
      const Info::Returns ret = T_info.ret(orig);
      for( Info::Returns::const_iterator it = ret.begin(); it != ret.end(); it++ )
      {
        Return rTrans = Return((*it)->first,(*it)->second,(*it)->third,dup);
        addReturn(rTrans);
      }
    }

    /**
     *
     * @brief get all call transitions in the collection of transitions associated 
     * with the NWA
     *
     * @return all call transitions in the collection of transitions associated 
     * with the NWA
     *
     */
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    const typename TransSet<St,Sym,Call,Internal,Return>::Calls & TransSet<St,Sym,Call,Internal,Return>::getCalls()
    {
      return callTrans;
    }
    
    /**
     *
     * @brief get all internal transitions in the collection of transitions 
     * associated with the NWA
     *
     * @return all internal transitions in the collection of transitions associated 
     * with the NWA
     *
     */
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    const typename TransSet<St,Sym,Call,Internal,Return>::Internals & TransSet<St,Sym,Call,Internal,Return>::getInternals()
    {
      return internalTrans;
    }
    
    /**
     *
     * @brief get all return transitions in the collection of transitions 
     * associated with the NWA
     *
     * @return all return transitions in the collection of transitions associated 
     * with the NWA
     *
     */
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    const typename TransSet<St,Sym,Call,Internal,Return>::Returns & TransSet<St,Sym,Call,Internal,Return>::getReturns()
    {
      return returnTrans;
    }
      
    /**
     *
     * @brief add the given call transition to the collection of transitions
     *
     * @parm the call transition to add to the collection of transitions
     * @return false if the call transition already exists in the collection
     *
     */   
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    bool TransSet<St,Sym,Call,Internal,Return>::addCall(Call addTrans)
    {
      if( callTrans.count(addTrans) > 0 )
        return false;      
      
      callTrans.insert(addTrans);
      T_info.addCall(&addTrans);

      return true;
    }
    
    /**
     *
     * @brief add the given internal transition to the collection of transitions
     *
     * @parm the internal transition to add to the collection of transitions
     * @return false if the internal transition already exists in the collection
     *
     */
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    bool TransSet<St,Sym,Call,Internal,Return>::addInternal(Internal addTrans)
    {
      if( internalTrans.count(addTrans) > 0 )
        return false;
      
      internalTrans.insert(addTrans);
      T_info.addIntra(&addTrans);

      return true;
    }
    
    /**
     *
     * @brief add the given return transition to the collection of transitions
     *
     * @parm the return transition to add to the collection of transitions
     * @return false if the return transition already exists in the collection
     *
     */
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    bool TransSet<St,Sym,Call,Internal,Return>::addReturn(Return addTrans)
    {
      if( returnTrans.count(addTrans) > 0 )
        return false;
        
      returnTrans.insert(addTrans);
      T_info.addRet(&addTrans);

      return true;
    }
      
    /**
     *
     * @brief add all transitions in the given collection to this
     * collection of transitions
     *
     * @parm the collection of transitions to add to this collection
     * of transitions
     *
     */
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    void TransSet<St,Sym,Call,Internal,Return>::addAllTrans(TransSet<St,Sym,Call,Internal,Return> addTransSet)
    {   
      for( TransSet::callIterator it = addTransSet.beginCall(); 
            it != addTransSet.endCall(); it ++ )
      {
        addCall(*it);
      }
      for( TransSet::internalIterator it = addTransSet.beginInternal(); 
            it != addTransSet.endInternal(); it ++ )
      {
        addInternal(*it);
      }
      for( TransSet::returnIterator it = addTransSet.beginReturn(); 
            it != addTransSet.endReturn(); it ++ )
      {
        addReturn(*it);
      }
    }
      
    /**
     *
     * @brief remove the given call transition from the collection of 
     * transitions
     *
     * @parm the call transition to remove from the collection
     * @return false if the given call transition does not exist in the 
     * collection, true otherwise.
     *
     */
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    bool TransSet<St,Sym,Call,Internal,Return>::removeCall(Call removeTrans)
    {
      if( callTrans.count(removeTrans) == 0 )
        return false;
        
      callTrans.erase(removeTrans);
      T_info.removeCall(&removeTrans);

      return true;
    }
    
    /**
     *
     * @brief remove the given internal transition from the collection of 
     * transitions
     *
     * @parm the internal transition to remove from the collection
     * @return false if the given internal transition does not exist in the 
     * collection, true otherwise.
     *
     */
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    bool TransSet<St,Sym,Call,Internal,Return>::removeInternal(Internal removeTrans)
    {
      if( internalTrans.count(removeTrans) == 0 )
        return false;
        
      internalTrans.erase(removeTrans);
      T_info.removeIntra(&removeTrans);

      return true;
    }
    
    /**
     *
     * @brief remove the given return transition from the collection of 
     * transitions
     *
     * @parm the return transition to remove from the collection
     * @return false if the given return transition does not exist in the 
     * collection, true otherwise.
     *
     */
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    bool TransSet<St,Sym,Call,Internal,Return>::removeReturn(Return removeTrans)
    {
      if( returnTrans.count(removeTrans) == 0 )
        return false;
        
      returnTrans.erase(removeTrans);
      T_info.removeRet(&removeTrans);

      return true;
    }
     
    /**
     *
     * @brief test if the given call transition is in the collection of 
     * transitions associated with the NWA
     *
     * @param the call transition to check
     * @return true if the given call transition is in the collection of 
     * transitions associated with the NWA
     *
     */   
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    bool TransSet<St,Sym,Call,Internal,Return>::isCall(Call trans)
    {
      return (callTrans.count(trans) > 0);
    }
    
    /**
     *
     * @brief test if the given internal transition is in the collection of 
     * transitions associated with the NWA
     *
     * @param the internal transition to check
     * @return true if the given internal transition is in the collection of 
     * transitions associated with the NWA
     *
     */
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    bool TransSet<St,Sym,Call,Internal,Return>::isInternal(Internal trans)
    {
      return (internalTrans.count(trans) > 0);
    }
    
    /**
     *
     * @brief test if the given return transition is in the collection of 
     * transitions associated with the NWA
     *
     * @param the return transition to check
     * @return true if the given return transition is in the collection of 
     * transitions associated with the NWA
     *
     */
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    bool TransSet<St,Sym,Call,Internal,Return>::isReturn(Return trans)
    {
      return (returnTrans.count(trans) > 0);
    } 
         
    //Utilities	

    /**
     *
     * @brief print the collection of transitions
     *
     * @param the output stream to print to
     * @return the output stream that was printed to
     *
     */
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    std::ostream & TransSet<St,Sym,Call,Internal,Return>::print( std::ostream & o) const
    {    
      o << "Delta_c: " << "{ ";
      Calls::const_iterator cit = callTrans.begin();
      Calls::const_iterator citEND = callTrans.end();
      for( bool first=true; cit != citEND; cit++ )
      {
        if( !first )
          o << ", ";
        o << "(";
        cit->first.print(o);
        o << ", ";
        cit->second.print(o);
        o << ", "; 
        cit->third.print(o);
        o << ")";
        first=false;
      }
      o << " }\n";
      
      o << "Delta_i: " << "{ ";
      Internals::const_iterator iit = internalTrans.begin();
      Internals::const_iterator iitEND = internalTrans.end();
      for( bool first=true; iit != iitEND; iit++ )
      {
        if( !first )
          o << ", ";
        o << "(";
        iit->first.print(o);
        o << ", ";
        iit->second.print(o);
        o << ", ";
        iit->third.print(o);
        o << ")";
        first = false;
      }
      o << " }\n";
      
      o << "Delta_r: " << "{ ";
      Returns::const_iterator rit = returnTrans.begin();
      Returns::const_iterator ritEND = returnTrans.end();
      for( bool first=true; rit != ritEND; rit++ )
      {
        if( !first )
          o << ", ";
        o << "(";
        rit->first.print(o);
        o << ", ";
        rit->second.print(o);
        o << ", "; 
        rit->third.print(o);
        o << ", ";
        rit->fourth.print(o);
        o << ")";
        first = false;
      }
      o << " }\n";
      
      return o;
    }

    /**
     *
     * @brief tests whether this collection of transitions is equivalent 
     * to the collection of transitions 'other'
     *
     * @param the TransSet to compare this TransSet to
     * @return true if this TransSet is equivalent to the TransSet 'other'
     *
     */
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    bool TransSet<St,Sym,Call,Internal,Return>::operator==( TransSet<St,Sym,Call,Internal,Return> & other )
    { //TODO: Q: Do I want a deeper check here?
      return (  (callTrans == other.callTrans) &&
                (internalTrans == other.internalTrans) &&
                (returnTrans == other.returnTrans) );
    }

    /**
     *
     * @brief provides access to the call transitions in the collection 
     * through an iterator
     *
     * @return the starting point of an iterator through the call transitions
     * in the collection of transitions
     *
     */    
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    typename TransSet<St,Sym,Call,Internal,Return>::callIterator TransSet<St,Sym,Call,Internal,Return>::beginCall()
    {
      return callTrans.begin();
    }
    
    /**
     *
     * @brief provides access to the internal transitions in the collection 
     * through an iterator
     *
     * @return the starting point of an iterator through the internal transitions
     * in the collection of transitions
     *
     */
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    typename TransSet<St,Sym,Call,Internal,Return>::internalIterator TransSet<St,Sym,Call,Internal,Return>::beginInternal()
    {
      return internalTrans.begin();
    }
    
    /**
     *
     * @brief provides access to the return transitions in the collection 
     * through an iterator
     *
     * @return the starting point of an iterator through the return transitions
     * in the collection of transitions
     *
     */
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    typename TransSet<St,Sym,Call,Internal,Return>::returnIterator TransSet<St,Sym,Call,Internal,Return>::beginReturn()
    {
      return returnTrans.begin();
    }
      
    /**
     *
     * @brief provides access to the call transitions in the collection 
     * through an iterator
     *
     * @return the exit point of an iterator through the call transitions in
     * the collection of transitions
     *
     */    
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    typename TransSet<St,Sym,Call,Internal,Return>::callIterator TransSet<St,Sym,Call,Internal,Return>::endCall()
    {
      return callTrans.end();
    }
    
    /**
     *
     * @brief provides access to the internal transitions in the collection 
     * through an iterator
     *
     * @return the exit point of an iterator through the internal transitions in
     * the collection of transitions
     *
     */
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    typename TransSet<St,Sym,Call,Internal,Return>::internalIterator TransSet<St,Sym,Call,Internal,Return>::endInternal()
    {
      return internalTrans.end();
    }
    
    /**
     *
     * @brief provides access to the return transitions in the collection 
     * through an iterator
     *
     * @return the exit point of an iterator through the return transitions in
     * the collection of transitions
     *
     */
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    typename TransSet<St,Sym,Call,Internal,Return>::returnIterator TransSet<St,Sym,Call,Internal,Return>::endReturn()
    {
      return returnTrans.end();
    }
    
    /**
     *
     * @brief returns the number of call transitions in the collection of
     * transitions associated with the NWA
     *
     * @return the number of call transitions in the collection of transitions
     * assoicated with the NWA
     *
     */
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    size_t TransSet<St,Sym,Call,Internal,Return>::sizeCall( )
    {
      return callTrans.size();
    }
        
    /**
     *
     * @brief returns the number of internal transitions in the collection of
     * transitions associated with the NWA
     *
     * @return the number of internal transitions in the collection of transitions
     * assoicated with the NWA
     *
     */
    template < typename St,typename Sym,typename Call,typename Internal, typename Return > 
    size_t TransSet<St,Sym,Call,Internal,Return>::sizeInternal( )
    {
      return internalTrans.size();
    }
        
    /**
     *
     * @brief returns the number of return transitions in the collection of
     * transitions associated with the NWA
     *
     * @return the number of return transitions in the collection of transitions
     * assoicated with the NWA
     *
     */
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    size_t TransSet<St,Sym,Call,Internal,Return>::sizeReturn( )
    {
      return returnTrans.size();
    }
        
    /**
     *
     * @brief returns the total number of transitions in the collection of
     * transitions associated with the NWA
     *
     * @return the total number of transitions in the collection of transitions
     * assoicated with the NWA
     *
     */
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    size_t TransSet<St,Sym,Call,Internal,Return>::size( )
    {
      return (sizeCall() + sizeInternal() + sizeReturn());
    }
    
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    const std::set<Internal*> TransSet<St,Sym,Call,Internal,Return>::getTransFrom( St name )
    {
      return T_info.from( St name );
    }
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    const std::set<Internal*> TransSet<St,Sym,Call,Internal,Return>::getTransTo( St name )
    {
      return T_info.to( St name );
    }
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    const std::set<Call*> TransSet<St,Sym,Call,Internal,Return>::getTransCall( St name )
    {
      return T_info.call( St name );
    }
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    const std::set<Call*> TransSet<St,Sym,Call,Internal,Return>::getTransEntry( St name )
    {
      return T_info.entry( St name );
    }
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    const std::set<Return*> TransSet<St,Sym,Call,Internal,Return>::getTransExit( St name )
    {
      return T_info.exit( St name );
    }
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    const std::set<Return*> TransSet<St,Sym,Call,Internal,Return>::getTransPred( St name )
    {
      return T_info.pred( St name );
    }
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    const std::set<Return*> TransSet<St,Sym,Call,Internal,Return>::getTransRet( St name )
    {
      return T_info.ret( St name );
    }
    
    //Returns true if the given state is a certain kind of state, 
    //i.e. that it has a transition into or out of it in which it
    //plays the given role.
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    bool TransSet<St,Sym,Call,Internal,Return>::isFrom( St name )
    {
      return T_info.isFrom( St name );
    }
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    bool TransSet<St,Sym,Call,Internal,Return>::isTo( St name )
    {
      return T_info.isTo( St name );
    }
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    bool TransSet<St,Sym,Call,Internal,Return>::isCall( St name )
    {
      return T_info.isCall( St name );
    }
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    bool TransSet<St,Sym,Call,Internal,Return>::isEntry( St name )
    {
      return T_info.isEntry( St name );
    }
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    bool TransSet<St,Sym,Call,Internal,Return>::isExit( St name )
    {
      return T_info.isExit( St name );
    }
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    bool TransSet<St,Sym,Call,Internal,Return>::isPred( St name )
    {
      return T_info.isPred( St name );
    }
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    bool TransSet<St,Sym,Call,Internal,Return>::isRet( St name )
    {
      return T_info.isRet( St name );
    }
      
    /** 
     *
     * @brief removes all call transitions to or from the state with 
     * the given name  
     *
     * @parm the name of the state whose transitions to remove
     * @return false if no transitions were removed
     *
     */
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    bool TransSet<St,Sym,Call,Internal,Return>::removeCallTransWith( St name )
    {
      Calls removeTrans = Calls();
      for( callIterator cit = beginCall(); cit != endCall(); cit++ )
      {
        if( (cit->first == name) ||
            (cit->third == name) )
            removeTrans.insert(*cit);
      }     
      
      for( callIterator rit = removeTrans.begin();
            rit != removeTrans.end(); rit++ )
      {
        removeCall(*rit);
      }
      
      return removeTrans.size() > 0;
    }
  
    /** 
     *
     * @brief removes all internal transitions to or from the state
     * with the given name
     *
     * @parm the name of the state whose transitions to remove
     * @return false if no transitions were removed
     *
     */
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    bool TransSet<St,Sym,Call,Internal,Return>::removeInternalTransWith( St name )
    {
      Internals removeTrans = Internals();
      for( internalIterator iit = beginInternal();
            iit != endInternal(); iit++ )
      {
        if( (iit->first == name) ||
            (iit->third == name) )
            removeTrans.insert(*iit);
      }     
      
      for( internalIterator rit = removeTrans.begin();
            rit != removeTrans.end(); rit++ )
      {
        removeInternal(*rit);
      }
      
      return removeTrans.size() > 0;
    }
  
    /** 
     *
     * @brief removes all return transitions to or from the state with
     * the given name as well as return transitions corresponding to 
     * calls from the state with the given name
     *
     * @parm the name of the state whose transitions to remove
     * @return false if no transitions were removed
     *
     */
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    bool TransSet<St,Sym,Call,Internal,Return>::removeReturnTransWith( St name )
    {
      Returns removeTrans = Returns();
      for( returnIterator rit = beginReturn();
            rit != endReturn(); rit++ )
      {
        if( (rit->first == name) ||
            (rit->second == name) ||
            (rit->fourth == name) )
            removeTrans.insert(*rit);
      }     
      
      for( returnIterator rit = removeTrans.begin();
            rit != removeTrans.end(); rit++ )
      {
        removeReturn(*rit);
      }
      
      return removeTrans.size() > 0;
    }
    
    /** 
     *
     * @brief removes all call transitions with the given symbol 
     *
     * @parm the name of the symbol whose transitions to remove
     * @return false if no transitions were removed
     *
     */
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    bool TransSet<St,Sym,Call,Internal,Return>::removeCallTransSym(Sym sym)
    {
      Calls removeTrans = Calls();
      for( callIterator cit = callTrans.begin();
            cit != callTrans.end(); cit++ )
      {
        if( cit->second == sym )
          removeTrans.insert(*cit);
      }     
      
      for( callIterator rit = removeTrans.begin();
            rit != removeTrans.end(); rit++ )
      {
        removeCall(*rit);
      }
      
      return removeTrans.size() > 0;  
    }
    
    /** 
     *
     * @brief removes all internal transitions with the given symbol 
     *
     * @parm the name of the symbol whose transitions to remove
     * @return false if no transitions were removed
     *
     */
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    bool TransSet<St,Sym,Call,Internal,Return>::removeInternalTransSym(Sym sym)
    {
      Internals removeTrans = Internals();
      for( internalIterator iit = internalTrans.begin();
            iit != internalTrans.end(); iit++ )
      {
        if( iit->second == sym )
          removeTrans.insert(*iit);
      }     
      
      for( internalIterator rit = removeTrans.begin();
            rit != removeTrans.end(); rit++ )
      {
        removeInternal(*rit);
      }
      
      return removeTrans.size() > 0; 
    }
    
    /** 
     *
     * @brief removes all return transitions with the given symbol 
     *
     * @parm the name of the symbol whose transitions to remove
     * @return false if no transitions were removed
     *
     */
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    bool TransSet<St,Sym,Call,Internal,Return>::removeReturnTransSym(Sym sym)
    {
      Returns removeTrans = Returns();
      for( returnIterator rit = returnTrans.begin();
            rit != returnTrans.end(); rit++ )
      {
        if( rit->third == sym )
          removeTrans.insert(*rit);
      }     
      
      for( returnIterator rit = removeTrans.begin();
            rit != removeTrans.end(); rit++ )
      {
        removeReturn(*rit);
      }
      
      return removeTrans.size() > 0; 
    }
    
    /**
     *
     * @brief test if there exists a call transition with the given from state 
     * and symbol in the collection of transitions associated with the NWA
     *
     * @param from: the desired from state for the call transition
     * @param sym: the desired symbol for the call transition
     * @return true if there exists a call transition with the given from state and
     * symbol in the collection of transitions associated with the NWA
     *
     */
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    bool TransSet<St,Sym,Call,Internal,Return>::callExists(St from,Sym sym)
    {
      for( callIterator cit = beginCall();
            cit != endCall(); cit++ )
      {
        if( (cit->first == from) && (cit->second == sym) )
          return true;
      }  
      return false;    
    }
    
    template < typename St,typename Sym,typename Call,typename Internal,typename Return >
    const typename TransSet<St,Sym,Call,Internal,Return>::Calls TransSet<St,Sym,Call,Internal,Return>::getCalls(St from,Sym sym)
    {
      Calls result;
      for( callIterator cit = beginCall();
            cit != endCall(); cit++ )
      {
        if( (cit->first == from) && (cit->second == sym) )
          result.insert(*cit);
      } 
      return result;
    }
    
    /**
     *
     * @brief test if there exists an internal transition with the given from state 
     * and symbol in the collection of transitions associated with the NWA
     *
     * @param from: the desired from state for the internal transition
     * @param sym: the desired symbol for the internal transition
     * @return true if there exists an internal transition with the given from state and
     * symbol in the collection of transitions associated with the NWA
     *
     */
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    bool TransSet<St,Sym,Call,Internal,Return>::internalExists(St from,Sym sym)
    {
      for( internalIterator iit = beginInternal();
            iit != endInternal(); iit++ )
      {
        if( (iit->first == from) && (iit->second == sym) )
          return true;    
      }     
      return false;
    }
    
    template < typename St,typename Sym,typename Call,typename Internal,typename Return >
    const typename TransSet<St,Sym,Call,Internal,Return>::Internals TransSet<St,Sym,Call,Internal,Return>::getInternals(St from,Sym sym)
    {
      Internals result;
      for( internalIterator iit = beginInternal();
            iit != endInternal(); iit++ )
      {
        if( (iit->first == from) && (iit->second == sym) )
          result.insert(*iit);
      } 
      return result;
    }
    
    /**
     *
     * @brief test if there exists a return transition with the given from state, 
     * predecessor state, and symbol in the collection of transitions associated 
     * with the NWA
     *
     * @param from: the desired from state for the return transition
     * @param pred: the desired predecessor state for the return transition
     * @param sym: the desired symbol for the return transition
     * @return true if there exists a return transition with the given from state and
     * symbol in the collection of transitions associated with the NWA
     *
     */
    template < typename St,typename Sym,typename Call,typename Internal, typename Return >
    bool TransSet<St,Sym,Call,Internal,Return>::returnExists(St from, St pred, Sym sym)
    {
      for( returnIterator rit = beginReturn();
            rit != endReturn(); rit++ )
      {
        if( (rit->first == from) && (rit->second == pred) && (rit->third == sym) )
            return true;      
      }     
      return false;
    }   
    
    template < typename St,typename Sym,typename Call,typename Internal,typename Return >
    const typename TransSet<St,Sym,Call,Internal,Return>::Returns TransSet<St,Sym,Call,Internal,Return>::getReturns(St from,Sym sym)
    {
      Returns result;
      for( returnIterator rit = beginReturn();
            rit != endReturn(); rit++ )
      {
        if( (rit->first == from) && (rit->third == sym) )
          result.insert(*rit);
      } 
      return result;
    } 
  }
}
#endif