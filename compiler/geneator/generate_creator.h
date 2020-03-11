


#ifndef __GENERATE_CREATOR__
#define __GENERATE_CREATOR__

#include "../backend/ast_visitor_args.h"
#include "../backend/visitor.h" 

enum generator_class
{
	x86_64_linux,
	x86_linux,
	x86_64_win,
	x86_win,
};

class generate_creator_args
{
public:
	generator_class *type;
	ast_visitor_args *gargs;
};

class generate_creator
{
public:
	generate_creator();
	~generate_creator();
	virtual visitor *create(generate_creator_args *args) = 0;
};

#endif