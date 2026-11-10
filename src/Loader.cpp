#include "Exception.hpp"
#include "LoaderImpl.hpp"
#include <fstream>

using namespace m3g;

Document Loader::loadDocument(const std::vector<char>& buffer, int offset) {
    if (offset < 0 || static_cast<size_t>(offset) >= buffer.size()) {
        throw IllegalArgumentException(__FILE__, __func__, "Invalid buffer offset.");
    }

    const char* p = buffer.data();
    const size_t length = buffer.size() - static_cast<size_t>(offset);

    if (length > static_cast<size_t>(std::numeric_limits<int>::max())) {
        throw IOException(__FILE__, __func__, "Input buffer is too large");
    }

    LoaderImpl loader;

    return loader.load(buffer.data() + offset, static_cast<int>(length));
}

Document Loader::loadDocument(const char* filename) {
    if (!filename) {
        throw NullPointerException(__FILE__, __func__, "File name is null.");
    }

    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    if (!file) {
        throw IOException(__FILE__, __func__, "Can't open the file: %s", filename);
    }

    const auto end = file.tellg();

    if (end < 0 || static_cast<std::uintmax_t>(end) > static_cast<std::uintmax_t>(std::numeric_limits<int>::max())) {
        throw IOException(__FILE__, __func__, "Invalid file size.");
    }

    std::vector<char> buffer(static_cast<size_t>(end));

    file.seekg(0, std::ios::beg);

    if (!file.read(buffer.data(), static_cast<std::streamsize>(buffer.size()))) {
        throw IOException(__FILE__, __func__, "Failed to read file: %s", filename);
    }

    return loadDocument(buffer);
}