

#include <wchar.h>
#include "Parser.h"
#include "Scanner.h"




void Parser::SynErr(int n) {
	if (errDist >= minErrDist) errors->SynErr(la->line, la->col, n);
	errDist = 0;
}

void Parser::SemErr(const wchar_t* msg) {
	if (errDist >= minErrDist) errors->Error(t->line, t->col, msg);
	errDist = 0;
}

void Parser::Get() {
	for (;;) {
		t = la;
		la = scanner->Scan();
		if (la->kind <= maxT) { ++errDist; break; }

		if (dummyToken != t) {
			dummyToken->kind = t->kind;
			dummyToken->pos = t->pos;
			dummyToken->col = t->col;
			dummyToken->line = t->line;
			dummyToken->next = NULL;
			coco_string_delete(dummyToken->val);
			dummyToken->val = coco_string_create(t->val);
			t = dummyToken;
		}
		la = t;
	}
}

void Parser::Expect(int n) {
	if (la->kind==n) Get(); else { SynErr(n); }
}

void Parser::ExpectWeak(int n, int follow) {
	if (la->kind == n) Get();
	else {
		SynErr(n);
		while (!StartOf(follow)) Get();
	}
}

bool Parser::WeakSeparator(int n, int syFol, int repFol) {
	if (la->kind == n) {Get(); return true;}
	else if (StartOf(repFol)) {return false;}
	else {
		SynErr(n);
		while (!(StartOf(syFol) || StartOf(repFol) || StartOf(0))) {
			Get();
		}
		return StartOf(syFol);
	}
}

void Parser::Pro() {
		AST b = ast_h->CreateStatementBlock(); 
		SourceCode(b);
		ast_h->GenerateCode(b,ss,sl);
}

void Parser::SourceCode(AST&b_node) {
		
		while (StartOf(1)) {
			AST stat;
			Statement(stat);
			ast_h->StatementBlockAdd(b_node,stat);
		}
}

void Parser::Statement(AST&stat) {
		AST e_stat;
		switch (la->kind) {
		case _t_pointer: case _name: case 14 /* "(" */: case 19 /* "," */: case 20 /* ":" */: case 21 /* "[" */: {
			if (isVarDecl()) {
				VarDecl();
				Expect(11 /* ";" */);
				stat = ast_h->EmptyNode(); 
			} else {
				Designator(stat);
				if (la->kind == 12 /* "=" */) {
					Get();
					Expr(e_stat);
					if(!sreq(ast_h->GetObjOfNode(stat)->type,ast_h->GetObjOfNode(e_stat)->type)){
					cout<<";error de asignacion, tipos diferentes."<<endl; 
					dump(ast_h->GetObjOfNode(stat)->type,2);
					dump(ast_h->GetObjOfNode(e_stat)->type,2);
					exit(1);
					}else{
					stat = ast_h->AssignmentNode(stat,e_stat);
					Obj*o = new Obj(1,new string(),new Struct()); ast_h->SetObjOfNode(stat,o);
					} 
					
					
				} else if (la->kind == 14 /* "(" */) {
					ActParams(stat,false);
				} else SynErr(46);
				Expect(11 /* ";" */);
			}
			break;
		}
		case 23 /* "extern" */: case 24 /* "func" */: {
			FuncDecl(stat);
			break;
		}
		case 13 /* "print" */: {
			Get();
			Expect(14 /* "(" */);
			Expr(e_stat);
			Expect(15 /* ")" */);
			Expect(11 /* ";" */);
			stat = ast_h->CreatePrintNode(e_stat);
			break;
		}
		case 16 /* "ret" */: {
			Get();
			if (StartOf(2)) {
				Expr(e_stat);
				if(!sreq(symbol_table::currMethod()->type->elemType,ast_h->GetObjOfNode(e_stat)->type)){cout<<";El valor q retorna no coincide con la firma. "<<*symbol_table::currMethod()->name<<endl; exit(1);}
				stat = ast_h->ReturnNode(e_stat); ast_h->SetObjOfNode(stat,ast_h->GetObjOfNode(e_stat)); 
				
			} else if (la->kind == 11 /* ";" */) {
				stat = ast_h->ReturnNode();
			} else SynErr(47);
			Expect(11 /* ";" */);
			break;
		}
		case 17 /* "if" */: {
			Get();
			Expect(14 /* "(" */);
			Expr(e_stat);
			Expect(15 /* ")" */);
			stat = ast_h->CreateIfNode(e_stat); 
			StatementBLock(stat);
			if (la->kind == 18 /* "else" */) {
				Get();
				StatementBLock(stat);
			}
			break;
		}
		case 9 /* "{" */: {
			StatementBLock(stat);
			break;
		}
		case 11 /* ";" */: {
			Get();
			break;
		}
		default: SynErr(48); break;
		}
}

