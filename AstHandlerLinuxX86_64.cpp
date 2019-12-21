#include "AstHandlerLinuxX86_64.hpp"

namespace CODE_GENERATOR
{
namespace x86_64
{

int orp = 15;//offset register params

void randomString(int size, char* output) // pass the destination size and the destination itself
  {
   
      srand(time(NULL)); // seed with time

      char src[size];
      size = rand() % size; // this randomises the size (optional)

      src[size] = '\0'; // start with the end of the string...

      // ...and work your way backwards
      while(--size > -1)
          src[size] = (rand() % 94) + 32; // generate a string ranging from the space character to ~ (tilde)

      strcpy(output, src); // store the random string
  };

string * _to_reg_(int r,int offset){
  
 //return new string("R"+to_string(r));
  
  switch(r+offset){
      case 1:{
          return new string("rax");
      }        
      case 2:{
          return new string("rbx");
      }        
      case 3:{
          return new string("rcx");
      }        
      case 4:{
          return new string("rdx");
      }case 5:{
          return new string("rdi");
      }case 6:{
          return new string("rsi");
      }case 7:{
          return new string("r8");
      }case 8:{
          return new string("r9");
      }case 9:{
          return new string("r10");
      }case 10:{
          return new string("r11");
      }case 11:{
          return new string("r12");
      }case 12:{
          return new string("r13");
      }case 13:{
          return new string("r14");
      }case 14:{
          return new string("r15");
      }
      case 15:{//parametros
          return new string("rdi");
      }case 16:{ 
          return new string("rsi");
      }case 17:{
          return new string("rdx");
      }case 18:{
          return new string("rcx");
      }case 19:{
          return new string("r8");
      }case 20:{
          return new string("r9");
      }
  }
  
}

string * _to_reg_(int r){
  return _to_reg_(r,0);
}


bool StructEq(Struct*o1,Struct*o2){
  if(o1->elemType!=NULL && o2->elemType!=NULL){
    if(o1->kind==o2->kind){
      StructEq(o1->elemType,o2->elemType);
    }else{
      return false;
    } 
  }else{
    if(o1->elemType==NULL && o2->elemType!=NULL){
      return false;
    }else if(o2->elemType==NULL && o1->elemType!=NULL){
      return false;
    }
    return o1->kind==o2->kind;
  }
}


class INTTOFLOATNODE:public EXPR{
  public:
    INTTOFLOATNODE(AST i){
      child = i;
      
    }
    AST child;
    virtual void load(int R) ;
    virtual void lea(int R)  { operation(Lea, R); }
    virtual void operation(optypes O, int R);
};
void INTTOFLOATNODE::load(int r){
  child->loadPlace = S_FPU;
  child->load(r);
  
  //cout<<"cvtsi2ss xmm0,"<<*_to_reg_(r)<<endl;  
  cout<<";coercion int to float, load: "<<*_to_reg_(r)<<endl;
  //operation(Load,r);
}
void INTTOFLOATNODE::operation(optypes o,int r){
  child->loadPlace = S_FPU;
    child->load(r);
    cout<<";coercion int to float,op: "<<*_to_reg_(r)<<endl;
}
 /////////////////////////////////////////////////////////////
 
  
class BINOPNODE : public EXPR
{ public:
  BINOPNODE(optypes O, EXPR* L, EXPR* R)   { 
    op = O; left = L; right = R;
    if(!StructEq(left->obj->type,right->obj->type)){

      if(
        *left->obj->type == *symbol_table::intType &&
        *right->obj->type == *symbol_table::floatType
        ){
           BINOPNODE* r = dynamic_cast<BINOPNODE*>(left);
          if(r!=nullptr){
            
          //r->left->obj->type = left->obj->type;
          //r->right->obj->type = left->obj->type;
            r->left->loadPlace = S_FPU;
            r->right->loadPlace = S_FPU;
            //left->obj->type = left->obj->type;
          }
          
          //left->obj->type = right->obj->type;
          left->loadPlace = S_FPU;
          right->loadPlace = S_FPU;
          obj->type = right->obj->type;
      }else if(
        *left->obj->type == *symbol_table::floatType &&
        *right->obj->type == *symbol_table::intType
        ){
          
          BINOPNODE* r = dynamic_cast<BINOPNODE*>(right);
          if(r!=nullptr){
            
          //r->left->obj->type = left->obj->type;
          //r->right->obj->type = left->obj->type;
            r->left->loadPlace = S_FPU;
            r->right->loadPlace = S_FPU;
            //right->obj->type = left->obj->type;
          }
          left->loadPlace = S_FPU;
          right->loadPlace = S_FPU;

        obj->type = left->obj->type;
      }else if(
        *left->obj->type == *symbol_table::floatType &&
        *right->obj->type == *symbol_table::floatType
        ){
           left->loadPlace = S_FPU;
          right->loadPlace = S_FPU;
           obj->type = left->obj->type;
      }else{
        cout<<"error de aritmetico, error de tipos"<<endl; 
        exit(1);
      }

    }else{
      obj->type=left->obj->type;
      if(left->obj->type->kind == Struct::Float || loadPlace==S_FPU){
        left->loadPlace = S_FPU;
        right->loadPlace = S_FPU;
      }
    }
  }
    virtual void load(int R);
  //protected:
    optypes op;
    EXPR* left, *right;
    void operation(optypes O, int R);
    void loadreg(int R){dlog("LOAD REG IWTH LOAD "+to_string(R),0); load(R); }
};

void BINOPNODE::operation(optypes op, int R)
{ 
  
  if(loadPlace != S_FPU){
    switch (op)
  { 
    case Store: printf("STO "); break;
    case Load:  printf("LDA"); break;
    case Plus:  printf("ADD"); break;
    case Minus: printf("SUB"); break;
    case Times: printf("IMUL"); break;
    //case Slash: printf("IDVD"); break;
    case BitOr: printf("OR"); break;
    case BitAnd: printf("AND"); break;
    case BitXor: printf("XOR"); break;
    
    //default: cout<<";"<<op; break; 

  }
   if(op == Slash||op == Mod){
     //Register*RAX = RegisterManager::getInstance()->getProviderBy(64)->reserve_by_name("RAX");
      cout<<"MOV rdx, 0   ; clear dividend, var"<<endl;
     
      //cout<<"MOV "<<RAX->getName()<<", "<<*_to_reg_(R)<<"   ; dividend"<<endl;
      cout<<"MOV rax, "<<*_to_reg_(R)<<"   ; dividend"<<endl;

      cout<<"MOV rcx, "<<*_to_reg_(R+1)<<"   ; divisor"<<endl;
      cout<<"IDIV rcx     ; EAX = resultado, EDX = modulo"<<endl;
      
      //cout<<"MOV [rbp-"<<(obj->adr)<<"],"<<*_to_reg_(op == Slash?1:4)<<endl;
      cout<<"MOV "<<*_to_reg_(R)<<","<<*_to_reg_(op == Slash?1:4)<<endl;


    }else{
      cout<<" " <<*_to_reg_(R)<<","<<*_to_reg_(R+1)<<" ;bin"<<endl;
    }
  }else{//load place == fpu
    if(op==Plus)
        cout<<"fadd; bin"<<endl;
      
      if(op==Minus)
      cout<<"fsub; bin"<<endl;

        if(op==Slash)
      cout<<"fdiv; bin"<<endl;

        if(op==Times)
      cout<<"fmul; bin"<<endl;
  }

  //if(op != Store) cout<<" R"<<R<<",R"<<R+1<<endl;
  //if(op == Store) cout<<" "<<*name<<",R"<<R<<endl;
  //obj->type->kind=2;
  //printf(" R%d,R%d\n", R, R + 1);
}

 
void BINOPNODE::load(int R)
{ 
 
  if (!left || !right) return;
  //cout<<";IFRETURNOFFUNC left "<<left->ifReturnOfFunc<<endl;
  //cout<<";IFRETURNOFFUNC right "<<right->ifReturnOfFunc<<endl;
  
  if(right->ifReturnOfFunc && left->ifReturnOfFunc){
      
      right->load(R);
      right->store(R+1);
      left->loadreg(R+1); left->operation(op, R);
  }else{

    if(right->ifReturnOfFunc && (op != Slash)&&(op != Mod)){
      
      right->load(R);
      left->loadreg(R+1); left->operation(op, R);

    }else{
      left->load(R); right->loadreg(R+1); right->operation(op, R);
    }
  }

  delete left; delete right;
}


AST AstHandlerLinuxX86_64::BinOpNode(optypes op,AST left,AST right)
{ if (false&&left && right && left->defined && right->defined)
  { // constant folding
    switch (op)
    { case Plus:  left->value += right->value; break;
      case Minus: left->value -= right->value; break;
      case Times: left->value *= right->value; break;
      case Slash: left->value /= right->value; break;
      case Mod: left->value %= right->value; break;
    }
    delete right; return left;
  }

  if(!StructEq(left->obj->type,right->obj->type)){

      if(
        *left->obj->type == *symbol_table::intType &&
        *right->obj->type == *symbol_table::floatType
        ){

          INTTOFLOATNODE*nk = new INTTOFLOATNODE(left);
          nk->obj->type = right->obj->type;
          left = nk;
          
      }else if(
        *left->obj->type == *symbol_table::floatType &&
        *right->obj->type == *symbol_table::intType
        ){
          
           INTTOFLOATNODE*nk = new INTTOFLOATNODE(right);
          nk->obj->type = left->obj->type;
          right = nk;
        
      }else if(
        *left->obj->type == *symbol_table::floatType &&
        *right->obj->type == *symbol_table::floatType
        ){
          
      }else{
        cout<<"error de aritmetico, error de tipos"<<endl; 
        exit(1);
      }

    }

  return new BINOPNODE(op, (EXPR*)left, (EXPR*)right);
}


