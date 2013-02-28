/**
 * @author Akash Lal
 * @author Nicholas Kidd
 * @version $Id$
 */

// ::wali
#include "wali/Common.hpp"
#include "wali/SemElem.hpp"
#include "wali/Worklist.hpp"
#include "wali/KeySource.hpp"
#include "wali/KeyPairSource.hpp"

// ::wali::wfa
#include "wali/wfa/ITrans.hpp"
#include "wali/wfa/Trans.hpp"
#include "wali/wfa/State.hpp"
#include "wali/wfa/TransFunctor.hpp"
#include "wali/wfa/TransSet.hpp"

// ::wali::wpds
#include "wali/wpds/Config.hpp"
#include "wali/wpds/RuleFunctor.hpp"
#include "wali/wpds/Wrapper.hpp"
#include "wali/wpds/GenKeySource.hpp"

// ::wali::wpds::ewpds
#include "wali/wpds/ewpds/ERule.hpp"
#include "wali/wpds/ewpds/EWPDS.hpp"
#include "wali/wpds/ewpds/ETrans.hpp"

#include <iostream>
#include <cassert>

#if defined(JL_EWPDS_DEBUG)
extern std::ostream * jl_ewpds_dot;
extern std::ostream * jl_ewpds_log;
extern std::ostream * jl_wfa_log;
#endif

namespace wali
{
  using wfa::TransSet;
  using wfa::WFA;
  using wfa::State;

  namespace wpds 
  {
    using ::wali::wpds::Config;

    namespace ewpds
    {

      class ERuleCopier : public ConstRuleFunctor
      {
        public:
          EWPDS& e;
          ref_ptr<Wrapper> wrapper;
          ERuleCopier(EWPDS& ewpds,ref_ptr<Wrapper> wr) : e(ewpds),wrapper(wr) {}
          virtual void operator()(const rule_t& r)
          {
            // TODO - can be made static_cast
            const ERule* erule = dynamic_cast<const ERule*>(r.get_ptr());
            assert(erule != NULL);
            sem_elem_t se = erule->weight();
            merge_fn_t mf = erule->merge_fn();
            if (wrapper.is_valid())
            {
              se = wrapper->unwrap(se);
              if (r->to_stack2() != WALI_EPSILON)
                mf = wrapper->unwrap(mf);
            }

            e.add_rule(
                erule->from_state(), erule->from_stack(),
                erule->to_state(), erule->to_stack1(), erule->to_stack2(),
                se, mf);
          }
      };

      const std::string EWPDS::XMLTag("EWPDS");

      EWPDS::EWPDS() : WPDS(), addEtrans(false)
      {
      }

      EWPDS::EWPDS( ref_ptr<Wrapper> wr ) : WPDS(wr), addEtrans(false)
      { 
      }


      EWPDS::EWPDS( const EWPDS& e ) :
        WPDS(e.wrapper),
        addEtrans(false)
      {
        ERuleCopier copier(*this,wrapper);
        e.for_each(copier);
      }

      EWPDS::~EWPDS()
      {
        //*waliErr << "~EWPDS()" << std::endl;
        merge_rule_hash.clear();
      }

      bool EWPDS::add_rule(
          Key from_state,
          Key from_stack,
          Key to_state,
          sem_elem_t se )
      {
        return add_rule(from_state,from_stack,to_state,WALI_EPSILON,WALI_EPSILON,se,(merge_fn_t)(NULL) );
      }

      bool EWPDS::add_rule(
          Key from_state,
          Key from_stack,
          Key to_state,
          Key to_stack1,
          sem_elem_t se )
      {
        return add_rule(from_state,from_stack,to_state,to_stack1,WALI_EPSILON,se,(merge_fn_t)(NULL) );
      }
    
      bool EWPDS::add_rule(
          Key from_state,
          Key from_stack,
          Key to_state,
          Key to_stack1,
          Key to_stack2,
          sem_elem_t se )
      {
        return add_rule(from_state,from_stack,to_state,to_stack1,to_stack2,se,(merge_fn_t)(NULL), false );
      }

