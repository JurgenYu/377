#include <map>
#include <queue>
#include <set>
#include <string>

#include "inverter.h"

using namespace std;

string build_inverted_index(string filename)
{
    ifstream in;
    in.open(filename);
    if (!in.is_open())
    {
        cerr << "Cannot open source file" << endl;
        return"";
    }
    string s;
    in >> s;
    if (s == ""){
        cerr << "Empty source file" << endl;
        return "";
    }
    int fileNum = 0;
    map<string, set<int>> invertedIndex;
    while (!in.eof())
    {
        string nameBuf;
        in >> nameBuf;
        if (nameBuf == "")
        {
            break;
        }
        ifstream thisFile;
        thisFile.open(nameBuf);
        if (!thisFile.is_open())
        {
            cerr << "Cannot open files" << endl;
            return"";
        }
        if (thisFile.eof())
        {
            continue;
        }
        while (!thisFile.eof())
        {
            string theWord;
            thisFile >> theWord;
            int head = 0;
            int pt = head;
            int pos = 0;
            for (string::iterator i = theWord.begin(); i != theWord.end(); ++i)
            {
                if (i + 1 == theWord.end())
                {
                    if (((*i >= 'a' && *i <= 'z') || (*i >= 'A' && *i <= 'Z')))
                    {
                        invertedIndex[theWord.substr(head, pos + 1)].insert(fileNum);
                    }
                    else
                    {
                        invertedIndex[theWord.substr(head, pos)].insert(fileNum);
                    }
                }
                if (!((*i >= 'a' && *i <= 'z') || (*i >= 'A' && *i <= 'Z')))
                {
                    invertedIndex[theWord.substr(head, pos)].insert(fileNum);
                    head = pt + 1;
                    pt++;
                    pos = 0;
                    continue;
                }
                ++pt;
                ++pos;
            }
        }
        ++fileNum;
    }
    invertedIndex.erase("");
    string returnString;
    for (map<string, set<int>>::iterator i = invertedIndex.begin(); i != invertedIndex.end(); ++i)
    {
        returnString.append(i->first);
        returnString.append(":");
        for (auto j = i->second.begin(); j != i->second.end(); ++j)
        {
            returnString.append(" ");
            returnString.append(to_string(*j));
        }
        returnString.append("\n");
    }
    return returnString;
}
