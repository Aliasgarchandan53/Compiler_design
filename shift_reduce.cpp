#include <bits/stdc++.h>
using namespace std;

#define epsilon 'e' 

class Production{
    public:
        string lhs,rhs;
};

vector<Production> productions;
vector<char> NT;
vector<char> T;
char startSymbol;

bool contains(vector<char> arr,char c){
    for(auto ch:arr){
        if(ch==c)
            return true;
    }
    return false; 
}
void remove(vector<char> arr,char c){
    auto it= find(arr.begin(),arr.end(),c);
    arr.erase(it);
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
        if(i==0)startSymbol=s[0];
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

void printParsingStep(stack<char> &stk,string inputBuffer, string action){
    stack<char> temp=stk;
    string str;
    while(!temp.empty()){
        str=temp.top()+str;
        temp.pop();
    }
    cout<<setw(20)<<left<<str
    <<setw(20)<<left<<inputBuffer
    <<setw(30)<<left<<action<<endl;
}

void Shift_reduce_parser(){
    string inputBuffer;
    cout<<"Enter the input string (-1 to exit):\n";
    cin>>inputBuffer;
    if(inputBuffer=="-1")return ;
    inputBuffer+='$';
    stack<char> stk;
    stk.push('$');
    cout << setw(20) << left << "Stack"
         << setw(20) << left << "Input Buffer"
         << setw(30) << left << "Action" << endl;
    cout << string(70, '-') << endl;
    while(true){
        if(stk.top()==startSymbol && inputBuffer=="$"){
            cout<<"String is accepted\n";
            break;
        }
        string str;
        stack<char> temp =stk;
        bool flag=true;
        while(temp.top()!='$' && flag){
            str=temp.top()+str;
            temp.pop();
            for(auto prod:productions){
                if(prod.rhs==str && flag){
                    for(int i=0;i<prod.rhs.length();i++)
                        stk.pop();
                    stk.push(prod.lhs[0]);
                    printParsingStep(stk,inputBuffer,"reduce "+prod.lhs+" -> "+prod.rhs);
                    flag=false;
                }
                else if(prod.rhs==str && !flag){
                    cout<<"Reduce - Reduce conflict\n";
                    return;
                }
            }
        }
        if(temp.top()=='$' && inputBuffer=="$" && flag){
            printParsingStep(stk,inputBuffer,"Error: string cannot be parsed\n");
            return;
        }
        if(flag && inputBuffer!="$"){
            stk.push(inputBuffer[0]);
            inputBuffer.erase(0,1);
            printParsingStep(stk,inputBuffer,"shift");
            flag=false;
        }
    }
    Shift_reduce_parser();
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
}

int main(){
    ReadGrammar();
    result();
    Shift_reduce_parser();
    return 0;
}