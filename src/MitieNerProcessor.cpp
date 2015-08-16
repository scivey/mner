#include "MitieNerProcessor.h"

#include <string>
#include <sstream>
#include <vector>
#include <tuple>

#include <mitie/conll_tokenizer.h>
#include <mitie/named_entity_extractor.h>

using namespace std;
using mitie::conll_tokenizer;
using mitie::named_entity_extractor;

namespace scivey {
namespace mner {

vector<string> tokenizeString(const string &str) {
    istringstream textStream(str);
    conll_tokenizer tokenizer(textStream);
    vector<string> tokens;
    string token;
    while (tokenizer(token)) {
        tokens.push_back(token);
    }
    return tokens;
}

void MitieNerProcessor::initExtractor() {
    dlib::deserialize(modelPath_) >> classname_ >> ner_;
    tagStrings_ = ner_.get_tag_name_strings();
}

MitieNerProcessor::MitieNerProcessor(const string &modelPath):
    modelPath_(modelPath) {
        initExtractor();
}

vector<tuple<string, string, double>> MitieNerProcessor::process(const string &text, double confidenceThreshold) {
    auto tokens = tokenizeString(text);
    vector<pair<unsigned long, unsigned long>> chunks;
    vector<unsigned long> chunk_tags;
    vector<double> chunk_scores;
    vector<tuple<string, string, double>> output;
    ner_.predict(tokens, chunks, chunk_tags, chunk_scores);

    for (size_t i = 0; i < chunks.size(); ++i) {
        if (chunk_scores[i] > confidenceThreshold) {
            ostringstream currentEntityText;
            size_t end = chunks[i].second;
            for (size_t j = chunks[i].first; j < end; ++j) {
                currentEntityText << tokens[j];
                if (j < end) {
                   currentEntityText << " ";
                }
            }
            output.emplace_back(
                tagStrings_[chunk_tags[i]],
                currentEntityText.str(),
                chunk_scores[i]
            );
        }
    }
    return output;
}

} // mner
} // scivey
