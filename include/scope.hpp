#ifndef __SCOPE_HPP__
#define __SCOPE_HPP__

#include <memory>
#include <set>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#include "function.hpp"
#include "name.hpp"
#include "type.hpp"
#include "variable.hpp"

#define __GLOBAL_SCOPE_NAME__ "__NYA__"

/* ---------------------------------------------------------------------------------------------------- */

namespace CINT {

/* ---------------------------------------------------------------------------------------------------- */

class Scope {
public:
    using ScopeName = Name;

    class ScopeSet {
    public:
        class SharedPtrScopeCmp {
        public:
            bool operator()(const std::shared_ptr<Scope> & _lhs, const std::shared_ptr<Scope> & _rhs) const noexcept {
                return *_lhs < *_rhs;
            }
        };

    private:
        std::set<std::shared_ptr<Scope>, SharedPtrScopeCmp> __scopeSet;

    public:
        inline ScopeSet() : __scopeSet() {}
        inline ScopeSet(std::set<std::shared_ptr<Scope>, SharedPtrScopeCmp> && _scopeSet) : __scopeSet(std::move(_scopeSet)) {}

        inline bool insert(const std::shared_ptr<Scope> & _src) { return __scopeSet.insert(_src).second; }
        inline bool insert(std::shared_ptr<Scope> && _src) { return __scopeSet.insert(std::move(_src)).second; }
        inline const std::shared_ptr<Scope> & find(const Name & _key) {
            auto iter = __scopeSet.find(std::make_shared<Scope>(_key));
            if(iter != __scopeSet.end()) return *iter;
            return NOSCOPE;
        }
    };

private:
    ScopeName __scopeName;

    Type::TypeSet __typeSet;
    Variable::VariableSet __variableSet;
    Function::FunctionSet __functionSet;
    ScopeSet __scopeSet;

public:
    inline Scope(const ScopeName & _scopeName) : __scopeName(_scopeName), __typeSet(), __variableSet(), __functionSet(), __scopeSet() {}
    inline Scope(ScopeName && _scopeName) : __scopeName(std::move(_scopeName)), __typeSet(), __variableSet(), __functionSet(), __scopeSet() {}

    inline const ScopeName & scope_name() const noexcept { return __scopeName; }
    inline Type::TypeSet & type_set() noexcept { return __typeSet; }
    inline Variable::VariableSet & varibale_set() noexcept { return __variableSet; }
    inline Function::FunctionSet & function_set() noexcept { return __functionSet; }
    inline ScopeSet & scope_set() noexcept { return __scopeSet; }

    inline bool operator<(const Scope & _rhs) const noexcept { return __scopeName < _rhs.__scopeName; }

public:
    static const std::shared_ptr<Scope> NOSCOPE;

    static std::vector<std::shared_ptr<Scope>> currentScopeStack;
    static std::vector<std::shared_ptr<Scope>> globalUsingScopes;
};

/* ---------------------------------------------------------------------------------------------------- */

} // namespace CINT

/* ---------------------------------------------------------------------------------------------------- */

#endif /* __SCOPE_HPP__ */