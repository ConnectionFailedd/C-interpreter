#include "function.hpp"
#include "syntax_tree.hpp"
#include "type.hpp"
#include <memory>

/* ---------------------------------------------------------------------------------------------------- */

namespace CINT::IR {

/* ---------------------------------------------------------------------------------------------------- */

const std::shared_ptr<Type> & FunctionNode::type() const { return __function->return_type(); } 

/* ---------------------------------------------------------------------------------------------------- */

}

/* ---------------------------------------------------------------------------------------------------- */
