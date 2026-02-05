#include <iostream>

int main() {

    // --------------------------------------------------
    // 1) Operator precedence: * runs before +
    // --------------------------------------------------
    int a = 10 + 5 * 2;
    // Evaluated as: 10 + (5 * 2)
    // Result: 20

    int b = (10 + 5) * 2;
    // Parentheses override precedence
    // Result: 30

    std::cout << "a = " << a << "\n";
    std::cout << "b = " << b << "\n\n";

    // --------------------------------------------------
    // 2) Operator precedence example
    // --------------------------------------------------
    int s1 = 2 - 3 * 4;
    // '*' has higher precedence than '-'
    // Evaluated as: 2 - (3 * 4)
    // Result: -10

    std::cout << "2 - 3 * 4 = " << s1 << "\n\n";

    // --------------------------------------------------
    // 3) Comparison vs arithmetic precedence
    // --------------------------------------------------
    bool c = 5 + 3 > 6;
    // '+' runs before '>'
    // Evaluated as: (5 + 3) > 6 → true

    std::cout << "c = " << c << "\n\n";

    // --------------------------------------------------
    // 4) Equality vs assignment
    // --------------------------------------------------
    int x = 10;
    bool d = (x == 10);
    // '==' compares values
    // '=' assigns values

    std::cout << "d = " << d << "\n\n";

    // --------------------------------------------------
    // 5) Logical operator precedence
    // --------------------------------------------------
    bool e = true || false && false;
    // '&&' has higher precedence than '||'
    // Evaluated as: true || (false && false)
    // Result: true

    bool f = (true || false) && false;
    // Parentheses change evaluation order
    // Result: false

    std::cout << "e = " << e << "\n";
    std::cout << "f = " << f << "\n\n";


    // --------------------------------------------------
    // 7) Associativity: assignment (right-to-left)
    // --------------------------------------------------
    int y = 5;

    x = y;
    // '=' associates right-to-left
    // x gets the value of y

    std::cout << "(x, y) after x = y : "
              << x << ", " << y << "\n";

    // --------------------------------------------------
    // 8) Compound assignment associativity 
    // --------------------------------------------------
    x += y -= 2;
    // '+=' and '-=' have the same precedence
    // Both associate right-to-left
    //
    // Step-by-step:
    // 1) y -= 2  → y = 3
    // 2) x += y  → x = 5 + 3 = 8

    std::cout << "(x, y) after x += y -= 2 : "
              << x << ", " << y << "\n\n";

    // --------------------------------------------------
    // 9) Prefix vs postfix operators
    // --------------------------------------------------
    int n = 5;
    int h = ++n * 2;
    // '++' runs before '*'
    // n becomes 6, then multiplication happens
    // h = 12

    std::cout << "h = " << h << "\n";
    std::cout << "n = " << n << "\n";

    return 0;
}

  //g++ -o operators operators.cpp && ./operators