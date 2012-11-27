#if 0
#include "gtest/gtest.h"

#include "wali/domains/SemElemSet.hpp"
#include "wali/ShortestPathSemiring.hpp"

namespace wali {
    namespace domains {


        TEST(wali$domains$SemElemSet$$SemElemSet, oneArgConstructorMakesZero)
        {
            sem_elem_t ten = new ShortestPathSemiring(10);
            SemElemSet s(ten);
            EXPECT_EQ(0u, s.getElements().size());
        }        

        
        TEST(wali$domains$SemElemSet$$SemElemSet, twoArgConstructorRemembersElements)
        {
            sem_elem_t ten = new ShortestPathSemiring(10);
            SemElemSet::ElementSet es;
            es.push_back(ten);
            
            SemElemSet s(ten, es);
            EXPECT_EQ(es, s.getElements());
        }    

        
        TEST(wali$domains$SemElemSet$$SemElemSet, twoArgConstructorRemovesDuplicates)
        {
            sem_elem_t ten = new ShortestPathSemiring(10);
            SemElemSet::ElementSet es;
            es.push_back(ten);
            es.push_back(ten);
            
            SemElemSet s(ten, es);
            ASSERT_EQ(1u, s.getElements().size());
            EXPECT_EQ(ten, s.getElements()[0]);
        }    

        
        TEST(wali$domains$SemElemSet$$SemElemSet, twoArgConstructorRemovesDuplicatesEvenIfTheyAreSeparateObjects)
        {
            sem_elem_t ten1 = new ShortestPathSemiring(10);
            sem_elem_t ten2 = new ShortestPathSemiring(10);
            SemElemSet::ElementSet es;
            es.push_back(ten1);
            es.push_back(ten2);
            
            SemElemSet s(ten1, es);
            EXPECT_EQ(1u, s.getElements().size());
            EXPECT_EQ(ten1, s.getElements()[0]); // actually this test is stricter than I want, but whatever
        }    

        
        TEST(wali$domains$SemElemSet$$SemElemSet, twoArgConstructorDoesNotRemoveNonduplicates)
        {
            sem_elem_t ten = new ShortestPathSemiring(10);
            sem_elem_t twenty = new ShortestPathSemiring(20);
            SemElemSet::ElementSet es;
            es.push_back(ten);
            es.push_back(twenty);
            
            SemElemSet s(ten, es);
            EXPECT_EQ(es, s.getElements());
        }    

        
        TEST(wali$domains$SemElemSet$$zero, zeroProducesZero)
        {
            sem_elem_t ten = new ShortestPathSemiring(10);
            SemElemSet::ElementSet es;
            es.push_back(ten);
            SemElemSet s(ten, es);

            sem_elem_t zero_se = s.zero();
            SemElemSet * zero_down = dynamic_cast<SemElemSet *>(zero_se.get_ptr());
            
            EXPECT_EQ(0u, zero_down->getElements().size());
        }

        
        TEST(wali$domains$SemElemSet$$one, oneProducesOne)
        {
            sem_elem_t ten = new ShortestPathSemiring(10);
            SemElemSet::ElementSet es;
            es.push_back(ten);
            SemElemSet s(ten, es);

            sem_elem_t one_se = s.one();
            SemElemSet * one_down = dynamic_cast<SemElemSet *>(one_se.get_ptr());
            
            EXPECT_EQ(1u, one_down->getElements().size());
            EXPECT_TRUE(one_down->getElements()[0]->equal(ten->one()));
        }


        TEST(wali$domains$SemElemSet$$various, waliSemElemTests)
        {
            sem_elem_t ten = new ShortestPathSemiring(10);
            SemElemSet::ElementSet es;
            es.push_back(ten);
            
            sem_elem_t interesting_value = new SemElemSet(ten, es);
            test_semelem_impl(interesting_value);
        }

        
        TEST(wali$domains$SemElemSet$$combine, combineTakesUnion)
        {
            sem_elem_t ten = new ShortestPathSemiring(10);
            sem_elem_t twenty = new ShortestPathSemiring(20);
            sem_elem_t thirty = new ShortestPathSemiring(30);
            SemElemSet::ElementSet es1, es2, all;
            es1.push_back(ten);
            es1.push_back(twenty);
            es2.push_back(thirty);
            all.push_back(ten);
            all.push_back(twenty);
            all.push_back(thirty);
            
            SemElemSet s1(ten, es1);
            SemElemSet s2(thirty, es2);

            sem_elem_t answer = s1.combine(&s2);
            SemElemSet * answer_down = dynamic_cast<SemElemSet *>(answer.get_ptr());
            
            ASSERT_EQ(3u, answer_down->getElements().size());
            EXPECT_EQ(all, answer_down->getElements()); // too strict: enforces order
        }