      bool EWPDS::replace_rule(
          Key from_state,
          Key from_stack,
          Key to_state,
          Key to_stack1,
          Key to_stack2,
          sem_elem_t se )
      {
        return add_rule(from_state,from_stack,to_state,to_stack1,to_stack2,se,(merge_fn_t)(NULL), true );
      }

      bool EWPDS::add_rule(
          Key from_state,
          Key from_stack,
          Key to_state,
          Key to_stack1,
          Key to_stack2,
          sem_elem_t se,
          merge_fn_t mf )
      {
	return add_rule(from_state,from_stack,to_state,to_stack1,to_stack2,se,mf, false );
      }

      bool EWPDS::replace_rule(
          Key from_state,
          Key from_stack,
          Key to_state,
          Key to_stack1,
          Key to_stack2,
          sem_elem_t se,
          merge_fn_t mf )
      {
	return add_rule(from_state,from_stack,to_state,to_stack1,to_stack2,se,mf,true );
      }

      bool EWPDS::add_rule(
          Key from_state,
          Key from_stack,
          Key to_state,
          Key to_stack1,
          Key to_stack2,
          sem_elem_t se,
          merge_fn_t mf,
	  bool replace_weight )
      {
        // Every rule must have these 3 pieces defined
        assert( from_state != WALI_EPSILON );
        assert( from_stack != WALI_EPSILON );
        assert( to_state   != WALI_EPSILON );
        wpds::Config *from = make_config(from_state,from_stack);
        wpds::Config *to = make_config(to_state,to_stack1);

        pds_states.insert(from_state);
        pds_states.insert(to_state);

        rule_t r(new ERule(from,to,to_stack2,se,mf));
        // WPDS::make_rule returns [true] if the rule already
        // existed. I.e., rb == false <--> r \notin this.
        bool rb = make_rule(from,to,to_stack2,replace_weight,r);

        if (!rb && to_stack1 == WALI_EPSILON )
        {
          assert( to_stack2 == WALI_EPSILON );
          rule_zeroes.insert( to );
        }

        // Make sure the same push rule was not inserted again.
        // This causes problems with merge functions because
        // there isn't an interface for combining them like one
        // would normally do with weights.
        if (rb && !replace_weight && to_stack2 != WALI_EPSILON) 
        {
          if (!mf.is_valid()) 
          {
            // ERule = (..keys..), W, MFun
            ERule* erule = dynamic_cast<ERule*>(r.get_ptr());
            assert(erule != NULL);
            merge_fn_t rule_mf = erule->merge_fn();
            MergeFn* mfun = dynamic_cast<MergeFn*>(erule->merge_fn().get_ptr());
            // This is a default merge function. We can therefore combine
            // the weights on it.
            if (NULL != mfun)
            {
              // We get here if:
              // 1. The user added push rule r_push without an IMergeFn
              //     - Causing EWPDS to create a default MergeFn
              // 2. The user REadded push rule r_push w/out an IMergeFn
              //     - Though possibly w/ a different weight
              // 
              // This means that the user has yet to specify an IMergeFn
              // for this push rule and thus is using it as in WPDSs. 
              // So we fix up the default MergeFn so that it's weight
              // is equal to the combine of the two weights annotating
              // r_push, one for each insertion.
              // (Note that the combine has already taken place in the call
              //  to make_rule.)
              //
              erule->set_merge_fn( new MergeFn(erule->weight()) );
            }
            else {
              *waliErr << "[ERROR] EWPDS :: Cannot add again the same push rule.\n";
              r->print( *waliErr << "    : " ) << std::endl;
              throw r;
            }
          }
          else {
	    // Now lets ensure that the old merge function and the new one being
	    // added here are the same. This is the only case we allow.
            ERule* erule = dynamic_cast<ERule*>(r.get_ptr());
            assert(erule != NULL);
	    if(!mf->equal(erule->merge_fn())) {
	      *waliErr << "[ERROR] EWPDS :: Cannot add again the same push rule.\n";
	      r->print( *waliErr << "    : " ) << std::endl;
	      throw r;
	    }
          }
        }

	// Need to wrap the merge function if the rule is new or
	// if the rule existed before, but replace_weight was set, in
	// which case the old wrapped merge function got replaced
	if ( (!rb  || (rb && replace_weight)) && wrapper.is_valid() && to_stack2 != WALI_EPSILON) {
	  ERule* x = (ERule*)r.get_ptr();
	  x->set_merge_fn( wrapper->wrap(*x,x->merge_fn()) );
	}

        // If rb is false then the rule is new.
        // If there exists a rule r' = (p,c) -> (q,e,r)
        // such that (q,e,r) is the same r.h.s. for
        // r, then this is an error b/c merge functions
        // are indexed by the triple (q,e,r).
        if ( to_stack2 != WALI_EPSILON && !rb ) 
        {
          r2hash_t::iterator r2it = r2hash.find( to_stack2 );
          if( r2it == r2hash.end() ) 
          {
            std::list< rule_t > ls;
            r2it = r2hash.insert( to_stack2,ls ).first;
          }
          r2it->second.push_back( r );

          merge_rule_hash_t::iterator rhash_it = merge_rule_hash.find(KeyTriple(to_state,to_stack1,to_stack2));
          if(rhash_it == merge_rule_hash.end()) 
          {
            merge_rule_hash.insert(KeyTriple(to_state,to_stack1,to_stack2), r);
          } 
          else 
          {
            ERule* x = (ERule*)rhash_it->second.get_ptr();
	    ERule *er = (ERule*)(r.get_ptr());
	    if(!x->merge_fn()->equal(er->merge_fn())) {
	      // FIXME: raise exception
	      *waliErr << "[ERROR] EWPDS :: Cannot give two push rules with same r.h.s.\n";
	      r->print( *waliErr << "    : " ) << std::endl;
	    }
          }
        }
        // Set up theZero weight
        if (!theZero.is_valid() && r->weight().is_valid()) 
        {
          theZero = r->weight()->zero();
        }

        return rb;
      }

