#pragma once
#include <vector>
#include <string>

namespace scivey {
namespace mner {

std::vector<std::string> tokenizeStringFn(const std::string &str);

class MitieTokenizer {
public:
    virtual std::vector<std::string> tokenizeString(const std::string &str);
    virtual ~MitieTokenizer();
};

} // mner
} // scivey