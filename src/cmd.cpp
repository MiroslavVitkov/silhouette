#include "cmd.h"

#include "io.h"

#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>

#include <vector>


namespace cmd
{


void CamDetectShow::execute()
{
    io::Camera cam{ io::Mode::_colour };
    io::VideoPlayer player{ "faces" };
    cv::Mat frame;
    std::vector< cv::Rect > rects;

    cv::HOGDescriptor detector;
    detector.setSVMDetector( cv::HOGDescriptor::getDefaultPeopleDetector() );

    while( cam >> frame )
    {
        detector.detectMultiScale( frame, rects );
        io::draw_rects( frame, rects );
        rects.clear();
        player << frame;
    }

}


}  // namespace cmd