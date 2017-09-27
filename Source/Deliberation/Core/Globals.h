#pragma once

#include <cassert>
#include <functional>
#include <memory>
#include <utility>
#include <vector>

namespace deliberation {

class AbstractGlobalHandle {
public:
    virtual ~AbstractGlobalHandle() = default;

    virtual long useCount() = 0;
};

template<typename T>
class GlobalHandle : public AbstractGlobalHandle {
public:
    explicit GlobalHandle(const std::shared_ptr<T> &global) : m_global(global) {}

    long useCount() override { return m_global.use_count(); }

public:
    std::shared_ptr<T> m_global;
};

/**
 * Init the Singleton of type T. Caller needs to keep the returned shared_ptr<> in order to manage the global's lifetime
 */
template<typename T>
void InitGlobal();

template<typename T>
std::shared_ptr<T> GetGlobal();

/**
 * Internal - do not touch directly.
 * Only meant to keep a ordered list of Globals to delete them in appropriate order
 */
class Globals final {
private:
    template<typename T>
    friend void InitGlobal();

    friend void DeleteGlobals();

private:
    std::vector<std::unique_ptr<AbstractGlobalHandle>> handles;
};

extern Globals globals;

inline void DeleteGlobals() {
    while (!globals.handles.empty()) {
        assert(globals.handles.back()->useCount() == 1);
        globals.handles.pop_back();
    }
}

}

#define DELIBERATION_DEFINE_GLOBAL(Type) \
    std::weak_ptr<Type> Instance_##Type; \
    template<> \
    void InitGlobal<Type>() \
    { \
        auto instance = std::make_shared<Type>(); \
        Instance_##Type = instance; \
        auto handle = std::make_unique<GlobalHandle<Type>>(instance); \
        globals.handles.emplace_back(std::move(handle)); \
    } \
    template<> \
    std::shared_ptr<Type> GetGlobal<Type>() \
    { \
       return Instance_##Type.lock(); \
    }
