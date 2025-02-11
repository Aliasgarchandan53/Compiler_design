#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <iomanip>
#include <unordered_map>
#include <unordered_set>
using namespace std;

#define epsilon 'e'
class production
{
public:
    string lhs, rhs;
};

vector<production> Productions;
vector<char> T, NT;
char start;
unordered_map<char, vector<char>> firstSet;
unordered_map<char, vector<char>> followSet;

bool containsChar(vector<char> arr, char c)
{
    for (auto ch : arr)
    {
        if (ch == c)
            return true;
    }
    return false;
}

void ReadGrammar()
{
    int n;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        string s;
        cin >> s;
        production p;
        p.lhs = s[0];
        p.rhs = s.substr(3);
        if (i == 0)
            start = s[0];
        Productions.push_back(p);
        for (int i = 0; i < s.length(); i++)
        {
            if (s[i] == '-' || s[i] == '>')
                continue;
            else if (isupper(s[i]))
            {
                if (!containsChar(NT, s[i]))
                    NT.push_back(s[i]);
            }
            else
            {
                if (!containsChar(T, s[i]))
                    T.push_back(s[i]);
            }
        }
    }
}

vector<char> computeFirstSet(char c)
{
    vector<char> first;
    // if c is terminal
    if (containsChar(T, c))
    {
        first.push_back(c);
        return first;
    }
    // else
    for (auto prod : Productions)
    {
        if (prod.lhs[0] == c)
        {
            // check rhs
            for (int i = 0; i < prod.rhs.length(); i++)
            {
                if ((c == epsilon && prod.rhs[i] == 1) || containsChar(T, prod.rhs[i]))
                {
                    if (!containsChar(first, prod.rhs[i]))
                    {
                        first.push_back(prod.rhs[i]);
                        break;
                    }
                }
                else
                {
                    vector<char> temp;
                    if (prod.rhs[i] == c)
                        break;
                    if (firstSet.find(prod.rhs[i]) == firstSet.end())
                        temp = computeFirstSet(prod.rhs[i]);
                    else
                        temp = firstSet[prod.rhs[i]];
                    // if first of next symbol has epsilon or not
                    if (!containsChar(temp, epsilon))
                    {
                        for (auto ch : temp)
                        {
                            if (!containsChar(first, ch))
                                first.push_back(ch);
                        }
                        break;
                    }
                    else
                    {
                        for (auto ch : temp)
                        {
                            if (!containsChar(first, ch) && ch != epsilon)
                                first.push_back(ch);
                        }
                    }
                }
            }
        }
    }
    return first;
}
unordered_set<char> visitedFollow;
vector<char> computeFollowSet(char c)
{

    if (visitedFollow.find(c) != visitedFollow.end())
        return followSet[c];
    // mark visited
    visitedFollow.insert(c);
    vector<char> follow;
    if (c == start && !containsChar(follow, '$'))
        follow.push_back('$');
    for (auto p : Productions)
    {
        for (int i = 0; i < p.rhs.length(); i++)
        {
            if (p.rhs[i] == c)
            {
                if (i != p.rhs.length() - 1)
                {
                    {
                        // if its not last character
                        int next = p.rhs[i + 1];
                        if (containsChar(T, next))
                        {
                            if (!containsChar(follow, next))
                                follow.push_back(next);
                        }
                        else
                        {
                            // when next is nt
                            // find first of next
                            vector<char> fnext = firstSet[next];
                            for (auto ch : fnext)
                            {
                                if (!containsChar(follow, ch) && ch != epsilon)
                                    follow.push_back(ch);
                            }
                            if (containsChar(fnext, epsilon))
                            {
                                // if first has 'e'
                                vector<char> followC = followSet.find(p.lhs[0]) != followSet.end() ? followSet[p.lhs[0]] : computeFollowSet(p.lhs[0]);
                                for (char ch : followC)
                                {
                                    if (!containsChar(follow, ch))
                                        follow.push_back(ch);
                                }
                            }
                        }
                    }
                }
                else
                {
                    // when its last char
                    vector<char> temp = (followSet.find(p.lhs[0]) == followSet.end())
                                            ? computeFollowSet(p.lhs[0])
                                            : followSet[p.lhs[0]];

                    for (char ch : temp)
                    {
                        if (!containsChar(follow, ch))
                        {
                            follow.push_back(ch);
                        }
                    }
                }
            }
        }
    }
    followSet[c] = follow;
    return follow;
}

void printResult()
{
    cout << "Non-terminals :\n";
    for (auto nt : NT)
        cout << nt << ",";
    cout << endl;
    cout << "Terminals :\n";
    for (auto t : T)
        cout << t << ",";
    cout << endl;
    cout << "Start symbol : " << start << endl;
    cout << "Productions :\n";
    for (auto p : Productions)
        cout << p.lhs << "->" << p.rhs << endl;
    cout << "First sets :\n";
    for (auto fset : firstSet)
    {
        cout << fset.first << " : {";
        for (auto ch : fset.second)
            cout << ch << ",";
        cout << "}\n";
    }
    cout << "Follow sets :\n";
    for (auto fset : followSet)
    {
        cout << fset.first << " : {";
        for (auto ch : fset.second)
            cout << ch << ",";
        cout << "}\n";
    }
}

int main()
{
    ReadGrammar();
    for (int i = 0; i < NT.size(); i++)
        firstSet[NT[i]] = computeFirstSet(NT[i]);
    for (int i = 0; i < NT.size(); i++)
    {
        if (followSet.find(NT[i]) == followSet.end())
            computeFollowSet(NT[i]);
    }
    printResult();
    return 0;
}