#include "bitmap.h"

#include <cppcodec/base64_rfc4648.hpp>
#include <nlohmann/json.hpp>
#include <zlc/zlibcomplete.hpp>

#include <fstream>


// This whole module reimplements the following python function:
//def base64_2_mask(s):
//    z = zlib.decompress(base64.b64decode(s))
//    n = np.fromstring(z, np.uint8)
//    mask = cv2.imdecode(n, cv2.IMREAD_UNCHANGED)[:, :, 3].astype(bool)
//    return mask


namespace bitmap
{


struct JsonParser
{
    JsonParser( const std::string & json_path )
        :  _parsed{ nlohmann::json::parse( std::ifstream{ json_path } ) }
    {
    }


    cv::Size get_image_size() const
    {
        const auto height = _parsed[ "size" ][ "height" ];
        const auto width = _parsed[ "size" ][ "width" ];
        return { width, height};
    }



    BitmapsRaw get_bitmaps() const
    {
        BitmapsRaw ret;
        for( const auto & obj : _parsed[ "objects" ] )
        {
            const auto pos = obj[ "bitmap" ][ "origin" ];
            assert( pos.size() == 2 );

            cv::Size s{ pos[ 0 ], pos[ 1 ] };
            ret.emplace_back( s, obj[ "bitmap" ][ "data" ] );
        }
        return ret;
    }


private:
    nlohmann::json _parsed;
};


std::vector< uint8_t > decode_base64( const std::string & encoded )
{
    using base64 = cppcodec::base64_rfc4648;

    const auto decoded = base64::decode( encoded );
    return decoded;
}


std::string decompress( const std::vector< uint8_t > in )
{
    zlibcomplete::ZLibDecompressor zlib;
    const std::string in_str( in.cbegin(), in.cend() );
    const auto decompressed = zlib.decompress( in_str );
    return decompressed;
}


// Produce a binary pixel mask.
cv::Mat to_mask( std::string & decompressed, cv::Size size )
{
    const cv::Mat bitmap( size, CV_8UC1, decompressed.data() );
    const auto decoded = cv::imdecode( bitmap, cv::IMREAD_UNCHANGED );
    return decoded;
}


Bitmaps read( const std::string & json_path )
{
    Bitmaps ret;
    JsonParser j{ json_path };
    for( const auto & b : j.get_bitmaps() )
    {
        const auto decoded = decode_base64( b.second );
        auto decompressed = decompress( decoded );
        const auto mask = to_mask( decompressed, cv::Size{} );
    }
    return ret;
}


}  // namespace bitmap
