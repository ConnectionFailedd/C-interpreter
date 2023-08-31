#ifndef __SCOPE_HPP__
#define __SCOPE_HPP__

#include <memory>
#include <set>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#define __GLOBAL_SCOPE_NAME__ "__NYA__"

/* ---------------------------------------------------------------------------------------------------- */

namespace CINT {

/* ---------------------------------------------------------------------------------------------------- */

class UnconfirmedScope;

class Match {
public:
    enum class Status : uint8_t {
        NOT_MATCHED = 0,
        SPECIFIED_MATCHED = 1,
        LOCAL_MATCHED = 2,
        USING_MATCHED = 4
    };

private:
    Status __status;

public:
    inline Match(Status _status) : __status(_status) {}

    inline operator Status() const noexcept{
        return __status;
    }

    inline operator bool() const noexcept {
        return __status != Match::Status::NOT_MATCHED;
    }
};

class Scope {
private:
    std::vector<std::string> __scopeNames;

public:
    inline Scope() noexcept : __scopeNames({__GLOBAL_SCOPE_NAME__}) {}
    inline Scope(std::vector<std::string> && _scopeNames) : __scopeNames(std::move(_scopeNames)) {}
    inline Scope(const Scope & _src) : __scopeNames(_src.__scopeNames) {}
    inline Scope(Scope && _src) noexcept : __scopeNames(std::move(_src.__scopeNames)) {}

    inline bool operator<(const Scope & _rhs) const & { return __scopeNames.back() < _rhs.__scopeNames.back(); }
    inline bool operator==(const Scope & _rhs) const & { return __scopeNames == _rhs.__scopeNames; }
    inline bool operator!=(const Scope & _rhs) const & { return !operator==(_rhs); }
    Scope operator+(const Scope &) const &;
    Scope operator+(const std::string &) const &;
    Match operator==(const UnconfirmedScope &) const &;

public:
    class ScopeMultiSet {
    public:
        class SharedPtrScopeCmp {
        public:
            inline bool operator()(const std::shared_ptr<Scope> & _lhs, const std::shared_ptr<Scope> & _rhs) const & {
                return *_lhs < *_rhs;
            }
        };

    private:
        std::multiset<std::shared_ptr<Scope>, SharedPtrScopeCmp> __scopeMultiSet;

    public:
        inline ScopeMultiSet(std::multiset<std::shared_ptr<Scope>, SharedPtrScopeCmp> && _scopeMultiSet) : __scopeMultiSet(_scopeMultiSet) {}

        static ScopeMultiSet init();

        inline void insert(const std::shared_ptr<Scope> & _src) { __scopeMultiSet.insert(_src); }
        inline void insert(std::shared_ptr<Scope> && _src) { __scopeMultiSet.insert(std::move(_src)); }
        std::shared_ptr<Scope> find(const UnconfirmedScope &) const &;
    };

public:
    static std::vector<std::shared_ptr<Scope>> currentScopeStack;

    const static std::shared_ptr<Scope> NOSCOPE;
    const static std::shared_ptr<Scope> GLOBAL_SCOPE;

    static ScopeMultiSet scopeMultiSet;
};

class UnconfirmedScope {
private:
    std::shared_ptr<Scope> __specifiedScope;
    std::vector<std::shared_ptr<Scope>> __currentScopeStack;
    std::vector<std::shared_ptr<Scope>> __usingScopes;
    std::string __baseName;

public:
    inline UnconfirmedScope(const std::shared_ptr<Scope> & _specifiedScope, const std::vector<std::shared_ptr<Scope>> & _currentScopeStack, const std::vector<std::shared_ptr<Scope>> & _usingScopes, const std::string & _baseName) : __currentScopeStack(_currentScopeStack), __specifiedScope(_specifiedScope), __usingScopes(_usingScopes), __baseName(_baseName) {}

    inline const std::shared_ptr<Scope> & specified_scope() const noexcept { return __specifiedScope; }
    inline const std::vector<std::shared_ptr<Scope>> current_scope_stack() const noexcept { return __currentScopeStack; }
    inline const std::vector<std::shared_ptr<Scope>> using_scopes() const noexcept { return __usingScopes; }
    inline const std::string & base_name() const noexcept { return __baseName; }

    inline Match operator==(const Scope & _rhs) const & { return _rhs == *this; }
};

/* ---------------------------------------------------------------------------------------------------- */

} // namespace CINT

/* ---------------------------------------------------------------------------------------------------- */

#endif /* __SCOPE_HPP__ */