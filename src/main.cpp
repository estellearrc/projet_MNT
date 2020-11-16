#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <proj_api.h>

using namespace std;

int main()
{
    cout << "Hello world" << endl;
    projLPZ coord = projUVW{48.19517876, -3.028937895, -117.442};

    return EXIT_SUCCESS;
}