void Parser::StatementBLock(AST&b_node) {
		
		Expect(9 /* "{" */);
		while (StartOf(1)) {
			AST stat;
			Statement(stat);
			ast_h->StatementBlockAdd(b_node,stat); 
		}
		Expect(10 /* "}" */);
}

void Parser::VarDecl() {
		int c_declaraciones = 0;
		MetaData ** declaraciones = (MetaData**)malloc(sizeof(MetaData*)*100);
		string * Tipe = new string(""); 
		MetaData * md = new MetaData();
		VarDcl(md);
		declaraciones[c_declaraciones++]=md;
		while (la->kind == 19 /* "," */) {
			md = new MetaData();
			Get();
			VarDcl(md);
			declaraciones[c_declaraciones++]=md;
		}
		Expect(20 /* ":" */);
		Expect(_name);
		Tipe = new string(""); LexString(Tipe);
		
		cout << " " << *Tipe << endl<<endl; 
		
		for (int i = 0; i < c_declaraciones; i++)
		{
		
		Struct * tmp = last(declaraciones[i]->type);
		*tmp = *symbol_table::find(Tipe)->type;
		
		}
		
		for (int i = 0; i < c_declaraciones; i++)
		{
		
		cout<<";adr: "<<symbol_table::insert(
			Obj::Var,
			declaraciones[i]->tmpName,
			declaraciones[i]->type
		)->adr<<endl;;
		
		dump(declaraciones[i]->type,0);
		}
		
}

void Parser::Designator(AST&d_ast) {
		Obj*obj = new Obj();  string * ns=new string(""); Obj * oex = new Obj(); 
		if (la->kind == _name) {
			Get();
			LexString(ns); *obj=*symbol_table::find(ns); d_ast = ast_h->VarNode(ns); ast_h->SetObjOfNode(d_ast,obj); cout<<"; load "<<*ast_h->GetObjOfNode(d_ast)->name<<endl; 
		} else if (la->kind == _t_pointer) {
			Get();
			Factor(d_ast);
			d_ast = ast_h->DesfRefVarNode(d_ast); cout<<"; desreferenciar "<<*ast_h->GetObjOfNode(d_ast)->name<<endl; un_pointer(ast_h->GetObjOfNode(d_ast)); dump(ast_h->GetObjOfNode(d_ast)->type,2); 
		} else SynErr(49);
		if (StartOf(3)) {
			while (la->kind == 44 /* "." */) {
				Get();
				Expect(_name);
				LexString(ns); *obj=*symbol_table::findField(ns,obj->type); cout<<"; load "<<*obj->name<<endl; 
			}
		} else if (la->kind == 21 /* "[" */) {
			Get();
			Expr(d_ast);
			Expect(22 /* "]" */);
			un_pointer(obj); 
			while (la->kind == 21 /* "[" */) {
				Get();
				Expr(d_ast);
				Expect(22 /* "]" */);
				un_pointer(obj); 
			}
		} else SynErr(50);
}

void Parser::Expr(AST&e_node) {
		SimExpression(e_node);
}