 /////////////////////////////////////////////////////////////
  class VARNODE : public  EXPR
  {  public:
     
      VARNODE(string * C)      { name = C; }
      virtual void load(int R) { operation(Load, R); }
      virtual void lea(int R)  { operation(Lea, R); }

      string * name;
      virtual void operation(optypes O, int R);
  };
 
  void VARNODE::operation(optypes op, int R)
  { 
    dlog("VARNODE OPERATION "+to_string(R),0)
   // cout<<";VARNODE OPERATION loadPlace: "<<loadPlace<<endl;

  if(storePlace==S_FPU||loadPlace==S_FPU ){

   if(loadPlace!= S_FPU){
      switch (op)
      { 
        case Store: printf("FSTP"); break;
        case Load:  printf("FLD"); break;
        case Plus:  printf("FADD"); break;
        case Minus: printf("FSUB"); break;
        case Times: printf("FIMUL"); break;
        case Slash: printf("FDIV"); break;
        
      }
   }
  }else{
    switch (op)
      { 
        case Lea:   printf("LEA"); break;
        case Store: printf("MOV"); break;
        case Load:  printf("MOV"); break;
        case Plus:  printf("ADD"); break;
        case Minus: printf("SUB"); break;
        case Times: printf("IMUL"); break;
        //case Slash: printf("DVD"); break;
        case BitOr: printf("OR"); break;
        case BitAnd: printf("AND"); break;
        case BitXor: printf("XOR"); break;
      }
  }
    if(loadPlace != S_FPU && op == Slash||op == Mod){
      cout<<"MOV rdx, 0   ; clear dividend, var"<<endl;
     
      cout<<"MOV rax, "<<*_to_reg_(R)<<"   ; dividend"<<endl;

      cout<<"MOV rcx, [rbp-"<<(obj->adr)<<"]   ; divisor"<<endl;
      cout<<"IDIV rcx     ; EAX = resultado, EDX = modulo"<<endl;
      
      //cout<<"MOV [rbp-"<<(obj->adr)<<"],"<<*_to_reg_(op == Slash?1:4)<<endl;
      cout<<"MOV "<<*_to_reg_(R)<<","<<*_to_reg_(op == Slash?1:4)<<endl;


    }else{

    if(op != Store) {
      if(obj->kind == Obj::Func){
        cout<<" "<<*_to_reg_(R)<<","<<(*obj->name)<<" ;var ("<<*obj->name<<","<<Struct::sizes[obj->type->kind]<<")"<<endl;

      }else{
        if(loadPlace == S_FPU){
            if(op==Load){
               if(obj->type->kind == Struct::Int){
                cout<<"fild dword [rbp-"<<(obj->adr)<<"] ;var ("<<*obj->name<<","<<Struct::sizes[obj->type->kind]<<")"<<endl;
             
              }

              if(obj->type->kind == Struct::Float){
                cout<<"fld dword [rbp-"<<(obj->adr)<<"] ;var ("<<*obj->name<<","<<Struct::sizes[obj->type->kind]<<")"<<endl;
             
              }
            }
            if(op == Plus||op == Minus||op == Times||op == Slash){
              if(obj->type->kind == Struct::Int){
                cout<<"fild dword [rbp-"<<(obj->adr)<<"] ;var ("<<*obj->name<<","<<Struct::sizes[obj->type->kind]<<")"<<endl;
             
              }

              if(obj->type->kind == Struct::Float){
                cout<<"fld dword [rbp-"<<(obj->adr)<<"] ;var ("<<*obj->name<<","<<Struct::sizes[obj->type->kind]<<")"<<endl;
             
              }

             if(op==Plus)
                cout<<"fadd; varnode"<<endl;
              
              if(op==Minus)
              cout<<"fsub; varnode"<<endl;

                if(op==Slash)
              cout<<"fdiv; varnode"<<endl;

                if(op==Times)
              cout<<"fmul; varnode"<<endl;

               
              
            }
        }else{

        cout<<" "<<*_to_reg_(R)<<",[rbp-"<<(obj->adr)<<"] ;var ("<<*obj->name<<","<<Struct::sizes[obj->type->kind]<<")"<<endl;
        }

      }
    }
    if(op == Store){
      if(storePlace==S_FPU){

      cout<<" DWORD [rbp-"<<obj->adr<<"]"<<" ;store place: "<<storePlace<<endl;
      }else{
      cout<<" [rbp-"<<obj->adr<<"],"<<*_to_reg_(R)<<" ;var ("<<*obj->name<<")"<<" ;store place: "<<storePlace<<endl;

      }
    }
    
    }
    //printf(" R%d,%c\n", R, name);
  }
  
