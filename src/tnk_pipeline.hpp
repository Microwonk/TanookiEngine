#pragma once

#include <string>
#include <vector>

namespace tnk {
    class TnkPipeline {
    public:
        TnkPipeline(const std::string& vertFilePath, const std::string& fragFilePath);

    private:
        static std::vector<char> readFile();
    };
}