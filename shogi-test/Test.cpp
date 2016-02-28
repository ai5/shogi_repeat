#include <gtest/gtest.h>

#include "TestCommon.h"

#if _MSC_VER
#pragma comment(lib, "gtestd.lib")
#ifdef _DLL 
#pragma comment(lib, "gtest_main-mdd.lib")
#else
#pragma comment(lib, "gtest_maind.lib")
#endif
#endif



