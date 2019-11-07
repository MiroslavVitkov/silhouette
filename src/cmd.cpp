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
    unsigned real{}, detected{};
    while( r >> frame )
    {
        const auto det = algo::detect_pedestrians( frame, algo::Detector::_LBP );
        io::draw_rects( frame, det, io::Colour::_red );
        detected += det.size();

        const auto vs = r.get_last_silhouettes();
        const auto vb = [&] () { std::vector<cv::Rect> ret; for(const auto & s : vs){ret.push_back(s._box);} return ret; } ();
        io::draw_rects( frame, vb, io::Colour::_green );
        real += vb.size();
        p << frame;
    }

    std::cout <<  "detected " << detected << " people out of " << real << std::endl;

}


float intersect( const cv::Rect & r1, const cv::Rect & r2 )
{
    const auto intersect =  r1 & r2;
    return intersect.area() / r1.area();
}


void DatDetectSummarise::execute()
{
    io::SuperviselyReader r{"/media/share/downloads/supervisely_person_dataset/"};
    cv::Mat frame;
    unsigned real{}, true_positives{}, false_positives{};
    while( r >> frame )
    {
        const auto det = algo::detect_pedestrians( frame, algo::Detector::_LBP );

        const auto vs = r.get_last_silhouettes();
        const auto vb = [&] () { std::vector<cv::Rect> ret; for(const auto & s : vs){ret.push_back(s._box);} return ret; } ();
        real += vb.size();

        for( const auto & d : det )
        {
            unsigned before = true_positives;
            for( const auto & r : vb )
            {
                if( intersect( d, r ) > 0.5)
                {
                    ++true_positives;
                }
            }

            if(true_positives == before)
            {
                false_positives++;
            }
        }
        std::cout <<  "detected " << det.size() << " people out of " << vs.size() << std::endl;
        if(vs.empty())
        {
            std::cout << "asdad";
        }
    }

    std::cout << "total humans: " << real << ", true_positives: " << true_positives
              << ", false positives: " << false_positives;

}


void SegmentBackground::execute()
{
    //io::Camera cam;
    io::VideoReader cam{ "me.mp4" };
    io::VideoWriter vw( "detected.avi", cam.get_size() );
    io::VideoPlayer player1{"detected"};
    io::VideoPlayer player2{"segmented"};
    cv::Mat frame;

    const auto bg = cv::createBackgroundSubtractorKNN();
    //const auto bg = cv::createBackgroundSubtractorMOG2();
    while( cam >> frame )
    {
        const auto det = algo::detect_pedestrians( frame, algo::Detector::_LBP );
            io::draw_rects( frame, det);

        cv::Mat frame2;

        bg->apply( frame, frame2 );
        if(!det.empty())
        {cv::Mat frame3( frame2, det[0] );
        player2 << frame3;}
        player1 << frame;
        vw << frame;
    }
}


}  // namespace cmd
