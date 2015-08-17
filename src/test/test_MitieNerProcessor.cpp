#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "test/TestHelpers.h"
#include <glog/logging.h>

#include <vector>
#include <tuple>
#include <memory>
#include <string>

#include "gen-cpp2/NerTagger_types.h"

#include "MitieNerProcessor.h"
#include "MitieEntityExtractor.h"
#include "MitieTokenizer.h"

using namespace std;
using namespace scivey::mner;
using scivey::mner::services::EntityType;
using ::testing::Return;
using ::testing::_;
using TagResult = tuple<EntityType, string, double>;

class MockTokenizer: public MitieTokenizer {
public:
    MOCK_METHOD1(tokenizeString, vector<string>(const string &text));
};

class MockExtractor: public MitieEntityExtractor {
public:
    MockExtractor(const string &modelPath){
    }
    MOCK_METHOD1(extract, vector<TagResult>(const vector<string> &tokens));
};

TEST(MitieNerProcessor, BasicTest) {
    string toProcess {
        "Barack Obama is the president of the United States of America."
    };
    vector<string> tokens = {"these", "are", "tokens"};
    MockTokenizer tokenizer;
    EXPECT_CALL(tokenizer, tokenizeString(toProcess))
        .WillOnce(Return(tokens));
    MockExtractor extractor("testing");

    vector<TagResult> tagged {
        make_tuple(EntityType::PERSON, "Barack Obama", 0.6),
        make_tuple(EntityType::LOCATION, "United States", 0.7),
        make_tuple(EntityType::MISC, "the president", 0.3)
    };
    EXPECT_CALL(extractor, extract(tokens))
        .WillOnce(Return(tagged));

    shared_ptr<MockTokenizer> tokenizerPtr(&tokenizer, NonDeleter<MockTokenizer>());
    shared_ptr<MockExtractor> extractorPtr(&extractor, NonDeleter<MockExtractor>());
    MitieNerProcessor processor(tokenizerPtr, extractorPtr);
    tagged = processor.process(toProcess, 0.5);
    EXPECT_EQ(2, tagged.size());
    EXPECT_EQ(EntityType::PERSON, get<0>(tagged.at(0)));
    EXPECT_EQ("Barack Obama", get<1>(tagged.at(0)));
    EXPECT_EQ(EntityType::LOCATION, get<0>(tagged.at(1)));
    EXPECT_EQ("United States", get<1>(tagged.at(1)));

}
