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


std::unique_ptr<cmd::Base> parse( int argc, Argv argv )
{
    const std::string keys =
        "{help h usage ? |      | print this message}"
        "{detect d       |      | cam -> detect -> show}"
        ;

    cv::CommandLineParser parser( argc, argv, keys );

    if( parser.has( "help" ) )
    {
        return help( parser );
    }

    if( parser.has( "detect" ) )
    {
        return std::make_unique< cmd::CamDetectShow >();
    }

    return help( parser );
}


}  // namespace cli
