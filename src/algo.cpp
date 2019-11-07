#include "algo.h"

#include <opencv2/objdetect.hpp>

#include <vector>

namespace algo
{


std::vector< cv::Rect > detect_pedestrians_HOGandSVM( const cv::Mat & frame )
{
    cv::HOGDescriptor hog;
    hog.setSVMDetector(  cv::HOGDescriptor::getDefaultPeopleDetector() );

    std::vector< cv::Rect > ret;
    hog.detectMultiScale( frame, ret);
    // perhaps apply non-maximum suppression
    return ret;
}



cv::CascadeClassifier create_classifier( const std::string & cascades_dir
                                       , const std::string & name )
{
    const auto xml_path = cascades_dir + '/' + name + ".xml";
    cv::CascadeClassifier c( xml_path );
    if( c.empty() )
    {
        Exception e{ "Failed to load cascade classifier: " + xml_path };
    }

    return c;
}


// Detect pedestrians usng Local Bnary Patterns.
// https://docs.opencv.org/3.2.0/d1/de5/classcv_1_1CascadeClassifier.html
struct DetectorLBP
{
    DetectorLBP( const std::string & cascades_dir )
        :_classifier{ create_classifier( cascades_dir, "haarcascade_fullbody" ) }
    {}

    std::vector<cv::Rect> locate( const cv::Mat & frame
                                , double min_confidence = 0.8
                                )
{
    assert( ! frame.empty() );

    //cv::Mat frame2;
    //cv::cvtColor( frame, frame2, CV_8UC1 );
    // Increase contrast in under- or over-exposed areas of the image.
   // cv::equalizeHist( frame2, frame2 );

    std::vector<cv::Rect> rects;
    _classifier.detectMultiScale( frame, rects );
    // Perhaps do low-pass filtering over 3 consecutive frames to evade false positives.
    (void)min_confidence;

    return rects;
}
private:
    cv::CascadeClassifier _classifier;
};


std::vector< cv::Rect > detect_pedestrians_LBP( const cv::Mat & frame )
{
    DetectorLBP d{"/home/vorac/proj/face/res/haarcascades"};
    return d.locate( frame );
}


std::vector< cv::Rect > detect_pedestrians( const cv::Mat & frame, Detector d )
{
    switch( d )
    {
    case Detector::_HOGandSVM:
        return detect_pedestrians_HOGandSVM( frame );
    case Detector::_LBP:
        return detect_pedestrians_LBP( frame );
    default:
        assert( false );
    }
}


}  // namespace algo
