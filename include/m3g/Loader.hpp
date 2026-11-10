#ifndef __M3G_LOADER_HPP__
#define __M3G_LOADER_HPP__

#include "Document.hpp"
#include <vector>

namespace m3g {
    /**
     * @~English  Download and deserializes scene graph nodes and node components, as well as entire scene graphs.
     * @~Japanese M3Gファイルおよび画像ファイルのローダー.
     */
    class Loader {
    public:
        /**
         * @~English  Deserializes Object3D instances form the given byte array.
         * @~Japanese 与えられたバイトストリームのオフセット位置からObject3Dのインスタンスを生成する.
         */
        static Document loadDocument(const std::vector<char>& buffer, int offset = 0);

        /**
         * @~English  Deserializes Object3D instances from the named resource.
         * @~Japanese 与えられた名前のリソースからObject3Dのインスタンスを生成する.
         */
        static Document loadDocument(const char* file_name);
    };

} // namespace m3g

#endif
