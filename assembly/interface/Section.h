

#ifndef __SECTION__
#define __SECTION__
#include <vector>
#include <iostream>
//#include "Slock.h"
using namespace std;
namespace LEAF
{
namespace ASSEMBLY
{

enum Sections
{
	data,
	bss,
	text,
	_extern,
	global,
};

class Section
{
private:
	Sections type;
	vector<Slot *> slots;
	SlotVisitor *visitor;
	
public:
	Section(SlotVisitor *);
	Section(SlotVisitor *,Sections);

	void write(ostream &);
	void add(Slot *);
};

/* class SectionData : public Section
{
public:
	void write();
};
class SectionBss : public Section
{
public:
	void write();
};

class SectionExtern : public Section
{
public:
	void write();
};

class SectionGlobal : public Section
{
public:
	void write();
};
class SectionText : public Section
{
private:
	vector<Slock *> slocks;

public:
	void write();
}; */

} // namespace ASSEMBLY
} // namespace LEAF
#endif