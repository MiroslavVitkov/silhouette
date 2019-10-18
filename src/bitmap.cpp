#include <cppcodec/base64_rfc4648.hpp>

#include "bitmap.h"


namespace bitmap
{


std::vector< uint8_t > decode_base64( const std::string & encoded )
{
    using base64 = cppcodec::base64_rfc4648;

    const auto decoded = base64::decode( encoded );
    return decoded;
}


}  // namespace bitmap
