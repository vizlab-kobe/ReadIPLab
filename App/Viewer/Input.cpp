#include "Input.h"


Input::Input( int argc, char** argv ):
    tindex( 0 ),
    sindex( 0 ),
    dirname( "" )
{
    m_commandline = kvs::CommandLine( argc, argv );
    m_commandline.addHelpOption();
    m_commandline.addOption( "t","Time index (default: 0).", 1, false );
    m_commandline.addOption( "s","Slice index (default: 0).", 1, false );
    m_commandline.addValue( "Input directory", true );
}

bool Input::parse()
{
    if ( !m_commandline.parse() ) { return false; }

    if ( m_commandline.hasOption("t") ) tindex = m_commandline.optionValue<int>("t");
    if ( m_commandline.hasOption("s") ) sindex = m_commandline.optionValue<int>("s");
    if ( m_commandline.hasValues() ) dirname = m_commandline.value<std::string>();

    return true;
}

void Input::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Time index: " << tindex << std::endl;
    os << indent << "Slice index: " << sindex << std::endl;
    os << indent << "Directory: " << dirname << std::endl;
}
