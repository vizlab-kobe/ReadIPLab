#include "Program.h"
#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/glut/Label>
#include <kvs/ImageObject>
#include <kvs/CommandLine>
#include <Lib/IPLab.h>
#include <Lib/IPLabList.h>
#include <Lib/Importer.h>
#include "Input.h"
#include "Event.h"


int Program::exec( int argc, char** argv )
{
    kvs::glut::Application app( argc, argv );
    kvs::glut::Screen screen( &app );

    Input input( argc, argv );
    if ( !input.parse() ) { return 1; }

    ReadIPLab::IPLabList ipl_list( input.dirname );
    ReadIPLab::IPLab ipl = ipl_list.load( input.tindex );

    kvs::ImageObject* image = ReadIPLab::ImportImageObject( ipl, input.sindex );
    image->setName("Image");
    screen.registerObject( image );

    kvs::glut::Label label( &screen );
    label.setTextColor( kvs::RGBColor::White() );
    label.show();

    KeyPressEvent key_press_event( ipl_list, ipl, input );
    PaintEvent paint_event( ipl, input, label );
    screen.addEvent( &key_press_event );
    screen.addEvent( &paint_event );
    screen.show();

    return app.run();
}
