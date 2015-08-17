#pragma once

#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <tuple>

#include <mitie/conll_tokenizer.h>
#include <mitie/named_entity_extractor.h>

#include "MitieTokenizer.h"
#include "MitieEntityExtractor.h"
#include "gen-cpp2/NerTagger_types.h"

namespace {
    using namespace std;
    using scivey::mner::services::EntityType;
}

namespace scivey {
namespace mner {

class MitieNerProcessor {
    shared_ptr<MitieTokenizer> tokenizer_;
    shared_ptr<MitieEntityExtractor> extractor_;
protected:
    MitieNerProcessor();
public:
    MitieNerProcessor(shared_ptr<MitieTokenizer> tokenizer, shared_ptr<MitieEntityExtractor> extractor);
    virtual vector<tuple<EntityType, string, double>> process(const string &text, double confidenceThreshold = 0.5);
    virtual ~MitieNerProcessor();
};

} // mner
} // scivey
