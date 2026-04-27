#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <random>

#include "hashmap.h"

using namespace std;
using namespace testing;

namespace {

class Random {
 private:
  static mt19937 rng;

 public:
  static void seed(int s) {
    Random::rng.seed(s);
  }

  /**
   * Generate a random integer in the range 0 (inclusive) to `max` (exclusive)
   */
  static int randInt(int max) {
    // Even though mt19937 is standardized, STL distributions aren't!
    // So, unfortunately, even though this is biased, we're forced to
    // do something like this. Technically uint32_fast_t isn't...
    // totally consistent across platforms? But within reason it works.
    return rng() % (max + 1);
  }
};

std::mt19937 Random::rng;

TEST(HashMapConstructorTest, DefaultConstructorStartsEmpty) {
  HashMap<string, int> hm;

  EXPECT_TRUE(hm.empty());
  EXPECT_EQ(hm.size(), 0);
  EXPECT_EQ(hm.get_capacity(), 10);
  EXPECT_NE(hm.get_data(), nullptr);
}

TEST(HashMapConstructorTest, CapacityConstructorUsesRequestedCapacity) {
  HashMap<string, int> hm(24);

  EXPECT_TRUE(hm.empty());
  EXPECT_EQ(hm.size(), 0);
  EXPECT_EQ(hm.get_capacity(), 24);
  EXPECT_NE(hm.get_data(), nullptr);
}

TEST(HashMapConstructorTest, DifferentCapacityValuesStillStartEmpty) {
  HashMap<int, string> hm1(2);
  HashMap<int, string> hm2(11);
  HashMap<int, string> hm3(77);

  EXPECT_EQ(hm1.get_capacity(), 2);
  EXPECT_EQ(hm2.get_capacity(), 11);
  EXPECT_EQ(hm3.get_capacity(), 77);

  EXPECT_TRUE(hm1.empty());
  EXPECT_TRUE(hm2.empty());
  EXPECT_TRUE(hm3.empty());

  EXPECT_EQ(hm1.size(), 0);
  EXPECT_EQ(hm2.size(), 0);
  EXPECT_EQ(hm3.size(), 0);
}

// ============================================================
// General contains / insert / at / clear / erase tests
// ============================================================

TEST(HashMapContainsTest, EmptyMapDoesNotContainGivenKey) {
  HashMap<string, int> hm(10);

  EXPECT_FALSE(hm.contains("sky"));
}

TEST(HashMapContainsTest, MissingKeyStillReturnsFalse) {
  HashMap<string, int> hm(10);
  hm.insert("lebron", 24);

  EXPECT_FALSE(hm.contains("headphones"));
}

TEST(HashMapInsertTest, InsertingOneMappingChangesSize) {
  HashMap<string, int> hm(10);

  hm.insert("lebron", 24);

  EXPECT_EQ(hm.size(), 1);
  EXPECT_FALSE(hm.empty());
  EXPECT_TRUE(hm.contains("lebron"));
}

TEST(HashMapInsertTest, DuplicateInsertDoesNotIncreaseSize) {
  HashMap<string, int> hm(10);

  hm.insert("lebron", 24);
  hm.insert("lebron", 77);

  EXPECT_EQ(hm.size(), 1);
}

TEST(HashMapInsertTest, ResizeHappensAfterLoadFactorExceedsOnePointFive) {
  HashMap<int, int> hm(2);

  hm.insert(2, 11);
  hm.insert(4, 24);
  hm.insert(11, 77);
  hm.insert(24, 2);

  EXPECT_EQ(hm.get_capacity(), 4);
}

TEST(HashMapAtTest, AtReturnsStoredValueForExistingKey) {
  HashMap<string, int> hm(10);

  hm.insert("gentry", 11);

  EXPECT_EQ(hm.at("gentry"), 11);
}

TEST(HashMapAtTest, AtThrowsOutOfRangeForMissingKey) {
  HashMap<string, int> hm(10);

  EXPECT_THROW(hm.at("fallen"), out_of_range);
}

TEST(HashMapAtTest, AtKeepsOriginalValueAfterDuplicateInsertIsIgnored) {
  HashMap<string, int> hm(10);

  hm.insert("sky", 4);
  hm.insert("sky", 77);

  EXPECT_EQ(hm.at("sky"), 4);
}

TEST(HashMapClearTest, ClearKeepsEmptyMapEmpty) {
  HashMap<string, int> hm(10);

  hm.clear();

  EXPECT_TRUE(hm.empty());
  EXPECT_EQ(hm.size(), 0);
}

TEST(HashMapClearTest, ClearRemovesSingleMapping) {
  HashMap<string, int> hm(10);

  hm.insert("lebron", 24);
  hm.clear();

  EXPECT_TRUE(hm.empty());
  EXPECT_EQ(hm.size(), 0);
  EXPECT_FALSE(hm.contains("lebron"));
}

TEST(HashMapClearTest, ClearAllowsMapToBeUsedAgain) {
  HashMap<string, int> hm(10);

  hm.insert("fallen", 11);
  hm.insert("gentry", 77);

  hm.clear();
  hm.insert("headphones", 24);

  EXPECT_FALSE(hm.empty());
  EXPECT_EQ(hm.size(), 1);
  EXPECT_TRUE(hm.contains("headphones"));
  EXPECT_EQ(hm.at("headphones"), 24);
}

TEST(HashMapClearTest, ClearDoesNotChangeCapacity) {
  HashMap<string, int> hm(24);

  hm.insert("a", 1);
  hm.insert("b", 2);

  hm.clear();

  EXPECT_EQ(hm.get_capacity(), 24);
}

TEST(HashMapDestructorTest, UsedMapCanGoOutOfScopeWithoutCrashing) {
  {
    HashMap<string, int> hm(10);

    hm.insert("lebron", 24);
    hm.insert("fallen", 11);
    hm.insert("gentry", 77);
  }

  SUCCEED();
}

TEST(HashMapEraseTest, EraseReturnsRemovedValueForExistingKey) {
  HashMap<string, int> hm(10);

  hm.insert("sky", 4);

  EXPECT_EQ(hm.erase("sky"), 4);
  EXPECT_TRUE(hm.empty());
  EXPECT_EQ(hm.size(), 0);
}

TEST(HashMapEraseTest, EraseRemovesRequestedKeyOnly) {
  HashMap<string, int> hm(10);

  hm.insert("lebron", 24);
  hm.insert("fallen", 11);

  hm.erase("lebron");

  EXPECT_FALSE(hm.contains("lebron"));
  EXPECT_TRUE(hm.contains("fallen"));
  EXPECT_EQ(hm.size(), 1);
}

// ============================================================
// Core tests
// Keep the HashMapCore* prefix for the autograder filter.
// ============================================================

TEST(HashMapCoreCtor, DefaultConstructorStartsWithSizeZero) {
  HashMap<string, int> hm;

  EXPECT_EQ(hm.size(), 0);
  EXPECT_TRUE(hm.empty());
  EXPECT_EQ(hm.get_capacity(), 10);
}

TEST(HashMapCoreCtor, CapacityConstructorStartsWithSizeZero) {
  HashMap<string, int> hm(24);

  EXPECT_EQ(hm.size(), 0);
  EXPECT_TRUE(hm.empty());
  EXPECT_EQ(hm.get_capacity(), 24);
}

TEST(HashMapCoreEmpty, EmptyChangesAfterInsertAndClear) {
  HashMap<string, int> hm(10);

  EXPECT_TRUE(hm.empty());

  hm.insert("lebron", 24);
  EXPECT_FALSE(hm.empty());

  hm.clear();
  EXPECT_TRUE(hm.empty());
}

TEST(HashMapCoreInsert, DuplicateInsertKeepsOriginalValue) {
  HashMap<string, int> hm(10);

  hm.insert("lebron", 24);
  hm.insert("lebron", 77);

  EXPECT_EQ(hm.size(), 1);
  EXPECT_EQ(hm.at("lebron"), 24);
}

TEST(HashMapCoreAt, AtThrowsWhenRequestedKeyIsMissing) {
  HashMap<string, int> hm(10);

  EXPECT_THROW(hm.at("headphones"), out_of_range);
}

TEST(HashMapCoreAt, AtReturnsCorrectValueWhenKeysCollide) {
  HashMap<string, int> hm(1);

  hm.insert("lebron", 24);
  hm.insert("fallen", 11);
  hm.insert("gentry", 77);

  EXPECT_EQ(hm.at("lebron"), 24);
  EXPECT_EQ(hm.at("fallen"), 11);
  EXPECT_EQ(hm.at("gentry"), 77);
}

TEST(HashMapCoreCopy, CopyConstructorCopiesAllMappings) {
  HashMap<string, int> hm(10);

  hm.insert("lebron", 24);
  hm.insert("headphones", 77);

  HashMap<string, int> copy(hm);

  EXPECT_FALSE(copy.empty());
  EXPECT_EQ(copy.size(), 2);
  EXPECT_TRUE(copy.contains("lebron"));
  EXPECT_TRUE(copy.contains("headphones"));
  EXPECT_EQ(copy.at("lebron"), 24);
  EXPECT_EQ(copy.at("headphones"), 77);
}

TEST(HashMapCoreCopy, CopyConstructorCreatesIndependentMap) {
  HashMap<string, int> hm(1);

  hm.insert("fallen", 11);
  hm.insert("sky", 4);

  HashMap<string, int> copy(hm);
  hm.clear();

  EXPECT_TRUE(hm.empty());

  EXPECT_EQ(copy.size(), 2);
  EXPECT_TRUE(copy.contains("fallen"));
  EXPECT_TRUE(copy.contains("sky"));
}

TEST(HashMapCoreAssign, AssignmentCopiesContentsIntoExistingMap) {
  HashMap<string, int> hm1(1);

  hm1.insert("lebron", 24);
  hm1.insert("fallen", 11);

  HashMap<string, int> hm2(24);

  hm2.insert("old", 77);
  hm2 = hm1;

  EXPECT_EQ(hm2.size(), 2);
  EXPECT_EQ(hm2.get_capacity(), hm1.get_capacity());

  EXPECT_TRUE(hm2.contains("lebron"));
  EXPECT_TRUE(hm2.contains("fallen"));
  EXPECT_FALSE(hm2.contains("old"));

  EXPECT_EQ(hm2.at("lebron"), 24);
  EXPECT_EQ(hm2.at("fallen"), 11);
}

TEST(HashMapCoreAssign, AssignmentCreatesIndependentCopy) {
  HashMap<string, int> hm1(1);

  hm1.insert("lebron", 24);
  hm1.insert("fallen", 11);

  HashMap<string, int> hm2(10);
  hm2 = hm1;

  hm1.erase("lebron");

  EXPECT_FALSE(hm1.contains("lebron"));
  EXPECT_TRUE(hm2.contains("lebron"));
  EXPECT_EQ(hm2.at("lebron"), 24);
  EXPECT_EQ(hm2.at("fallen"), 11);
}

TEST(HashMapCoreAssign, AssignmentHandlesSelfAssignmentSafely) {
  HashMap<string, int> hm(1);

  hm.insert("lebron", 24);
  hm.insert("fallen", 11);

  HashMap<string, int>& self = hm;
  hm = self;

  EXPECT_EQ(hm.size(), 2);
  EXPECT_TRUE(hm.contains("lebron"));
  EXPECT_TRUE(hm.contains("fallen"));
  EXPECT_EQ(hm.at("lebron"), 24);
  EXPECT_EQ(hm.at("fallen"), 11);
}

TEST(HashMapCoreErase, EraseThrowsWhenRequestedKeyIsMissing) {
  HashMap<string, int> hm(10);

  hm.insert("lebron", 24);

  EXPECT_THROW(hm.erase("gentry"), out_of_range);
}

TEST(HashMapCoreErase, EraseRemovesOnlyRequestedKeyWhenKeysCollide) {
  HashMap<string, int> hm(1);  // force collisions into one bucket

  hm.insert("lebron", 24);
  hm.insert("fallen", 11);
  hm.insert("sky", 4);

  EXPECT_EQ(hm.erase("fallen"), 11);

  EXPECT_EQ(hm.size(), 2);
  EXPECT_TRUE(hm.contains("lebron"));
  EXPECT_FALSE(hm.contains("fallen"));
  EXPECT_TRUE(hm.contains("sky"));
  EXPECT_EQ(hm.at("lebron"), 24);
  EXPECT_EQ(hm.at("sky"), 4);
}

// ============================================================
// Augmented tests
// Keep the HashMapAugmented* prefix for the autograder filter.
// ============================================================

TEST(HashMapAugmentedEqual, EqualMapsCompareTrueWithDifferentBucketCounts) {
  HashMap<string, int> hm1(10);
  HashMap<string, int> hm2(24);

  hm1.insert("lebron", 24);
  hm1.insert("fallen", 11);

  hm2.insert("fallen", 11);
  hm2.insert("lebron", 24);

  EXPECT_TRUE(hm1 == hm2);
  EXPECT_TRUE(hm2 == hm1);
}

TEST(HashMapAugmentedEqual, DifferentSizesCompareFalse) {
  HashMap<string, int> hm1(10);
  HashMap<string, int> hm2(10);

  hm1.insert("lebron", 24);

  EXPECT_FALSE(hm1 == hm2);
  EXPECT_FALSE(hm2 == hm1);
}

TEST(HashMapAugmentedEqual, DifferentValuesCompareFalse) {
  HashMap<string, int> hm1(10);
  HashMap<string, int> hm2(10);

  hm1.insert("lebron", 24);
  hm2.insert("lebron", 77);

  EXPECT_FALSE(hm1 == hm2);
  EXPECT_FALSE(hm2 == hm1);
}

TEST(HashMapAugmentedEqual, DifferentKeysCompareFalse) {
  HashMap<string, int> hm1(10);
  HashMap<string, int> hm2(10);

  hm1.insert("lebron", 24);
  hm2.insert("gentry", 24);

  EXPECT_FALSE(hm1 == hm2);
  EXPECT_FALSE(hm2 == hm1);
}

TEST(HashMapAugmentedIter, BeginThenNextReturnsFalseForEmptyMap) {
  HashMap<string, int> hm(10);

  hm.begin();

  string key;
  int value = 0;

  EXPECT_FALSE(hm.next(key, value));
}

TEST(HashMapAugmentedIter, BeginThenNextVisitsSingleStoredMapping) {
  HashMap<string, int> hm(10);

  hm.insert("headphones", 77);
  hm.begin();

  string key;
  int value = 0;

  EXPECT_TRUE(hm.next(key, value));
  EXPECT_EQ(key, "headphones");
  EXPECT_EQ(value, 77);

  EXPECT_FALSE(hm.next(key, value));
}

TEST(HashMapAugmentedIter, BeginThenNextEventuallyVisitsEveryStoredMapping) {
  HashMap<string, int> hm(1);

  hm.insert("lebron", 24);
  hm.insert("fallen", 11);
  hm.insert("gentry", 77);

  hm.begin();

  string key;
  int value = 0;

  int count = 0;
  bool saw_lebron = false;
  bool saw_fallen = false;
  bool saw_gentry = false;

  while (hm.next(key, value)) {
    count++;

    if (key == "lebron" && value == 24) {
      saw_lebron = true;
    }

    if (key == "fallen" && value == 11) {
      saw_fallen = true;
    }

    if (key == "gentry" && value == 77) {
      saw_gentry = true;
    }
  }

  EXPECT_EQ(count, 3);
  EXPECT_TRUE(saw_lebron);
  EXPECT_TRUE(saw_fallen);
  EXPECT_TRUE(saw_gentry);
}

}  // namespace
