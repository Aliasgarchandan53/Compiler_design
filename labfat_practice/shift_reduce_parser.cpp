#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <iomanip>
using namespace std;

class production{
    public:
    string lhs,rhs;
};

vector<production> Productions;
vector<char> T,NT;
char start;
bool  containsChar(vector<char> arr,char c){
    for(auto ch:arr){
        if(ch==c)return true;
    }
    return false;
}

void ReadGrammar(){
    int n;
    cin>>n;
    for(int i=0;i<n;i++){
        string s;
        cin>>s;
        production p;
        p.lhs=s[0];
        p.rhs=s.substr(3);
        if(i==0)start=s[0];
        Productions.push_back(p);
        for(int i=0;i<s.length();i++){
            if(s[i]=='-' || s[i]=='>')continue;
            else if(isupper(s[i])){
                if(!containsChar(NT,s[i]))NT.push_back(s[i]);
            }else{
                if(!containsChar(T,s[i]))T.push_back(s[i]);
            }
        }
    }
}

void printGrammar(){
    cout<<"Non-terminals :\n";
    for(auto nt:NT)cout<<nt<<",";
    cout<<endl;    
    cout<<"Terminals :\n";
    for(auto t:T)cout<<t<<",";
    cout<<endl;    
    cout<<"Start symbol : "<<start<<endl;
    cout<<"Productions :\n";
    for(auto p:Productions)cout<<p.lhs<<"->"<<p.rhs<<endl;
}

void shiftReduceParser(){
    cout<<"Enter the input string (-1 for exit) :\n";
    string inputBuffer;
    cin>>inputBuffer;
    if(inputBuffer=="-1")return;
    inputBuffer+='$';
    stack<char> stk;
    stk.push('$');

    while(true){
        if(stk.top()==start && inputBuffer=="$"){
            cout<<"The string is accepted.\n";
            break;
        }
        //checking reduce action
        stack<char> temp = stk;
        bool flag=true;
        string str;
        while(temp.top()!='$' && flag){
            str=temp.top()+str;
            temp.pop();
            for(auto prod:Productions){
                if(prod.rhs==str){
                    if(flag){
                        for(int i=0;i<prod.rhs.length();i++)stk.pop();
                        stk.push(prod.lhs[0]);
                        flag=false;
                        //reduced - print step
                    }else{
                        cout<<"Error : Reduce-reduce conflict.\n";
                        return;
                    }
                }
            }
        }
        if(temp.top()=='$' && inputBuffer=="$" && flag){
            cout<<"Error : Error matching the string.\n";
            //print step
            break;
        }
        //shift action
        if(flag && inputBuffer!="$"){
            stk.push(inputBuffer[0]);
            inputBuffer.erase(0,1);
            flag=false;
            //print step
        }
    }
    shiftReduceParser(); 
}

int main(){
    ReadGrammar();
    printGrammar();
    shiftReduceParser();
    return 0;
}