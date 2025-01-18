#include "compiler.h"
#include "parser.h"
#include "../utf8strings/utf8strings.h"
#include "../tnobject/TnObject.h"
#include <stdio.h>

BytecodeInstruction* BytecodeInstruction_new(BytecodeInstructionKind kind, int64_t number) {
    BytecodeInstruction* bytecode = malloc(sizeof(BytecodeInstruction));
    bytecode->kind = kind;
    bytecode->number = number;
    return bytecode;
}

void EMIT(Compiler* compiler, BytecodeInstruction* instruction) {
    ArrayList_append(compiler->instructions, instruction);
    compiler->counter += 1;
}

size_t _getVariableNumber(Compiler* compiler, UTF8_String* name) {
    size_t* number = HashMap_get(compiler->variables, name);
    if(number == NULL) {
        number = malloc(sizeof(size_t));
        *number = compiler->varcount;
        compiler->varcount += 1;
        HashMap_set(compiler->variables, UTF8_copy(name), number);
        ArrayList_append(compiler->varnames, UTF8_copy(name));
    }
    return *number;
}

size_t _getStringNumber(Compiler* compiler, UTF8_String* string) {
    size_t number = compiler->strings->length;
    ArrayList_append(compiler->strings, UTF8_copy(string));
    return number;
}

void _compilerStringLitteral(Compiler* compiler, UTF8_String* string) {
    size_t number = _getStringNumber(compiler, string);
    BytecodeInstruction* bytecode = BytecodeInstruction_new(I_LOAD_STRING, number);
    EMIT(compiler, bytecode);
}

void _compileNumberLitteral(Compiler* compiler, int64_t value) {
    BytecodeInstruction* bytecode = BytecodeInstruction_new(I_LOAD_NUMBER, value);
    EMIT(compiler, bytecode);
}

void _compileVariable(Compiler* compiler, UTF8_String* name) {
    size_t number = _getVariableNumber(compiler, name);
    EMIT(compiler, BytecodeInstruction_new(I_LOAD_VARIABLE, number));
}

void _compileAttribute(Compiler* compiler, UTF8_String* name) {
    size_t number = _getStringNumber(compiler, name);
    EMIT(compiler, BytecodeInstruction_new(I_LOAD_ATTR, number));
}

void _compileLitteral(Compiler* compiler, AST_Expression* expression) {
    switch (expression->litteral.kind) {
    case AST_STRING:
        _compilerStringLitteral(compiler, expression->litteral.string);
        break;
    case AST_VARIABLE:
        _compileVariable(compiler, expression->litteral.name);
        break;
    case AST_ATTRIBUTE:
        _compileAttribute(compiler, expression->litteral.name);
        break;
    case AST_NUMBER:
        _compileNumberLitteral(compiler, expression->litteral.number);
        break;
    default:
        break;
    }
}

void _compileBinaryExpression(Compiler* compiler, AST_Expression* expression) {
    _compileExpression(compiler, expression->binaryExpr.left);
    _compileExpression(compiler, expression->binaryExpr.right);
    switch (expression->binaryExpr.operator) {
        case AST_ADDITION:
            EMIT(compiler, BytecodeInstruction_new(I_BINARY_ADD, 0));
            break;
        case AST_SUBSTRACTION:
            EMIT(compiler, BytecodeInstruction_new(I_BINARY_SUB, 0));
            break;
        case AST_MULTIPLICATION:
            EMIT(compiler, BytecodeInstruction_new(I_BINARY_MUL, 0));
            break;
        case AST_EQUAL:
            EMIT(compiler, BytecodeInstruction_new(I_CMP_EQUAL, 0));
            break;
        case AST_NOT_EQUAL:
            EMIT(compiler, BytecodeInstruction_new(I_CMP_NEQUAL, 0));
            break;
        case AST_GREATER_OR_EQUAL:
            EMIT(compiler, BytecodeInstruction_new(I_CMP_GE, 0));
            break;
        case AST_LOWER_OR_EQUAL:
            EMIT(compiler, BytecodeInstruction_new(I_CMP_LE, 0));
            break;
        case AST_GREATER_THAN:
            EMIT(compiler, BytecodeInstruction_new(I_CMP_GT, 0));
            break;
        case AST_LOWER_THAN:
            EMIT(compiler, BytecodeInstruction_new(I_CMP_LT, 0));
            break;
        default:
            break;
    }
}

