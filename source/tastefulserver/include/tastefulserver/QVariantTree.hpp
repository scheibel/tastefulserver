#pragma once

#include <tastefulserver/QVariantTree.h>

namespace tastefulserver {

template <class T>
T QVariantAbstractTree::value()
{
    return asQVariant().value<T>();
}

} // namespace tastefulserver
