

#include "generator.h"

/**
 * Constructor
 * */
x86_64_linux_generator_visitor::x86_64_linux_generator_visitor(ast_visitor_args *args)
{
    out_file = new ofstream(ConfManager::get().get<char *>("leaf.compiler.generator.outfile"));
    assembly = args->ass;
    rp = RegisterManager::getInstance()->getProviderBy(64);
}

/**
 * @brief iniciar la generacion
 * */
void x86_64_linux_generator_visitor::init(ASTU start)
{
    cout << "x86_64 linux visitor" << endl;
    if (start == nullptr)
        return;
    assembly->get_program()->section(Sections::global)->add(new DInstruccion("global main"));
    assembly->get_program()->section(Sections::_extern)->add(new DInstruccion("extern printf"));
    assembly->get_program()->section(Sections::_extern)->add(new DInstruccion("extern puts"));

    reg = rp->reserve();
    start->accept(this);

    //3 espacio para firmar el archivo
    *out_file << endl;
    *out_file << endl;
    *out_file << endl;

    *out_file << "%include \"feactures/exceptions/linux64.asm\"" << endl;
    assembly->get_program()->write(*out_file);
    cout << "visits: " << visits_ins << endl;
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
void x86_64_linux_generator_visitor::visit1(BinOp *bin) //load_reg
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
    if (this->ax == Store)
        throw "can´t store here";
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
    sub_rsp->add_operand(new InmediateIntOperand(func->obj->type->size));
    st->add(sub_rsp);

    //body function
    vector<ASTU>::iterator x = func->nodos.begin();
    for (; x != func->nodos.end(); x++)
    {
        (*x)->accept(this);
    }

    Label *ret_label = new Label((char *)rand_label::newLabel(10)->c_str());
    st->add(ret_label);

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
    Register *current_reg = this->reg;
    
    this->ax = Load;
    as->right->accept(this);
    this->ax = Store;
    as->left->accept2(this);
    // rp->free(reg->id);
    this->ax = Load;
    
    this->reg = current_reg;
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
    UPDATE_STATICS

    Section *st = assembly->get_program()->section(Sections::text);
    if (this->ax == Load)
    {
        if(ident->obj->type->kind == symbol_table::floatType->kind){
        
        }
        DInstruccion *i = new DInstruccion("mov");
        MemoryOperand *ops = new MemoryOperand();
        ops->add_operand(new RegisterOperand("rbp"));
        ops->add_operand(load_mod::sub, new InmediateIntOperand(ident->obj->adr));
        RegisterOperand *opd = new RegisterOperand(this->reg->getName());
        i->add_operand(opd);
        i->add_operand(ops);
        st->add(i);
    }

    if (this->ax == op_types::Plus)
    {
        DInstruccion *i = new DInstruccion("add");
        MemoryOperand *ops = new MemoryOperand();
        ops->add_operand(new RegisterOperand("rbp"));
        ops->add_operand(load_mod::sub, new InmediateIntOperand(ident->obj->adr));
        RegisterOperand *opd = new RegisterOperand(this->reg->getName());
        i->add_operand(opd);
        i->add_operand(ops);
        st->add(i);
    }

    if (this->ax == op_types::Store)
    {
        DInstruccion *i = new DInstruccion("mov");
        if (ident->obj->type->kind == Struct::Float)
        {
            i->set_instruccion("movss");
        }
        MemoryOperand *ops = new MemoryOperand();
        ops->add_operand(new RegisterOperand("rbp"));
        ops->add_operand(load_mod::sub, new InmediateIntOperand(ident->obj->adr));
        RegisterOperand *opd = new RegisterOperand(this->reg->getName());
        i->add_operand(ops);
        i->add_operand(opd);
        st->add(i);
    }
}

void x86_64_linux_generator_visitor::visit(IntConst *number)
{
    UPDATE_STATICS
    number->accept2(this);
}
void x86_64_linux_generator_visitor::visit1(IntConst *number) {}
void x86_64_linux_generator_visitor::visit2(IntConst *number)
{
    UPDATE_STATICS
    Section *st = assembly->get_program()->section(Sections::text);
    if (this->ax == Load)
    {
        DInstruccion *i = new DInstruccion("mov");
        InmediateIntOperand *ops = new InmediateIntOperand(number->value);
        RegisterOperand *opd = new RegisterOperand(this->reg->getName());
        i->add_operand(opd);
        i->add_operand(ops);
        st->add(i);
    }

    if (this->ax == op_types::Plus)
    {
        DInstruccion *i = new DInstruccion("add");
        InmediateIntOperand *ops = new InmediateIntOperand(number->value);
        RegisterOperand *opd = new RegisterOperand(this->reg->getName());
        i->add_operand(opd);
        i->add_operand(ops);
        st->add(i);
    }
}

