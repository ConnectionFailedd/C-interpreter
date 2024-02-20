#include "function.hpp"
#include "function_signature.hpp"
#include "symbol_table.hpp"
#include "type.hpp"
#include "value.hpp"
#include <map>
#include <memory>
#include <new>
#include <set>
#include <vector>

/* ---------------------------------------------------------------------------------------------------- */

static const std::shared_ptr<CINT::IR::Value> assign_int_impl(const std::vector<std::shared_ptr<CINT::IR::Value>> & _arguments);
static const std::shared_ptr<CINT::IR::Value> add_int_impl(const std::vector<std::shared_ptr<CINT::IR::Value>> & _arguments);
static const std::shared_ptr<CINT::IR::Value> sub_int_impl(const std::vector<std::shared_ptr<CINT::IR::Value>> & _arguments);
static const std::shared_ptr<CINT::IR::Value> mul_int_impl(const std::vector<std::shared_ptr<CINT::IR::Value>> & _arguments);
static const std::shared_ptr<CINT::IR::Value> div_int_impl(const std::vector<std::shared_ptr<CINT::IR::Value>> & _arguments);
static const std::shared_ptr<CINT::IR::Value> mod_int_impl(const std::vector<std::shared_ptr<CINT::IR::Value>> & _arguments);

/* ---------------------------------------------------------------------------------------------------- */

namespace CINT::SymbolTable {

/* ---------------------------------------------------------------------------------------------------- */

std::set<std::shared_ptr<IR::Type>, TypeCmp> typeSet = {NativeTypes::INT_TYPE};

namespace NativeTypes {

const std::shared_ptr<IR::Type> INT_TYPE = std::make_shared<IR::NativeType>("int", 4 , std::align_val_t(4));

}

/* ---------------------------------------------------------------------------------------------------- */

std::map<std::string, std::shared_ptr<IR::Value>> variableMap = {};

/* ---------------------------------------------------------------------------------------------------- */

namespace NativeFunctions {

extern const std::shared_ptr<IR::Function> ASSIGN_INT = std::make_shared<IR::NativeFunction>(IR::NativeFunction(NativeTypes::INT_TYPE, IR::FunctionSignature("=", {NativeTypes::INT_TYPE, NativeTypes::INT_TYPE}), assign_int_impl));
extern const std::shared_ptr<IR::Function> ADD_INT = std::make_shared<IR::NativeFunction>(IR::NativeFunction(NativeTypes::INT_TYPE, IR::FunctionSignature("+", {NativeTypes::INT_TYPE, NativeTypes::INT_TYPE}), add_int_impl));
extern const std::shared_ptr<IR::Function> SUB_INT = std::make_shared<IR::NativeFunction>(IR::NativeFunction(NativeTypes::INT_TYPE, IR::FunctionSignature("-", {NativeTypes::INT_TYPE, NativeTypes::INT_TYPE}), sub_int_impl));
extern const std::shared_ptr<IR::Function> MUL_INT = std::make_shared<IR::NativeFunction>(IR::NativeFunction(NativeTypes::INT_TYPE, IR::FunctionSignature("*", {NativeTypes::INT_TYPE, NativeTypes::INT_TYPE}), mul_int_impl));
extern const std::shared_ptr<IR::Function> DIV_INT = std::make_shared<IR::NativeFunction>(IR::NativeFunction(NativeTypes::INT_TYPE, IR::FunctionSignature("/", {NativeTypes::INT_TYPE, NativeTypes::INT_TYPE}), div_int_impl));
extern const std::shared_ptr<IR::Function> MOD_INT = std::make_shared<IR::NativeFunction>(IR::NativeFunction(NativeTypes::INT_TYPE, IR::FunctionSignature("%", {NativeTypes::INT_TYPE, NativeTypes::INT_TYPE}), mod_int_impl));

}

std::set<std::shared_ptr<IR::Function>, FunctionCmp> functionSet = {NativeFunctions::ASSIGN_INT, NativeFunctions::ADD_INT, NativeFunctions::SUB_INT, NativeFunctions::MUL_INT, NativeFunctions::DIV_INT, NativeFunctions::MOD_INT};

/* ---------------------------------------------------------------------------------------------------- */

namespace None {

const std::shared_ptr<IR::Type> NONE_TYPE = std::make_shared<CINT::IR::TypeKey>(CINT::IR::TypeKey(""));
const std::shared_ptr<IR::Value> NONE_VALUE = std::make_shared<CINT::IR::Value>(CINT::IR::Value(NONE_TYPE));
const std::shared_ptr<IR::Function> NONE_FUNCTION = std::make_shared<CINT::IR::FunctionKey>(CINT::IR::FunctionKey(CINT::IR::FunctionSignature("", {})));

}

/* ---------------------------------------------------------------------------------------------------- */

}

/* ---------------------------------------------------------------------------------------------------- */

static const std::shared_ptr<CINT::IR::Value> assign_int_impl(const std::vector<std::shared_ptr<CINT::IR::Value>> & _arguments) {
    _arguments[0]->as<int>() = _arguments[1]->as<int>();
    return _arguments[0];
}

static const std::shared_ptr<CINT::IR::Value> add_int_impl(const std::vector<std::shared_ptr<CINT::IR::Value>> & _arguments) {
    auto result = std::make_shared<CINT::IR::Value>(CINT::IR::Value(CINT::SymbolTable::NativeTypes::INT_TYPE));
    result->as<int>() = _arguments[0]->as<int>() + _arguments[1]->as<int>();
    return result;
}

static const std::shared_ptr<CINT::IR::Value> sub_int_impl(const std::vector<std::shared_ptr<CINT::IR::Value>> & _arguments) {
    auto result = std::make_shared<CINT::IR::Value>(CINT::IR::Value(CINT::SymbolTable::NativeTypes::INT_TYPE));
    result->as<int>() = _arguments[0]->as<int>() - _arguments[1]->as<int>();
    return result;
}

static const std::shared_ptr<CINT::IR::Value> mul_int_impl(const std::vector<std::shared_ptr<CINT::IR::Value>> & _arguments) {
    auto result = std::make_shared<CINT::IR::Value>(CINT::IR::Value(CINT::SymbolTable::NativeTypes::INT_TYPE));
    result->as<int>() = _arguments[0]->as<int>() * _arguments[1]->as<int>();
    return result;
}

static const std::shared_ptr<CINT::IR::Value> div_int_impl(const std::vector<std::shared_ptr<CINT::IR::Value>> & _arguments) {
    auto result = std::make_shared<CINT::IR::Value>(CINT::IR::Value(CINT::SymbolTable::NativeTypes::INT_TYPE));
    result->as<int>() = _arguments[0]->as<int>() / _arguments[1]->as<int>();
    return result;
}

static const std::shared_ptr<CINT::IR::Value> mod_int_impl(const std::vector<std::shared_ptr<CINT::IR::Value>> & _arguments) {
    auto result = std::make_shared<CINT::IR::Value>(CINT::IR::Value(CINT::SymbolTable::NativeTypes::INT_TYPE));
    result->as<int>() = _arguments[0]->as<int>() % _arguments[1]->as<int>();
    return result;
}
