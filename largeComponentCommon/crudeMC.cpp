#include "largeComponentCommon/crudeMC.h"
#include "isLargeComponentPossible.h"
#include "observation.h"
namespace largeComponent
{
	std::size_t crudeMC(crudeMCArgs& args)
	{
		const context& contextObj = args.contextObj;
		std::size_t componentSize = contextObj.getComponentSize();
		std::size_t hadLargeComponentCount = 0;
		isLargeComponentPossibleTemporaries temp;
		for(std::size_t i = 0; i < args.n; i++)
		{
			observation obs(contextObj, args.randomSource);
			if(isLargeComponentOn(contextObj.getGraph(), obs.getState(), componentSize, temp))
			{
				hadLargeComponentCount++;
			}
		}
		return hadLargeComponentCount;
	}
}