  AST AstHandlerLinuxX86_64::VarNode(string * name)
  { return new VARNODE(name); }
 

/////////////////////////////////////////////////////////////
  class DESREFVARNODE : public EXPR
  {  public:
     
      DESREFVARNODE(AST c){ 
        child = c; type = 0; obj = c->obj; /*obj->type = c->obj->type->elemType;*/

        depth = ((DESREFVARNODE*)c)->depth +1;
        if(depth == 1){
          greater = new int(1);
        }
        *greater = depth;
        ((DESREFVARNODE*)child)->greater = greater;
      }
      DESREFVARNODE(AST c,int * depth){
        child = c; type = 0; obj = c->obj; /*obj->type = c->obj->type->elemType;*/

      }
      virtual void load(int R){
        child->load(R);
        operation(Load, R); 

      }

      virtual void store(int R){
        child->load(R);
        operation(Store, R); 

      }

      virtual void storereg(int R){
        child->load(R);

      }
    //protected:
      AST child;
      int  depth = 0;
      int * greater = new int(0);
      virtual void operation(optypes O, int R);
      void loadreg(int R){dlog("LOAD REG IWTH LOAD "+to_string(R),0); load(R); }
  };
 
  void DESREFVARNODE::operation(optypes op, int R)
  { 
    dlog("DESREFVARNODE OPERATION "+to_string(R),0)
    //cout<<"DESREFVARNODE OPERATION "<<R<<" depth: "<<depth<<" greater: "<<*greater<<endl;
    string * name = child->obj->name;
  switch (op)
    { 
      case Store: printf("MOV"); break;
      case Load:  printf("MOV"); break;
      case Plus:  printf("ADD"); break;
      case Minus: printf("SUB"); break;
      case Times: printf("IMUL"); break;
      //case Slash: printf("DVD"); break;
      case Lea:   printf("LEA"); break;
      case BitOr: printf("OR"); break;
      case BitAnd: printf("AND"); break;
      case BitXor: printf("XOR"); break;
    }
    
     if(op == Slash||op == Mod){
      cout<<"MOV rdx, 0   ; clear dividend, DESREFVARNODE"<<endl;
     
      cout<<"MOV rax, "<<*_to_reg_(R)<<"   ; dividend"<<endl;

      cout<<"MOV rcx, "<<*_to_reg_(R+1)<<" ; divisor"<<endl;
      cout<<"IDIV rcx     ; EAX = resultado, EDX = modulo"<<endl;
      
      //cout<<"MOV [rbp-"<<(obj->adr)<<"],"<<*_to_reg_(op == Slash?1:4)<<endl;
      cout<<"MOV "<<*_to_reg_(R)<<","<<*_to_reg_(op == Slash?1:4)<<endl;


    }else{
      if(op != Store ) {
        if(*greater==depth && op != Load){
          cout<<" "<<*_to_reg_(R)<<","<<*_to_reg_(R+1)<<" ;DESREFVARNODE "<<Struct::sizes[obj->type->kind]<<")"<<endl;
        }else{
          cout<<" "<<*_to_reg_(R)<<",["<<*_to_reg_(R)<<"] ;DESREFVARNODE,"<<Struct::sizes[obj->type->kind]<<")"<<endl;
        }
      }
      if(op == Store) {
        cout<<" ["<<*_to_reg_(R+1)<<"],"<<*_to_reg_(R)<<" ;DESREFVARNODE"<<endl;
      }

    }
    
    //printf(" R%d,%c\n", R, name);
  }
  
