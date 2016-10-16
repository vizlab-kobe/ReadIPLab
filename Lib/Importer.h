#pragma once

#include <kvs/ImageObject>
#include "IPLab.h"
#include "IPLabList.h"


namespace ReadIPLab
{

kvs::ImageObject* ImportImageObject( const ReadIPLab::IPLab& ipl, const size_t sindex );
kvs::ImageObject* ImportImageObject( const ReadIPLab::IPLabList& ipl_list, const size_t tindex, const size_t sindex );

} // end of namespace ReadIPLab
