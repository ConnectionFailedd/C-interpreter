#ifndef __NAME_HPP__
#define __NAME_HPP__

#include <string>

/* ---------------------------------------------------------------------------------------------------- */

namespace CINT {

/* ---------------------------------------------------------------------------------------------------- */

class Name {
private:
    std::string __name;

public:
    inline Name(const std::string & _name) : __name(_name) {}
    inline Name(std::string && _name) noexcept : __name(std::move(_name)) {}
    inline Name(const char * _name) : __name(_name) {}

    inline operator const std::string &() const noexcept { return __name; }

    inline bool operator<(const Name & _rhs) const noexcept { return __name < _rhs.__name; }
    inline bool operator==(const std::string & _rhs) const noexcept { return __name == _rhs; }
};

/* ---------------------------------------------------------------------------------------------------- */

} // namespace CINT

/* ---------------------------------------------------------------------------------------------------- */

#endif /* __NAME_HPP__ */