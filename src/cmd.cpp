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

    // Read the bitmap from file.
    std::ifstream file{ "/media/share/downloads/supervisely_person_dataset/ds1/ann/bodybuilder-weight-training-stress-38630.png.json" };
    const auto jon = json::parse( file );
    const auto bitmap = jon["objects"][0]["bitmap"]["data"];

    // Decode base64 -> binary.
    const auto decoded{ io::decode_base64( bitmap ) };

    // Decompress.
    zlibcomplete::GZipDecompressor decomp;
    const std::string decoded_str( decoded.cbegin(), decoded.cend() );
    std::cout << decoded_str.length() << std::endl;
    const auto decompressed = decomp.decompress( decoded_str );

    for(const auto & p : decompressed)
    {
        std::cout << std::to_string(p) << ' ';
    }
    std::cout << std::endl << std::endl << decompressed.size() << std::endl;

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