  AST AstHandlerLinuxX86_64::DesfRefVarNode(AST c)
  { return new DESREFVARNODE(c); }


/////////////////////////////////////////////////////////////
  class REFVARNODE : public EXPR
  {  public:
     
      REFVARNODE(AST c){ 
        child = c; 
        type = 0; obj = c->obj; /*obj->type = c->obj->type->elemType;*/

        depth = ((REFVARNODE*)c)->depth +1;
        if(depth == 1){
          greater = new int(1);
        }
        *greater = depth;
        ((REFVARNODE*)child)->greater = greater;
      }
      REFVARNODE(AST c,int * depth){
        child = c; type = 0; obj = c->obj; /*obj->type = c->obj->type->elemType;*/

      }
      virtual void load(int R){
        child->lea(R);
        //operation(Lea, R);
      }
      virtual void lea(int R){
        child->lea(R);
        operation(Lea, R);
      }

      virtual void store(int R){
        cout<<"No permitido"<<endl;
        exit(1);
        //child->load(R);
        //operation(Store, R); 

      }

      virtual void storereg(int R){
        //cout<<"No permitido"<<endl;
       // exit(1);
       child->load(R);

      }
    //protected:
      AST child;
      int  depth = 0;
      int * greater = new int(0);
      virtual void operation(optypes O, int R);
      void loadreg(int R){dlog("LOAD REG IWTH LOAD "+to_string(R),0); load(R); }
  };
 
  void REFVARNODE::operation(optypes op, int R)
  { 
    dlog("REFVARNODE OPERATION "+to_string(R),0)
    //cout<<"REFVARNODE OPERATION "<<R<<" depth: "<<depth<<" greater: "<<*greater<<endl;
    string * name = child->obj->name;
  switch (op)
    { 
      case Store: printf("MOV"); break;
      case Load:  printf("MOV"); break;
      case Plus:  printf("ADD"); break;
      case Minus: printf("SUB"); break;
      case Times: printf("IMUL"); break;
      case Slash: printf("DVD"); break;
      case Lea:   printf("LEA"); break;
      case BitOr: printf("OR"); break;
      case BitAnd: printf("AND"); break;
      case BitXor: printf("XOR"); break;
    }
    
    if(op != Store ) {
      cout<<" "<<*_to_reg_(R)<<",["<<*_to_reg_(R)<<"] ;REFVARNODE"<<endl;
    }
    if(op == Store) {
      cout<<" ["<<*_to_reg_(R+1)<<"],"<<*_to_reg_(R)<<" ;REFVARNODE"<<endl;
    }
    
    //printf(" R%d,%c\n", R, name);
  }
  
  AST AstHandlerLinuxX86_64::RefVarNode(AST c)
  { return new REFVARNODE(c); }

 
 

 ////////////////////////////////////////////////////////////////////////////
  class CONSTNODE : public EXPR
  { public:
      CONSTNODE(int V)                   { value = V; defined = 1; type = 1;}
      virtual void load(int R)           { operation(Load, R); }
    //protected:
      virtual void operation(optypes O, int R);
  };
 
  void CONSTNODE::operation(optypes op, int R)
  { switch (op)
    { 
      case Store: printf("MOV"); break;
      case Load:  printf("MOV"); break;
      case Plus:  printf("ADD"); break;
      case Minus: printf("SUB"); break;
      case Times: printf("IMUL"); break;
      //case Slash: printf("IDIV"); break;
      case BitOr: printf("OR"); break;
      case BitAnd: printf("AND"); break;
      case BitXor: printf("XOR"); break;
      //mcase Eq: printf("cmp"); break;
    }
    if(op == Slash||op == Mod){
      cout<<"MOV rdx, 0   ; clear dividend, const"<<endl;
      
      if(R!=1){
         cout<<"MOV rax, "<<*_to_reg_(R)<<"   ; dividend"<<endl;
      }

      cout<<"MOV rcx, "<<value<<"   ; divisor"<<endl;
      cout<<"IDIV rcx     ; EAX = resultado, EDX = modulo"<<endl;
      
      //if(R!=1){
        cout<<"MOV "<<*_to_reg_(R)<<","<<*_to_reg_(op == Slash?1:4)<<endl;
      //}

    }else{
      if(op != Store) cout<<" "<<*_to_reg_(R)<<","<<value<<" ;const"<<endl;
      if(op == Store) cout<<" "<<value<<","<<*_to_reg_(R)<<" ;const"<<endl;
    }

    //cout <<*_to_reg_(R)<<","*_to_reg_(R+1)<<endl;
    //printf(" R%d,%d\n", R, value);
  }
 
