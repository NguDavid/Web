#ifndef MODULEMANAGER_HPP
    #define MODULEMANAGER_HPP

#include <unordered_map>
#include <vector>
#include <string>

#include "IModule.hpp"

class ModuleManager {
    public:
        ModuleManager();
        ~ModuleManager();
        ModuleManager(const ModuleManager &other);
        ModuleManager& operator=(const ModuleManager &other);

        void loadModules(const std::unordered_map<std::string, std::string> &config);
        const std::vector<IModule *> &getModules() const;

    private:
        std::vector<IModule *> modules;
        std::vector<void *> handles;
};

#endif /* MODULEMANAGER_HPP */
