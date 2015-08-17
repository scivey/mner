#include "gtest/gtest.h"
#include <vector>
#include <string>

#include "MitieNerProcessor.h"

using scivey::mner::tokenizeString;

TEST(TokenizeString, BasicTest) {
    string toSplit = "this is a test";
    auto tokens = tokenizeString(toSplit);
    EXPECT_EQ(4, tokens.size());
    EXPECT_EQ("this", tokens.at(0));
    EXPECT_EQ("is", tokens.at(1));
    EXPECT_EQ("a", tokens.at(2));
    EXPECT_EQ("test", tokens.at(3));
}

