#include "StringUtils.h"
#include <gtest/gtest.h>

using std::string;

TEST(StringUtilsTest, SliceTest) {
  string slice_test = "slicedstring";

  EXPECT_EQ(StringUtils::Slice(slice_test, 0, 4), "slic");
  EXPECT_EQ(StringUtils::Slice(slice_test, 4, 9), "edstr");
  EXPECT_EQ(StringUtils::Slice(slice_test, 6), "string");
  EXPECT_EQ(StringUtils::Slice(slice_test, -6), "string");
}

TEST(StringUtilsTest, Capitalize) {
  EXPECT_EQ(StringUtils::Capitalize("lower"), "Lower");
  EXPECT_EQ(StringUtils::Capitalize("Lower"), "Lower");
  EXPECT_EQ(StringUtils::Capitalize("uPPEr"), "UPPEr");
}

TEST(StringUtilsTest, Upper) {
  EXPECT_EQ(StringUtils::Upper("Lower"), "LOWER");
  EXPECT_EQ(StringUtils::Upper("UPPER"), "UPPER");
  EXPECT_EQ(StringUtils::Upper("lower"), "LOWER");
}

TEST(StringUtilsTest, Lower) {
  EXPECT_EQ(StringUtils::Lower("lower"), "lower");
  EXPECT_EQ(StringUtils::Lower("Lower"), "lower");
  EXPECT_EQ(StringUtils::Lower("UPPER"), "upper");
}

TEST(StringUtilsTest, LStrip) {
  EXPECT_EQ(StringUtils::LStrip("  lstrip"), "lstrip");
  EXPECT_EQ(StringUtils::LStrip("  lstrip  "), "lstrip  ");
  EXPECT_EQ(StringUtils::LStrip("lstrip  "), "lstrip  ");
}

TEST(StringUtilsTest, RStrip) {
  EXPECT_EQ(StringUtils::RStrip("  rstrip"), "  rstrip");
  EXPECT_EQ(StringUtils::RStrip("  rstrip  "), "  rstrip");
  EXPECT_EQ(StringUtils::RStrip("rstrip  "), "rstrip");
}

TEST(StringUtilsTest, Strip) {
  EXPECT_EQ(StringUtils::Strip("  strip"), "strip");
  EXPECT_EQ(StringUtils::Strip("  strip  "), "strip");
  EXPECT_EQ(StringUtils::Strip("strip  "), "strip");
}

TEST(StringUtilsTest, Center) {
  EXPECT_EQ(StringUtils::Center("example", 15), "    example    ");
  EXPECT_EQ(StringUtils::Center("example", 15, '+'), "++++example++++");
}

TEST(StringUtilsTest, LJust) {
  EXPECT_EQ(StringUtils::LJust("example", 11), "example    ");
  EXPECT_EQ(StringUtils::LJust("example", 11, '+'), "example++++");
}

TEST(StringUtilsTest, RJust) {
  EXPECT_EQ(StringUtils::RJust("example", 11), "    example");
  EXPECT_EQ(StringUtils::RJust("example", 11, '+'), "++++example");
}

TEST(StringUtilsTest, Replace) {
  EXPECT_EQ(StringUtils::Replace("oldreplace", "old", "new"), "newreplace");
  EXPECT_EQ(StringUtils::Replace("oldreplace", "new", "old"), "oldreplace");
}

TEST(StringUtilsTest, Split) {
  EXPECT_EQ(StringUtils::Split("one two"),
            std::vector({string("one"), string("two")}));
  EXPECT_EQ(StringUtils::Split("one,two", ","),
            std::vector({string("one"), string("two")}));
  EXPECT_EQ(StringUtils::Split("one,,two", ",,"),
            std::vector({string("one"), string("two")}));
  EXPECT_EQ(StringUtils::Split("one two", ","),
            std::vector({string("one two")}));
}

TEST(StringUtilsTest, Join) {
  EXPECT_EQ(StringUtils::Join(" ", std::vector({string("one"), string("two")})),
            "one two");
  EXPECT_EQ(StringUtils::Join(",", std::vector({string("one"), string("two")})),
            "one,two");
}

TEST(StringUtilsTest, ExpandTabs) {
  EXPECT_EQ(StringUtils::ExpandTabs("expand\ttabs"), "expand    tabs");
  EXPECT_EQ(StringUtils::ExpandTabs("expand\ttabs", 2), "expand  tabs");
  EXPECT_EQ(StringUtils::ExpandTabs("expandtabs"), "expandtabs");
}

TEST(StringUtilsTest, EditDistance) {
  EXPECT_EQ(StringUtils::EditDistance("DIST", "DIST"), 0);
  EXPECT_EQ(StringUtils::EditDistance("DIST", "DEST"), 1);
  EXPECT_EQ(StringUtils::EditDistance("DIST", "DEXT"), 2);
  EXPECT_EQ(StringUtils::EditDistance("DIST", "dext", true), 2);
}
