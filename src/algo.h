#ifndef ALGO_H_
#define ALGO_H_


// In this file: wrappers around OpenCV algorithms.


#include "except.h"

#include <opencv2/opencv.hpp>

#include <vector>


namespace algo
{


std::vector< cv::Rect > detect_pedestrians( const cv::Mat & frame );


}  // namespace algo


#endif // defined( ALGO_H_ )