      rule_t EWPDS::lookup_rule(Key to_state, Key to_stack1, Key to_stack2) const {
        merge_rule_hash_t::const_iterator rhash_it = merge_rule_hash.find(KeyTriple(to_state,to_stack1,to_stack2));
        if(rhash_it == merge_rule_hash.end()) {
          return NULL;
        } 
        return rhash_it->second;
      }

      // delta is the delta weight on t2
      void EWPDS::prestar_handle_call(
          wfa::ITrans* t1,
          wfa::ITrans* t2,
          rule_t &r,
          sem_elem_t delta
          )
      {
        //std::cout << "Prestar_handle_call:\n";
        //t1->print(std::cout << "Trans1 = ") << "\n";
        //t2->print(std::cout << "Trans2 = ") << "\n";
        //delta->print(std::cout << "delta = ") << "\n";
        //r->print(std::cout << "r = ") << "\n";

        ETrans *et1 = dynamic_cast<ETrans *> (t1);
        ETrans *et2 = dynamic_cast<ETrans *> (t2);

        assert(!(et1 == 0 && et2 != 0));

        sem_elem_t w1, wNew;

        // Compute weight on the resulting transition
        if(et1 != 0) {
          erule_t er = (ERule *)(r.get_ptr());
          w1 = er->merge_fn()->apply_f(t1->weight()->one(), t1->weight());
          wNew = w1->extend(delta);
        } else {
          w1 = r->weight()->extend(t1->weight());
        }
        wNew = w1->extend(delta);

        // Find the appropriate type for the resulting transition
        if(et2 != 0) {
          // Create ETrans

          update_etrans( r->from()->state(), r->from()->stack(), t2->to(),
              wNew,r->from() );

        } else {
          // Create Trans
          update( r->from()->state(), r->from()->stack(), t2->to(),
              wNew,r->from() );
        }

      }