  AST AstHandlerLinuxX86_64::ConstNode(int value)
  { return new CONSTNODE(value); }
 

 ////////////////////////////////////////////////////////////////////////////
  class CONSFLOATTNODE : public EXPR
  { public:
      CONSFLOATTNODE(float V){ fValue = V; defined = 1; type = 1;}
      virtual void load(int R){ operation(Load, R); }
    //protected:
      float fValue=0.0; 
      virtual void operation(optypes O, int R);
  };
 
  void CONSFLOATTNODE::operation(optypes op, int R)
  { 
    if(false && loadPlace == S_FPU || storePlace == S_FPU){
        switch (op)
      { 
        case Store: printf("FSTP"); break;
        case Load:  printf("FLD"); break;
        case Plus:  printf("FADD"); break;
        case Minus: printf("FSUB"); break;
        case Times: printf("FMUL"); break;
        case Slash: printf("FDIV"); break;
      
      }
    }else{
      /*switch (op)
      { 
        case Store: printf("MOV"); break;
        case Load:  printf("MOV"); break;
        case Plus:  printf("ADDSD"); break;
        case Minus: printf("SUB"); break;
        case Times: printf("IMUL"); break;
        //case Slash: printf("IDIV"); break;
        case BitOr: printf("OR"); break;
        case BitAnd: printf("AND"); break;
        case BitXor: printf("XOR"); break;
      }*/
    }
    if(loadPlace != S_FPU && op == Slash||op == Mod){
     

    }else{
      if(op != Store){
        if(op==Load){
          cout<<"mov DWORD [rbp-"<<obj->adr<<"],__float32__("<<to_string(fValue)<<") ;const float,  loadplace: "<<loadPlace<<endl;
          if(loadPlace == S_FPU){

            cout<<"fld DWORD [rbp-"<<obj->adr<<"]"<<endl; 
          }
          if(loadPlace == REG){
              cout<<"mov "<<*_to_reg_(R)<<", [rbp-"<<obj->adr<<"]"<<endl; 
          }
          
        }

        if(op==Plus||op==Minus||op==Slash||op==Times){
          cout<<"mov DWORD [rbp-"<<obj->adr<<"],__float32__("<<to_string(fValue)<<") ;const float,  loadplace: "<<loadPlace<<endl;
          cout<<"fld DWORD [rbp-"<<obj->adr<<"]"<<endl;
         // cout<<"fadd ; const float"<<endl;
          
          switch (op)
              {
              case Plus:
                cout<<"fadd; ;const floa"<<endl;
                break;
              case Minus:
                cout<<"fsub; ;const floa"<<endl;
                break;
              case Slash:
                cout<<"fdiv; ;const floa"<<endl;
                break;
              case Times:
                cout<<"fmul; ;const floa"<<endl;
                break;
              
              default:
                break;
              }
        }
      }
      if(op == Store) {
        cout<<" "<<fValue<<","<<*_to_reg_(R)<<" ;const float"<<endl;
      }
    }

    //cout <<*_to_reg_(R)<<","*_to_reg_(R+1)<<endl;
    //printf(" R%d,%d\n", R, value);
  }
 
  AST AstHandlerLinuxX86_64::ConstFloatNode(float value)
  { return new CONSFLOATTNODE(value); }
 
/////////////////////////////////////////////////////////////////////////////
  class CONSSTRINGTNODE : public EXPR
  { public:
      CONSSTRINGTNODE(string * key){ key_value = key; defined = 1; type = 1;}
      virtual void load(int R){ operation(Load, R); }
    //protected:
      string * key_value; 
      virtual void operation(optypes O, int R);
  };
 
  void CONSSTRINGTNODE::operation(optypes op, int R)
  { 

    switch (op)  
    { 
      case Store: printf("MOV"); break;
      case Load:  printf("MOV"); break;
      case Plus:  printf("ADDSD"); break;
      case Minus: printf("SUB"); break;
      case Times: printf("IMUL"); break;
      //case Slash: printf("IDIV"); break;
      case BitOr: printf("OR"); break;
      case BitAnd: printf("AND"); break;
      case BitXor: printf("XOR"); break;
    }   
    if(op != Store) cout<<" "<<*_to_reg_(R)<<","<<*key_value<<" ;string lit"<<endl;

    if(op == Slash||op == Mod){
      
    }else{
      if(op != Store){
        
      }
      //if(op == Store) cout<<" "<<fValue<<","<<*_to_reg_(R)<<" ;const float"<<endl;
    }

    //cout <<*_to_reg_(R)<<","*_to_reg_(R+1)<<endl;
    //printf(" R%d,%d\n", R, value);
  }
 
AST AstHandlerLinuxX86_64::ConstStringNode(string * key)
  { return new CONSSTRINGTNODE(key); }
 
////////////////////////////////////////////////////

