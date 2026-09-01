#include "IGraphics_brii.h"


Brii_RHI* Brii_RHI_Manager::get_RHI_RawPtr() const
{
 return graphics_RHI.get();
}

