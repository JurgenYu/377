#include <map>
#include <queue>
#include <set>
#include <string>

#include "inverter.h"

using namespace std;

int error_handle()
{
}

string build_inverted_index(string filename)
{
    FILE *fp = fopen(filename.c_str(), "r");
    if (fp == 0)
    {
        cerr << "Cannot open file" << endl;
    }
    return "";
}
