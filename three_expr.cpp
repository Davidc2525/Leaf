

#include "tree.h"


#include <stdio.h>
#include <iostream>

using namespace std;
class EXPR:public NODE{

  };
  
  class BINOPNODE : public EXPR
  { public:
      BINOPNODE(optypes O, AST L, AST R)   { op = O; left = L; right = R; }
      virtual void load(int R);

      optypes op;
      EXPR* left, right;
      void operation(optypes O, int R);
      void loadreg(int R){dlog("LOAD REG IWTH LOAD "+to_string(R),0); load(R); }
  };
 
  void BINOPNODE::operation(optypes op, int R)
  { switch (op)
    { 
      case Store:  printf("bin STO "); break;
      case Load:  printf("bin LDA"); break;
      case Plus:  printf("bin ADD"); break;
      case Minus: printf("bin SUB"); break;
      case Times: printf("bin MUL"); break;
      case Slash: printf("bin DVD"); break;
    }
    //if(op != Store) cout<<" R"<<R<<",R"<<R+1<<endl;
    //if(op == Store) cout<<" "<<*name<<",R"<<R<<endl;
    cout<<" " <<*_to_reg_(R)<<","<<*_to_reg_(R+1)<<endl;
    //printf(" R%d,R%d\n", R, R + 1);
  }
 
 
  void BINOPNODE::load(int R)
  { if (!left || !right) return;
    left->load(R); right->loadreg(R+1); right->operation(op, R);
    delete left; delete right;
  }
 
 
  AST BinOpNode(optypes op, AST left, AST right)
  { if (left && right && left->defined && right->defined)
    { // constant folding
      switch (op)
      { case Plus:  left->value += right->value; break;
        case Minus: left->value -= right->value; break;
        case Times: left->value *= right->value; break;
        case Slash: left->value /= right->value; break;
      }
      delete right; return left;
    }
    return new BINOPNODE(op, left, right);
  }
 
  class VARNODE : public NODE
  {  public:
      VARNODE(string * C)                { name = C; type = 1;}
      virtual void load(int R)           { operation(Load, R); }
    //protected:
      string * name;
      virtual void operation(optypes O, int R);
  };
 
  void VARNODE::operation(optypes op, int R)
  { 
    dlog("VARNODE OPERATION "+to_string(R),0)
   // cout<<"VARNODE OPERATION "<<R<<endl;

  switch (op)
    { 
      case Store:  printf("MOV"); break;
      case Load:  printf("MOV"); break;
      case Plus:  printf("var ADD"); break;
      case Minus: printf("SUB"); break;
      case Times: printf("MUL"); break;
      case Slash: printf("DVD"); break;
    }
    if(op != Store) cout<<" "<<*_to_reg_(R)<<",[rbp-"<<*name<<"]"<<endl;
    if(op == Store) cout<<" [rbp-"<<*name<<"],"<<*_to_reg_(R)<<endl;
    
    //printf(" R%d,%c\n", R, name);
  }
 
  AST VarNode(string * name)
  { return new VARNODE(name); }
 
  class CONSTNODE : public NODE
  { public:
      CONSTNODE(int V)                   { value = V; defined = 1; type = 1;}
      virtual void load(int R)           { operation(Load, R); }
    //protected:
      virtual void operation(optypes O, int R);
  };
 
  void CONSTNODE::operation(optypes op, int R)
  { switch (op)
    { 
      case Store:  printf("MOV"); break;
      case Load:  printf("MOV"); break;
      case Plus:  printf("const ADI"); break;
      case Minus: printf("SBI"); break;
      case Times: printf("MLI"); break;
      case Slash: printf("DVI"); break;
    }
    if(op != Store) cout<<" "<<*_to_reg_(R)<<","<<value<<endl;
    if(op == Store) cout<<" "<<value<<","<<*_to_reg_(R)<<endl;
    //cout <<*_to_reg_(R)<<","*_to_reg_(R+1)<<endl;
    //printf(" R%d,%d\n", R, value);
  }
 
  AST ConstNode(int value)
  { return new CONSTNODE(value); }
 
