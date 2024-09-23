#include <bits/stdc++.h>
using namespace std;

#define epsilon 'e' ;

class Production{
    public:
        string lhs,rhs;
};

vector<Production> productions;
vector<char> NT;
vector<char> T;
unordered_map<char,vector<char>> leadingSet;
unordered_map<char,vector<char>> trailingSet;
vector<vector<int>> parsingTable;


bool contains(vector<char> arr,char c){
    for(auto ch:arr){
        if(ch==c)
            return true;
    }
    return false;
}

bool isTerminal(char c){
    for(auto t:T){
        if(t==c)
            return true;
    }
    return false;
}

bool isNonTerminal(char c){
    for(auto nt:NT){
        if(nt==c)
            return true;
    }
    return false;
}

void ReadGrammar(){
    int n;
    cin>>n;
    for(int i=0;i<n;i++){
        string s;
        cin>>s;
        Production p;
        p.lhs=s[0];
        p.rhs=s.substr(3);
        productions.push_back(p);
        for(int j=0;j<s.length();j++){
            if(s[j]=='-'||s[j]=='>')
                continue;
            else if(isupper(s[j])){
                if(!contains(NT,s[j]))
                    NT.push_back(s[j]);
            }else{
                if(!contains(T,s[j]))
                    T.push_back(s[j]);
            }
        }
    }
}

vector<char> findLeading(char c){
    vector<char> leadC;
    for(auto p:productions){
        if(p.lhs[0]==c){
            for(int j=0;j<p.rhs.length();j++){
                if(isTerminal(p.rhs[j])){
                    leadC.push_back(p.rhs[j]);
                    break;
                }else{
                    if(p.rhs[j]==c)
                        continue;
                    vector<char> temp;
                    if(leadingSet.find(p.rhs[j])==leadingSet.end()){
                        temp = findLeading(p.rhs[j]);
                        
                    }else{
                        temp=leadingSet.at(p.rhs[j]);
                    }
                    for(auto ch:temp){
                            if(!contains(leadC,ch))
                                leadC.push_back(ch);
                    }
                }
            }
        }
    }
    return leadC;
}

vector<char> findTrailing(char c){
    vector<char> trailC;
    for(auto p:productions){
        if(p.lhs[0]==c){
            for(int j=p.rhs.length()-1;j>=0;j--){
                if(isTerminal(p.rhs[j])){
                    trailC.push_back(p.rhs[j]);
                    break;
                }else{
                    if(p.rhs[j]==c)
                        continue;
                    vector<char> temp;
                    if(trailingSet.find(p.rhs[j])==trailingSet.end()){
                        temp = findTrailing(p.rhs[j]);
                        
                    }else{
                        temp=trailingSet.at(p.rhs[j]);
                    }
                    for(auto ch:temp){
                            if(!contains(trailC,ch))
                                trailC.push_back(ch);
                    }
                }
            }
        }
    }
    return trailC;
}

void result(){
    cout<<"Terminals :\n";
    for(auto t:T)
        cout<<t<<",";
    cout<<endl;
    cout<<"Non-terminals :\n";
    for(auto nt:NT)
        cout<<nt<<",";
    cout<<endl;
    cout<<"Productions :\n";
    for(auto p :productions)
        cout<<p.lhs<<" : "<<p.rhs<<endl;
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

int main(){
    ReadGrammar();
    for(int i=0;i<NT.size();i++){
        leadingSet[NT[i]] = findLeading(NT[i]);
        trailingSet[NT[i]]=findTrailing(NT[i]);
    }
    result();
    return 0;
}