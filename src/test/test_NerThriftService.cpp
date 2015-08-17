#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "test/TestHelpers.h"

#include <glog/logging.h>

#include <vector>
#include <tuple>
#include <memory>
#include <string>
#include "gen-cpp2/NerTagger_types.h"

#include "NerThriftService.h"

using namespace std;
using namespace scivey::mner;
using scivey::mner::services::EntityType;
using scivey::mner::services::NerItem;
using ::testing::Return;
using ::testing::_;
using TagResult = tuple<EntityType, string, double>;

class MockProcessor: public MitieNerProcessor {
public:
    MockProcessor(): MitieNerProcessor(){}
    MOCK_METHOD2(process, vector<TagResult>(const string &text, double confidence));
};

TEST(TestNerThriftService, BasicTest) {
    string toProcess = "Barack Obama is the president of the United States of America.";
    vector<TagResult> tagged {
        make_tuple(EntityType::PERSON, "Barack Obama", 0.6),
        make_tuple(EntityType::LOCATION, "United States", 0.7)
    };

    MockProcessor processor;
    EXPECT_CALL(processor, process(toProcess, 0.5))
        .WillOnce(Return(tagged));

    shared_ptr<MockProcessor> processorPtr(&processor, NonDeleter<MockProcessor>());
    NerThriftService service(processorPtr);

    unique_ptr<string> toProcessPtr {
        new string(toProcess)
    };
    vector<NerItem> result;
    service.tag(result, move(toProcessPtr), 0.5);
    EXPECT_EQ(2, result.size());

    EXPECT_EQ(EntityType::PERSON, result.at(0).entityType);
    EXPECT_EQ("Barack Obama", result.at(0).entityName);
    EXPECT_EQ(0.6, result.at(0).confidence);

    EXPECT_EQ(EntityType::LOCATION, result.at(1).entityType);
    EXPECT_EQ("United States", result.at(1).entityName);
    EXPECT_EQ(0.7, result.at(1).confidence);


}
