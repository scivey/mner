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
#include "MitieTokenizer.h"
#include "MitieEntityExtractor.h"
#include "NerThriftService.h"
#include "gen-cpp2/NerTagger.h"

using namespace folly;
using namespace std;
using scivey::mner::MitieNerProcessor;
using scivey::mner::MitieTokenizer;
using scivey::mner::NerThriftService;
using scivey::mner::MitieEntityExtractor;


using scivey::mner::services::NerTaggerSvIf;
using scivey::mner::services::NerItem;
using scivey::mner::services::EntityType;


int main(int argc, char** argv) {
    LOG(INFO) << "start";
    string modelPath = argv[1];
    thread t1([modelPath](){
        auto extractor = make_shared<MitieEntityExtractor>(modelPath);
        auto tokenizer = make_shared<MitieTokenizer>();
        auto processor = make_shared<MitieNerProcessor>(tokenizer, extractor);
        auto service = make_shared<NerThriftService>(processor);
        bool allowInsecureLoopback = true;
        string saslPolicy = "";
        auto server = new apache::thrift::ThriftServer(saslPolicy, allowInsecureLoopback);
        server->setInterface(service);
        server->setPort(8089);
        LOG(INFO) << "listening...";
        server->serve();
    });
    t1.join();
    LOG(INFO) << "end";
}
