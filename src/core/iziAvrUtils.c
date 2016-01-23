#include <iziConf.h>
#include <core/iziKernel.h>
#include <core/iziAvrUtils.h>
#include <device/iziDevicePriv.h>

#if IZI_KERNEL_SIZE > IZI_KERNEL_SIZE_TINY

#ifndef IZI_LIB_VERSION
	//#error Library version must be defined during library building
	#define IZI_LIB_VERSION 0
#endif

#ifndef IZI_BUILD_NUMBER
	//#error Build number must be defined during library building
	#define IZI_BUILD_NUMBER 0
#endif

#ifndef IZI_REVISION_NUMBER
	//#error Repository revision must be defined during library building
	#define IZI_REVISION_NUMBER 0
#endif

#ifndef IZI_BUILD_DATE
	//#error Building date must be defined during library building
	#define IZI_BUILD_DATE 0,0,0
#endif

struct SIziKernelInfo
{
	TIziKernelVersion _libraryVersion;
	uint16_t _repositoryRevision;
	TIziCompilationDate	_buildDate;
};
typedef struct SIziKernelInfo TIziKernelInfo;

IZI_CONST(TIziKernelInfo, iziKernelInfo) = {{IZI_LIB_VERSION,IZI_BUILD_NUMBER},IZI_REVISION_NUMBER,{IZI_BUILD_DATE}};

//------------------------------------------------------------------------------
TIziKernelVersion iziKernelGetVersion()
{
	TIziKernelVersion kernelInfo;
	kernelInfo.majorVersion = IZI_CONST_READ_BYTE(&(iziKernelInfo._libraryVersion.majorVersion));
	kernelInfo.minorVersion = IZI_CONST_READ_WORD(&(iziKernelInfo._libraryVersion.minorVersion));
	return kernelInfo;
}

//------------------------------------------------------------------------------
TIziCompilationDate iziKernelGetBuildDate()
{
	TIziCompilationDate buildDate;
	buildDate.year = IZI_CONST_READ_WORD(&(iziKernelInfo._buildDate.year));
	buildDate.month = IZI_CONST_READ_BYTE(&(iziKernelInfo._buildDate.month));
	buildDate.day = IZI_CONST_READ_BYTE(&(iziKernelInfo._buildDate.day));
	return buildDate;
}

//------------------------------------------------------------------------------
uint16_t iziKernelGetRevision()
{
	uint16_t revision = IZI_CONST_READ_WORD(&(iziKernelInfo._repositoryRevision));
	return revision;
}

#endif //IZI_KERNEL_SIZE