      void EWPDS::prestar_handle_trans(
          wfa::ITrans * t ,
          WFA & fa   ,
          rule_t & r,
          sem_elem_t delta
          )
      {
        //std::cout << "Prestar_handle_trans:\n";
        //t->print(std::cout << "Trans = ") << "\n";
        //delta->print(std::cout << "delta = ") << "\n";
        //r->print(std::cout << "r = ") << "\n";

        ETrans *et = dynamic_cast<ETrans *>(t);

        Key fstate = r->from()->state();
        Key fstack = r->from()->stack();

        sem_elem_t wrule_trans;
        if(r->stack2() == WALI_EPSILON) {
          wrule_trans = r->weight()->extend( delta );

          if(et != 0) {
            update_etrans( fstate, fstack, t->to(), wrule_trans, r->from() );
          } else {
            update( fstate, fstack, t->to(), wrule_trans, r->from() );
          }

        } else { 
          erule_t er = (ERule *)(r.get_ptr());

          if(et == 0) {
            wrule_trans = r->weight()->extend( delta );
          } else {
            wrule_trans = er->merge_fn()->apply_f(delta->one(), delta);
          }

          KeyPair kp( t->to(),r->stack2() );
          WFA::kp_map_t::iterator kpit = fa.kpmap.find( kp );
          WFA::kp_map_t::iterator kpitEND = fa.kpmap.end();
          TransSet::iterator tsit;

          if(kpit != kpitEND)
          {
            TransSet & transSet = kpit->second;
            for( tsit = transSet.begin(); tsit != transSet.end(); tsit++ )
            {
              wfa::ITrans* tprime = *tsit;
              sem_elem_t wtp = wrule_trans->extend( tprime->weight() );

              ETrans *etprime = dynamic_cast<ETrans *> (tprime);
              if(etprime == 0) {
                update( fstate, fstack, tprime->to(), wtp, r->from() );
              } else {
                assert(et != 0);
                update_etrans( fstate, fstack, tprime->to(), wtp, r->from() );
              }
            }
          }
        }
      }


      void EWPDS::prestar( WFA const & input, WFA& fa )
      {

        // Add ETrans when Rule0 transitions are added
        addEtrans = true;
        prestarSetupFixpoint(input, fa);
        addEtrans = false;

        prestarComputeFixpoint(fa);
        unlinkOutput(fa);
        currentOutputWFA = 0;
      }


      void EWPDS::post( wfa::ITrans* t, WFA& fa )
      {
#if defined(JL_EWPDS_DEBUG)
        *jl_wfa_log << "-----------------------------------------------------------------\n";
        t->print(*jl_wfa_log) << std::endl << "--------------\n";
        currentOutputWFA->print(*jl_wfa_log) << std::endl;

        fa.print_dot(*jl_ewpds_dot);
        *jl_ewpds_log << "<-"; t->print(*jl_ewpds_log) << std::endl;
        *jl_ewpds_log << "tweight: \n";
        t->weight()->print(*jl_ewpds_log) << std::endl;
        *jl_ewpds_log << "tdelta: \n";
        t->getDelta()->print(*jl_ewpds_log) << std::endl;
#endif

        // Get config
        Config * config = t->getConfig();

        // Get delta
        sem_elem_t dnew = t->getDelta();
        
        // Reset delta of t to zero to signify completion
        // of work for that delta
        t->setDelta(theZero);

        sem_elem_t se = t->weight();
        t->setSeOld(se);

        // For each forward rule of config
        // Apply rule to create new transition
        if( WALI_EPSILON != t->stack() )
        {
          //std::cerr << "Looking for rules from " << key2str(config->kp.first) << " (" << config->kp.first << ") to " << key2str(config->kp.second) << " (" << config->kp.second << ")\n";
          Config::iterator fwit = config->begin();
          for( ; fwit != config->end() ; fwit++ ) {
            rule_t & r = *fwit;
#if defined(JL_EWPDS_DEBUG)
            *jl_ewpds_log << "rweight:\n";
            r->weight()->print(*jl_ewpds_log) << std::endl;
#endif
            poststar_handle_trans( t,fa,r,dnew );
          }
        }
        else {
          sem_elem_t se_old = t->getSeOld();
          sem_elem_t se_propagated = t->getSePropagated();
          sem_elem_t difference = se_old->computeDifference(se_propagated, dnew);

          // t:(p,eps,q) + tprime:(q,y,q') => (p,y,q')
          State * state = fa.getState( t->to() );
          State::iterator it = state->begin();
          for(  ; it != state->end() ; it++ )
          {
            wfa::ITrans* tprime = *it;
#if defined(JL_EWPDS_DEBUG)
            *jl_ewpds_log << "tweight2:\n";
            tprime->print(*jl_ewpds_log) << std::endl;
            tprime->weight()->print(*jl_ewpds_log) << std::endl;
            tprime->getDelta()->print(*jl_ewpds_log) << std::endl;
            *jl_ewpds_log << "tweight3:\n";
#endif
            poststar_handle_eps_trans(t, tprime, difference);
          }
        }

        t->setSePropagated(t->getSeOld());
      }


