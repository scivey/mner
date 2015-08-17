#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include <glog/logging.h>

#include "NerThriftService.h"
#include "MitieNerProcessor.h"
#include "gen-cpp2/NerTagger_types.h"

using namespace std;
using scivey::mner::services::NerTaggerSvIf;
using scivey::mner::services::NerItem;
using scivey::mner::services::EntityType;

namespace scivey {
namespace mner {

NerThriftService::NerThriftService(shared_ptr<MitieNerProcessor> nerProcessor) {
    nerProcessor_ = nerProcessor;
}

void NerThriftService::tag(vector<NerItem> &resultVec, unique_ptr<string> text, double confidence) {
    auto result = nerProcessor_->process(*text, confidence);
    for (auto elem: result) {
        NerItem item;
        item.entityType = get<0>(elem);
        item.entityName = get<1>(elem);
        item.confidence = get<2>(elem);
        resultVec.push_back(item);
    }
}

} // mner
} // scivey