void Parser::ActParams(AST&f_node,bool ifc) {
		Obj * obj = ast_h->GetObjOfNode(f_node);  int n = 0; f_node = ast_h->FuncCall(f_node,ifc);
		Expect(14 /* "(" */);
		if (StartOf(2)) {
			AST s_2;
			Expr(s_2);
			n++; ast_h->StatementBlockAdd(f_node,s_2); 
			while (la->kind == 19 /* "," */) {
				Get();
				AST s_2;
				Expr(s_2);
				n++; ast_h->StatementBlockAdd(f_node,s_2); 
			}
		}
		Expect(15 /* ")" */);
		if(n!=obj->nPars){
		cout<<";la funcion recibe n parametros: "<<obj->nPars<<" actual: "<<n<<  endl;
		exit(1);
		}
		
		unwrap_call_function(ast_h->GetObjOfNode(f_node));
		
}

void Parser::FuncDecl(AST&f_node) {
		int np=0; 
		bool extern_s = false;
		Obj *obj = new Obj();
		MetaData *fd = new MetaData();
		string * FName = new string("anonima_"); 
		FName->append(to_string(++cout_annon_func).c_str()); 
		string * Tipe = new string(""); 
		int anon = 1;
		if (la->kind == 23 /* "extern" */) {
			Get();
			extern_s = true;
		}
		Expect(24 /* "func" */);
		cout << "; fun "; 
		if (la->kind == _name) {
			Get();
			anon=0; FName = new string(""); LexString(FName);  
		}
		if(anon==1){
		cout<<" anonima retorna "<<*Tipe<<" " ;
		}else{
		cout<<*FName<<" retorna "; 
		} 
		
		obj=symbol_table::insert(4,FName,new Struct()); 
		
		
		symbol_table::pushCurrMethod(obj); 
		symbol_table::openScope(); 
		
		
		if (la->kind == 14 /* "(" */) {
			Get();
			if (StartOf(4)) {
				VarDecl();
				np++;
				while (la->kind == 19 /* "," */) {
					Get();
					VarDecl();
					np++;
				}
			}
			Expect(15 /* ")" */);
		}
		if (StartOf(5)) {
			FuncDcl(fd);
		}
		Expect(20 /* ":" */);
		Expect(_name);
		Tipe = new string(""); LexString(Tipe); *last(fd->type) = *symbol_table::find(Tipe)->type; cout<<";"<< *Tipe << endl; 
		funcion(fd->type); dump(fd->type,1); 
		
		obj->type = fd->type;
		(*obj).nPars = np;
		if(extern_s){
			obj->_extern = true;
			ss->add(obj->name);
		}
		
		f_node = ast_h->FuncDeclNode(FName);
		ast_h->SetObjOfNode(f_node,obj);
		
		
		
		cout<<";nPars "<<np<<endl;
		
		obj->nPars=np;;
		cout<<";nPars "<<obj->nPars<<endl;
		
		if (!extern_s) {
			StatementBLock(f_node);
		} else if (la->kind == 11 /* ";" */) {
			Get();
		} else SynErr(51);
		symbol_table::currMethod()->locals = symbol_table::curScope->locals;
		symbol_table::currMethod()->type->size = symbol_table::curScope->offset-8;
		
		symbol_table::popCurrMethod(); 
		symbol_table::closeScope(); 
}

void Parser::VarDcl(MetaData * md) {
		if (la->kind == _t_pointer) {
			Get();
			VarDcl(md);
			cout << " puntero a";
			puntero(last(md->type));
			
		} else if (StartOf(6)) {
			VarDirectDcl(md);
		} else SynErr(52);
}

void Parser::VarDirectDcl(MetaData * md) {
		string * Name = new string("");; 
		if (la->kind == _name) {
			Get();
			LexString(Name);
			cout <<endl<< "; " << *Name << " es"; 
			md->tmpName=Name;
			
		} else if (la->kind == 14 /* "(" */) {
			Get();
			VarDcl(md);
			Expect(15 /* ")" */);
		} else if (StartOf(7)) {
			cout <<endl<< "; noname " << " es";  
		} else SynErr(53);
		while (la->kind == 14 /* "(" */ || la->kind == 21 /* "[" */) {
			VarSuffix(md);
		}
}

