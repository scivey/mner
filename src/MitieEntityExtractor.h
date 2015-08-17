#pragma once

#include <vector>
#include <string>
#include <tuple>
#include <mitie/named_entity_extractor.h>

#include "gen-cpp2/NerTagger_types.h"

namespace scivey {
namespace mner {

class MitieEntityExtractor {
    mitie::named_entity_extractor extractor_;
    std::string classname_;
    std::string modelPath_;
    vector<std::string> tagStrings_;
protected:
    MitieEntityExtractor();
public:
    virtual void loadModel();
    MitieEntityExtractor(const std::string &modelPath);
    using ResultTuple = typename std::tuple<scivey::mner::services::EntityType, std::string, double>;
    virtual void predict(
        const vector<string> &tokens,
        vector<pair<unsigned long, unsigned long>> &chunks,
        vector<unsigned long> &chunk_tags,
        vector<double> &chunk_scores
    );
    virtual std::vector<ResultTuple> extract(const std::vector<std::string> &tokens);
    virtual ~MitieEntityExtractor();
};

} // mner
} // scivey
