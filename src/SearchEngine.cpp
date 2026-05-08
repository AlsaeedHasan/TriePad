#include "SearchEngine.h"
using namespace std;

vector<int> SearchEngine::computeLPS(const string &pattern) const
{
    int m = pattern.length();
    vector<int> lps(m, 0);
    int len = 0;
    int i = 1;
    while (i < m)
    {
        if (pattern[i] == pattern[len])
        {
            len++;
            lps[i] = len;
            i++;
        }
        else
        {
            if (len != 0)
            {
                len = lps[len - 1];
            }
            else
            {
                lps[i] = 0;
                i++;
            }
        }
    }
    return lps;
}

vector<size_t> SearchEngine::searchAll(const string &text, const string &pattern) const
{
    vector<size_t> results;
    int n = text.length();
    int m = pattern.length();
    if (m == 0 || n < m)
        return results;

    vector<int> lps = computeLPS(pattern);
    int i = 0;
    int j = 0;
    while (n - i >= m - j)
    {
        if (pattern[j] == text[i])
        {
            j++;
            i++;
        }
        if (j == m)
        {
            results.push_back(i - j);
            j = lps[j - 1];
        }
        else if (i < n && pattern[j] != text[i])
        {
            if (j != 0)
            {
                j = lps[j - 1];
            }
            else
            {
                i++;
            }
        }
    }
    return results;
}

size_t SearchEngine::searchNext(const string &text, const string &pattern, size_t startPos) const
{
    if (startPos >= text.length())
        return string::npos;
    if (pattern.empty())
        return startPos;

    int n = text.length() - startPos;
    int m = pattern.length();
    if (n < m)
        return string::npos;

    vector<int> lps = computeLPS(pattern);
    int i = startPos;
    int j = 0;

    while (text.length() - i >= m - j)
    {
        if (pattern[j] == text[i])
        {
            j++;
            i++;
        }
        if (j == m)
        {
            return i - j;
        }
        else if (i < text.length() && pattern[j] != text[i])
        {
            if (j != 0)
            {
                j = lps[j - 1];
            }
            else
            {
                i++;
            }
        }
    }
    return string::npos;
}
