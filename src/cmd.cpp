#include "cmd.h"

#include "bitmap.h"
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
    const auto decoded{ bitmap::decode_base64( bitmap ) };

    // Decompress.
    zlibcomplete::ZLibDecompressor decomp;
    const std::string decoded_str( decoded.cbegin(), decoded.cend() );
    std::cout << decoded_str << std::endl;
    const auto decompressed = decomp.decompress( decoded_str );

    // Produce a binary pixel mask.
    // Location and size are separately specified in the json.
//    const std::vector<char> charvect(decompressed.begin(), decompressed.end());
//    cv::Mat bitmap2( charvect.cbegin(), charvect.cend() );
//    cv::imdecode( bitmap2, cv::IMREAD_UNCHANGED );

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
