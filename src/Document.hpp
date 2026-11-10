
#pragma once

#include "Object3D.hpp"

#include <memory>
#include <vector>
#include <utility>
#include <type_traits>

namespace m3g {

class Document {
public:
    Document() = default;
    ~Document() = default;

    Document(const Document&) = delete;
    Document& operator=(const Document&) = delete;

    Document(Document&&) noexcept = default;
    Document& operator=(Document&&) noexcept = default;

    template<typename T, typename... Args>
    T* create(Args&&... args) {
        static_assert(std::is_base_of_v<Object3D, T>);

        auto object = std::make_unique<T>(std::forward<Args>(args)...);

        T* ptr = object.get();
        m_objects.push_back(std::move(object));

        return ptr;
    }

    template<typename T>
    T* findFirst() const {
        static_assert(std::is_base_of_v<Object3D, T>);

        for (const auto& object : m_objects) {
            if (auto* ptr = dynamic_cast<T*>(object.get())) return ptr;
        }

        return nullptr;
    }

    void merge(Document&& other);

    size_t size() const noexcept { return m_objects.size(); }

    void clear() noexcept { m_objects.clear(); }

private:
    std::vector<std::unique_ptr<Object3D>> m_objects;
};

} // namespace m3g