      void EWPDS::poststar_handle_eps_trans(wfa::ITrans *teps, wfa::ITrans*tprime, sem_elem_t delta)
      {
#if defined(JL_EWPDS_DEBUG)
        *jl_ewpds_log << "difference:\n";
        delta->print(*jl_ewpds_log) << std::endl;
#endif
        sem_elem_t wght = tprime->poststar_eps_closure( delta );
#if defined(JL_EWPDS_DEBUG)
        *jl_ewpds_log << "after poststar_eps_closure:\n";
        wght->print(*jl_ewpds_log) << std::endl;
#endif
        Config * config = make_config( teps->from(),tprime->stack() );
        update( teps->from()
            , tprime->stack()
            , tprime->to()
            , wght
            , config
            );
      }


      void EWPDS::poststar_handle_trans(
          wfa::ITrans * t, // t is a non-epsilon transition 
          WFA & fa,
          rule_t & r,
          sem_elem_t delta
          )
      {
        Key rtstate = r->to_state();
        Key rtstack = r->to_stack1();

        if( r->to_stack2() == WALI_EPSILON ) { // r is a rule 0 or rule 1 
          //update( rtstate, rtstack, t->to(), wrule_trans, r->to() );
#if defined(JL_EWPDS_DEBUG)
          *jl_ewpds_log << "old_tnew and tnew:\n";
#endif
          update( r->to_state(), r->to_stack1(), t->to(), r->to(), t, r->weight(), delta );
        }
        else 
        {
          //sem_elem_t wrule_trans = delta->extend( er->extended_weight() );
          sem_elem_t wrule_trans = delta->extend(r->weight());

          // Is a rule 2 so we must generate a state
          // and create 2 new transitions
          Key gstate = gen_state( rtstate,rtstack );

          wfa::ITrans* tprime = update_prime( gstate, t, r, delta );

#if defined(JL_EWPDS_DEBUG)
          *jl_ewpds_log << "old_tprime and tprime:\n";
          tprime->print(*jl_ewpds_log) << std::endl;
          tprime->weight()->print(*jl_ewpds_log) << std::endl;
          tprime->getDelta()->print(*jl_ewpds_log) << std::endl;
#endif

          // Setup the weight for taking quasione
          // TODO: This does not use the diff operator because for pair we would
          // need it to be (delta,second) + (first,delta). Otherwise taking
          // an extend of the components doesn't work out
          //sem_elem_t called_weight = SEM_PAIR_COLLAPSE(tprime->weight());

          //called_weight->print(std::cout << "called_weight = ") << "\n";

          // Take quasione
          State * state = fa.getState( gstate );

          // This combine operation is redundant because called_weight is
          // computed on the transition weight, not its delta
          //sem_elem_t quasi = state->quasi->combine( called_weight );
          //state->quasi = quasi; 
          state->quasi = state->quasi->combine( wrule_trans->quasi_one() );

#if defined(JL_EWPDS_DEBUG)
          *jl_ewpds_log << "old_quasit and quasit:\n";
#endif
          //sem_elem_t quasi_extended = new SemElemPair(quasi->quasi_one(), quasi->one());
          //update( rtstate, rtstack, gstate, quasi_extended, r->to() );
          update( rtstate, rtstack, gstate, state->quasi, r->to() );

          // Trans with generated from states do not go on the worklist
          // and there is no Config matching them so pass 0 (NULL) as the
          // Config * for update_prime
          if( tprime->modified() )
          {
            //std::cout <<
            // "[EWPDS::poststar] tprime modified...searching for eps trans\n";

            WFA::eps_map_t::iterator epsit = fa.eps_map.find( tprime->from() );
            if( epsit != fa.eps_map.end() )
            {
              // tprime stack key
              Key tpstk = tprime->stack();
              // tprime to key
              Key tpto = tprime->to();
              // get epsilon list ref
              TransSet& transSet = epsit->second;
              // iterate
              TransSet::iterator tsit = transSet.begin();
              for( ; tsit != transSet.end() ; tsit++ )
              {
                wfa::ITrans* teps = *tsit;

                Config* config = make_config( teps->from(),tpstk );
                sem_elem_t epsW = tprime->poststar_eps_closure( teps->weight() );

#if defined(JL_EWPDS_DEBUG)
                teps->print(*jl_ewpds_log) << std::endl;
                *jl_ewpds_log << "old_tnew and tnew2:\n";
#endif
                update( teps->from(),tpstk,tpto, epsW, config );
              }
            }
          }
        }
      }

