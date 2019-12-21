#ifndef _SIZE__
#define _SIZE__

enum sizes
{
	none,
	byte,
	hword,
	word,
	dword,
	qword,
};

const char *size_to_string(sizes s);
#endif