void Parser::VarSuffix(MetaData * md) {
		if (la->kind == 21 /* "[" */) {
			string * buff = new string(""); 
			Get();
			cout << " array ["; 
			if (la->kind == _number) {
				Get();
				LexString(buff);
				cout << atoi(buff->c_str()); 
			}
			Expect(22 /* "]" */);
			_array(last(md->type)); cout << "] de"; 
		} else if (la->kind == 14 /* "(" */) {
			Get();
			if (StartOf(4)) {
				VarDecl();
				while (la->kind == 19 /* "," */) {
					Get();
					VarDecl();
				}
			}
			Expect(15 /* ")" */);
			funcion(last(md->type)); md->isF = true; ;cout << " ;function q retorna"; 
		} else SynErr(54);
}

void Parser::FuncDcl(MetaData * fd) {
		if (la->kind == _t_pointer) {
			Get();
			FuncDcl(fd);
			cout << "; puntero a"; 
			puntero(last(fd->type));
		} else if (StartOf(8)) {
			FuncDirectDcl(fd);
		} else SynErr(55);
}

void Parser::FuncDirectDcl(MetaData * fd) {
		string * Name = new string(""); cout<<""; 
		if (la->kind == _name) {
			Get();
		} else if (la->kind == 14 /* "(" */) {
			Get();
			FuncDcl(fd);
			Expect(15 /* ")" */);
		} else if (StartOf(9)) {
		} else SynErr(56);
		while (la->kind == 14 /* "(" */ || la->kind == 21 /* "[" */) {
			FuncSuffix(fd);
		}
}

void Parser::FuncSuffix(MetaData * fd) {
		if (la->kind == 21 /* "[" */) {
			string * buff = new string(""); 
			Get();
			cout << " array ["; 
			if (la->kind == _number) {
				Get();
				LexString(buff);
				cout << atoi(buff->c_str()); 
			}
			Expect(22 /* "]" */);
			_array(last(fd->type));  cout << "] de"; 
		} else if (la->kind == 14 /* "(" */) {
			Get();
			if (StartOf(4)) {
				VarDecl();
				while (la->kind == 19 /* "," */) {
					Get();
					VarDecl();
				}
			}
			Expect(15 /* ")" */);
			funcion(last(fd->type)); cout << " ;function q retorna"; 
		} else SynErr(57);
}

void Parser::SimExpression(AST &E) {
		BitShiffOp(E);
}

void Parser::BitShiffOp(AST &E) {
		AST T;  
		BitAndOrOp(E);
		while (la->kind == 25 /* "<<" */ || la->kind == 26 /* ">>" */) {
			if (la->kind == 25 /* "<<" */) {
				Get();
				BitAndOrOp(T);
				E = ast_h->BinOpNode(BitShiftL, E, T); 
			} else {
				Get();
				BitAndOrOp(T);
				E = ast_h->BinOpNode(BitShiftR, E, T); 
			}
		}
}

void Parser::BitAndOrOp(AST &E) {
		AST T;  
		if (la->kind == 27 /* "~" */) {
			Get();
		}
		AndOrOp(E);
		while (la->kind == _t_r_pointer || la->kind == 28 /* "|" */ || la->kind == 29 /* "^" */) {
			if (la->kind == _t_r_pointer) {
				Get();
				AndOrOp(T);
				E = ast_h->BinOpNode(BitAnd, E, T); 
			} else if (la->kind == 28 /* "|" */) {
				Get();
				AndOrOp(T);
				E = ast_h->BinOpNode(BitOr, E, T); 
			} else {
				Get();
				AndOrOp(T);
				E = ast_h->BinOpNode(BitXor, E, T); 
			}
		}
}

void Parser::AndOrOp(AST &E) {
		AST T;  
		RelaOp(E);
		while (la->kind == 30 /* "&&" */ || la->kind == 31 /* "||" */) {
			if (la->kind == 30 /* "&&" */) {
				Get();
				RelaOp(T);
				E = ast_h->BinOpNode(And, E, T); 
			} else {
				Get();
				RelaOp(T);
				E = ast_h->BinOpNode(Or, E, T); 
			}
		}
}