      std::ostream & EWPDS::print( std::ostream & o ) const
      {
        return WPDS::print(o);
      }

      std::ostream & EWPDS::marshall( std::ostream & o ) const
      {
        RuleMarshaller rm(o);
        o << "<EWPDS>\n";
        WPDS::for_each( rm );
        o << "</EWPDS>" << std::endl;
        return o;
      }

      /////////////////////////////////////////////////////////////////
      // Protected Methods
      /////////////////////////////////////////////////////////////////

      void EWPDS::operator()( wfa::ITrans const * orig )
      {
        if( is_strict() && is_pds_state(orig->to())) {
          *waliErr << "WALi Error: cannot have incoming transition to a PDS state\n";
          assert(0);
        }

        Config *c = make_config( orig->from(),orig->stack() );
        sem_elem_t se = 
          (wrapper.is_valid()) ? wrapper->wrap(*orig) : orig->weight();

        wfa::ITrans *t = orig->copy();

        t->setConfig(c);
        t->setWeight(se);

        // fa.addTrans takes ownership of passed in pointer
        currentOutputWFA->addTrans( t );

        // add t to the worklist for saturation
#if defined(JL_EWPDS_DEBUG)
        *jl_ewpds_log << "->"; t->print(*jl_ewpds_log) << std::endl;
#endif
        worklist->put( t );
      }

      void EWPDS::update_etrans(
          Key from,
          Key stack,
          Key to,
          sem_elem_t se,
          Config * cfg
          )
      {

        addEtrans = true;
        update(from, stack, to, se, cfg);
        addEtrans = false;
      }

      void EWPDS::update(
          Key from,
          Key stack,
          Key to,
          sem_elem_t se,
          Config * cfg
          )
      {

        wfa::ITrans *t;
        if(addEtrans) {
          t = currentOutputWFA->insert(new ETrans(from, stack, to,
                0, se, 0));
        } else {
          t = currentOutputWFA->insert(new wfa::Trans(from, stack, to, se));
        }

        t->setConfig(cfg);

#if defined(JL_EWPDS_DEBUG)
        t->print(*jl_ewpds_log) << std::endl;
        t->weight()->print(*jl_ewpds_log) << std::endl;
        t->getDelta()->print(*jl_ewpds_log) << std::endl;
#endif

        if (t->modified()) {
          //t->print(std::cout << "Adding transition: ") << "\n";
#if defined(JL_EWPDS_DEBUG)
          *jl_ewpds_log << "->"; t->print(*jl_ewpds_log) << std::endl;
#endif
          worklist->put( t );
        }

      }

