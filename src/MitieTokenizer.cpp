#include <vector>
#include <string>
#include <sstream>
#include <mitie/conll_tokenizer.h>

#include "MitieTokenizer.h"

using namespace std;
using mitie::conll_tokenizer;

namespace scivey {
namespace mner {

vector<string> tokenizeStringFn(const string &str) {
    istringstream textStream(str);
    conll_tokenizer tokenizer(textStream);
    vector<string> tokens;
    string token;
    while (tokenizer(token)) {
        tokens.push_back(token);
    }
    return tokens;
}

vector<string> MitieTokenizer::tokenizeString(const string &str) {
    return tokenizeStringFn(str);
}

MitieTokenizer::~MitieTokenizer(){}

} // mner
} // scivey