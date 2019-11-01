#include "cmd.h"

#include "bitmap.h"
#include "io.h"

#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>

#include <vector>


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


void DatDetectShow::execute()
{
    io::SuperviselyReader r{"/media/share/downloads/supervisely_person_dataset/"};
    cv::Mat frame;
    io::VideoPlayer p{"kur"};
    while( r >> frame )
    {
        std::cout << "new picture!" << std::endl;
        try{
        p << frame;}catch(...){}
    }

}


}  // namespace cmd
