#include "Data.h"
#include <kvs/Assert>
#include <kvs/ImageImporter>


Data::Data( const Input& input ):
    m_dirname( input.dirname ),
    m_tindex( input.tindex ),
    m_sindex( input.sindex )
{
    m_ipl_list.read( input.dirname );
    m_ipl = m_ipl_list.load( input.tindex );
}

void Data::incrementTimeIndex()
{
    const int max_value = m_ipl_list.filenames().size() - 1;
    const size_t tindex = kvs::Math::Min( static_cast<int>(m_tindex) + 1, max_value );
    this->setTimeIndex( tindex );
}

void Data::decrementTimeIndex()
{
    const int min_value = 0;
    const size_t tindex = kvs::Math::Max( static_cast<int>(m_tindex) - 1, min_value );
    this->setTimeIndex( tindex );
}

void Data::incrementSliceIndex()
{
    const int max_value = m_ipl.numberOfFrames() - 1;
    const size_t sindex = kvs::Math::Min( static_cast<int>(m_sindex) + 1, max_value );
    this->setSliceIndex( sindex );
}

void Data::decrementSliceIndex()
{
    const int min_value = 0;
    const size_t sindex = kvs::Math::Max( static_cast<int>(m_sindex) - 1, min_value );
    this->setSliceIndex( sindex );
}

void Data::setTimeIndex( const size_t tindex )
{
    KVS_ASSERT( tindex < m_ipl_list.filenames().size() );
    m_tindex = tindex;
    m_ipl = m_ipl_list.load( m_tindex );
}

void Data::setSliceIndex( const size_t sindex )
{
    KVS_ASSERT( sindex < m_ipl.numberOfFrames() );
    m_sindex = sindex;
}

kvs::ImageObject* Data::import( const std::string name )
{
    m_ipl.setImportingFrameIndex( m_sindex );
    kvs::ImageObject* image = new kvs::ImageImporter( &m_ipl );
    image->setName( name );
    return image;
}
