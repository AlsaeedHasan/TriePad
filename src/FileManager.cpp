#include "FileManager.h"
#include <fstream>
#include <sstream>

using namespace std;

bool OpenFile(shared_ptr<TextBuffer> &buffer, const string &filepath)
{
    ifstream file(filepath);
    if (!file.is_open())
        return false;

    stringstream ss;
    ss << file.rdbuf();
    buffer->load(ss.str());
    file.close();
    return true;
}

bool SaveToFile(const shared_ptr<TextBuffer> &buffer, const string &filepath)
{
    ofstream file(filepath);
    if (!file.is_open())
        return false;

    file << buffer->save();
    file.close();
    return true;
}