void Parser::RelaOp(AST &E) {
		AST T;  
		AddOp(E);
		while (StartOf(10)) {
			switch (la->kind) {
			case 32 /* "==" */: {
				Get();
				AddOp(T);
				E = ast_h->BinOpNode(Eq, E, T); 
				break;
			}
			case 33 /* "!=" */: {
				Get();
				AddOp(T);
				E = ast_h->BinOpNode(Neq, E, T); 
				break;
			}
			case 34 /* ">" */: {
				Get();
				AddOp(T);
				E = ast_h->BinOpNode(Gtr, E, T); 
				break;
			}
			case 35 /* ">=" */: {
				Get();
				AddOp(T);
				E = ast_h->BinOpNode(GtrEq, E, T); 
				break;
			}
			case 36 /* "<" */: {
				Get();
				AddOp(T);
				E = ast_h->BinOpNode(Less, E, T); 
				break;
			}
			case 37 /* "<=" */: {
				Get();
				AddOp(T);
				E = ast_h->BinOpNode(LessEq, E, T); 
				break;
			}
			}
		}
}

void Parser::AddOp(AST&t_node) {
		AST t_node_2; Obj*obj2 = new Obj(); optypes op; 
		Term(t_node);
		while (la->kind == 38 /* "+" */ || la->kind == 39 /* "-" */) {
			if (la->kind == 38 /* "+" */) {
				Get();
				op = Plus; 
			} else {
				Get();
				op = Minus; 
			}
			Term(t_node_2);
			t_node = ast_h->BinOpNode(op,t_node,t_node_2);
		}
}

void Parser::Term(AST&t_node) {
		AST t_node_2; Obj*obj2 = new Obj(); optypes op; 
		Factor(t_node);
		while (la->kind == _t_pointer || la->kind == 40 /* "/" */ || la->kind == 41 /* "%" */) {
			if (la->kind == _t_pointer) {
				Get();
				op = Times; 
			} else if (la->kind == 40 /* "/" */) {
				Get();
				op = Slash; 
			} else {
				Get();
				op = Mod; 
			}
			Factor(t_node_2);
			t_node = ast_h->BinOpNode(op,t_node,t_node_2); 
		}
}

void Parser::Factor(AST&f_node) {
		Obj* obj = new Obj(); string * ns=new string(""); string * buff = new string(""); 
		switch (la->kind) {
		case _t_pointer: case _name: {
			Designator(f_node);
			break;
		}
		case 42 /* "true" */: {
			Get();
			*obj=*new Obj(0,new string(""),symbol_table::boolType);  f_node = ast_h->ConstNode(1); ast_h->SetObjOfNode(f_node,obj);
			break;
		}
		case 43 /* "false" */: {
			Get();
			*obj=*new Obj(0,new string(""),symbol_table::boolType);  f_node = ast_h->ConstNode(0); ast_h->SetObjOfNode(f_node,obj);
			break;
		}
		case _t_r_pointer: {
			Get();
			Factor(f_node);
			cout<<"; referenciar "<<*ast_h->GetObjOfNode(f_node)->name<<endl;  
			make_ref(ast_h->GetObjOfNode(f_node));  /*dump(obj->type,2);*/ 
			f_node = ast_h->RefVarNode(f_node);
			
			break;
		}
		case _floatLit: {
			Get();
			LexString(buff);
			*obj=*symbol_table::insert(1,new string(""),symbol_table::floatType);
			//*obj=*new Obj(0,new string(""),symbol_table::floatType); 
			//obj->val = stof(buff->c_str());
			f_node = ast_h->ConstFloatNode(stof(buff->c_str()));
			//cout<<"float "<<stof(buff->c_str())<<endl;
			ast_h->SetObjOfNode(f_node,obj);
			
			break;
		}
		case _stringConst: {
			Get();
			LexString(buff);
			//*obj=*symbol_table::insert(0,new string(""),symbol_table::stringType);
			*obj=*new Obj(0,new string(""),symbol_table::stringType); 
			//make_ref(obj);
			string * key = sl->add(buff);
			
			//obj->val = stof(buff->c_str());
			f_node = ast_h->ConstStringNode(key);
			//cout<<"float "<<stof(buff->c_str())<<endl;
			ast_h->SetObjOfNode(f_node,obj);
			
			break;
		}
		case _number: {
			Get();
			LexString(buff);  
			*obj=*new Obj(0,new string(""),symbol_table::intType); 
			
			obj->val = atoi(buff->c_str());
			f_node = ast_h->ConstNode(obj->val);
			ast_h->SetObjOfNode(f_node,obj);
			
			break;
		}
		case 14 /* "(" */: {
			Get();
			Expr(f_node);
			Expect(15 /* ")" */);
			break;
		}
		case 23 /* "extern" */: case 24 /* "func" */: {
			FuncDecl(f_node);
			break;
		}
		default: SynErr(58); break;
		}
		while (la->kind == 14 /* "(" */) {
			ActParams(f_node,true);
			cout << " ;call "<<*ast_h->GetObjOfNode(f_node)->name<<endl; 
		}
}




