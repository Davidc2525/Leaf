#include "generator.h"

/**
 * Constructor
 * */
x86_64_linux_generator_visitor::x86_64_linux_generator_visitor(ast_visitor_args *args)
{
    out_file = new ofstream(args->outfile->c_str());
    assembly = args->ass;
    rp = RegisterManager::getInstance()->getProviderBy(64);
}

/**
 * @brief iniciar la generacion
 * */
void x86_64_linux_generator_visitor::init(ASTU start)
{
    cout << ";x86_64 linux visitor" << endl;
    if (start == nullptr)
        return;
    reg = rp->reserve();
    start->accept(this);

    assembly->get_program()->write(*out_file);
    cout << "\n;visits: " << visits_ins << endl;
}

void x86_64_linux_generator_visitor::visit(Empty *empty)
{
    UPDATE_STATICS
}

/**
 * @brief generar codigo para operaciones binarias
 * patrong:
 * 	( a + b )
 * */
void x86_64_linux_generator_visitor::visit(BinOp *bin)
{
    UPDATE_STATICS

    bin->l->accept(this);
    Register *current = this->reg;

    reg = rp->reserve();
    bin->r->accept1(this);

    this->op_reg = reg;
    reg = current;
    this->ax = bin->op;
    bin->r->accept2(this);

    rp->free(op_reg->id);
    this->ax = Load;
}
/**
 * @brief generar codigo para la carga derecha de binop 
 * patron:
 * 	+(a+b)
 * */
void x86_64_linux_generator_visitor::visit1(BinOp *bin)
{
    UPDATE_STATICS
    bin->accept(this);
}

/**
 * @brief generar codigo para la operacion de la parte derecha de binop 
 * patron:
 * 	+(a+b)
 * */
void x86_64_linux_generator_visitor::visit2(BinOp *bin)
{
    UPDATE_STATICS
    Section *st = assembly->get_program()->section(Sections::text);

    if (this->ax == Plus)
    {
        DInstruccion *i = new DInstruccion("add");

        i->add_operand(new RegisterOperand(reg->getName()));
        i->add_operand(new RegisterOperand(op_reg->getName()));
        st->add(i);
    }
}

/**
 * @brief generar codigo para definicion de funciones y metodos
 * patron: 
 * 		fun name():int{...}
 * */
void x86_64_linux_generator_visitor::visit(FuncDeclStatementNode *func)
{
    UPDATE_STATICS
    Section *st = assembly->get_program()->section(Sections::text);

    //label of name function
    st->add(new Label((char *)func->name->c_str()));

    //preamble
    DInstruccion *push_rbp = new DInstruccion("push");
    push_rbp->add_operand(new RegisterOperand("rbp"));
    st->add(push_rbp);
    DInstruccion *mov_to_rbp = new DInstruccion("mov");
    mov_to_rbp->add_operand(new RegisterOperand("rbp"));
    mov_to_rbp->add_operand(new RegisterOperand("rsp"));
    st->add(mov_to_rbp);
    DInstruccion *sub_rsp = new DInstruccion("sub");
    sub_rsp->add_operand(new RegisterOperand("rsp"));
    sub_rsp->add_operand(new InmediateIntOperand(0));
    st->add(sub_rsp);

    //body function
    vector<ASTU>::iterator x = func->nodos.begin();
    for (; x != func->nodos.end(); x++)
    {
        (*x)->accept(this);
    }

    //epilogo
    DInstruccion *restore_rbp = new DInstruccion("mov");
    restore_rbp->add_operand(new RegisterOperand("rsp"));
    restore_rbp->add_operand(new RegisterOperand("rbp"));
    st->add(restore_rbp);

    DInstruccion *pop_rbp = new DInstruccion("pop");
    pop_rbp->add_operand(new RegisterOperand("rbp"));
    st->add(pop_rbp);

    DInstruccion *ret = new DInstruccion("ret");
    st->add(ret);
}

/**
 * @brief generacion de codigo para los bloques
 * patron:
 * 	{...}
 * */
void x86_64_linux_generator_visitor::visit(BlockStatemntNode *block)
{
    UPDATE_STATICS
    //cout<<"block load : "<<block->s<<endl;

    vector<ASTU>::iterator x = block->nodos.begin();
    for (; x != block->nodos.end(); x++)
    {
        (*x)->accept(this);
    }
}

/**
 * @brief generacion de codigo para asignacion 
 * patron:
 * 	a = b;
 * */
void x86_64_linux_generator_visitor::visit(AssignStatement *as)
{
    UPDATE_STATICS

    Section *st = assembly->get_program()->section(Sections::text);

    this->ax = Load;
    as->right->accept(this);
    this->ax = Store;
    as->left->accept2(this);

    this->ax = Load;
}

/**
 * @brief generar codigo para carga de variables
 * patron:
 * 	
 * 	a = b; //aqui "b" seria el identificador a cargar o visitar
 * 	
 * */
void x86_64_linux_generator_visitor::visit(Ident *ident)
{
    UPDATE_STATICS

    ident->accept2(this);
}
void x86_64_linux_generator_visitor::visit1(Ident *ident) {}
void x86_64_linux_generator_visitor::visit2(Ident *ident)
{
    Section *st = assembly->get_program()->section(Sections::text);
    if (this->ax == Load)
    {

        DInstruccion *i = new DInstruccion("mov");
        MemoryOperand *ops = new MemoryOperand();
        ops->add_operand(new RegisterOperand("rbx"));
        ops->add_operand(new InmediateIntOperand(ident->obj->adr));
        RegisterOperand *opd = new RegisterOperand(this->reg->getName());
        i->add_operand(opd);
        i->add_operand(ops);
        st->add(i);
    }

    if (this->ax == op_types::Plus)
    {
        DInstruccion *i = new DInstruccion("add");
        MemoryOperand *ops = new MemoryOperand();
        ops->add_operand(new RegisterOperand("rbx"));
        ops->add_operand(new InmediateIntOperand(ident->obj->adr));
        RegisterOperand *opd = new RegisterOperand(this->reg->getName());
        i->add_operand(opd);
        i->add_operand(ops);
        st->add(i);
    }

    if (this->ax == op_types::Store)
    {
        DInstruccion *i = new DInstruccion("mov");
        MemoryOperand *ops = new MemoryOperand();
        ops->add_operand(new RegisterOperand("rbx"));
        ops->add_operand(new InmediateIntOperand(ident->obj->adr));
        RegisterOperand *opd = new RegisterOperand(this->reg->getName());
        i->add_operand(ops);
        i->add_operand(opd);
        st->add(i);
    }
}

/**/
