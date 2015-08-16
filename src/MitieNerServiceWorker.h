#pragma once

#include <memory>
#include <string>
#include <tuple>
#include <vector>


#include "MitieNerProcessor.h"
#include "gen-cpp2/NerTagger.h"

namespace {
    using namespace folly;
    using namespace std;
    using scivey::mner::MitieNerProcessor;
    using scivey::mner::services::NerTaggerSvIf;
    using scivey::mner::services::NerItem;
    using scivey::mner::services::EntityType;
}

namespace scivey {
namespace mner {

class MitieNerServiceWorker: public NerTaggerSvIf {
    unique_ptr<MitieNerProcessor> nerProcessor_;
public:
    MitieNerServiceWorker(unique_ptr<MitieNerProcessor> nerProcessor) {
        nerProcessor_ = std::move(nerProcessor);
    }
    void tag(vector<NerItem> &resultVec, unique_ptr<string> text, double confidence) {
        auto result = nerProcessor_->process(*text);
        for(auto elem: result) {
            auto entityType = std::get<0>(elem);
            auto name = std::get<1>(elem);
            auto elemConfidence = std::get<2>(elem);
            EntityType et;
            if (entityType == "PERSON") {
                et = EntityType::PERSON;
            } else if (entityType == "LOCATION") {
                et = EntityType::LOCATION;
            } else if (entityType == "ORGANIZATION") {
                et = EntityType::ORGANIZATION;
            } else if (entityType == "MISC") {
                et = EntityType::MISC;
            } else {
                et = EntityType::MISC;
            }
            NerItem item;
            item.entityType = et;
            item.entityName = name;
            item.confidence = elemConfidence;
            resultVec.push_back(item);
        }
    }
};

} // mner
} // scivey
