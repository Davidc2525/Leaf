

#ifndef __PLATFORM_GENEATOR__
#define __PLATFORM_GENEATOR__

#include "../backend/visitor.h"

class platform_generator
{
public:
	virtual visitor *create_generator() = 0;
};

#endif