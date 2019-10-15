#include "cmd.h"

#include "io.h"

#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>

#include <vector>

#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <zlc/zlibcomplete.hpp>
#include <stdint.h>
using json = nlohmann::json;
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


    // Decode base64 -> binary.
    std::ifstream file{ "/media/share/downloads/supervisely_person_dataset/ds1/ann/bodybuilder-weight-training-stress-38630.png.json" };
    const auto j = json::parse( file );
    const auto k = j["objects"][0]["bitmap"]["data"];
    const auto putka {io::decode_base64( k )};

    zlibcomplete::GZipDecompressor d;
    const std::string putka2{ putka.cbegin(), putka.cend() };
    const auto dec = d.decompress( putka2 );


    for(const auto & p : putka)
    {
        std::cout << std::to_string(p) << ' ';
    }
    std::cout << std::endl << std::endl << putka.size() << std::endl;

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
