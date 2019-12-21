
#include "sizes.h"

const char *size_to_string(sizes s)
{
	switch (s)
	{
	case none:
		return "";
	case byte:
		return "BYTE";
	case hword:
		return "";
	case word:
		return "WORD";
	case dword:
		return "DWORD";
	case qword:
		return "QWORD";
	}
	return "";
}
