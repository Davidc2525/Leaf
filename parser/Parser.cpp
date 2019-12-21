

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
		ASTU b = block_stat(); 
		SourceCode(b);
		visitor_generator->init(b); 
}

void Parser::SourceCode(ASTU&b_node) {
		
		while (StartOf(1)) {
			ASTU stat;
			Statement(stat);
			block_add(b_node,stat);
		}
}

void Parser::Statement(ASTU&stat) {
		ASTU e_stat;
		switch (la->kind) {
		case _t_pointer: case _name: case 14 /* "(" */: case 20 /* "," */: case 21 /* ":" */: case 22 /* "[" */: {
			if (isVarDecl()) {
				VarDecl();
				Expect(11 /* ";" */);
				stat = empty_node(); 
			} else {
				Designator(stat);
				if (la->kind == 12 /* "=" */) {
					Get();
					Expr(e_stat);
					if(!sreq(get_obj(stat)->type,get_obj(e_stat)->type)){
					cout<<";error de asignacion, tipos diferentes."<<endl; 
					dump(get_obj(stat)->type,2);
					dump(get_obj(e_stat)->type,2);
					exit(1);
					}else{
					stat = assign(stat,e_stat);
					Obj*o = new Obj(1,new string(),new Struct()); 
					set_obj(stat,o);
					} 
					
					
				} else if (la->kind == 14 /* "(" */) {
					ActParams(stat,false);
				} else SynErr(47);
				Expect(11 /* ";" */);
			}
			break;
		}
		case 24 /* "extern" */: case 25 /* "fun" */: {
			FuncDecl(stat);
			break;
		}
		case 13 /* "print" */: {
			Get();
			Expect(14 /* "(" */);
			Expr(e_stat);
			Expect(15 /* ")" */);
			Expect(11 /* ";" */);
			stat = print_node(e_stat);
			break;
		}
		case 16 /* "sizeof" */: {
			Get();
			Expect(14 /* "(" */);
			Expr(e_stat);
			Expect(15 /* ")" */);
			break;
		}
		case 17 /* "ret" */: {
			Get();
			if (StartOf(2)) {
				Expr(e_stat);
				if(!sreq(symbol_table::currMethod()->type->elemType,get_obj(e_stat)->type)){cout<<";El valor q retorna no coincide con la firma. "<<*symbol_table::currMethod()->name<<endl; exit(1);}
				stat = return_node(e_stat); set_obj(stat,get_obj(e_stat)); 
				
			} else if (la->kind == 11 /* ";" */) {
				stat = return_node();
			} else SynErr(48);
			Expect(11 /* ";" */);
			break;
		}
		case 18 /* "if" */: {
			Get();
			Expect(14 /* "(" */);
			Expr(e_stat);
			Expect(15 /* ")" */);
			stat = if_node(e_stat); 
			StatementBLock(stat);
			if (la->kind == 19 /* "else" */) {
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
		default: SynErr(49); break;
		}
}

void Parser::StatementBLock(ASTU&b_node) {
		
		Expect(9 /* "{" */);
		while (StartOf(1)) {
			ASTU stat;
			Statement(stat);
			block_add(b_node,stat); 
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
		while (la->kind == 20 /* "," */) {
			md = new MetaData();
			Get();
			VarDcl(md);
			declaraciones[c_declaraciones++]=md;
		}
		Expect(21 /* ":" */);
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

void Parser::Designator(ASTU&d_ast) {
		Obj*obj = new Obj();  string * ns=new string(""); Obj * oex = new Obj(); 
		if (la->kind == _name) {
			Get();
			LexString(ns); 
			*obj=*symbol_table::find(ns); 
			ASTU nc = ident(ns); 
			set_obj(nc,obj); 
			d_ast=nc;
			cout<<"; load "<<*get_obj(nc)->name<<", adr "<<get_obj(nc)->adr<<endl; 
		} else if (la->kind == _t_pointer) {
			Get();
			Factor(d_ast);
			d_ast = (d_ast); cout<<"; desreferenciar "<<*get_obj(d_ast)->name<<endl; un_pointer(get_obj(d_ast)); dump(get_obj(d_ast)->type,2); 
		} else SynErr(50);
		if (StartOf(3)) {
			while (la->kind == 45 /* "." */) {
				Get();
				Expect(_name);
				LexString(ns); *obj=*symbol_table::findField(ns,obj->type); cout<<"; load "<<*obj->name<<endl; 
			}
		} else if (la->kind == 22 /* "[" */) {
			Get();
			Expr(d_ast);
			Expect(23 /* "]" */);
			un_pointer(obj); 
			while (la->kind == 22 /* "[" */) {
				Get();
				Expr(d_ast);
				Expect(23 /* "]" */);
				un_pointer(obj); 
			}
		} else SynErr(51);
}

void Parser::Expr(ASTU&e_node) {
		SimExpression(e_node);
}

void Parser::ActParams(ASTU&f_node,bool ifc) {
		Obj * obj = get_obj(f_node);  int n = 0; f_node = func_call(f_node,ifc);
		Expect(14 /* "(" */);
		if (StartOf(2)) {
			ASTU s_2;
			Expr(s_2);
			n++; block_add(f_node,s_2); 
			while (la->kind == 20 /* "," */) {
				Get();
				ASTU s_2;
				Expr(s_2);
				n++; block_add(f_node,s_2); 
			}
		}
		Expect(15 /* ")" */);
		if(n!=obj->nPars){
		cout<<";la funcion recibe n parametros: "<<obj->nPars<<" actual: "<<n<<  endl;
		exit(1);
		}
		
		unwrap_call_function(get_obj(f_node));
		
}

void Parser::FuncDecl(ASTU&f_node) {
		int np=0; 
		bool extern_s = false;
		Obj *obj = new Obj();
		MetaData *fd = new MetaData();
		string * FName = new string("anonima_"); 
		FName->append(to_string(++cout_annon_func).c_str()); 
		string * Tipe = new string(""); 
		int anon = 1;
		if (la->kind == 24 /* "extern" */) {
			Get();
			extern_s = true;
		}
		Expect(25 /* "fun" */);
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
				while (la->kind == 20 /* "," */) {
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
		Expect(21 /* ":" */);
		Expect(_name);
		Tipe = new string(""); LexString(Tipe); *last(fd->type) = *symbol_table::find(Tipe)->type; cout<<";"<< *Tipe << endl; 
		funcion(fd->type); dump(fd->type,1); 
		
		obj->type = fd->type;
		(*obj).nPars = np;
		if(extern_s){
			obj->_extern = true;
			//ss->add(obj->name);
		}
		
		f_node = func_decl(FName);
		set_obj(f_node,obj);
		
		
		
		cout<<";nPars "<<np<<endl;
		
		obj->nPars=np;;
		cout<<";nPars "<<obj->nPars<<endl;
		
		if (!extern_s) {
			StatementBLock(f_node);
		} else if (la->kind == 11 /* ";" */) {
			Get();
		} else SynErr(52);
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
		} else SynErr(53);
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
		} else SynErr(54);
		while (la->kind == 14 /* "(" */ || la->kind == 22 /* "[" */) {
			VarSuffix(md);
		}
}

void Parser::VarSuffix(MetaData * md) {
		if (la->kind == 22 /* "[" */) {
			string * buff = new string(""); 
			Get();
			cout << " array ["; 
			if (la->kind == _number) {
				Get();
				LexString(buff);
				cout << atoi(buff->c_str()); 
			}
			Expect(23 /* "]" */);
			_array(last(md->type)); cout << "] de"; 
		} else if (la->kind == 14 /* "(" */) {
			Get();
			if (StartOf(4)) {
				VarDecl();
				while (la->kind == 20 /* "," */) {
					Get();
					VarDecl();
				}
			}
			Expect(15 /* ")" */);
			funcion(last(md->type)); md->isF = true; ;cout << " ;function q retorna"; 
		} else SynErr(55);
}

void Parser::FuncDcl(MetaData * fd) {
		if (la->kind == _t_pointer) {
			Get();
			FuncDcl(fd);
			cout << "; puntero a"; 
			puntero(last(fd->type));
		} else if (StartOf(8)) {
			FuncDirectDcl(fd);
		} else SynErr(56);
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
		} else SynErr(57);
		while (la->kind == 14 /* "(" */ || la->kind == 22 /* "[" */) {
			FuncSuffix(fd);
		}
}

void Parser::FuncSuffix(MetaData * fd) {
		if (la->kind == 22 /* "[" */) {
			string * buff = new string(""); 
			Get();
			cout << " array ["; 
			if (la->kind == _number) {
				Get();
				LexString(buff);
				cout << atoi(buff->c_str()); 
			}
			Expect(23 /* "]" */);
			_array(last(fd->type));  cout << "] de"; 
		} else if (la->kind == 14 /* "(" */) {
			Get();
			if (StartOf(4)) {
				VarDecl();
				while (la->kind == 20 /* "," */) {
					Get();
					VarDecl();
				}
			}
			Expect(15 /* ")" */);
			funcion(last(fd->type)); cout << " ;function q retorna"; 
		} else SynErr(58);
}

void Parser::SimExpression(ASTU &E) {
		BitShiffOp(E);
}

void Parser::BitShiffOp(ASTU &E) {
		ASTU T;  
		BitAndOrOp(E);
		while (la->kind == 26 /* "<<" */ || la->kind == 27 /* ">>" */) {
			if (la->kind == 26 /* "<<" */) {
				Get();
				BitAndOrOp(T);
				E = bin_op(BitShiftL, E, T); 
			} else {
				Get();
				BitAndOrOp(T);
				E = bin_op(BitShiftR, E, T); 
			}
		}
}

void Parser::BitAndOrOp(ASTU &E) {
		ASTU T;  
		if (la->kind == 28 /* "~" */) {
			Get();
		}
		AndOrOp(E);
		while (la->kind == _t_r_pointer || la->kind == 29 /* "|" */ || la->kind == 30 /* "^" */) {
			if (la->kind == _t_r_pointer) {
				Get();
				AndOrOp(T);
				E = bin_op(BitAnd, E, T); 
			} else if (la->kind == 29 /* "|" */) {
				Get();
				AndOrOp(T);
				E = bin_op(BitOr, E, T); 
			} else {
				Get();
				AndOrOp(T);
				E = bin_op(BitXor, E, T); 
			}
		}
}

void Parser::AndOrOp(ASTU &E) {
		ASTU T;  
		RelaOp(E);
		while (la->kind == 31 /* "&&" */ || la->kind == 32 /* "||" */) {
			if (la->kind == 31 /* "&&" */) {
				Get();
				RelaOp(T);
				E = bin_op(And, E, T); 
			} else {
				Get();
				RelaOp(T);
				E = bin_op(Or, E, T); 
			}
		}
}

void Parser::RelaOp(ASTU &E) {
		ASTU T;  
		AddOp(E);
		while (StartOf(10)) {
			switch (la->kind) {
			case 33 /* "==" */: {
				Get();
				AddOp(T);
				E = bin_op(Eq, E, T); 
				break;
			}
			case 34 /* "!=" */: {
				Get();
				AddOp(T);
				E = bin_op(Neq, E, T); 
				break;
			}
			case 35 /* ">" */: {
				Get();
				AddOp(T);
				E = bin_op(Gtr, E, T); 
				break;
			}
			case 36 /* ">=" */: {
				Get();
				AddOp(T);
				E = bin_op(GtrEq, E, T); 
				break;
			}
			case 37 /* "<" */: {
				Get();
				AddOp(T);
				E = bin_op(Less, E, T); 
				break;
			}
			case 38 /* "<=" */: {
				Get();
				AddOp(T);
				E = bin_op(LessEq, E, T); 
				break;
			}
			}
		}
}

void Parser::AddOp(ASTU&t_node) {
		ASTU t_node_2; Obj*obj2 = new Obj(); op_types op; 
		Term(t_node);
		while (la->kind == 39 /* "+" */ || la->kind == 40 /* "-" */) {
			if (la->kind == 39 /* "+" */) {
				Get();
				op = Plus; 
			} else {
				Get();
				op = Minus; 
			}
			Term(t_node_2);
			t_node = bin_op(op,t_node,t_node_2);
		}
}

void Parser::Term(ASTU&t_node) {
		ASTU t_node_2; Obj*obj2 = new Obj(); op_types op; 
		Factor(t_node);
		while (la->kind == _t_pointer || la->kind == 41 /* "/" */ || la->kind == 42 /* "%" */) {
			if (la->kind == _t_pointer) {
				Get();
				op = Times; 
			} else if (la->kind == 41 /* "/" */) {
				Get();
				op = Slash; 
			} else {
				Get();
				op = Mod; 
			}
			Factor(t_node_2);
			t_node = bin_op(op,t_node,t_node_2); 
		}
}

void Parser::Factor(ASTU&f_node) {
		Obj* obj = new Obj(); string * ns=new string(""); string * buff = new string(""); 
		switch (la->kind) {
		case _t_pointer: case _name: {
			Designator(f_node);
			break;
		}
		case 43 /* "true" */: {
			Get();
			*obj=*new Obj(0,new string(""),symbol_table::boolType);  f_node = const_int_node(1); set_obj(f_node,obj);
			break;
		}
		case 44 /* "false" */: {
			Get();
			*obj=*new Obj(0,new string(""),symbol_table::boolType);  f_node = const_int_node(0); set_obj(f_node,obj);
			break;
		}
		case _t_r_pointer: {
			Get();
			Factor(f_node);
			cout<<"; referenciar "<<*get_obj(f_node)->name<<endl;  
			make_ref(get_obj(f_node));  /*dump(obj->type,2);*/ 
			f_node = (f_node);
			
			break;
		}
		case _floatLit: {
			Get();
			LexString(buff);
			*obj=*symbol_table::insert(1,new string(""),symbol_table::floatType);
			//*obj=*new Obj(0,new string(""),symbol_table::floatType); 
			//obj->val = stof(buff->c_str());
			f_node = const_float_node(stof(buff->c_str()));
			//cout<<"float "<<stof(buff->c_str())<<endl;
			set_obj(f_node,obj);
			
			break;
		}
		case _stringConst: {
			Get();
			LexString(buff);
			//*obj=*symbol_table::insert(0,new string(""),symbol_table::stringType);
			*obj=*new Obj(0,new string(""),symbol_table::stringType); 
			//make_ref(obj);
			//string * key = sl->add(buff);
			
			//obj->val = stof(buff->c_str());
			//f_node = const_string_node(key);
			//cout<<"float "<<stof(buff->c_str())<<endl;
			//set_obj(f_node,obj);
			
			break;
		}
		case _number: {
			Get();
			LexString(buff);  
			*obj=*new Obj(0,new string(""),symbol_table::intType); 
			
			obj->val = atoi(buff->c_str());
			f_node = const_int_node(obj->val);
			set_obj(f_node,obj);
			
			break;
		}
		case 14 /* "(" */: {
			Get();
			Expr(f_node);
			Expect(15 /* ")" */);
			break;
		}
		case 24 /* "extern" */: case 25 /* "fun" */: {
			FuncDecl(f_node);
			break;
		}
		default: SynErr(59); break;
		}
		while (la->kind == 14 /* "(" */) {
			ActParams(f_node,true);
			cout << " ;call "<<*get_obj(f_node)->name<<endl; 
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
	maxT = 46;

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

	static bool set[11][48] = {
		{T,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x},
		{x,T,x,x, T,x,x,x, x,T,x,T, x,T,T,x, T,T,T,x, T,T,T,x, T,T,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x},
		{x,T,T,T, T,x,T,T, x,x,x,x, x,x,T,x, x,x,x,x, x,x,x,x, T,T,x,x, T,x,x,x, x,x,x,x, x,x,x,x, x,x,x,T, T,x,x,x},
		{x,T,T,x, x,x,x,x, x,x,x,T, T,x,T,T, x,x,x,x, T,x,T,T, x,x,T,T, x,T,T,T, T,T,T,T, T,T,T,T, T,T,T,x, x,T,x,x},
		{x,T,x,x, T,x,x,x, x,x,x,x, x,x,T,x, x,x,x,x, T,T,T,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x},
		{x,T,x,x, T,x,x,x, x,x,x,x, x,x,T,x, x,x,x,x, x,T,T,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x},
		{x,x,x,x, T,x,x,x, x,x,x,x, x,x,T,T, x,x,x,x, T,T,T,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x},
		{x,x,x,x, x,x,x,x, x,x,x,x, x,x,T,T, x,x,x,x, T,T,T,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x},
		{x,x,x,x, T,x,x,x, x,x,x,x, x,x,T,T, x,x,x,x, x,T,T,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x},
		{x,x,x,x, x,x,x,x, x,x,x,x, x,x,T,T, x,x,x,x, x,T,T,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x},
		{x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,x,x,x, x,T,T,T, T,T,T,x, x,x,x,x, x,x,x,x}
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
			case 16: s = coco_string_create(L"\"sizeof\" expected"); break;
			case 17: s = coco_string_create(L"\"ret\" expected"); break;
			case 18: s = coco_string_create(L"\"if\" expected"); break;
			case 19: s = coco_string_create(L"\"else\" expected"); break;
			case 20: s = coco_string_create(L"\",\" expected"); break;
			case 21: s = coco_string_create(L"\":\" expected"); break;
			case 22: s = coco_string_create(L"\"[\" expected"); break;
			case 23: s = coco_string_create(L"\"]\" expected"); break;
			case 24: s = coco_string_create(L"\"extern\" expected"); break;
			case 25: s = coco_string_create(L"\"fun\" expected"); break;
			case 26: s = coco_string_create(L"\"<<\" expected"); break;
			case 27: s = coco_string_create(L"\">>\" expected"); break;
			case 28: s = coco_string_create(L"\"~\" expected"); break;
			case 29: s = coco_string_create(L"\"|\" expected"); break;
			case 30: s = coco_string_create(L"\"^\" expected"); break;
			case 31: s = coco_string_create(L"\"&&\" expected"); break;
			case 32: s = coco_string_create(L"\"||\" expected"); break;
			case 33: s = coco_string_create(L"\"==\" expected"); break;
			case 34: s = coco_string_create(L"\"!=\" expected"); break;
			case 35: s = coco_string_create(L"\">\" expected"); break;
			case 36: s = coco_string_create(L"\">=\" expected"); break;
			case 37: s = coco_string_create(L"\"<\" expected"); break;
			case 38: s = coco_string_create(L"\"<=\" expected"); break;
			case 39: s = coco_string_create(L"\"+\" expected"); break;
			case 40: s = coco_string_create(L"\"-\" expected"); break;
			case 41: s = coco_string_create(L"\"/\" expected"); break;
			case 42: s = coco_string_create(L"\"%\" expected"); break;
			case 43: s = coco_string_create(L"\"true\" expected"); break;
			case 44: s = coco_string_create(L"\"false\" expected"); break;
			case 45: s = coco_string_create(L"\".\" expected"); break;
			case 46: s = coco_string_create(L"??? expected"); break;
			case 47: s = coco_string_create(L"invalid Statement"); break;
			case 48: s = coco_string_create(L"invalid Statement"); break;
			case 49: s = coco_string_create(L"invalid Statement"); break;
			case 50: s = coco_string_create(L"invalid Designator"); break;
			case 51: s = coco_string_create(L"invalid Designator"); break;
			case 52: s = coco_string_create(L"invalid FuncDecl"); break;
			case 53: s = coco_string_create(L"invalid VarDcl"); break;
			case 54: s = coco_string_create(L"invalid VarDirectDcl"); break;
			case 55: s = coco_string_create(L"invalid VarSuffix"); break;
			case 56: s = coco_string_create(L"invalid FuncDcl"); break;
			case 57: s = coco_string_create(L"invalid FuncDirectDcl"); break;
			case 58: s = coco_string_create(L"invalid FuncSuffix"); break;
			case 59: s = coco_string_create(L"invalid Factor"); break;

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


