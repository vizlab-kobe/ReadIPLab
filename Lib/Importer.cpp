#include "Importer.h"


namespace ReadIPLab
{

kvs::ImageObject* ImportImageObject( const ReadIPLab::IPLab& ipl, const size_t sindex )
{
    kvs::ImageObject* object = new kvs::ImageObject();
    object->setSize( ipl.width(), ipl.height() );
    object->setPixels( ipl.data( sindex ), kvs::ImageObject::Gray8 );
    return object;
}

kvs::ImageObject* ImportImageObject( const ReadIPLab::IPLabList& ipl_list, const size_t tindex, const size_t sindex )
{
    ReadIPLab::IPLab ipl = ipl_list.load( tindex );
    return ImportImageObject( ipl, sindex );
}

} // end of namespace ReadIPLab
