#include "NFDHandler.h"

nfdresult_t NFDHandler::OpenDialog(nfdfilteritem_t* filters, nfdfiltersize_t filterCount) {

	nfdresult_t result = NFD::OpenDialog(m_nfd_out_path, filters, filterCount);

    if (result == NFD_OKAY) {
        
        LOG("Dialog opened successfully");
    }
    else if (result == NFD_CANCEL) {
        
        LOG("User cancelled");
    }
    else {
        
        LOG("Error - {}", NFD::GetError());
    }

    return result;
}