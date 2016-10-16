#pragma once
#include <Lib/IPLab.h>
#include <Lib/IPLabList.h>
#include <kvs/ImageObject>
#include <string>
#include "Input.h"


class Data
{
private:

    std::string m_dirname;
    ReadIPLab::IPLabList m_ipl_list;
    ReadIPLab::IPLab m_ipl;
    size_t m_tindex;
    size_t m_sindex;

public:

    Data( const Input& input );

    const std::string& dirname() const { return m_dirname; }
    const ReadIPLab::IPLabList& IPLabList() const { return m_ipl_list; }
    const ReadIPLab::IPLab& IPLab() const { return m_ipl; }
    size_t timeIndex() const { return m_tindex; }
    size_t sliceIndex() const { return m_sindex; }

    void incrementTimeIndex();
    void decrementTimeIndex();
    void incrementSliceIndex();
    void decrementSliceIndex();
    void setTimeIndex( const size_t tindex );
    void setSliceIndex( const size_t sindex );
    kvs::ImageObject* import( const std::string name = "" );
};
