#include <iostream>
#include "matrix.h"

using namespace std;

int main() {
    matrix<int> m({{1, 2, 3},
                   {2, 1, 4}});
    matrix<double> m2({{3.5, 2.1, 1.2}});
    matrix<int> m4({{1, 2, 3}});

    matrix<double> nn = m * m4.transpose();
    cout << nn << endl;

    matrix<double> n = m;
    const matrix<int> mm = m;

    cout << "n size:" << n.size()[0] << " " << n.size()[1] << endl;

    cout << n << endl;
    cout << mm.transpose().transpose() << endl;

    cout << m.transpose() * 2.2 << endl;
    cout << 2.2 * -m << endl;
    cout << m * m2.transpose() << endl;
    cout << m2 * m4.transpose() << endl;
    auto m5 = m2 * m4.transpose();
    cout << m5 << endl;

    //cout << m4 * m4.transpose() * m << endl;

    matrix<int> m3 = m * m2.transpose();
    cout << m3 << endl;
    auto m6 = m5 * m;
    cout << m4 * m4.transpose() * m6 << endl;
    cout << m6 << "+" << m << "=";
    cout << m6 + m << endl;
    cout << m6 << "-" << m << "=" << m6 - m << endl;

    return 0;
}

