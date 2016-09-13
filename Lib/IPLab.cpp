#include "IPLab.h"
#include <iostream>
#include <fstream>
#include <kvs/Endian>


namespace ReadIPLab
{

bool IPLab::read( const std::string& filename )
{
    std::ifstream ifs( filename.c_str(), std::ios::binary | std::ios::in );
    if( !ifs.is_open() )
    {
        kvsMessageError( "Cannot open %s.", filename.c_str() );
        return false;
    }

    kvs::Int8 version[4];
    ifs.read( (char*)version, 4 );

    kvs::UInt8 format;
    ifs.read( (char*)&format, 1 );

    kvs::UInt8 data_type; // 6 = ushort
    ifs.read( (char*)&data_type, 1 );

    ifs.read( (char*)&m_width, 4 );
    kvs::Endian::Swap( &m_width );

    ifs.read( (char*)&m_height, 4 );
    kvs::Endian::Swap( &m_height );

    kvs::Int8 researved1[6];
    ifs.read( (char*)researved1, 6 );

    ifs.read( (char*)&m_nframes, 2 );
    kvs::Endian::Swap( &m_nframes );

    kvs::Int8 researved2[50];
    ifs.read( (char*)researved2, 50 );

    kvs::Int8 clut[2048];
    ifs.read( (char*)clut, 2048 );

    for ( kvs::UInt16 i = 0; i < m_nframes; i++ )
    {
        kvs::ValueArray<kvs::UInt8> data( m_width * m_height );
        for ( size_t j = 0; j < data.size(); j++ )
        {
            kvs::UInt16 temp;
            ifs.read( (char*)&temp, 2 );
            kvs::Endian::Swap( &temp );
            data[j] = (int)( temp / 4095.0f * 255.0f );
        }
        m_dataset.push_back( data );
    }

    ifs.close();

    return true;
}

} // end of namespace ReadIPLab
