#include <iostream>

#include "parser.tab.h"
#include "utils.h"

using namespace std;

int main() {
    return yyparse();
}
