#run ./main test/dec.test > pro.asm && nasm -f elf64 pro.asm && gcc pro.o && ./a.out
CC = gcc
CXX = g++
DEBUG=false
DVVM = 
NAME = "leafc"
# -Wall  -Wextra
#-ldl -lffi
FLAGS = $(DVVM) -g  -m64
CFLAGS = -std=c99 $(FLAGS)
CXXFLAGS = -std=c++11 $(FLAGS)

LIB_TARGET = ./libLeaf.a

LIB_OBJ =  parser/Scanner.o \
            parser/Parser.o \
            tree.o \
			threeFactory.o \
			AstHandlerLinuxX86_64.o \
            compiler/symb_tab/struct.o \
            compiler/symb_tab/obj.o \
            compiler/symb_tab/symbol_table.o \
			compiler/registers/register.o \
			compiler/registers/register_manager.o \
			compiler/registers/register_provider.o \
			compiler/registers/x86_64_register_provider.o \
			compiler/registers/x86_32_register_provider.o \
            

TARGET =  ./leafc
OBJ =  main.o $(LIB_TARGET)

all: build_parser compile

compile: $(TARGET)
	printf "\n\033[1;32mDvCompiler compilado\033[0;0m\n"


$(LIB_TARGET): $(LIB_OBJ)
	printf "\033[1;33mEmpaquetando: \033[1;34m%s\033[0;0m \033[0;0m\n" $@
	ar rcs $@ $^

$(TARGET): $(OBJ)
	printf "\033[1;31mCompilando principal: \033[1;34m%s\033[0;0m \033[0;0m\n" $@
	$(CXX) -o $@ $^ $(CXXFLAGS) -DDEBUG=${DEBUG}

%.o: %.cpp
	printf "\033[1;36mCompilando c++: \033[1;34m%s\033[0;0m -> \033[1;35m%s\033[0;0m  \033[0;0m\n" $^ $@
	$(CXX) -o $@ $(CXXFLAGS) -c $^ -DDEBUG=${DEBUG}

%.o: %.cc
	printf "\033[1;36mCompilando c++: \033[1;34m%s\033[0;0m -> \033[1;35m%s\033[0;0m  \033[0;0m\n" $^ $@
	$(CXX) -o $@ $(CXXFLAGS) -c $^ -DDEBUG=${DEBUG}

%.o: %.c
	printf "\033[1;36mCompilando c: \033[1;34m%s\033[0;0m -> \033[1;35m%s\033[0;0m  \033[0;0m\n" $^ $@
	$(CC) -o $@ $(CFLAGS) -c $^ -DDEBUG=${DEBUG}



build_parser:
	cococpp -o ./parser -frames ./frames/  decl_w_struct.atg
	rm src/parser/*.old &


clear:
	rm *.o &
	rm parser/*.o &
	rm parser/*.old &
	rm compiler/registers/*.o &
	rm compiler/symb_tab/*.o &
