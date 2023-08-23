#ifndef HW_EXAMPLE
#define HW_EXAMPLE

#include "SH_Common.hlsli"

struct alignas(16)  ExampleStruct
{
	int example0;
	int example1;
	int example2;
	int example3;
};

SBUFFER(ExampleSBuffer, ExampleStruct, t, 20);

#endif
