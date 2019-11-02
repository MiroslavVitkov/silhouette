#include "bitmap.h"

#include <cppcodec/base64_rfc4648.hpp>
#include <nlohmann/json.hpp>
#include <zlc/zlibcomplete.hpp>

#include <fstream>
#include <tuple>

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
    // pair.first: coordinates of top left corner of bitmap
    // pair.second: encoded or decoded silhouette
    // Each bitmap corresponds to one person obviously.
    using BitmapsRaw = std::vector< std::pair< cv::Size, std::string > >;


    JsonParser( const std::string & json_path )
        :  _parsed{ nlohmann::json::parse( std::ifstream{ json_path } ) }
    {
    }


    cv::Size get_image_size() const
    {
        const int height = _parsed[ 0 ][ "size" ][ "height" ];
        const int width = _parsed[ 0 ][ "size" ][ "width" ];
        return { width, height};
    }



    BitmapsRaw get_bitmaps() const
    {
        BitmapsRaw ret;
        for( const auto & obj : _parsed[ 0 ][ "objects" ] )
        {
            if(obj[ "classTitle" ] != "person_bmp" )
            {
                continue;
            }

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
cv::Mat to_mask( std::string & decompressed, cv::Size )
{
    const cv::Mat bitmap( decompressed.size(), 1, CV_8UC1, decompressed.data() );
    const auto decoded = cv::imdecode( bitmap, cv::IMREAD_UNCHANGED );
    return decoded;
}


std::vector< Silhouette > read( const std::string & json_path )
{
    JsonParser j{ json_path };

    std::vector< Silhouette > ret;

    for( const auto & b : j.get_bitmaps() )
    {
        const auto decoded = decode_base64( b.second );
        auto decompressed = decompress( decoded );

        // This is wrong. b.first denotes offset of bitmap in the image,
        // not bitmap size. But it works. Go figure!
        const auto mask = to_mask( decompressed, b.first );
        Silhouette s{ cv::Rect{ b.first.width, b.first.height, mask.cols, mask.rows }
                    , mask };
        ret.push_back( s );
    }
    return ret;
}


}  // namespace bitmap