void _compileFunctionCallExpression(Compiler* compiler, AST_Expression* expression) {
    for(size_t i = 0 ; i < expression->functionCall.arguments->length ; i += 1) {
        _compileExpression(compiler, ArrayList_get(expression->functionCall.arguments, i));
    }
    size_t number = _getStringNumber(compiler, expression->functionCall.funcName);
    EMIT(compiler, BytecodeInstruction_new(I_CALL, number));
}

void _compileMethodCallExpression(Compiler* compiler, AST_Expression* expression) {
    EMIT(compiler, BytecodeInstruction_new(I_DUP_TOP, 0));
    size_t number = _getStringNumber(compiler, expression->methodCall.funcName);
    EMIT(compiler, BytecodeInstruction_new(I_LOAD_ATTR, number));
    for(size_t i = 0 ; i < expression->functionCall.arguments->length ; i += 1) {
        _compileExpression(compiler, ArrayList_get(expression->methodCall.arguments, i));
    }
    EMIT(compiler, BytecodeInstruction_new(I_METHOD_CALL, expression->functionCall.arguments->length));
}

void _compileGetattributeExpression(Compiler* compiler, AST_Expression* expression) {
    for(size_t i = 0 ; i < expression->chain->length ; i += 1) {
        _compileExpression(compiler, ArrayList_get(expression->chain, i));
    }
}

void _compileSetattributeExpression(Compiler* compiler, AST_Expression* expression) {
    _compileExpression(compiler, expression->setAttribute.expression);
    // EMIT(compiler, BytecodeInstruction_new(I_DUP_TOP, 0));
    size_t attrNumber = _getStringNumber(compiler, expression->setAttribute.name);
    EMIT(compiler, BytecodeInstruction_new(I_STORE_ATTR, attrNumber));
}

void _compileAssignementExpression(Compiler* compiler, AST_Expression* expression) {
    _compileExpression(compiler, expression->assignement.expression);
    EMIT(compiler, BytecodeInstruction_new(I_DUP_TOP, 0));
    size_t varNumber = _getVariableNumber(compiler, expression->assignement.name);
    EMIT(compiler, BytecodeInstruction_new(I_STORE, varNumber));
}

void _compileExpression(Compiler* compiler, AST_Expression* expression) {
    switch (expression->kind) {
    case AST_BINARY_EXPR:
        _compileBinaryExpression(compiler, expression);
        break;
    case AST_UNARY_EXPR:
        break;
    case AST_LITTERAL_EXPR:
        _compileLitteral(compiler, expression);
        break;
    case AST_FUNCTION_CALL_EXPR:
        _compileFunctionCallExpression(compiler, expression);
        break;
    case AST_METHOD_CALL_EXPR:
        _compileMethodCallExpression(compiler, expression);
        break;
    case AST_GETATTRIBUTE_EXPR:
        _compileGetattributeExpression(compiler, expression);
        break;
    case AST_ASSIGNEMENT_EXPR:
        _compileAssignementExpression(compiler, expression);
        break;
    case AST_SETATTRIBUTE_EXPR:
        _compileSetattributeExpression(compiler, expression);
    default:
        break;
    }
}

void _compileExpressionStatement(Compiler* compiler, AST_Statement* instruction) {
    _compileExpression(compiler, instruction->expression);
    EMIT(compiler, BytecodeInstruction_new(I_POP_TOP, 0));
}

void _compileConditionalStatement(Compiler* compiler, AST_Statement* instruction) {
    _compileExpression(compiler, instruction->condition.expr);
    BytecodeInstruction* jmpFalse = BytecodeInstruction_new(I_JUMP_IF_FALSE, 0);
    BytecodeInstruction* jmp = BytecodeInstruction_new(I_JUMP, 0);
    EMIT(compiler, jmpFalse);
    _compileInstructions(compiler, instruction->condition.trueBlock);
    jmpFalse->number = compiler->counter;
    EMIT(compiler, jmp);
    _compileInstructions(compiler, instruction->condition.falseBlock);
    jmp->number = compiler->counter - 1;
}

void _compileInstruction(Compiler* compiler, AST_Statement* instruction) {
    switch (instruction->kind) {
        case AST_EXPRESSION_STATEMENT:
            _compileExpressionStatement(compiler, instruction);
            break;
        case AST_CONDITIONAL_STATEMENT:
            _compileConditionalStatement(compiler, instruction);
            break;
        default:
            break;
    }
}

