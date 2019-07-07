#include <Utils.hpp>

namespace Utils {
    std::string _mAssetPath;
    std::vector<std::string> _mAssetPaths;

    void SetAssetPath(const std::string& path)
    {
        //LogInfo("Setting Asset Path: %s\n", path.c_str());
        _mAssetPath = path;
        _mAssetPaths.clear();
    }

    std::string GetAssetPath()
    {
        return _mAssetPath;
    }

    std::vector<std::string> GetResourcePaths()
    {
        if (_mAssetPaths.empty()) {
            std::stringstream ss(GetAssetPath());
            std::string path;
            while (std::getline(ss, path, '&')) {
                if (path.empty()) continue;
                if (path.back() != '/') path.push_back('/');

                _mAssetPaths.push_back(path);
            }
            _mAssetPaths.push_back("");
            std::reverse(_mAssetPaths.begin(), _mAssetPaths.end());
        }
        return _mAssetPaths;
    }
};