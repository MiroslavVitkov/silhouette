#include "bitmap.h"

#include <cppcodec/base64_rfc4648.hpp>

#include <fstream>
#include <nlohmann/json.hpp>
#include <zlc/zlibcomplete.hpp>


using json = nlohmann::json;


namespace bitmap
{


std::string read_json_field( const std::string & path )
{
    (void)path;
    std::ifstream file{ "/media/share/downloads/supervisely_person_dataset/ds1/ann/bodybuilder-weight-training-stress-38630.png.json" };
    const auto parsed = json::parse( file );
    const auto bitmap = parsed[ "objects" ][ 0 ][ "bitmap" ][ "data" ];
    return bitmap;
}


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
// Location and size are separately specified in the json.
//    const std::vector<char> charvect(decompressed.begin(), decompressed.end());
//    cv::Mat bitmap2( charvect.cbegin(), charvect.cend() );
//    cv::imdecode( bitmap2, cv::IMREAD_UNCHANGED );


void read( const std::string & path )
{
    const auto field = read_json_field( path );
    const auto decoded = decode_base64( field );
    const auto decompressed = decompress( decoded );
}


}  // namespace bitmap
