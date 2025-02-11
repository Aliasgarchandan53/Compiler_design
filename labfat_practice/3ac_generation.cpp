#include <bits/stdc++.h>
using namespace std;

int precedence(char c){
    switch(c){
        case  '+': case '-' : return 1;
        case '*' : case '/' : return 2;
        case '(' : case ')' : return -1;
        default : cout<<"unknown operator \n"; return -2;
    }
}

string getPostfixExpr(string code){
    //a=b+c*d ie bcd*+
    stack<char> stk;
    string postfix="";
    for(char c:code ){
        if(isalnum(c))
            postfix+=c;
        else if(c=='('){
            stk.push(c);
        }else if(c==')'){
            while(!stk.empty() && stk.top()!='('){
                postfix+=stk.top();
                stk.pop();
            }
            stk.pop();
        }else{
            int currPrecedence = precedence(c);
            while(!stk.empty() && currPrecedence<=precedence(stk.top())){
                postfix+=stk.top();
                stk.pop();
            }
            stk.push(c);
        }
    }
    while(!stk.empty()){
        postfix+=stk.top();
        stk.pop();
    }
    return postfix;
}

void getThreeAddressCode(string postfix,char start){
    stack<string> stk;
    int tempIndex=1;
    //traverse postfix expression
    for(char c:postfix){

        if(isalnum(c))
            stk.push(string(1,c));
        else{
            //operator
            string temp1=stk.top();
            stk.pop();
            string temp2=stk.top();
            stk.pop();

            string tempVar = "temp"+to_string(tempIndex++);
            cout<<tempVar<<" = "<<temp2<<" "<<c<<" "<<temp1<<endl;
            stk.push(tempVar);
        }
    }
    cout<<string(1,start)<<" = temp"<<to_string(--tempIndex)<<endl;
}

int main(){
    string code;
    cout<<"Enter code expression :\n";
    cin>>code;
    string postfix=getPostfixExpr(code.substr(2));
    cout<<"Postfix expression : "<<postfix<<endl;
    cout<<"Three address code :\n";
    getThreeAddressCode(postfix,code[0]);
    return 0;
}