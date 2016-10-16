#include "Program.h"
#include <string>
#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/glut/Label>
#include <kvs/ImageObject>
#include <kvs/KeyPressEventListener>
#include <kvs/PaintEventListener>
#include <kvs/CommandLine>
#include <kvs/Indent>
#include <Lib/IPLab.h>
#include <Lib/IPLabList.h>
#include <Lib/Importer.h>


namespace
{

struct Parameter
{
    int tindex; // time index
    int sindex; // slice index
    std::string dirname; // directory name

    Parameter( const kvs::CommandLine& commandline ):
        tindex( 0 ),
        sindex( 0 ),
        dirname( "" )
    {
        this->read( commandline );
    }

    void read( const kvs::CommandLine& commandline )
    {
        if ( commandline.hasOption("t") ) tindex = commandline.optionValue<int>("t");
        if ( commandline.hasOption("s") ) sindex = commandline.optionValue<int>("s");
        if ( commandline.hasValues() ) dirname = commandline.value<std::string>();
    }

    void print( std::ostream& os, const kvs::Indent& indent ) const
    {
        os << indent << "Time index: " << tindex << std::endl;
        os << indent << "Slice index: " << sindex << std::endl;
        os << indent << "Directory: " << dirname << std::endl;
    }
};

class PaintEvent : public kvs::PaintEventListener
{
private:
    ReadIPLab::IPLab& m_ipl;
    Parameter& m_parameter;
    kvs::glut::Label& m_label;

public:
    PaintEvent( ReadIPLab::IPLab& ipl, Parameter& parameter, kvs::glut::Label& label ):
        m_ipl( ipl ),
        m_parameter( parameter ),
        m_label( label )
    {
        m_label.setMargin( 10 );
        m_label.setText("Data: %s", kvs::File( m_parameter.dirname ).baseName().c_str() );
        m_label.addText("Width: %d", m_ipl.width() );
        m_label.addText("Height: %d", m_ipl.height() );
        m_label.addText("Time index: %d", m_parameter.tindex );
        m_label.addText("Slice index: %d", m_parameter.sindex );
    }

    void update()
    {
        m_label.setText("Data: %s", kvs::File( m_parameter.dirname ).baseName().c_str() );
        m_label.addText("Width: %d", m_ipl.width() );
        m_label.addText("Height: %d", m_ipl.height() );
        m_label.addText("Time index: %d", m_parameter.tindex );
        m_label.addText("Slice index: %d", m_parameter.sindex );
        screen()->redraw();
    }
};

class KeyPress : public kvs::KeyPressEventListener
{
    ReadIPLab::IPLabList& m_ipl_list;
    ReadIPLab::IPLab& m_ipl;
    Parameter& m_parameter;

public:

    KeyPress(
        ReadIPLab::IPLabList& ipl_list,
        ReadIPLab::IPLab& ipl,
        Parameter& parameter ):
        m_ipl_list( ipl_list ),
        m_ipl( ipl ),
        m_parameter( parameter ) {}

    void update( kvs::KeyEvent* event )
    {
        switch ( event->key() )
        {
        case kvs::Key::Up:
        {
            const int max_value = m_ipl.numberOfFrames() - 1;
            m_parameter.sindex = kvs::Math::Min( m_parameter.sindex + 1, max_value );
            break;
        }
        case kvs::Key::Down:
        {
            const int min_value = 0;
            m_parameter.sindex = kvs::Math::Max( m_parameter.sindex - 1, min_value );
            break;
        }
        case kvs::Key::Right:
        {
            const int max_value = m_ipl_list.filenames().size() - 1;
            m_parameter.tindex = kvs::Math::Min( m_parameter.tindex + 1, max_value );
            m_ipl = m_ipl_list.load( m_parameter.tindex );
            break;
        }
        case kvs::Key::Left:
        {
            const int min_value = 0;
            m_parameter.tindex = kvs::Math::Max( m_parameter.tindex - 1, min_value );
            m_ipl = m_ipl_list.load( m_parameter.tindex );
            break;
        }
        default: break;
        }

        scene()->removeObject( "Image" );
        scene()->registerObject( ReadIPLab::ImportImageObject( m_ipl, m_parameter.sindex ) );
        screen()->redraw();
    }
};

}

int Program::exec( int argc, char** argv )
{
    kvs::glut::Application app( argc, argv );
    kvs::glut::Screen screen( &app );

    kvs::CommandLine commandline( argc, argv );
    commandline.addHelpOption();
    commandline.addOption( "t","Time index (default: 0).", 1, false );
    commandline.addOption( "s","Slice index (default: 0).", 1, false );
    commandline.addValue( "Input directory", true );
    if ( !commandline.parse() ) { return 1; }

    ::Parameter parameter( commandline );
    ReadIPLab::IPLabList ipl_list( parameter.dirname );
    ReadIPLab::IPLab ipl = ipl_list.load( parameter.tindex );

    kvs::ImageObject* image = ReadIPLab::ImportImageObject( ipl, parameter.sindex );
    screen.registerObject( image );

    kvs::glut::Label label( &screen );
    label.setTextColor( kvs::RGBColor::White() );
    label.show();

    ::KeyPress key_press( ipl_list, ipl, parameter );
    ::PaintEvent paint_event( ipl, parameter, label );
    screen.addEvent( &key_press );
    screen.addEvent( &paint_event );
    screen.show();

    return app.run();
}
