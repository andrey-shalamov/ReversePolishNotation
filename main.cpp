#include <iostream>
#include "rpn.h"

using namespace std;

int main() {
    std::string sExpression = "(2+2)*(2-(1-5))/2.5";

    rpn<float> rpn_;
    float res = 0.f;
    rpn<float>::error error = rpn_.calculate(sExpression, res);
    if (!error) {
        cout << sExpression << " = " << res << endl;
    }
    else {
        std::cerr << error.what_ << std::endl;
    }
    return 0;
}