#include <nfd/nfd.hpp>
#include <utils/logging.hpp>

class NFDHandler {

private:

	NFD::Guard m_nfd_guard;
	NFD::UniquePath m_nfd_out_path;

public:

	NFDHandler() = default;
	~NFDHandler() = default;

	nfdresult_t OpenDialog(nfdfilteritem_t* filters, nfdfiltersize_t filterCount);
	const char* GetPath() { return static_cast<const char*>(m_nfd_out_path.get()); }
};