/*float*/
void x86_64_linux_generator_visitor::visit(FloatConst *number)
{
    UPDATE_STATICS
    if (!number->in_data)
    {
        string *label_data = new string(number->adr_label->c_str());
        label_data->append(" dq ");
        char *chars_val = new char[20];
        sprintf(chars_val, "%.7lf", number->value);
        label_data->append(chars_val);
        assembly->get_program()->section(Sections::data)->add(new RegisterOperand(label_data->c_str()));
        number->in_data = true;
    }
    this->reg = this->rp->reserve_by_size(128);
    number->accept2(this);
    rp->free(this->reg->id);
}
void x86_64_linux_generator_visitor::visit1(FloatConst *number) {}
void x86_64_linux_generator_visitor::visit2(FloatConst *number)
{
    UPDATE_STATICS
    if (!number->in_data)
    {
        string *label_data = new string(number->adr_label->c_str());
        label_data->append(" dq ");
        char *chars_val = new char[20];
        sprintf(chars_val, "%.7lf", number->value);
        label_data->append(chars_val);
        assembly->get_program()->section(Sections::data)->add(new RegisterOperand(label_data->c_str()));
        number->in_data = true;
    }

    Section *st = assembly->get_program()->section(Sections::text);
    Section *dt = assembly->get_program()->section(Sections::data);
    Register *current_reg = this->reg;
    this->reg = this->rp->reserve_by_size(128);
    if (this->ax == Load)
    {
        DInstruccion *i = new DInstruccion("movss");
        MemoryOperand *ops = new MemoryOperand();
        RegisterOperand *adr_label = new RegisterOperand(number->adr_label->c_str());
        ops->add_operand(adr_label);
        RegisterOperand *opd = new RegisterOperand(this->reg->getName());
        i->add_operand(opd);
        i->add_operand(ops);
        st->add(i);
    }

    if (this->ax == op_types::Plus)
    {
        DInstruccion *i = new DInstruccion("addss");
        MemoryOperand *ops = new MemoryOperand();
        RegisterOperand *adr_label = new RegisterOperand(number->adr_label->c_str());
        ops->add_operand(adr_label);
        RegisterOperand *opd = new RegisterOperand(this->reg->getName());
        i->add_operand(opd);
        i->add_operand(ops);
        st->add(i);
    }
}

/**
 * @brief generar instrucciones para el bloque de la sentencia 'try'
 * patron:
 * 	
 * 	tyr{...}...
 * 	
 * */
void x86_64_linux_generator_visitor::visit(TryStatementNode *_try_)
{
    UPDATE_STATICS
    Section *st = assembly->get_program()->section(Sections::text);

    DInstruccion *i = new DInstruccion(";start try ");
    i->add_operand(new RegisterOperand(_try_->end_label->c_str()));
    st->add(i);

    //prepare try instruccion

    bool rax_bussy = rp->isBussy(rp->get_by_name("rax")->id);
    if (rax_bussy)
    {
        i = new DInstruccion("push ");
        i->add_operand(new RegisterOperand("rax"));
        st->add(i);
    }

    i = new DInstruccion("new_list");
    i->add_operand(new InmediateIntOperand(_try_->_caths_.size()));
    st->add(i);

    Register *tmp = rp->reserve();
    i = new DInstruccion("mov");
    i->add_operand(new RegisterOperand(tmp->getName()));
    i->add_operand(new RegisterOperand("rax"));
    st->add(i);

    vector<ASTU>::iterator _catchs_ = _try_->_caths_.begin();
    int index = 0;
    for (; _catchs_ != _try_->_caths_.end(); _catchs_++)
    {
        i = new DInstruccion("new_handler");
        int type_id = symbol_table::get_id(((CatchStatementNode *)(*_catchs_))->ex_var->obj->type, 0);
        i->add_operand(new InmediateIntOperand(type_id));
        string *label = ((CatchStatementNode *)(*_catchs_))->start_label;
        i->add_operand(new RegisterOperand(label->c_str()));
        st->add(i);

        i = new DInstruccion("list_set");
        i->add_operand(new RegisterOperand(tmp->getName()));
        i->add_operand(new InmediateIntOperand(index));
        i->add_operand(new RegisterOperand("rax"));
        st->add(i);

        index++;
    }
    i = new DInstruccion("_push_");
    i->add_operand(new RegisterOperand(tmp->getName()));
    st->add(i);

    i = new DInstruccion("call _dump_stack");
    //st->add(i);
    rp->free(tmp->id);

    if (rax_bussy)
    {
        i = new DInstruccion("pop ");
        i->add_operand(new RegisterOperand("rax"));
        st->add(i);
    }
    i = new DInstruccion(";end prepare try ");
    i->add_operand(new RegisterOperand(_try_->end_label->c_str()));
    st->add(i);
    //

    //_try_->child->accept(this);

    vector<ASTU>::iterator x = _try_->nodos.begin();
    for (; x != _try_->nodos.end(); x++)
    {
        (*x)->accept(this);
    }

    i = new DInstruccion("jmp");
    i->add_operand(new RegisterOperand(_try_->end_label->c_str()));
    st->add(i);

    _catchs_ = _try_->_caths_.begin();
    for (; _catchs_ != _try_->_caths_.end(); _catchs_++)
    {
        (*_catchs_)->accept(this);
        DInstruccion *i = new DInstruccion("jmp");
        i->add_operand(new RegisterOperand(_try_->end_label_wout_pop->c_str()));
        st->add(i);
    }

    i = new DInstruccion(";end try ");
    i->add_operand(new RegisterOperand(_try_->end_label->c_str()));
    st->add(i);

    Label *try_label = new Label((char *)_try_->end_label->c_str());
    st->add(try_label);

    i = new DInstruccion("_pop_");
    st->add(i);

    try_label = new Label((char *)_try_->end_label_wout_pop->c_str());
    st->add(try_label);
}

