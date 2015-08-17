#include "gtest/gtest.h"
#include <vector>
#include <string>

#include "MitieTokenizer.h"

using namespace std;
using scivey::mner::tokenizeStringFn;
using scivey::mner::MitieTokenizer;

TEST(TokenizeStringFn, BasicTest) {
    string toSplit = "this is a test";
    auto tokens = tokenizeStringFn(toSplit);
    EXPECT_EQ(4, tokens.size());
    EXPECT_EQ("this", tokens.at(0));
    EXPECT_EQ("is", tokens.at(1));
    EXPECT_EQ("a", tokens.at(2));
    EXPECT_EQ("test", tokens.at(3));
}

TEST(MitieTokenizer, UsesTokenizeStringFn) {
    string toSplit = "this is a test";
    MitieTokenizer tokenizer;
    auto tokens = tokenizer.tokenizeString(toSplit);
    EXPECT_EQ(4, tokens.size());
    EXPECT_EQ("this", tokens.at(0));
    EXPECT_EQ("is", tokens.at(1));
    EXPECT_EQ("a", tokens.at(2));
    EXPECT_EQ("test", tokens.at(3));
}
