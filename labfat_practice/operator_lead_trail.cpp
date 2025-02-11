#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <unordered_map>
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
unordered_map<char, vector<char>> leadingSet;
unordered_map<char, vector<char>> trailingSet;

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
        // epsilon in production
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

bool isOperatorGrammar()
{
    for (auto p : Productions)
    {
        if (p.rhs[0] == epsilon)
            return false;
        if (p.rhs.length() > 1)
        {
            for (int i = 0; i < p.rhs.length() - 1; i++)
            {
                if (isupper(p.rhs[i]) && isupper(p.rhs[i + 1]))
                    return false;
            }
        }
    }
    return true;
}

vector<char> computeLeading(char c){
    vector<char> leading;
    for(auto p:Productions){
        if(p.lhs[0]==c){
            for(int i=0;i<p.rhs.length();i++){
                if(containsChar(T,p.rhs[i])){
                    if(!containsChar(leading,p.rhs[i]))
                        leading.push_back(p.rhs[i]);
                    break;
                }else{
                    if(p.rhs[i]==c)
                        continue;
                    vector<char> leadC = leadingSet.find(p.rhs[i])==leadingSet.end()?
                                        computeLeading(p.rhs[i]):leadingSet[p.rhs[i]];
                    for(auto ch:leadC){
                        if(!containsChar(leading,ch))leading.push_back(ch);
                    }
                }
            }
        }
    }
    return leading;
}

vector<char> computeTrailing(char c){
    vector<char> trailing;
    for(auto p:Productions){
        if(p.lhs[0]==c){
            for(int i=p.rhs.length()-1;i>=0;i--){
                if(containsChar(T,p.rhs[i])){
                    if(!containsChar(trailing,p.rhs[i]))
                        trailing.push_back(p.rhs[i]);
                    break;
                }else{
                    if(p.rhs[i]==c)
                        continue;
                    vector<char> trailC = trailingSet.find(p.rhs[i])==trailingSet.end()?
                                        computeTrailing(p.rhs[i]):trailingSet[p.rhs[i]];
                    for(auto ch:trailC){
                        if(!containsChar(trailing,ch))trailing.push_back(ch);
                    }
                }
            }
        }
    }
    return trailing;
}

void printResult()
{   
    if(!isOperatorGrammar()){
        cout<<"The given grammar is not operator grammar.\n";
        return;
    }else  
        cout<<"The given grammar is operator grammar.\n";
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
    cout<<"Leading sets :\n";
    for(auto lset:leadingSet){
        cout<<lset.first<<" : {";
        for(auto ch:lset.second){
            cout<<ch<<",";
        }
        cout<<"}\n";
    }
    cout<<"Trailing sets :\n";
    for(auto tset:trailingSet){
        cout<<tset.first<<" : {";
        for(auto ch:tset.second){
            cout<<ch<<",";
        }
        cout<<"}\n";
    }
}


int main()
{
    ReadGrammar();
    for(int i=0;i<NT.size();i++){
        if(leadingSet.find(NT[i])==leadingSet.end())
            leadingSet[NT[i]]=computeLeading(NT[i]);
        if(trailingSet.find(NT[i])==trailingSet.end())
            trailingSet[NT[i]]=computeTrailing(NT[i]);
    }
    printResult();
    return 0;
}