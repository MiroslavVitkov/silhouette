#include "cli.h"

#include <opencv2/opencv.hpp>

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>


namespace cli
{


std::unique_ptr< cmd::Base > help( const cv::CommandLineParser & parser )
{
    parser.printMessage();
    return std::make_unique< cmd::NoOp >();
}


std::unique_ptr< cmd::Base > parse( int argc, Argv argv )
{
    const std::string keys =
        "{help h usage ? |      | print this message}"
        "{cam c          |      | cam -> detect -> show}"
        "{dataset d      |      | Supervisely dataset -> detect -> show}"
        "{accuracy a     |      | Supervisely dataset -> detect -> calc accuracy}"
        "{background b   |      | camera -> segment background}"
        ;

    cv::CommandLineParser parser( argc, argv, keys );

    if( parser.has( "help" ) )
    {
        return help( parser );
    }

    if( parser.has( "cam" ) )
    {
        return std::make_unique< cmd::CamDetectShow >();
    }

    if( parser.has( "dataset" ) )
    {
        return std::make_unique< cmd::DatDetectShow >();
    }

    if( parser.has( "accuracy" ) )
    {
        return std::make_unique< cmd::DatDetectSummarise >();
    }

    if( parser.has( "background" ) )
    {
        return std::make_unique< cmd::SegmentBackground >();
    }


    return help( parser );
}


}  // namespace cli
