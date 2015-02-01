//#define CXXTEST_HAVE_EH
//#define CXXTEST_ABORT_TEST_ON_FAIL
#include <iostream>
#include <cxxtest/ErrorPrinter.h>
using std::cout;
using std::endl;

int main()
{
    cout << "[wntr] Test suite starting..." << endl;
    int status = CxxTest::ErrorPrinter().run();
    cout << "[wntr] Test suite complete with status " << status << endl;
    return status;
}

<CxxTest world>
