
#pragma once


#ifdef BAI2
#define BAI2_API __declspec(dllexport)
#else
#define BAI2_API __declspec(dllimport)
#endif


extern "C" BAI2_API int Message();

