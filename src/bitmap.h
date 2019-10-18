#ifndef BITMAP_H_
#define BITMAP_H_


// In this file: extract person silhouette masks from Supervisely's JSON format.


#include <string>


namespace bitmap
{


void read( const std::string & path );


}  // namespace bitmap


#endif  // defined( BITMAP_H_ )
