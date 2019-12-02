/*
 thrice -> hold up a light
 */
#include <stdio.h>
#include <iostream>
#include "tree.hpp"
 
using namespace std; 
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


bool sreq(Struct*o1,Struct*o2){
  if(o1->elemType!=NULL && o2->elemType!=NULL){
    if(o1->kind==o2->kind){
      sreq(o1->elemType,o2->elemType);
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
  