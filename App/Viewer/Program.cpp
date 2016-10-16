#include "Program.h"
#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/glut/Label>
#include "Input.h"
#include "Data.h"
#include "Event.h"


int Program::exec( int argc, char** argv )
{
    kvs::glut::Application app( argc, argv );
    kvs::glut::Screen screen( &app );

    Input input( argc, argv );
    if ( !input.parse() ) { return 1; }

    Data data( input );
    screen.registerObject( data.import( "Image" ) );

    kvs::glut::Label label( &screen );
    label.setMargin( 10 );
    label.setTextColor( kvs::RGBColor::White() );
    label.show();

    KeyPressEvent key_press_event( data );
    PaintEvent paint_event( data, label );
    screen.addEvent( &key_press_event );
    screen.addEvent( &paint_event );
    screen.show();

    return app.run();
}
