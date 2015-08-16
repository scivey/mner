#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <memory>
#include <tuple>
#include <thread>
#include <vector>

#include <cstdlib>

#include <glog/logging.h>

#include <mitie/conll_tokenizer.h>
#include <mitie/named_entity_extractor.h>

#include <folly/Format.h>
#include <folly/FBString.h>

#include <thrift/lib/cpp2/server/ThriftServer.h>
#include <thrift/lib/cpp2/async/AsyncProcessor.h>

#include "MitieNerProcessor.h"
#include "MitieNerServiceWorker.h"
#include "gen-cpp2/NerTagger.h"

using namespace folly;
using namespace std;
using scivey::mner::MitieNerProcessor;
using scivey::mner::MitieNerServiceWorker;

using scivey::mner::services::NerTaggerSvIf;
using scivey::mner::services::NerItem;
using scivey::mner::services::EntityType;


int main(int argc, char** argv) {
    LOG(INFO) << "start";
    string modelPath = argv[1];
    thread t1([modelPath](){
        LOG(INFO) << "loading MITIE ner model...";
        auto processor = folly::make_unique<MitieNerProcessor>(modelPath);
        auto service = make_shared<MitieNerServiceWorker>(std::move(processor));
        bool allowInsecureLoopback = true;
        string saslPolicy = "";
        auto server = new apache::thrift::ThriftServer(saslPolicy, allowInsecureLoopback);
        server->setInterface(service);
        server->setPort(8089);
        LOG(INFO) << "listening...";
        server->serve();
    });
    t1.join();
    // MitieNerProcessor processor(argv[1]);
    // ifstream ifs("./test_data/cnn_article.txt");
    // ostringstream oss;
    // char c;
    // while (ifs.get(c)) {
    //     oss << c;
    // }
    // auto result = processor.process(oss.str());
    // for (auto elem: result) {
    //     ostringstream line;
    //     line << get<0>(elem) << "\t" << get<1>(elem) << "\t" << get<2>(elem);
    //     LOG(INFO) << line.str();
    // }
    LOG(INFO) << "end";

}