 /*STM*/
 class StatementBlockNode : public BLOCK {
 public:
    AST * stms;
    int s = 0;
    StatementBlockNode(){
      stms = (AST*) malloc(sizeof(AST)*1000);
    }
    virtual void add(AST E);
    virtual void load(int R);
    //p
    virtual void operation(optypes O, int R){};
    virtual void loadreg(int R){dlog("LOAD REG IWTH LOAD "+to_string(R),0); load(R); }
   

 };
 void StatementBlockNode::add(AST e){

    stms[s] = e;
    s++;
 }
void StatementBlockNode::load(int R){
  for(int x = 0;x<s;x++){
    //cout<<stms[x]<<endl;
    stms[x]->load(1);
    if( !stms[x]->obj->_extern)
      cout<<endl; 
  }
}

 
//////////////////////////////////////////////////////////////////////////

class FuncDeclStm : public BLOCK{
  public:
    
    FuncDeclStm(string *n){
      stms = (AST*) malloc(sizeof(AST)*1000);
      name = n;

    }
   virtual void add(AST E);
    virtual void load(int R);
    virtual void lea(int R);
    virtual void store(int R){};

    virtual void operation(optypes O, int R);
    virtual void loadreg(int R){dlog("LOAD REG IWTH FUNC "+to_string(R),0); load(R); }
    AST * stms;
    int s = 0;
    string * name;
    Obj*aa=NULL;
  };

 void FuncDeclStm::add(AST e){
    cout<<";FuncDeclStm::add "<<e<<endl;
    stms[s] = e;
    s++;
 }
void FuncDeclStm::load(int R){
  if(!obj->_extern)
    operation(Load,R);
}
void FuncDeclStm::lea(int R){
  if(!obj->_extern)
    operation(Lea,R);
}
void FuncDeclStm::operation(optypes op,int r){
  if(obj->_extern)
      return;

  if(op == Lea){
    cout<<"MOV "<<*_to_reg_(r)<<","<<*obj->name <<endl;
  }

  if(op == Load){
    for(int x = 0;x<s;x++){

      if(stms[x]->obj->kind==4){

        stms[x]->load(r);
        
        cout<<endl;
      }
    } 
  cout<<*name<<":"<<endl;
  cout<<"PUSH rbp"<<endl;
  cout<<"MOV rbp, rsp"<<endl;
  cout<<"SUB rsp,"<< obj->type->size <<endl;
  
  //cout <<";params: "<<obj->nPars<<endl;
  int y =0;
   for(int x = 0;x<obj->nPars;x++,y++){
       
      if(aa==NULL){
        aa = obj->locals;
      }else{
        aa=aa->next;
      }
      cout<<"MOV [rbp-"<<aa->adr<<"],"<<*_to_reg_(x+orp)<<" ;("<<*aa->name<<")"<<endl;

   }

  //cout <<";stats: "<<s<<endl;
  for(int x = 0;x<s;x++){

      if(stms[x]->obj->kind!=4){

        stms[x]->load(r);
        
        cout<<endl;
      }

   }
   //cout<<".ret:"<<endl;
   cout<<"MOV rsp,rbp "<<endl;
   cout<<"POP rbp "<<endl;   
   cout<<"RET"<<endl;// essentially identical to: pop [register] -> jmp [register]
   
  }
}
  
  NODE* AstHandlerLinuxX86_64::FuncDeclNode(string *name){
    return new FuncDeclStm(name);
  }


 
 class ASSIGNMENT : public NODE
 {  public:
        ASSIGNMENT(AST L, AST R){
            
        op = Store; left = L; right = R;
        }   
            
    virtual void load(int R);
    //protected:
      optypes op;
      AST left, right;
      virtual void operation(optypes O, int R);
      virtual void loadreg(int R){dlog("LOAD REG IWTH LOAD "+to_string(R),0); load(R); }
    
 };
 
 void ASSIGNMENT::operation(optypes op, int R)
  { switch (op)
    { 
      case Store:  printf("STORE"); break;
      case Load:  printf("LDA"); break;
      case Plus:  printf("ADD"); break;
      case Minus: printf("SUB"); break;
      case Times: printf("IMUL"); break;
      case Slash: printf("DVD"); break;
    }
    printf(" R%d,R%d\n", R, R + 1);
  }
  
  void ASSIGNMENT::load(int R)
  { if (!left || !right) return;
    //cout<<";AssignmentNode "<<R<<endl;
    BINOPNODE* r;
    r = dynamic_cast<BINOPNODE*>(right);
    if(r!=nullptr){
      cout<<";ASSIGNMENT from binop"<<endl;
     
      if(r->obj->type->kind == Struct::Float){
        left->storePlace = LoadStorePlace::S_FPU;
      }

    }

    CONSTNODE*RC = dynamic_cast<CONSTNODE*>(right);
    if(RC!=nullptr){
       cout<<";ASSIGNMENT from const int"<<endl;
    }

    CONSFLOATTNODE*RF = dynamic_cast<CONSFLOATTNODE*>(right);
    if(RF!=nullptr){
       cout<<";ASSIGNMENT from const float"<<endl;
    }

    VARNODE*RV = dynamic_cast<VARNODE*>(right);
    if(RV!=nullptr){
       cout<<";ASSIGNMENT from var node"<<endl;
    }
    
    right->load(R);
    left->storereg(R+1); left->operation(Store,R);

    delete left; delete right;
  }
  
