#include "Event.h"


PaintEvent::PaintEvent( Data& data, kvs::glut::Label& label ):
    m_data( data ),
    m_label( label )
{
}

void PaintEvent::update()
{
    m_label.setText("Data: %s", kvs::File( m_data.dirname() ).baseName().c_str() );
    m_label.addText("Width: %d", m_data.IPLab().width() );
    m_label.addText("Height: %d", m_data.IPLab().height() );
    m_label.addText("Time index: %d", m_data.timeIndex() );
    m_label.addText("Slice index: %d", m_data.sliceIndex() );
    screen()->redraw();
}

KeyPressEvent::KeyPressEvent( Data& data ): m_data( data )
{
}

void KeyPressEvent::update( kvs::KeyEvent* event )
{
    switch ( event->key() )
    {
    case kvs::Key::Up: { m_data.incrementSliceIndex(); break; }
    case kvs::Key::Down: { m_data.decrementSliceIndex(); break; }
    case kvs::Key::Right: { m_data.incrementTimeIndex(); break; }
    case kvs::Key::Left: { m_data.decrementTimeIndex(); break; }
    default: break;
    }

    scene()->removeObject( "Image" );
    scene()->registerObject( m_data.import( "Image" ) );
    screen()->redraw();
}
