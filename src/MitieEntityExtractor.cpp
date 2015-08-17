#include "MitieEntityExtractor.h"

#include <vector>
#include <string>
#include <tuple>

#include <mitie/named_entity_extractor.h>
#include <glog/logging.h>

#include "gen-cpp2/NerTagger_types.h"

using namespace std;
using mitie::named_entity_extractor;
using scivey::mner::services::EntityType;
namespace scivey {
namespace mner {

using ResultTuple = typename std::tuple<EntityType, std::string, double>;


void MitieEntityExtractor::loadModel() {
    LOG(INFO) << "Loading MITIE NER model from " << modelPath_;
    dlib::deserialize(modelPath_) >> classname_ >> extractor_;
    tagStrings_ = extractor_.get_tag_name_strings();
}

MitieEntityExtractor::MitieEntityExtractor(){}

MitieEntityExtractor::MitieEntityExtractor(const string &modelPath): modelPath_(modelPath) {
    loadModel();
}

EntityType entityTypeOfTypeString(const string &str) {
    if (str == "PERSON") {
        return EntityType::PERSON;
    } else if (str == "LOCATION") {
        return EntityType::LOCATION;
    } else if (str == "ORGANIZATION") {
        return EntityType::ORGANIZATION;
    } else if (str == "MISC") {
        return EntityType::MISC;
    } else {
        VLOG(1) << "Unrecognized entity type: " << str;
        return EntityType::MISC;
    }
}

using ChunkVec = vector<pair<unsigned long, unsigned long>>;
using ChunkTagVec = vector<unsigned long>;

void MitieEntityExtractor::predict(const vector<string> &tokens, ChunkVec &chunks, ChunkTagVec &chunk_tags, vector<double> &chunk_scores) {
    extractor_.predict(tokens, chunks, chunk_tags, chunk_scores);
}

vector<ResultTuple> MitieEntityExtractor::extract(const vector<string> &tokens) {
    vector<pair<unsigned long, unsigned long>> chunks;
    vector<unsigned long> chunk_tags;
    vector<double> chunk_scores;
    vector<ResultTuple> output;
    predict(tokens, chunks, chunk_tags, chunk_scores);

    for (size_t i = 0; i < chunks.size(); ++i) {
        ostringstream currentEntityText;
        size_t end = chunks[i].second - 1;
        for (size_t j = chunks[i].first; j <= end; j++) {
            currentEntityText << tokens[j];
            if (j < end) {
               currentEntityText << " ";
            }
        }
        auto entityType = entityTypeOfTypeString(tagStrings_[chunk_tags[i]]);
        output.emplace_back(
            entityType,
            currentEntityText.str(),
            chunk_scores[i]
        );
    }
    return output;
}

MitieEntityExtractor::~MitieEntityExtractor(){}

} // mner
} // scivey
