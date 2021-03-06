#include <Rcpp.h>
#include <internal.h>
#include "crudeMC.h"
#include "importanceSampling.h"
#include "conditionalMC.h"
#include "sequentialMethod.h"
#include "defaultImportanceDensity.h"
#ifdef _MSC_VER
	#undef RcppExport
	#define RcppExport extern "C" __declspec(dllexport)
#endif
extern "C" 
{
	const char* package_name = "largeComponent";
	R_CallMethodDef callMethods[] = 
	{
		{"defaultImportanceDensity", (DL_FUNC)&defaultImportanceDensity, 2},
		{"crudeMC", (DL_FUNC)&crudeMC, 6}, 
		{"importanceSampling", (DL_FUNC)&importanceSampling, 7}, 
		{"conditionalMC", (DL_FUNC)&conditionalMC, 7}, 
		{"sequentialMethod", (DL_FUNC)&sequentialMethod, 7}, 
		{NULL, NULL, 0}
	};
	RcppExport void R_init_largeComponent(DllInfo *info)
	{
		std::vector<R_CallMethodDef> callMethodsVector;
		R_CallMethodDef* packageCallMethods = callMethods;
		while(packageCallMethods->name != NULL) packageCallMethods++;
		callMethodsVector.insert(callMethodsVector.begin(), callMethods, packageCallMethods);

		R_CallMethodDef* RcppStartCallMethods = Rcpp_get_call();
		R_CallMethodDef* RcppEndCallMethods = RcppStartCallMethods;
		while(RcppEndCallMethods->name != NULL) RcppEndCallMethods++;
		callMethodsVector.insert(callMethodsVector.end(), RcppStartCallMethods, RcppEndCallMethods);
		R_CallMethodDef blank = {NULL, NULL, 0};
		callMethodsVector.push_back(blank);

		R_registerRoutines(info, NULL, &(callMethodsVector[0]), NULL, NULL);
		init_Rcpp_cache();
	}
}
