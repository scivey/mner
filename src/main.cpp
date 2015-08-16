#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <tuple>
#include <vector>

#include <cstdlib>

#include <glog/logging.h>

#include <mitie/conll_tokenizer.h>
#include <mitie/named_entity_extractor.h>

#include <folly/Format.h>
#include <folly/FBString.h>

#include "MitieNerProcessor.h"

using namespace folly;
using scivey::mner::MitieNerProcessor;

int main(int argc, char** argv) {
    LOG(INFO) << "start";
    LOG(INFO) << "loading MITIE ner model...";
    MitieNerProcessor processor(argv[1]);
    ifstream ifs("./test_data/cnn_article.txt");
    ostringstream oss;
    char c;
    while (ifs.get(c)) {
        oss << c;
    }
    auto result = processor.process(oss.str());
    for (auto elem: result) {
        ostringstream line;
        line << get<0>(elem) << "\t" << get<1>(elem) << "\t" << get<2>(elem);
        LOG(INFO) << line.str();
    }
    LOG(INFO) << "end";

}