// If the user declared a method Init and a mehtod Destroy they should
// be called in the contructur and the destructor respctively.
//
// The following templates are used to recognize if the user declared
// the methods Init and Destroy.

template<typename T>
struct ParserInitExistsRecognizer {
	template<typename U, void (U::*)() = &U::Init>
	struct ExistsIfInitIsDefinedMarker{};

	struct InitIsMissingType {
		char dummy1;
	};
	
	struct InitExistsType {
		char dummy1; char dummy2;
	};

	// exists always
	template<typename U>
	static InitIsMissingType is_here(...);

	// exist only if ExistsIfInitIsDefinedMarker is defined
	template<typename U>
	static InitExistsType is_here(ExistsIfInitIsDefinedMarker<U>*);

	enum { InitExists = (sizeof(is_here<T>(NULL)) == sizeof(InitExistsType)) };
};

template<typename T>
struct ParserDestroyExistsRecognizer {
	template<typename U, void (U::*)() = &U::Destroy>
	struct ExistsIfDestroyIsDefinedMarker{};

	struct DestroyIsMissingType {
		char dummy1;
	};
	
	struct DestroyExistsType {
		char dummy1; char dummy2;
	};

	// exists always
	template<typename U>
	static DestroyIsMissingType is_here(...);

	// exist only if ExistsIfDestroyIsDefinedMarker is defined
	template<typename U>
	static DestroyExistsType is_here(ExistsIfDestroyIsDefinedMarker<U>*);

	enum { DestroyExists = (sizeof(is_here<T>(NULL)) == sizeof(DestroyExistsType)) };
};

// The folloing templates are used to call the Init and Destroy methods if they exist.

// Generic case of the ParserInitCaller, gets used if the Init method is missing
template<typename T, bool = ParserInitExistsRecognizer<T>::InitExists>
struct ParserInitCaller {
	static void CallInit(T *t) {
		// nothing to do
	}
};

// True case of the ParserInitCaller, gets used if the Init method exists
template<typename T>
struct ParserInitCaller<T, true> {
	static void CallInit(T *t) {
		t->Init();
	}
};

// Generic case of the ParserDestroyCaller, gets used if the Destroy method is missing
template<typename T, bool = ParserDestroyExistsRecognizer<T>::DestroyExists>
struct ParserDestroyCaller {
	static void CallDestroy(T *t) {
		// nothing to do
	}
};

// True case of the ParserDestroyCaller, gets used if the Destroy method exists
template<typename T>
struct ParserDestroyCaller<T, true> {
	static void CallDestroy(T *t) {
		t->Destroy();
	}
};

void Parser::Parse() {
	t = NULL;
	la = dummyToken = new Token();
	la->val = coco_string_create(L"Dummy Token");
	Get();
	Pro();
	Expect(0);
}

