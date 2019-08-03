#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>
#include <sstream>

namespace Utils {

    void SetAssetPath(const std::string& path);
    std::string GetAssetPath();
    std::vector<std::string> GetResourcePaths();
    std::string GetExtension(std::string path);
};
#endif // UTILS_HPP