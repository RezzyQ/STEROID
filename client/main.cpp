#include <iostream>
#include "connection.cpp"
#include "interface.h"
#include "errno.h"
#include <system_error>
using namespace std;

int main(int argc, const char** argv)
{
    Interface interface;
    if(!interface.Parser(argc, argv)){
        cout << interface.getDescription() << endl;
        return 1;
    }
    Params params = interface.getParams();
    conn(&params);
    return 0;
}
