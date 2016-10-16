#include "Event.h"


PaintEvent::PaintEvent( ReadIPLab::IPLab& ipl, Input& input, kvs::glut::Label& label ):
    m_ipl( ipl ),
    m_input( input ),
    m_label( label )
{
    m_label.setMargin( 10 );
    m_label.setText("Data: %s", kvs::File( m_input.dirname ).baseName().c_str() );
    m_label.addText("Width: %d", m_ipl.width() );
    m_label.addText("Height: %d", m_ipl.height() );
    m_label.addText("Time index: %d", m_input.tindex );
    m_label.addText("Slice index: %d", m_input.sindex );
}

void PaintEvent::update()
{
    m_label.setText("Data: %s", kvs::File( m_input.dirname ).baseName().c_str() );
    m_label.addText("Width: %d", m_ipl.width() );
    m_label.addText("Height: %d", m_ipl.height() );
    m_label.addText("Time index: %d", m_input.tindex );
    m_label.addText("Slice index: %d", m_input.sindex );
    screen()->redraw();
}

KeyPressEvent::KeyPressEvent(
    ReadIPLab::IPLabList& ipl_list,
    ReadIPLab::IPLab& ipl,
    Input& input ):
    m_ipl_list( ipl_list ),
    m_ipl( ipl ),
    m_input( input ) {}

void KeyPressEvent::update( kvs::KeyEvent* event )
{
    switch ( event->key() )
    {
    case kvs::Key::Up:
    {
        const int max_value = m_ipl.numberOfFrames() - 1;
        m_input.sindex = kvs::Math::Min( m_input.sindex + 1, max_value );
        break;
    }
    case kvs::Key::Down:
    {
        const int min_value = 0;
        m_input.sindex = kvs::Math::Max( m_input.sindex - 1, min_value );
        break;
    }
    case kvs::Key::Right:
    {
        const int max_value = m_ipl_list.filenames().size() - 1;
        m_input.tindex = kvs::Math::Min( m_input.tindex + 1, max_value );
        m_ipl = m_ipl_list.load( m_input.tindex );
        break;
    }
    case kvs::Key::Left:
    {
        const int min_value = 0;
        m_input.tindex = kvs::Math::Max( m_input.tindex - 1, min_value );
        m_ipl = m_ipl_list.load( m_input.tindex );
        break;
    }
    default: break;
    }

    kvs::ImageObject* image = ReadIPLab::ImportImageObject( m_ipl, m_input.sindex );
    image->setName("Image");

    scene()->removeObject( "Image" );
    scene()->registerObject( image );
    screen()->redraw();
}
