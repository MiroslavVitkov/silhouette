#include "cmd.h"

#include "io.h"

#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>

#include <vector>

#include <iostream>
namespace cmd
{


void CamDetectShow::execute()
{
    io::Camera cam{ io::Mode::_colour };
    io::VideoPlayer player{ "pedestrians" };
    cv::Mat frame;
    std::vector< cv::Rect > rects;
    std::vector< double > weights;

    cv::HOGDescriptor detector;
    detector.setSVMDetector( cv::HOGDescriptor::getDefaultPeopleDetector() );

    const std::string kur{"YW55IGNhcm5hbCBwbGVhc3VyZQ=="};
    const auto putka {io::decode_base64( kur )};
    std::cout << putka.size() << std::endl;

    while( cam >> frame )
    {
        detector.detectMultiScale( frame
                                 , rects
                                 , weights
                                 , 0
                                 , cv::Size{ 4, 4 }
                                 , cv::Size{ 8, 8 }
                                 , 1.05 );
        io::draw_rects( frame, rects );
        rects.clear();
        player << frame;
    }

}


}  // namespace cmd
