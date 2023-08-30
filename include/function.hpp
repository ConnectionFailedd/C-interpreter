#ifndef __FUNCTION_HPP__
#define __FUNCTION_HPP__

#include <functional>
#include <map>
#include <memory>
#include <tuple>
#include <utility>
#include <vector>

#include "name.hpp"
#include "syntax_tree.hpp"
#include "type.hpp"
#include "value.hpp"

/* ---------------------------------------------------------------------------------------------------- */

namespace CINT {
namespace Function {

/* ---------------------------------------------------------------------------------------------------- */

class Signature {
private:
    Name __name;
    std::shared_ptr<Types::Type> __returnType;
    std::vector<std::shared_ptr<Types::Type>> __argumentTypes;

public:
    Signature(const Name & _name) : __name(_name), __returnType(), __argumentTypes() {}
    Signature(const Name & _name, const std::shared_ptr<Types::Type> & _returnType, const std::vector<std::shared_ptr<Types::Type>> & _argumentTypes) : __name(_name), __returnType(_returnType), __argumentTypes(_argumentTypes) {}

    bool operator<(const Signature & _rhs) const & { return __name < _rhs.__name; }
};

using BuiltInFunction = std::shared_ptr<Value>(const std::vector<std::shared_ptr<Value>> &);

extern std::multimap<Signature, std::function<BuiltInFunction>> builtInFunctionMultiMap;
extern std::multimap<Signature, std::shared_ptr<SyntaxTree>> userDefinedFunctionMultiMap;

std::multimap<Signature, std::function<BuiltInFunction>> built_in_function_multi_map_init();
std::multimap<Signature, std::shared_ptr<SyntaxTree>> user_defined_function_multi_map_init();

namespace BuiltInFunctions {

template<class _T>
BuiltInFunction increment;

template<class _T>
BuiltInFunction decrement;

template<class _Tt, class _Ts>
BuiltInFunction cast;

template<class _T>
BuiltInFunction minus;

template<class _T>
BuiltInFunction logic_not;

template<class _T>
BuiltInFunction bitwise_not;

template<class _T>
BuiltInFunction dereference;

template<class _T>
BuiltInFunction assign;

template<class _T>
BuiltInFunction add;

}

/* ---------------------------------------------------------------------------------------------------- */

}
} // namespace CINT::FUNCTION

/* ---------------------------------------------------------------------------------------------------- */

#endif