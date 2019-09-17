#ifndef CLI_H_
#define CLI_H_


// In this file: a command line argument parser.


#include "cmd.h"

#include <memory>


namespace cli
{


using Argv = const char *const *const;

std::unique_ptr< cmd::Base > parse( int argc, Argv argv );


}  // namespace cli


#endif  // defined( CLI_H_ )
