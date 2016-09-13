#pragma once

#include <string>
#include <vector>
#include <kvs/Type>
#include <kvs/ValueArray>


namespace ReadIPLab
{

class IPLab
{
public:
    typedef kvs::ValueArray<kvs::UInt8> Data;
    typedef std::vector<Data> DataSet;

private:
    kvs::Int32 m_width; ///< data width
    kvs::Int32 m_height; ///< data height
    kvs::Int16 m_nframes; ///< number of frames
    DataSet m_dataset; ///< set of pixel data

public:
    IPLab(){}

    kvs::Int32 width() const { return m_width; }
    kvs::Int32 height() const { return m_height; }
    kvs::Int16 numberOfFrames() const { return m_nframes; }
    const DataSet& dataSet() const { return m_dataset; }
    const Data& data( const size_t index ) const { return m_dataset[index]; }

    bool read( const std::string& filename );
};

} // end of namespace Project