  AST AstHandlerLinuxX86_64::AssignmentNode(AST left, AST right){
    return new ASSIGNMENT(left,right);
  }

////////////////////////////////////////////////
class EmptyNodeAST:public EXPR {

public:
  EmptyNodeAST(){
  }

  virtual void add(AST E){};
  virtual void load(int R){
   
  };
  virtual void lea(int R){};

  virtual void operation(optypes O, int R){
   
  };
  virtual void loadreg(int R){dlog("LOAD REG IWTH FUNC "+to_string(R),0); load(R); }
   
};

  AST AstHandlerLinuxX86_64::EmptyNode()
  { return new EmptyNodeAST(); }
////////////////////////////////////////////////////////
  Obj* AstHandlerLinuxX86_64::GetObjOfNode(NODE* n){
    return n->obj;
  }
  void AstHandlerLinuxX86_64::SetObjOfNode(NODE* n,Obj*o){
    n->obj = o;
  }


NODE* AstHandlerLinuxX86_64::CreateStatementBlock(){
  return new StatementBlockNode();
}
void AstHandlerLinuxX86_64:: StatementBlockAdd(AST  BS,AST S){
   cout<<";StatementBlockAdd "<<BS<<" "<<S<<endl;
  ((BLOCK*)BS)->add((STAT*)S);
}


class RETURNODE:public EXPR {

public:
  RETURNODE(AST toReturn){
    child = toReturn;
  }

  virtual void add(AST E){};
  virtual void load(int R){
   child->load(1);
   operation(Load,R);
  };
  virtual void lea(int R){};

  virtual void operation(optypes O, int R){
   cout<<"mov rsp,rbp "<<endl;
   cout<<"pop rbp "<<endl;   
   cout<<"ret  "<<endl;
  };
  virtual void loadreg(int R){dlog("LOAD REG IWTH FUNC "+to_string(R),0); load(R); }
   
  AST child; 
};

AST AstHandlerLinuxX86_64::ReturnNode(AST toReturn){
  return new RETURNODE(toReturn);
}


class RETUREMPTYNODE:public EXPR {

public:
  RETUREMPTYNODE(){}

  virtual void add(AST E){};
  virtual void load(int R){
   operation(Load,R);
  };
  virtual void lea(int R){};

  virtual void operation(optypes op, int R){
    switch (op)
    { 
      case Store: printf("MOV"); break;
      case Load:  printf("MOV"); break;
      case Plus:  printf("ADD"); break;
      case Minus: printf("SUB"); break;
      case Times: printf("IMUL"); break;
      case Slash: printf("DVD"); break;
    }
    cout<<" "<<*_to_reg_(R)<<",0 ;RETUREMPTYNODE"<<endl;
    
    cout<<"mov rsp,rbp "<<endl;
   cout<<"pop rbp "<<endl;   
   cout<<"ret  "<<endl;
      
    
  };
  virtual void loadreg(int R){dlog("LOAD REG IWTH FUNC "+to_string(R),0); load(R); }
   
  AST child; 
};


AST AstHandlerLinuxX86_64::ReturnNode(){
  return new RETUREMPTYNODE();
}


////////////////////////////////////////////////////////////
class FuncCallExpr : public BLOCK {
 public:
  
    FuncCallExpr(AST f_node,bool isFuncExpre){
      s=0;
      this->isFuncExpre = isFuncExpre;
      stms = (AST*) malloc(sizeof(AST)*1000);
      child = f_node;
      obj=child->obj;

      ifReturnOfFunc = true;
      //f_node->ifReturnOfFunc = true;
      aa = obj->locals;
    };
    virtual void add(AST E);
    virtual void load(int R);
    virtual void lea(int R);
    virtual void operation(optypes O, int R);
    virtual void loadreg(int R){dlog("LOAD REG IWTH LOAD "+to_string(R),0); load(R); }
    bool isFuncExpre = false;
    AST * stms;
    AST child;
    Obj * aa;
    int s = 0;//parametros

 };
 void FuncCallExpr::add(AST e){
    if(s+1  > obj->nPars){
       cout<<"Supera el numero de parametros permitidos: "<<( obj->nPars)<<" actual: "<<(s+1)<<endl;
      exit(1);
    }
    if(StructEq(e->obj->type,aa->type)){
      stms[s] = e;
      s++;
      aa = aa->next;
    }else{
      cout<<"Argumento no valido en "<<(s+1)<<endl;
      exit(1);
    }
   
 }
void FuncCallExpr::load(int R){
  //cout<<"; FuncCallExpr::load "<<s<<endl;
  for( int x = s-1 ; x >= 0; x-- ){
    stms[x]->load((orp)+x);
  }
  operation(Load,R);
  
}
void FuncCallExpr::lea(int R){
  cout<<"; FuncCallExpr::lea "<<endl;
  int r = R+4;  
  child->lea(R);
  for( int x = s-1 ; x >= 0; x-- ){
    stms[x]->load((orp)+x);
  }
  operation(Load,R);
}
void FuncCallExpr::operation(optypes O, int R){
  //cout<<";FuncCallExpr::operation op: "<<O<<endl;
  if(O == Load){
    if(R-1 == 1){
      cout<<"PUSH "<<*_to_reg_(R-1)<<endl;
    }


    if( child->obj->adr == -1){//pendiente !!!!
      
      cout<<"CALL "<<*obj->name<<" ; r "<<R<<" op "<<O<<" ;FuncCallExpr"<<endl;

    }else{
      //child->load(R);
      cout<<"CALL [rbp-"<<obj->adr<<"] ; r "<<R<<" op "<<O<<" ;FuncCallExpr"<<endl;
    }

    if(isFuncExpre==false){
      cout<<"XOR rax,rax"<<endl;
    }else{
      if(R!=1){

        cout<<"MOV "<<*_to_reg_(R)<<","<<*_to_reg_(1)<<endl;
        if(R-1 == 1){
          cout<<"POP "<<*_to_reg_(R-1)<<endl;
        }
      }
    }
  }
  if(O==Plus){
     cout<<"ADD "<<*_to_reg_(R)<<","<<*_to_reg_(R+1)<<" ;FuncCallExpr" <<endl;;
  }
 
  if(O==Slash || O == Mod){
     //cout<<";IDVI "<<*_to_reg_(R)<<","<<*_to_reg_(R+1)<<" ;FuncCallExpr" <<endl;;
    cout<<"MOV rdx, 0   ; clear dividend, FuncCallExpr"<<endl;
     
    cout<<"MOV rax, "<<*_to_reg_(R)<<" ; dividend"<<endl;

    cout<<"MOV rcx, "<<*_to_reg_(R+1)<<"  ; divisor"<<endl;
    cout<<"IDIV rcx     ; EAX = resultado, EDX = modulo"<<endl;
    cout<<"MOV "<<*_to_reg_(R)<<","<<*_to_reg_(O == Slash?1:4)<<endl;

  }
}
AST AstHandlerLinuxX86_64::FuncCall(AST f_node,bool isFuncCall){
  return new FuncCallExpr(f_node,isFuncCall);
}
AST AstHandlerLinuxX86_64::ProcCall(AST f_node){

  return new FuncCallExpr(f_node,false);
}
////////////////////////////////////////////////////////////

class IFNODE : public BLOCK {
 public:
  
