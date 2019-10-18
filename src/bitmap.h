#ifndef BITMAP_H_
#define BITMAP_H_


// In this file: extract person silhouette masks from Supervisely's JSON format.


#include <string>
#include <vector>


namespace bitmap
{

std::vector< uint8_t > decode_base64( const std::string & encoded );


}  // namespace bitmap


#endif  // defined( BITMAP_H_ )
