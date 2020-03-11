

#if !defined(COCO_PARSER_H__)
#define COCO_PARSER_H__

#include <stdlib.h>
 #include <cstring>

 #include "../ast/ast.hpp"
 #include "../compiler/backend/visitor.h"
 #include "../compiler/symb_tab/symbol_table.hpp"
 //#include "../tree.hpp"
 #include <iostream>
 using namespace std;


#include "Scanner.h"



class Errors {
public:
	int count;			// number of errors detected

	Errors();
	Errors(Scanner *scanner);
	void printMsg(int line, int col, const wchar_t *s);

	void SynErr(int line, int col, int n);
	void Error(int line, int col, const wchar_t *s);
	void Warning(int line, int col, const wchar_t *s);
	void Warning(const wchar_t *s);
	void Exception(const wchar_t *s);


public:
	Scanner *scanner;

}; // Errors

class Parser {
private:
	enum {
		_EOF=0,
		_t_pointer=1,
		_t_r_pointer=2,
		_number=3,
		_name=4,
		_charConst=5,
		_stringConst=6,
		_floatLit=7,
		_binaryLit=8
	};
	int maxT;

	Token *dummyToken;
	int errDist;
	int minErrDist;

	void SynErr(int n);
	void Get();
	void Expect(int n);
	bool StartOf(int s);
	void ExpectWeak(int n, int follow);
	bool WeakSeparator(int n, int syFol, int repFol);

public:
	Scanner *scanner;
	Errors  *errors;

	Token *t;			// last recognized token
	Token *la;			// lookahead token

symbol_table *tab;
 int cout_annon_func = 0;
class MetaData{

public: 
	bool isF = false;
	Struct * type;
	string * tmpName;
	bool anon = true;
	MetaData(){
		type = new Struct();
		tmpName = new string("");
	}
};
	void _array(Struct * vd){
		_array1(vd,0);
	}
	void _array1(Struct * vd,int size){
		Struct * cvd = new Struct();
		cvd->kind = Struct::Arr;;
		cvd->elemType  = new Struct();
		*cvd->elemType = *vd;
		*vd = *cvd; 
	}
	void funcion(Struct * vd){
		Struct * cvd = new Struct();
		cvd->kind =Struct::Func;
		cvd->elemType  = new Struct();
		*cvd->elemType = *vd;
		*vd = *cvd; 
	}
	void puntero(Struct * vd){//puntero
		if(vd->kind == Struct::Func){
			//return;
		}
		//vd->kind=2;
		Struct * cvd = new Struct();
		cvd->kind = Struct::Pointer;
		cvd->elemType  = new Struct();
		*cvd->elemType = *vd;
		*vd = *cvd; 
	}
	void dump(Struct * vd,int d){
		for (int i = 0; i < d;i++)
		{
			cout<<" "; 
		}
		cout<<";kind: "<<vd->kind<<endl; 
		if(vd->elemType!=NULL){
			dump(vd->elemType,++d);
		}
	}
	Struct* last(Struct * vd){//importante
		
		if(vd->elemType==NULL){
			return vd;
		}else{
			return last(vd->elemType);
		}

	}

	void un_pointer(Obj*o){//desreferenciar
		int np=o->nPars;
		int adr=o->adr;
		if(
			(o->type->kind == Struct::Pointer ||
			o->type->kind == Struct::Arr )
			&& o->type->elemType!=NULL){
			//*o=*new Obj(o->kind,o->name,o->type->elemType);
			o->type = o->type->elemType;
			//o->adr = adr;
			//o->nPars=np;
		}else{
			cout<<";Error semnatico: "<<*o->name<<" no se puede desreferenciar."<<endl;
			if(o->type->kind != Struct::Pointer){
				cout<<"; no es un puntero"<<endl;
			}
			exit(1);
		}
	}

