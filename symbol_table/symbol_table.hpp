#ifndef __SYMBOL_TABLE_HPP__
#define __SYMBOL_TABLE_HPP__

/* ---------------------------------------------------------------------------------------------------- */

#include "function.hpp"
#include "syntax_tree.hpp"
#include "type.hpp"
#include "value.hpp"
#include <map>
#include <memory>
#include <set>

/* ---------------------------------------------------------------------------------------------------- */

namespace CINT::SymbolTable {

/* ---------------------------------------------------------------------------------------------------- */

class TypeCmp {
public:
    inline bool operator()(const std::shared_ptr<IR::Type> & _lhs, const std::shared_ptr<IR::Type> & _rhs) const noexcept {
        return *_lhs < *_rhs;
    }
};

extern std::set<std::shared_ptr<IR::Type>, TypeCmp> typeSet;

namespace NativeTypes {

extern const std::shared_ptr<IR::Type> INT_TYPE;

}

/* ---------------------------------------------------------------------------------------------------- */

extern std::map<std::string, std::shared_ptr<IR::Value>> variableMap;

/* ---------------------------------------------------------------------------------------------------- */

class FunctionCmp {
public:
    inline bool operator()(const std::shared_ptr<IR::Function> & _lhs, const std::shared_ptr<IR::Function> & _rhs) const noexcept {
        return *_lhs < *_rhs;
    }
};

extern std::set<std::shared_ptr<IR::Function> , FunctionCmp> functionSet;

namespace NativeFunctions {

extern const std::shared_ptr<IR::Function> ADD_INT;
extern const std::shared_ptr<IR::Function> SUB_INT;
extern const std::shared_ptr<IR::Function> MUL_INT;
extern const std::shared_ptr<IR::Function> DIV_INT;
extern const std::shared_ptr<IR::Function> MOD_INT;

}

/* ---------------------------------------------------------------------------------------------------- */

namespace None {

extern const std::shared_ptr<IR::Type> NONE_TYPE;
extern const std::shared_ptr<IR::Value> NONE_VALUE;
extern const std::shared_ptr<IR::Function> NONE_FUNCTION;

}

/* ---------------------------------------------------------------------------------------------------- */

} // namespace CINT::SymbolTable

/* ---------------------------------------------------------------------------------------------------- */

#endif