#pragma once

#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include "MitieNerProcessor.h"
#include "gen-cpp2/NerTagger.h"

namespace scivey {
namespace mner {

class NerThriftService: public services::NerTaggerSvIf {
    std::shared_ptr<MitieNerProcessor> nerProcessor_;
public:
    NerThriftService(std::shared_ptr<MitieNerProcessor> nerProcessor);
    void tag(std::vector<services::NerItem> &resultVec, std::unique_ptr<std::string> text, double confidence);
};

} // mner
} // scivey