/**
 * @brief generar instrucciones para el bloque 'catch' de la sentencia 'try'
 * patron:
 * 	
 * 	tyr{...}catch(ex:int){...}
 * 	
 * */
void x86_64_linux_generator_visitor::visit(CatchStatementNode *_catch_)
{

    UPDATE_STATICS
    Section *st = assembly->get_program()->section(Sections::text);
    DInstruccion *i = new DInstruccion(";start catch");
    st->add(i);

    Label *try_label = new Label((char *)_catch_->start_label->c_str());
    st->add(try_label);

    i = new DInstruccion("mov");
    MemoryOperand *var_ex = new MemoryOperand();
    var_ex->add_operand(new RegisterOperand("rbp"));
    var_ex->add_operand(load_mod::sub, new InmediateIntOperand(_catch_->ex_var->obj->adr));
    i->add_operand(var_ex);
    i->add_operand(new RegisterOperand("rax"));
    st->add(i);

    vector<ASTU>::iterator x = _catch_->nodos.begin();
    for (; x != _catch_->nodos.end(); x++)
    {
        (*x)->accept(this);
    }

    i = new DInstruccion(";end catch");
    st->add(i);
}

/**
 * @brief generar instrucciones para el bloque 'throw' de la sentencia 'throw'
 * patron:
 * 	
 * 	throw expre
 * 	
 * */
void x86_64_linux_generator_visitor::visit(ThrowStatementNode *_t_)
{
    UPDATE_STATICS
    Section *st = assembly->get_program()->section(Sections::text);
    DInstruccion *i = new DInstruccion(";throw");
    st->add(i);

    i = new DInstruccion("_pop_");
    st->add(i);

    i = new DInstruccion("mov");
    i->add_operand(new RegisterOperand("rcx"));
    i->add_operand(new RegisterOperand("rax"));
    st->add(i);

    i = new DInstruccion("list_find_h_by_type");
    i->add_operand(new RegisterOperand("rax"));
    i->add_operand(new InmediateIntOperand(symbol_table::get_id(_t_->child->obj->type, 0)));
    st->add(i);

    i = new DInstruccion("mov");
    i->add_operand(new RegisterOperand("rbx"));
    i->add_operand(new RegisterOperand("rax"));
    st->add(i);

    ax = Load;
    _t_->child->accept(this);
    ;

    i = new DInstruccion("mov");
    i->add_operand(new RegisterOperand("rax"));
    i->add_operand(new RegisterOperand(reg->getName()));
    st->add(i);

    i = new DInstruccion("mov");
    i->add_operand(new RegisterOperand("rbp"));
    MemoryOperand *m = new MemoryOperand();
    m->add_operand(new RegisterOperand("rcx"));
    m->add_operand(load_mod::add, new InmediateIntOperand(8));
    i->add_operand(m);
    st->add(i);

    i = new DInstruccion("mov");
    i->add_operand(new RegisterOperand("rsp"));
    m = new MemoryOperand();
    m->add_operand(new RegisterOperand("rcx"));
    m->add_operand(load_mod::add, new InmediateIntOperand(16));
    i->add_operand(m);
    st->add(i);

    i = new DInstruccion("J_H_RUTINE");
    i->add_operand(new RegisterOperand("rbx"));
    //i->add_operand(new RegisterOperand("rcx"));
    st->add(i);

    i = new DInstruccion(";end throw");
    st->add(i);
}

/**/
