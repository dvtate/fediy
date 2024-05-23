#pragma once

#include <functional>
#include <algorithm>
#include <cinttypes>
#include <string>
#include <optional>
#include <vector>
#include <ranges>
#include <mutex>

#include "ModuleInfo.hpp"
#include "Msg.hpp"


template <class T>
struct TaskPool {
protected:
    std::vector<size_t> m_free;
    std::vector<T*> m_tasks;
    std::mutex m_mtx;

public:
    TaskPool() = default;
    TaskPool(const size_t size):
        m_free(std::views::iota(0, size) | std::ranges::to<std::vector>())
    {
        m_tasks.reserve(size);
    }

    ~TaskPool() {
        for (auto* p : m_tasks)
            if (p != nullptr)
                delete p;
    }

    size_t started(T* task) {
        const std::lock_guard<std::mutex> lock(m_mtx);
        if (m_free.empty()) {
            auto ret = m_tasks.size();
            m_tasks.emplace_back(std::move(task));
            return ret;
        }

        auto ret = m_free.back();
        m_free.pop_back(); 
    }

    /*!
    \remarks calling other member functions invalidates result
    */
    std::vector<T&> active_tasks() {
        std::vector<T&> ret;
        for (auto* p : m_tasks)
            if (p != nullptr)
                ret.emplace_back(*p);
        return ret;
    }
};

/**
 * Apps we're supporting
 */
class Module {
public:
    // Basic module id info for checking message compatibility
    ModuleInfo m_info;

    TaskPool<Msg> m_tasks;

    explicit Module(const std::string& path);
    ~Module();

    void start();

    void stop();

    // Update message: reverse to/from and update payload
    bool send_message(Msg* msg);

protected:
    // Message Queue
    std::vector<Msg> m_queue;

    // .so file
    std::string m_binary;

};