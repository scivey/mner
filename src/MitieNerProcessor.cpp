#include "MitieNerProcessor.h"
#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <tuple>

#include <glog/logging.h>

#include <mitie/named_entity_extractor.h>

#include "MitieTokenizer.h"
#include "MitieEntityExtractor.h"
#include "gen-cpp2/NerTagger_types.h"


using namespace std;
using mitie::conll_tokenizer;
using mitie::named_entity_extractor;
using scivey::mner::services::EntityType;

namespace scivey {
namespace mner {

MitieNerProcessor::MitieNerProcessor(){}

MitieNerProcessor::MitieNerProcessor(shared_ptr<MitieTokenizer> tokenizer, shared_ptr<MitieEntityExtractor> extractor):
    tokenizer_(tokenizer), extractor_(extractor) {}

vector<tuple<EntityType, string, double>> MitieNerProcessor::process(const string &text, double confidenceThreshold) {
    auto tokens = tokenizer_->tokenizeString(text);
    auto result = extractor_->extract(tokens);

    vector<tuple<EntityType, string, double>> output;
    for (auto elem: result) {
        if (get<2>(elem) > confidenceThreshold) {
            output.push_back(elem);
        }
    }
    return output;
}

MitieNerProcessor::~MitieNerProcessor(){}

} // mner
} // scivey
