#include <iostream>
#include "expr.h"
#include "nullary.h"
#include "unary.h"
#include "binary.h"
#include "core.h"

int main()
{
    sym::Const c = 2.0;
    sym::Var x = "x", y = "y";

    sym::Expr e = - c;



    sym::Expr e = c * x + y;


    //sym::Expr de = e.diff(x);


   std::cout << "expression: " << e << std::endl;
   //std::cout << "differention: " << de << std::endl;
   //std::cout << "equality with " << c << ": " << (de == c ? "true" : "false") << std::endl;
}