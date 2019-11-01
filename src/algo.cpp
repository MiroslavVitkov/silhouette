#include "algo.h"

#include <opencv2/objdetect.hpp>

#include <vector>

namespace algo
{

std::vector< cv::Rect > detect_pedestrians( const cv::Mat & frame )
{
    cv::HOGDescriptor hog;
    hog.setSVMDetector(  cv::HOGDescriptor::getDefaultPeopleDetector() );

    std::vector< cv::Rect > ret;
    hog.detectMultiScale( frame, ret);
    // perhaps apply non-maximum suppression
    return ret;
}
}  // namespace algo
