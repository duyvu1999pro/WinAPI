
#pragma once


#ifdef BAI3
#define BAI3_API __declspec(dllexport)
#else
#define BAI3_API __declspec(dllimport)

#endif


extern "C" BAI3_API int string(const char* charArray);
extern "C" BAI3_API void writeFile(const char* filename, const char* content);

