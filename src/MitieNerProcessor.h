#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <tuple>

#include <mitie/conll_tokenizer.h>
#include <mitie/named_entity_extractor.h>


namespace { using namespace std; }

namespace scivey {
namespace mner {

vector<string> tokenizeString(const string &str);

class MitieNerProcessor {
    mitie::named_entity_extractor ner_;
    string modelPath_;
    string classname_;
    vector<string> tagStrings_;
    void initExtractor();
public:
    MitieNerProcessor(const string &modelPath);
    vector<tuple<string, string, double>> process(const string &text, double confidenceThreshold = 0.5);
};

} // mner
} // scivey
