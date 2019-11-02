#include "cmd.h"

#include "algo.h"
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
        const auto det = algo::detect_pedestrians( frame );
        std::cout << det.size() << std::endl;

        const auto vs = r.get_last_silhouettes();
        const auto vb = [&] () { std::vector<cv::Rect> ret; for(const auto & s : vs){ret.push_back(s._box);} return ret; } ();
        io::draw_rects( frame, vb );
        std::cout << "*****" << vb.size() << std::endl;
        p << frame;

    }

}


}  // namespace cmd
