#ifndef CRUDE_MC_HEADER_GUARD
#define CRUDE_MC_HEADER_GUARD
#include "largeComponentCommon/context.h"
namespace largeComponent
{
	struct crudeMCArgs
	{
	public:
		crudeMCArgs(const context& contextObj)
			: contextObj(contextObj)
		{}
		const context& contextObj;
	};
	void crudeMC(crudeMCArgs& args);
}
#endif
