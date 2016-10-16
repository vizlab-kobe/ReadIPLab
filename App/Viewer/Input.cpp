#include "Input.h"


Input::Input( const kvs::CommandLine& commandline ):
    tindex( 0 ),
    sindex( 0 ),
    dirname( "" )
{
    this->read( commandline );
}

void Input::read( const kvs::CommandLine& commandline )
{
    if ( commandline.hasOption("t") ) tindex = commandline.optionValue<int>("t");
    if ( commandline.hasOption("s") ) sindex = commandline.optionValue<int>("s");
    if ( commandline.hasValues() ) dirname = commandline.value<std::string>();
}

void Input::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Time index: " << tindex << std::endl;
    os << indent << "Slice index: " << sindex << std::endl;
    os << indent << "Directory: " << dirname << std::endl;
}
