#include "ModuleManager.hpp"
#include <dlfcn.h>
#include <iostream>

ModuleManager::ModuleManager() = default;

ModuleManager::~ModuleManager()
{
    int i = 0;

    while (i < modules.size()) {
        delete (modules[i]);
        i += 1;
    }
    i = 0;
    while (i < handles.size()) {
        dlclose(handles[i]);
        i += 1;
    }
}

ModuleManager::ModuleManager(const ModuleManager &other) = default;

ModuleManager& ModuleManager::operator=(const ModuleManager &other) = default;

void ModuleManager::loadModules(const std::unordered_map<std::string, std::string> &config)
{
    auto it = config.begin();

    while (it != config.end()) {
        const std::string &key = it->first;
        const std::string &path = it->second;
        if (key.compare(0, 7, "module.") != 0) {
            it++;
            continue;
        }
        void *handle = dlopen(path.c_str(), RTLD_NOW);
        if (!handle) {
            std::cout << "[ERROR] Failed to load module: " << path << " -> " << dlerror() << std::endl;
            it++;
            continue;
        }
        typedef IModule* (*CreateFunc)();
        CreateFunc createModule = (CreateFunc)dlsym(handle, "createModule");
        if (!createModule) {
            std::cerr << "[ERROR] createModule not found in " << path << std::endl;
            dlclose(handle);
            it++;
            continue;
        }
        IModule *module = createModule();
        if (!module) {
            std::cerr << "[ERROR] Failed to instantiate module from: " << path << std::endl;
            dlclose(handle);
            it++;
            continue;
        }
        modules.push_back(module);
        handles.push_back(handle);
        std::cout << "[INFO] Loaded module: " << key << " from " << path << std::endl;
        it++;
    }
}

const std::vector<IModule *> &ModuleManager::getModules() const
{
    return (modules);
}
