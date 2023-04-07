#ifndef __NAME_HPP__
#define __NAME_HPP__

#include <string>
#include <vector>

#include "scope.hpp"

/* ---------------------------------------------------------------------------------------------------- */

namespace CINT {

/* ---------------------------------------------------------------------------------------------------- */

class Name {
private:
    const Scope __scope;
    const std::string __baseName;

public:
    const static Name noName;

public:
    Name(const Scope & _scope, const std::string & _baseName) : __scope(_scope), __baseName(_baseName) {}
    Name(const Scope & _scope, std::string && _baseName) : __scope(_scope), __baseName(std::move(_baseName)) {}

    bool match(const Scope & _currentScope, const Scope & _specifiedScope, const std::vector<Scope> & _usingScope, const std::string & _baseName);
};

/* ---------------------------------------------------------------------------------------------------- */

} // namespace CINT

/* ---------------------------------------------------------------------------------------------------- */

#endif /* __NAME_HPP__ */