      void EWPDS::update(
          Key from,
          Key stack,
          Key to,
          Config * cfg,
          wfa::ITrans* t,
          const sem_elem_t & rweight, // 
          const sem_elem_t & delta
          )
      {
        sem_elem_t se = delta->extend(rweight); // delta; //dummy: tnew's weight will be set later in this function
       
        wfa::ITrans* _tnew;
        if(addEtrans) {
          _tnew = new ETrans(from, stack, to, 0, se, 0); // _tnew->moditied == true
        }
        else {
          _tnew = new wfa::Trans(from, stack, to, se); // _tnew->moditied == true
        }

        wfa::ITrans* told = currentOutputWFA->lookup_trans(_tnew);

        sem_elem_t prevWatCall = delta->zero();

        bool existold;
        if(told == 0)
        { // Not exist in currentOutputWFA
          // Insert the new trans to currentOutputWFA
          told = currentOutputWFA->insert_trans(_tnew);
          existold = false;
        }
        else 
        {
          existold = true;

#if defined(JL_EWPDS_DEBUG)
          told->print(*jl_ewpds_log) << std::endl;
          *jl_ewpds_log << "told:\n"; told->weight()->print(*jl_ewpds_log) << std::endl;
          *jl_ewpds_log << "dold:\n"; told->getDelta()->print(*jl_ewpds_log) << std::endl;
#endif

          if(addEtrans) {
            ETrans* tmptnew = dynamic_cast<ETrans*>(_tnew);
            prevWatCall = tmptnew->getWeightAtCall();
          }
        }

        sem_elem_t se_old = t->getSeOld();
        sem_elem_t se_propagated = t->getSePropagated();

#if defined(JL_EWPDS_DEBUG)
        *jl_ewpds_log << "delta:\n"; delta->print(*jl_ewpds_log) << std::endl;        
        *jl_ewpds_log << "delta->extend(rweight):\n"; se->print(*jl_ewpds_log) << std::endl;
#endif

        // se := delta extend r->weight()
        // dold := told->getDelta()
        // Default implementation of extendAndCombineTransWeights: 
        //   < se + told, dold combine (se - told), !dold->equal(dold combine (se - told)) >
        std::pair<std::pair<sem_elem_t, sem_elem_t>, bool> res = 
                t->weight()->extendAndCombineTransWeights( delta, rweight, se_old, se_propagated, told->weight(), told->getDelta(), existold );

        // Delete _tnew if told is an existing trans from currentOutputWFA and told does not equal to _tnew
        if( existold && told != _tnew ) {
          delete _tnew;
        }

        sem_elem_t ans = res.first.first;
        sem_elem_t ans_delta = res.first.second;
        bool is_changed = res.second || !existold;

        // Set the resultant weight and delta to tnew
        wfa::ITrans* tnew;
        tnew = told;
        tnew->setWeight(ans);
        tnew->setDelta(ans_delta);

        if(addEtrans) {
          // For setting newWatCAll
          sem_elem_t call_difference = se_old->computeDifference(se_propagated, delta);
          sem_elem_t newWatCall = prevWatCall->combine(call_difference);
          ETrans* tmptnew = dynamic_cast<ETrans*>(tnew);
          tmptnew->setWeightAtCall(newWatCall);
        }


#if defined(JL_EWPDS_DEBUG)
        tnew->print(*jl_ewpds_log) << std::endl;
        *jl_ewpds_log << "ans:\n"; tnew->weight()->print(*jl_ewpds_log) << std::endl;
        *jl_ewpds_log << "ans_delta:\n"; tnew->getDelta()->print(*jl_ewpds_log) << std::endl;
#endif
       
        // tnew->status = (is_changed ? SAME : MODIFIED)
        if(is_changed)
          tnew->setModified();
        else
          tnew->setSame();

        tnew->setConfig(cfg);

        if (tnew->modified()) {
          //tnew->print(std::cout << "Adding transition: ") << "\n";
#if defined(JL_EWPDS_DEBUG)
          *jl_ewpds_log << "->"; tnew->print(*jl_ewpds_log) << std::endl;
#endif
          worklist->put( tnew );
        }
        
      }

