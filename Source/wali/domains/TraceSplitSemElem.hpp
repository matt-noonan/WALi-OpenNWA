#if 0 // this file currently doesn't compile and this is needed to prevent
      // the header-compilation tests from failing

#ifndef WALI_TRACESPLIT_SEMELEMSET_HPP
#define WALI_TRACESPLIT_SEMELEMSET_HPP

#include "wali/SemElem.hpp"

#include <boost/iterator/iterator_facade.hpp>
#include <boost/optional.hpp>
#include <tr1/unordered_map>
#include <tr1/unordered_set>
#include <limits>

#include <iostream>

namespace {
  std::string
  clamp(std::string const & str)
  {
    const size_t maxlen = 50;
    if (str.size() < maxlen) {
      return str;
    }
    else {
      return str.substr(0, maxlen-3) + "...";
    }
  }
}  

namespace wali
{
  namespace domains
  {

    class Guard
      : public wali::Countable
      , public wali::Printable
    {
    public:
      typedef ref_ptr<Guard> Ptr;

      virtual size_t hash() const;
      virtual bool equal(Ptr other) const;
    };

    /// Represents a domain from a key to a weight.
    ///
    /// The "key" acts a bit like a guard, except that keys that are
    /// not present act like a specific default value ("most of the
    /// time", this will be 0/bottom/concretizes to nothing).
    ///
    /// This class differs from KeyedSemElemSet in the semiring
    /// definition: here extend (resp. combine) does key-wise extend
    /// (combine) instead of an all-pairs extend (union). This also
    /// stores the default value.
    ///
    /// Thus, if I denote an element by <k,w> where k is the key and w the
    /// weight:
    ///
    ///   o  zero is the empty set {} with default 0
    ///   o  one is the empty set {} with default 1
    ///   o  combine does key-wise combine
    ///   o  extend does key-wise extend
    ///
    /// The key is treated as a sem_elem_t, but it really doesn't have to be
    /// one -- in particular, keys are never either extended or combined.
    class TraceSplitSemElemSet
      : public wali::SemElem
    {
    public:
      typedef std::tr1::unordered_map<sem_elem_t, sem_elem_t,
                                      SemElemRefPtrHash, SemElemRefPtrEqual>
              BackingMap;

      typedef BackingMap::const_iterator const_iterator;

      typedef boost::function<void (sem_elem_t &, sem_elem_t &)> ReduceFunction;

      static
      void
      identity_reducer(sem_elem_t & UNUSED_PARAMETER(key),
                       sem_elem_t & UNUSED_PARAMETER(value))
      {
      }


      static
      size_t
      get_hash(BackingMap const & m) {
        size_t xor_part = 0u;
        size_t sum_part = 0u;

        for (BackingMap::const_iterator it = m.begin();
             it != m.end(); ++it)
        {
          size_t elem_hash = it->first->hash() + 17*it->second->hash();
          xor_part ^= elem_hash;
          sum_part += elem_hash;
        }
        return xor_part + 17*sum_part;
      }


      static
      BackingMap
      remove_zeroes(ReduceFunction reducer, BackingMap m)
      {
        for (BackingMap::const_iterator it = m.begin();
             it != m.end(); /*nothing*/ )
        {
          sem_elem_t guard = it->first;
          sem_elem_t weight = it->second;

          reducer(guard, weight);

          if (guard->equal(guard->zero())
              || weight->equal(weight->zero()))
          {
            it = m.erase(it);
          }
          else {
            ++it;
          }
        }
        return m;
      }
      

      TraceSplitSemElemSet(Reducefunction reducer,
                           BackingMap const & m,
                           sem_elem_t default_value)
        : reducer_(reducer)
        , map_(remove_zeroes(reducer, m))
        , default_(default_value)
        , hash_(get_hash(map_))
      {}

      std::pair<const_iterator, const_iterator>
      equal_range(sem_elem_t key) const {
        return map_.equal_range(key);
      }

      const_iterator
      begin() const {
        return map_.begin();
      }

      const_iterator
      end() const {
        return map_.end();
      }

      size_t size() const {
        return map_.size();
      }
      
      sem_elem_t one() const {
        BackingMap m;
        return new TraceSplitSemElemSet(reducer_, m, default_->one());
      }
      
      sem_elem_t zero() const {
        BackingMap m;
        return new TraceSplitSemElemSet(reducer_, m, default_->zero());
      }

      sem_elem_t extend(SemElem * se) {
        TraceSplitSemElemSet * that = dynamic_cast<TraceSplitSemElemSet*>(se);
        assert(that);

        BackingMap m;

        for (BackingMap::const_iterator this_guard = this->map_.begin();
             this_guard != this->map_.end(); ++this_guard)
        {
          
        }
        
        return new TraceSplitSemElemSet(m, one_key_, one_value_);
      }
      
      sem_elem_t combine(SemElem * se) {
        TraceSplitSemElemSet * that = dynamic_cast<TraceSplitSemElemSet*>(se);
        assert(that);

        BackingMap m(this->map_);
        for (BackingMap::const_iterator that_guard = that->map_.begin();
             that_guard != that->map_.end(); ++that_guard)
        {
          BackingMap::iterator new_loc = m.find(that_guard->first);

          if (new_loc == m.end()) {
            m.insert(new_loc, *that_guard);
          }
          else {
            sem_elem_t new_weight = new_loc->second->combine(that_guard->second);
            new_loc->second = new_weight;
          }
        }

        return new TraceSplitSemElemSet(m, one_key_, one_value_);
      }

      bool equal(SemElem * se) const {
        TraceSplitSemElemSet * that = dynamic_cast<TraceSplitSemElemSet*>(se);
        assert(that);

        if (this->map_.size() != that->map_.size()
            || !this->one_key_->equal(that->one_key_)
            || !this->one_value_->equal(that->one_value_))
        {
          return false;
        }

        for (BackingMap::const_iterator this_guard = this->map_.begin();
             this_guard != this->map_.end(); ++this_guard)
        {
          BackingMap::const_iterator that_loc = that->map_.find(this_guard->first);

          if (that_loc == that->map_.end()
              || !that_loc->second->equal(this_guard->second))
          {
            return false;
          }  
        }

        return true;
      }

      std::ostream& print( std::ostream & o ) const {
        o << "{ ";
        bool first = true;
        for (BackingMap::const_iterator element = this->map_.begin();
             element != this->map_.end(); ++element)
        {
          if (!first) {
            o << ", ";
          }
          first = false;          
          element->first->print(o) << ": ";
          element->second->print(o);
        }
        o << "}";
        return o;
      }


      virtual size_t hash() const { return hash_; }


      // Pull in the sem_elem_t overloads
      using SemElem::extend;
      using SemElem::combine;
      using SemElem::equal;

    private:      
      ReduceFunction reducer_;
      BackingMap map_;
      sem_elem_t one_key_, one_value_;
      size_t hash_;
    };
    
  }
}


// Yo emacs!
// Local Variables:
//     c-file-style: "ellemtel"
//     c-basic-offset: 2
//     indent-tabs-mode: nil
// End:

#endif

#endif