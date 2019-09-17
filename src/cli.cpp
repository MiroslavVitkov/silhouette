#include "cli.h"

#include <opencv2/opencv.hpp>

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>


namespace cli
{


std::unique_ptr<cmd::Base> parse( int argc, Argv argv )
{
    const std::string keys =
        "{help h usage ? |      | print this message}"
        ;

    cv::CommandLineParser parser( argc, argv, keys );

    if( parser.has( "help" ) )
    {
        parser.printMessage();
        return std::make_unique<cmd::NoOp>();
    }

    return std::make_unique<cmd::NoOp>();
}


}  // namespace cli