void _compileInstructions(Compiler* compiler, ArrayList* instructions) {
    for(size_t i = 0 ; i < instructions->length ; i += 1) {
        AST_Statement* instruction = ArrayList_get(instructions, i);
        _compileInstruction(compiler, instruction);
    }
}

Bytecode* compile(ArrayList* instructions) {
    Compiler compiler = {
        .counter = 0,
        .varcount = 0,
        .strings = ArrayList_new(UTF8_delete),
        .instructions = ArrayList_new(BytecodeInstruction_delete),
        .variables = HashMap_new(UTF8_delete, free, UTF8_equal, UTF8_hash),
        .varnames = ArrayList_new(UTF8_delete)
    };
    _compileInstructions(&compiler, instructions);
    Bytecode* bytecode = malloc(sizeof(Bytecode));
    bytecode->instructions = compiler.instructions;
    bytecode->strings = compiler.strings;
    bytecode->varcount = compiler.varcount;
    bytecode->varnames = compiler.varnames;
    bytecode->contextObject = NULL;
    // free the variables
    HashMap_delete(compiler.variables);
    return bytecode;
}

void Bytecode_print(Bytecode* bytecode) {
    printf("BYTECODE\n");
    printf("#string table:\n");
    for(size_t i = 0 ; i < bytecode->strings->length ; i += 1) {
        UTF8_String* string = ArrayList_get(bytecode->strings, i);
        printf("  %lu: \"%s\"\n", i, string->data);
    }
    printf("#instructions:\n");
    for(size_t i = 0 ; i < bytecode->instructions->length ; i += 1) {
        BytecodeInstruction* instruction = ArrayList_get(bytecode->instructions, i);
        printf("  %lu\t| %s %ld\n", i, BytecodeInstructionKind_repr(instruction->kind), instruction->number);
    }
}

void Bytecode_setContextObject(Bytecode* bytecode, Tn_Object* object) {
    bytecode->contextObject = object;
}

void BytecodeInstruction_delete(BytecodeInstruction* instruction) {
    free(instruction);
}

void Bytecode_delete(Bytecode* bytecode) {
    ArrayList_delete(bytecode->strings);
    ArrayList_delete(bytecode->instructions);
    ArrayList_delete(bytecode->varnames);
    if(bytecode->contextObject != NULL) {
        Tn_Object_delete(bytecode->contextObject);
    }
    free(bytecode);
}

void Compiler_delete(Compiler* compiler) {
    ArrayList_delete(compiler->strings);
    ArrayList_delete(compiler->instructions);
    free(compiler);
}

const char* BytecodeInstructionKind_repr(BytecodeInstructionKind kind) {
    switch (kind) {
    case I_LOAD_STRING:
        return "LOAD_STRING";
    case I_LOAD_NUMBER:
        return "LOAD_NUMBER";
    case I_BINARY_ADD:
        return "BINARY_ADD";
    case I_BINARY_SUB:
        return "BINARY_SUB";
    case I_BINARY_MUL:
        return "BINARY_MUL";
    case I_CMP_EQUAL:
        return "CMP_EQUAL";
    case I_CMP_NEQUAL:
        return "CMP_NEQUAL";
    case I_CMP_GE:
        return "CMP_GE";
    case I_CMP_GT:
        return "CMP_GT";
    case I_CMP_LE:
        return "CMP_LE";
    case I_CMP_LT:
        return "CMP_LT";
    case I_JUMP_IF_FALSE:
        return "JUMP_IF_FALSE";
    case I_JUMP:
        return "JUMP";
    case I_LOAD_VARIABLE:
        return "LOAD_VARIABLE";
    case I_CALL:
        return "CALL";
    case I_METHOD_CALL:
        return "METHOD_CALL";
    case I_POP_TOP:
        return "POP_TOP";
    case I_DUP_TOP:
        return "DUP_TOP";
    case I_STORE:
        return "STORE";
    case I_LOAD_ATTR:
        return "LOAD_ATTR";
    case I_STORE_ATTR:
        return "STORE_ATTR";
    case I_LOAD_METHOD:
        return "LOAD_METHOD";
    default:
        break;
    }
    return NULL; // unreachable
}