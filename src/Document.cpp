#include "Document.hpp"

namespace m3g {

void Document::merge(Document&& other) {
    m_objects.reserve(m_objects.size() + other.m_objects.size());

    for (auto& object : other.m_objects) {
        m_objects.push_back(std::move(object));
    }

    other.m_objects.clear();
}

}; // namespace m3g