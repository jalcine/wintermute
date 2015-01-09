#define CXXTEST_HAVE_EH
#define CXXTEST_ABORT_TEST_ON_FAIL
#include <cxxtest/ErrorPrinter.h>

int main()
{
    std::cout << "[wntr] Test suite starting..." << std::endl;
    int status = CxxTest::ErrorPrinter().run();
    std::cout << "[wntr] Test suite complete." << std::endl;
    return status;
}

<CxxTest world>
