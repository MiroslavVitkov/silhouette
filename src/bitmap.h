#ifndef BITMAP_H_
#define BITMAP_H_


// In this file: extract person silhouette masks from Supervisely's JSON format.


#include <opencv2/opencv.hpp>

#include <string>
#include <tuple>
#include <vector>


namespace bitmap
{


// pair.first: coordinates of top left corner of bitmap
// pair.second: encoded or decoded silhouette
// Each bitmap corresponds to one person obviously.
using BitmapsRaw = std::vector< std::pair< cv::Size, std::string > >;
using Bitmaps = std::vector< std::pair< cv::Size, cv::Mat > >;


Bitmaps read( const std::string & json_path );


}  // namespace bitmap


#endif  // defined( BITMAP_H_ )