      wfa::ITrans* EWPDS::update_prime(
          Key from, //<! Guaranteed to be a generated state
          wfa::ITrans* call, //<! The call transition
          rule_t r, //<! The push rule
          sem_elem_t delta, //<! Delta change on the call transition
          sem_elem_t _wWithRule //<! delta \extends r->weight()
          )
      {
        /*
        // !!NOTE!!
        // This code is copied in FWPDS::update_prime.
        // Changes here should be reflected there.
        //
        ERule* er = (ERule*)r.get_ptr();
        wfa::ITrans* tmp = 
          new ETrans(
              from, r->to_stack2(), call->to(),
              delta, wWithRule, er);
        wfa::ITrans* t = currentOutputWFA->insert(tmp);
        return t;
        */

        //sem_elem_t se = delta->extend(r->weight()); // dummy: tnew's weight will be set later in this function // delta; //

        sem_elem_t wWithRule = delta->extend(r->weight());

        ERule* er = (ERule*)r.get_ptr();
        wfa::ITrans* _tnew = new ETrans(from, r->to_stack2(), call->to(), delta, wWithRule, er);
        wfa::ITrans* told = currentOutputWFA->lookup_trans(_tnew);

        
        sem_elem_t prevWatCall; // previous weight at call

        bool existold;
        if(told == 0)
        {
          // Insert the new trans to currentOutputWFA
          told = currentOutputWFA->insert_trans(_tnew);
          existold = false;
          prevWatCall = delta->zero();
        }
        else {
          existold = true;
          ETrans* tmptnew = dynamic_cast<ETrans*>(_tnew);
          prevWatCall = tmptnew->getWeightAtCall();

#if defined(JL_EWPDS_DEBUG)
          told->print(*jl_ewpds_log) << std::endl;
          *jl_ewpds_log << "told:\n"; told->weight()->print(*jl_ewpds_log) << std::endl;
          *jl_ewpds_log << "dold:\n"; told->getDelta()->print(*jl_ewpds_log) << std::endl;
#endif
        }

        sem_elem_t se = call->weight();
        sem_elem_t se_old = call->getSeOld();
        sem_elem_t se_propagated = call->getSePropagated();

#if defined(JL_EWPDS_DEBUG)
        *jl_ewpds_log << "delta:\n"; delta->print(*jl_ewpds_log) << std::endl;
#endif

        // se := delta extend r->weight()
        // dold := told->getDelta()
        // Default implementation of extendAndCombineTransWeights: 
        //    < se + told, dold combine (se - told), !dold->equal(dold combine (se - told)) >
        std::pair<std::pair<sem_elem_t, sem_elem_t>, bool> res = 
              se->extendAndCombineTransWeights( delta, r->weight(), se_old, se_propagated, told->weight(), told->getDelta(), existold );

        // Delete _tnew if told is an existing trans from currentOutputWFA and told does not equal to _tnew
        if(existold && told != _tnew) {
          delete _tnew;
        }

        sem_elem_t ans = res.first.first;
        sem_elem_t ans_delta = res.first.second;
        bool is_changed = res.second || !existold;

        // Set the resultant weight and delta of tnew
        wfa::ITrans* tnew;
        tnew = told;
        tnew->setWeight(ans);
        tnew->setDelta(ans_delta);

        // For setting newWatCAll
        sem_elem_t call_difference = se_old->computeDifference(se_propagated, delta);
        sem_elem_t newWatCall = prevWatCall->combine(call_difference);
        ETrans* tmptnew = dynamic_cast<ETrans*>(tnew);
        tmptnew->setWeightAtCall(newWatCall);
        
#if defined(JL_EWPDS_DEBUG)
        tnew->print(*jl_ewpds_log) << std::endl;
        *jl_ewpds_log << "ans:\n"; tnew->weight()->print(*jl_ewpds_log) << std::endl;        
        *jl_ewpds_log << "ans_delta:\n"; tnew->getDelta()->print(*jl_ewpds_log) << std::endl;
#endif

        // tnew->status = (is_changed ? SAME : MODIFIED)
        if(is_changed)
          tnew->setModified();
        else
          tnew->setSame();

        return tnew;
      }

    } // namespace ewpds

  } // namespace wpds

} // namespace wali