    IFNODE(AST cond){
      s=0;
      stms = (AST*) malloc(sizeof(AST)*1000);
      child = cond;
      obj=child->obj;

      //f_node->ifReturnOfFunc = true;
      aa = obj->locals;
    };
    virtual void add(AST E);
    virtual void load(int R);
    virtual void lea(int R){};
    virtual void operation(optypes O, int R);
    virtual void loadreg(int R){dlog("LOAD REG IWTH LOAD "+to_string(R),0); load(R); }
    AST * stms;
    AST child;
    Obj * aa;
    int s = 0;//parametros

 };

void IFNODE::add(AST e){
  stms[s] = e;
  s++;
}

void IFNODE::load(int R){
  child->load(R);
  for( int x = s-1; x >= 0; x-- ){
    stms[x]->load(x);
  }
  operation(Load,R);
}

void IFNODE::operation(optypes op, int R){
  __asm__("mov %rax,%rax");
  cout<<"; if node"<<endl;

}

AST AstHandlerLinuxX86_64::CreateIfNode(AST cond){
  return new IFNODE(cond);
}
////////////////////////////////////////////////////////////
AST AstHandlerLinuxX86_64::CreateIfElseNode(AST,AST){
  return NULL;
}

////////////////////////////////////////////////////////////

class PrintNode: public STAT {
public:
    PrintNode(AST ex){
      child = ex;
      obj = ex->obj;
    };
    
  
    virtual void load(int R);
    virtual void lea(int R){};
    virtual void operation(optypes O, int R);
    virtual void loadreg(int R){dlog("LOAD REG IWTH LOAD "+to_string(R),0); load(R); }
    AST child;
};  

void PrintNode::load(int R){
       child->load(orp+R);
       operation(Load,R) ;
};     
 
void PrintNode::operation(optypes op, int R){
  if(obj->type->kind == Struct::Float){
    cout<<"MOV rdi,formato_real"<<endl; 
    cout<<"MOV rax,1"<<endl;
  }else if(StructEq(symbol_table::stringType,obj->type)){ 
    cout<<"MOV rdi,formato_string"   <<endl;  
    cout<<"MOV rax,0"<<endl;      
  }else if(obj->type->kind == Struct::Char){    
    cout<<"MOV rdi,formato_char"<<endl;
    cout<<"MOV rax,0"<<endl;
  }else{
    cout<<"MOV rdi,formato"<<endl;
    cout<<"MOV rax,0"<<endl;
  }
  cout<<"CALL printf"<<endl;
};

AST AstHandlerLinuxX86_64::CreatePrintNode(AST ex){
  return new PrintNode(ex);
} 

////////////////////////////////////////////////////////////
void AstHandlerLinuxX86_64::SetValueAsReturnOfFunn(AST v){ 
  v->ifReturnOfFunc = true;
}

////////////////////////////////////////////////////////////

void AstHandlerLinuxX86_64::GenerateCode(AST A,ExternSymbols*es,StringLiterals*sl)
{ 
  cout<<";-----------------------------"<<endl;
  cout<<"section .data"<<endl;
  sl->dump();
  cout<<"formato db \"%d\",10,0"<<endl;
  cout<<"formato_real db \"%f\",10,0"<<endl;
  cout<<"formato_string db \"%s\",0"<<endl;
  cout<<"formato_char db \"%c\",10,0"<<endl;
  cout<<""<<endl;
  cout<<"section .text"<<endl;
  cout<<"global main"<<endl;
  cout<<"extern printf"<<endl;
  es->dump();
  cout<<""<<endl;

  A->load(1); printf("\n");
}

}
}