	void make_ref(Obj*o){//referenciar
			Obj  tmp = *o;
			//*o = *new Obj();


			o->type = new Struct();
			o->type->kind = Struct::Pointer;
			o->type->elemType = new Struct();
			o->type->elemType = tmp.type;

			/*o->kind = tmp.kind;
			o->name = tmp.name;
			o->nPars = tmp.nPars;
			o->adr = tmp.adr;
			o->locals = tmp.locals;		*/
	}

	void unwrap_call_function(Obj*o){
		int np=o->nPars;

		if(o->type->kind == Struct::Func && o->type->elemType!=NULL){
			string * return_name = o->name;
			//return_name->append(o->name->c_str());
			//*o=*new Obj(Obj::Var,return_name,o->type->elemType);
			o->kind=Obj::Var;
			o->type = o->type->elemType;
			//o->nPars =np;
		}else{
			cout<<";Error semnatico: "<<*o->name;
			if(o->type->kind != Struct::Func){
				cout<<" no es funcion"<<endl;
				dump(o->type,2);
			}
			exit(1);
		}
	}

 void LexString(string *lex){
         *lex = *new string(string(coco_string_create_char(t->val)));
 }
 void LexStringla(string *lex){
    string * ft = new string("");
    ft->append(string(coco_string_create_char(la->val)));
    //cout<<"; ft "<<ft->c_str()<<endl;
    lex->append(*ft);
  }
  void LexStringtoken(Token*to,string *lex){
    string * ft = new string("");
    ft->append(string(coco_string_create_char(to->val)));
    //cout<<"; ft "<<ft->c_str()<<endl;
    lex->append(*ft);
  }
 bool isVarDecl(){
    scanner->ResetPeek();
    string*n = new string("");
    string*n2 = new string(":");
    Token* x = la;
    LexStringtoken(x,n);
     // cout<<";t: "<<*n<<" = "<<(*n2==*n)<<endl;
    
    if(*n2==*n) {scanner->ResetPeek(); return true;}
    while(1){
    	nc:
      n = new string("");
      x = scanner->Peek();//la
      LexStringtoken(x,n);
      //cout<<";la: "<<*n<<" = "<<(*n2==*n)<<endl;
    
    if (*n=="["||*n=="]"||*n=="("||*n==")")
    {
    	//scanner->ResetPeek();return false;
    }
    if(*n2==*n) {scanner->ResetPeek();return true;}
    if(*n=="") {scanner->ResetPeek();return false;}
    if(*n=="=") {scanner->ResetPeek();return false;}
    
    //if(*n=="[") {scanner->ResetPeek();return false;}
    //if(*n=="]") {scanner->ResetPeek();return false;}
    }
   scanner->ResetPeek();
   return false;
  }

Section * data_section;
//ExternSymbols*ss;
//StringLiterals*sl;
//AstHandler*ast_h;
visitor * visitor_generator;



	Parser(Scanner *scanner);
	~Parser();
	void SemErr(const wchar_t* msg);

	void Pro();
	void SourceCode(ASTU&b_node);
	void Statement(ASTU&stat);
	void StatementBLock(ASTU&b_node);
	void VarDeclDef(ASTU&s);
	void Designator(ASTU&d_ast);
	void Expr(ASTU&e_node);
	void ActParams(ASTU&f_node,bool ifc);
	void FuncDecl(ASTU&f_node);
	void TryCatchStatement(ASTU&stat);
	void PartCatchStatement(ASTU&_try_);
	void VarDeclDefCatch(ASTU&s);
	void VarDcl(MetaData * md);
	void VarDecl();
	void VarDirectDcl(MetaData * md);
	void VarSuffix(MetaData * md);
	void FuncDcl(MetaData * fd);
	void FuncDirectDcl(MetaData * fd);
	void FuncSuffix(MetaData * fd);
	void SimExpression(ASTU &E);
	void BitShiffOp(ASTU &E);
	void BitAndOrOp(ASTU &E);
	void AndOrOp(ASTU &E);
	void RelaOp(ASTU &E);
	void AddOp(ASTU&t_node);
	void Term(ASTU&t_node);
	void Factor(ASTU&f_node);

	void Parse();

}; // end Parser



#endif