Parser::Parser(Scanner *scanner) {
	maxT = 45;

	ParserInitCaller<Parser>::CallInit(this);
	dummyToken = NULL;
	t = la = NULL;
	minErrDist = 2;
	errDist = minErrDist;
	this->scanner = scanner;
	errors = new Errors(this->scanner);
}

bool Parser::StartOf(int s) {
	const bool T = true;
	const bool x = false;

	static bool set[11][47] = {
		{T,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x},
		{x,T,x,x, T,x,x,x, x,T,x,T, x,T,T,x, T,T,x,T, T,T,x,T, T,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x},
		{x,T,T,T, T,x,T,T, x,x,x,x, x,x,T,x, x,x,x,x, x,x,x,T, T,x,x,T, x,x,x,x, x,x,x,x, x,x,x,x, x,x,T,T, x,x,x},
		{x,T,T,x, x,x,x,x, x,x,x,T, T,x,T,T, x,x,x,T, x,T,T,x, x,T,T,x, T,T,T,T, T,T,T,T, T,T,T,T, T,T,x,x, T,x,x},
		{x,T,x,x, T,x,x,x, x,x,x,x, x,x,T,x, x,x,x,T, T,T,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x},
		{x,T,x,x, T,x,x,x, x,x,x,x, x,x,T,x, x,x,x,x, T,T,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x},
		{x,x,x,x, T,x,x,x, x,x,x,x, x,x,T,T, x,x,x,T, T,T,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x},
		{x,x,x,x, x,x,x,x, x,x,x,x, x,x,T,T, x,x,x,T, T,T,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x},
		{x,x,x,x, T,x,x,x, x,x,x,x, x,x,T,T, x,x,x,x, T,T,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x},
		{x,x,x,x, x,x,x,x, x,x,x,x, x,x,T,T, x,x,x,x, T,T,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x},
		{x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, T,T,T,T, T,T,x,x, x,x,x,x, x,x,x}
	};



	return set[s][la->kind];
}

Parser::~Parser() {
	ParserDestroyCaller<Parser>::CallDestroy(this);
	delete errors;
	delete dummyToken;
}

Errors::Errors() {
	count = 0;
}

Errors::Errors(Scanner *scanner)	{ 
	count = 0;
	this->scanner = scanner;
}

void Errors::printMsg(int line, int col, const wchar_t *s){
	string buff = "";
	int b,li;
	li = 1;
	scanner->buffer->SetPos(0);
	while(true){
	
		b =  scanner->buffer->Read(); 
		if(b == scanner->buffer->EoF)
		 	break;
		else{
			buff.append(string((char*)&b));
			if(b==10){

			
				if(li==line){

						printf(" <-- linea %d columna %d: %ls\n", line, col, s);

				}
				if(li >= line-2 && li <= line+2)
					cout<<endl;
				
				li++;
			}else{
				if(li >= line-2 && li <= line+2)
					cout<<(char)b;
			}
		}
	}
	
			
	//printf("%ls\n",	scanner->buffer->GetString(scanner->buffer->GetPos()-(line+col),scanner->buffer->GetPos()+(line+col)));
	
	//cout<<buff<<endl;
}

