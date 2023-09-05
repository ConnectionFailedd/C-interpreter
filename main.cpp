#include "name.hpp"
#include "driver.hpp"
#include <functional>
#include <memory>
#include <new>
#include <string>
#include <vector>

int main(int argc, char * argv[]) {
    // auto int_type = std::shared_ptr<CINT::Type::Type>();
    // if(CINT::Type::Type::typeMultiSet.find(std::make_shared<CINT::Type::BuiltInType>(0, std::align_val_t(0), CINT::Name("int"))) != CINT::Type::Type::typeMultiSet.end()) {
    //     int_type = *CINT::Type::Type::typeMultiSet.find(std::make_shared<CINT::Type::BuiltInType>(0, std::align_val_t(0), CINT::Name("int")));
    // }
    // auto value1 = std::make_shared<CINT::Value>(int_type, false, false);
    // value1->set_value(5);
    // auto value2 = std::make_shared<CINT::Value>(int_type, false, false);
    // value2->set_value(10);
    // auto fixedValueNode1 = CINT::SyntaxTree::make_fixed_value_node(value1);
    // auto fixedValueNode2 = CINT::SyntaxTree::make_fixed_value_node(value2);
    // auto arguments = std::vector({fixedValueNode1, fixedValueNode2});
    // auto func = std::function<std::shared_ptr<CINT::Value>(const std::vector<std::shared_ptr<CINT::Value>> &)>();
    // if(CINT::Function::builtInFunctionMultiMap.find(CINT::Function::Signature(CINT::Name("+"))) != CINT::Function::builtInFunctionMultiMap.end()) {
    //     func = CINT::Function::builtInFunctionMultiMap.find(CINT::Function::Signature(CINT::Name("+")))->second;
    // }
    // auto funcNode = CINT::SyntaxTree::make_built_in_function_node(func, arguments);
    // auto returnNode = CINT::SyntaxTree::make_return_node(funcNode);
    // auto tree = CINT::SyntaxTree(returnNode);
    // CINT::SyntaxTree::functionStack.push({}, {});
    // tree.evaluate();
    // int returnValue = CINT::SyntaxTree::functionStack.get_return_value()->get_value<int>();
    // return 0;


    CINT::PreProcess::Driver driver;
    driver.parse("test.txt");
    std::cout << driver.location << std::endl;
    return 0;
}