        TEST(wali$domains$SemElemSet$$combine, combineWillNotDuplicateAnElementEvenIfTheyAreSeparateObjects)
        {
            sem_elem_t ten1 = new ShortestPathSemiring(10);
            sem_elem_t ten2 = new ShortestPathSemiring(10);
            sem_elem_t thirty = new ShortestPathSemiring(30);
            SemElemSet::ElementSet es1, es2, all;
            es1.push_back(ten1);
            es2.push_back(thirty);
            es2.push_back(ten2);
            
            SemElemSet s1(ten1, es1);
            SemElemSet s2(thirty, es2);

            sem_elem_t answer = s1.combine(&s2);
            SemElemSet * answer_down = dynamic_cast<SemElemSet *>(answer.get_ptr());
            
            ASSERT_EQ(2u, answer_down->getElements().size());
            EXPECT_EQ(ten1, answer_down->getElements()[0]); // to strict: enforces order
            EXPECT_EQ(thirty, answer_down->getElements()[1]);
        }

        
        TEST(wali$domains$SemElemSet$$extend, extendTakesPairwiseExtend)
        {
            sem_elem_t
                one        = new ShortestPathSemiring( 1),
                two        = new ShortestPathSemiring( 2),
                ten        = new ShortestPathSemiring(10),
                eleven     = new ShortestPathSemiring(11),
                twelve     = new ShortestPathSemiring(12),
                twenty     = new ShortestPathSemiring(20),
                twenty_one = new ShortestPathSemiring(21),
                twenty_two = new ShortestPathSemiring(22);
            
            SemElemSet::ElementSet es1, es2;
            es1.push_back(one);
            es1.push_back(two);
            es2.push_back(ten);
            es2.push_back(twenty);
            
            SemElemSet s1(one, es1);
            SemElemSet s2(ten, es2);

            sem_elem_t answer = s1.extend(&s2);
            SemElemSet * answer_down = dynamic_cast<SemElemSet *>(answer.get_ptr());
            
            ASSERT_EQ(4u, answer_down->getElements().size());
            EXPECT_TRUE(eleven->equal(answer_down->getElements()[0])); // too strict: enforces order
            EXPECT_TRUE(twenty_one->equal(answer_down->getElements()[1]));            
            EXPECT_TRUE(twelve->equal(answer_down->getElements()[2]));
            EXPECT_TRUE(twenty_two->equal(answer_down->getElements()[3]));
        }


        TEST(wali$domains$SemElemSet$$extend, extendDoesNotDuplicateElementsEvenIfTheyAreSeparateObjects)
        {
            sem_elem_t
                ten    = new ShortestPathSemiring(10),
                twenty = new ShortestPathSemiring(20),
                thirty = new ShortestPathSemiring(30),
                fourty = new ShortestPathSemiring(40),
                fifty  = new ShortestPathSemiring(50),
                sixty  = new ShortestPathSemiring(60);
            
            SemElemSet::ElementSet es1, es2;
            es1.push_back(ten);
            es1.push_back(twenty);
            es2.push_back(thirty);
            es2.push_back(fourty);
            
            SemElemSet s1(ten, es1);
            SemElemSet s2(thirty, es2);

            sem_elem_t answer = s1.extend(&s2);
            SemElemSet * answer_down = dynamic_cast<SemElemSet *>(answer.get_ptr());
            
            ASSERT_EQ(3u, answer_down->getElements().size());
            EXPECT_TRUE(fourty->equal(answer_down->getElements()[0]));
            EXPECT_TRUE(fifty->equal(answer_down->getElements()[1]));
            EXPECT_TRUE(sixty->equal(answer_down->getElements()[2]));
        }


        TEST(wali$domains$SemElemSet$$equal, differentSetsAreUnequal)
        {
            sem_elem_t
                ten    = new ShortestPathSemiring(10),
                twenty = new ShortestPathSemiring(20),
                thirty = new ShortestPathSemiring(30),
                fourty = new ShortestPathSemiring(40);
            
            SemElemSet::ElementSet es1, es2;
            es1.push_back(ten);
            es1.push_back(twenty);
            es2.push_back(thirty);
            es2.push_back(fourty);
            
            SemElemSet s1(ten, es1);
            SemElemSet s2(thirty, es2);

            EXPECT_FALSE(s1.equal(&s2));
        }

        
        TEST(wali$domains$SemElemSet$$equal, equalityIsOrderIndependent)
        {
            sem_elem_t
                ten    = new ShortestPathSemiring(10),
                twenty = new ShortestPathSemiring(20);
            
            SemElemSet::ElementSet es1, es2;
            es1.push_back(ten);
            es1.push_back(twenty);
            es2.push_back(twenty);
            es2.push_back(ten);
            
            SemElemSet s1(ten, es1);
            SemElemSet s2(ten, es2);

            EXPECT_TRUE(s1.equal(&s2));
        }
        

        TEST(wali$domains$SemElemSet$$equal, equalElementsAreEqualEvenIfTheyAreSeparateObjects)
        {
            sem_elem_t
                ten1 = new ShortestPathSemiring(10),
                ten2 = new ShortestPathSemiring(10);
            
            SemElemSet::ElementSet es1, es2;
            es1.push_back(ten1);
            es2.push_back(ten2);
            
            SemElemSet s1(ten1, es1);
            SemElemSet s2(ten2, es2);

            EXPECT_TRUE(s1.equal(&s2));
        }
        
    }
}
#endif