void Errors::SynErr(int line, int col, int n) {
	wchar_t* s;
	switch (n) {
			case 0: s = coco_string_create(L"EOF expected"); break;
			case 1: s = coco_string_create(L"t_pointer expected"); break;
			case 2: s = coco_string_create(L"t_r_pointer expected"); break;
			case 3: s = coco_string_create(L"number expected"); break;
			case 4: s = coco_string_create(L"name expected"); break;
			case 5: s = coco_string_create(L"charConst expected"); break;
			case 6: s = coco_string_create(L"stringConst expected"); break;
			case 7: s = coco_string_create(L"floatLit expected"); break;
			case 8: s = coco_string_create(L"binaryLit expected"); break;
			case 9: s = coco_string_create(L"\"{\" expected"); break;
			case 10: s = coco_string_create(L"\"}\" expected"); break;
			case 11: s = coco_string_create(L"\";\" expected"); break;
			case 12: s = coco_string_create(L"\"=\" expected"); break;
			case 13: s = coco_string_create(L"\"print\" expected"); break;
			case 14: s = coco_string_create(L"\"(\" expected"); break;
			case 15: s = coco_string_create(L"\")\" expected"); break;
			case 16: s = coco_string_create(L"\"ret\" expected"); break;
			case 17: s = coco_string_create(L"\"if\" expected"); break;
			case 18: s = coco_string_create(L"\"else\" expected"); break;
			case 19: s = coco_string_create(L"\",\" expected"); break;
			case 20: s = coco_string_create(L"\":\" expected"); break;
			case 21: s = coco_string_create(L"\"[\" expected"); break;
			case 22: s = coco_string_create(L"\"]\" expected"); break;
			case 23: s = coco_string_create(L"\"extern\" expected"); break;
			case 24: s = coco_string_create(L"\"func\" expected"); break;
			case 25: s = coco_string_create(L"\"<<\" expected"); break;
			case 26: s = coco_string_create(L"\">>\" expected"); break;
			case 27: s = coco_string_create(L"\"~\" expected"); break;
			case 28: s = coco_string_create(L"\"|\" expected"); break;
			case 29: s = coco_string_create(L"\"^\" expected"); break;
			case 30: s = coco_string_create(L"\"&&\" expected"); break;
			case 31: s = coco_string_create(L"\"||\" expected"); break;
			case 32: s = coco_string_create(L"\"==\" expected"); break;
			case 33: s = coco_string_create(L"\"!=\" expected"); break;
			case 34: s = coco_string_create(L"\">\" expected"); break;
			case 35: s = coco_string_create(L"\">=\" expected"); break;
			case 36: s = coco_string_create(L"\"<\" expected"); break;
			case 37: s = coco_string_create(L"\"<=\" expected"); break;
			case 38: s = coco_string_create(L"\"+\" expected"); break;
			case 39: s = coco_string_create(L"\"-\" expected"); break;
			case 40: s = coco_string_create(L"\"/\" expected"); break;
			case 41: s = coco_string_create(L"\"%\" expected"); break;
			case 42: s = coco_string_create(L"\"true\" expected"); break;
			case 43: s = coco_string_create(L"\"false\" expected"); break;
			case 44: s = coco_string_create(L"\".\" expected"); break;
			case 45: s = coco_string_create(L"??? expected"); break;
			case 46: s = coco_string_create(L"invalid Statement"); break;
			case 47: s = coco_string_create(L"invalid Statement"); break;
			case 48: s = coco_string_create(L"invalid Statement"); break;
			case 49: s = coco_string_create(L"invalid Designator"); break;
			case 50: s = coco_string_create(L"invalid Designator"); break;
			case 51: s = coco_string_create(L"invalid FuncDecl"); break;
			case 52: s = coco_string_create(L"invalid VarDcl"); break;
			case 53: s = coco_string_create(L"invalid VarDirectDcl"); break;
			case 54: s = coco_string_create(L"invalid VarSuffix"); break;
			case 55: s = coco_string_create(L"invalid FuncDcl"); break;
			case 56: s = coco_string_create(L"invalid FuncDirectDcl"); break;
			case 57: s = coco_string_create(L"invalid FuncSuffix"); break;
			case 58: s = coco_string_create(L"invalid Factor"); break;

		default:
		{
			wchar_t format[20];
			coco_swprintf(format, 20, L"error %d", n);
			s = coco_string_create(format);
		}
		break;
	}
	printMsg(line, col, s);
	coco_string_delete(s);
	count++;
}

void Errors::Error(int line, int col, const wchar_t *s) {
	
	printMsg(line, col, s);
	count++;
}

void Errors::Warning(int line, int col, const wchar_t *s) {
	printMsg(line, col, s);
}
void Errors::Warning(const wchar_t *s) {
	printf("%ls\n", s);
}

void Errors::Exception(const wchar_t* s) {
	printf("%ls", s); 
	exit(1);
}


