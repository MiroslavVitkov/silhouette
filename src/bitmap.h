#ifndef BITMAP_H_
#define BITMAP_H_


// In this file: extract person silhouette masks from Supervisely's JSON format.


#include <opencv2/opencv.hpp>

#include <string>
#include <vector>


namespace bitmap
{


// cv::Mat< CV_8UC1 >: background pixels are 0, human silhouettes are nonzero.
cv::Mat read( const std::string & json_path );


}  // namespace bitmap


#endif  // defined( BITMAP_H_ )
