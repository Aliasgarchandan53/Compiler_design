#include <bits/stdc++.h>
using namespace std;

/*
    - assume scope to be main and if statements
    - assume memory addresses to start at 1000
    -components scope,symbol name,type,address,used
*/
int symbol_address=1000;
class symbol{
    public:
    string scope,name,type,value;
    bool used;
    int address;
    symbol(string token,string type){
        this->name=token;
        this->type=type;
        value="uninitialised";
        this->address=symbol_address;
        symbol_address++;  
    }
};


vector<symbol*> symbolTable;
vector<string> Tokens;
vector<string> keywords = {"void","int","char","string","bool","float","long","void*","int*","char*","string*","bool*","float*","long*","include","stdio.h","main","return","new","if","else","printf","scanf"};
vector<char> operators={'+','-','/','*'};

bool containsSymbol(string token){
    for(auto symb:symbolTable){
        if(token==symb->name)return true;
    }
    return false;
}

bool isIdentifier(string token){
    if(!(find(keywords.begin(),keywords.end(),token)==keywords.end()))return false;
    if(!isalpha(token[0]) && token[0]!='_')return false;
    return true;
}

void getTokensFromString(string str){
    string token;
    for(int i=0;i<str.length();i++){
        //skip paranthesis , angular brackets , #
        if(str[i]=='(' || str[i]==')' || str[i]=='<' || str[i]=='>' || str[i]=='#' || str[i]=='{' || str[i]=='}' ||str[i]==';'||str[i]==','||str[i]==' ')continue;
        else if(str[i]=='"'||str[i]=='\''){
            Tokens.push_back(token);
            token="";
        }
        else if(token=="void"||token=="int"||token=="char"||token=="bool"||token=="float"||token=="string"  && str[i]=='*')token+=str[i];
        else if(str[i]=='+' || str[i]=='-' || str[i]=='*' || str[i]=='/'){
            Tokens.push_back(token);
            Tokens.push_back(to_string(str[i]));
            token="";
        }else token+=str[i];
    }
    Tokens.push_back(token);
}

void formSymbolTable(){
    //classification
    for(int i=0;i<Tokens.size();i++){
        if(!containsSymbol(Tokens[i])){
            if(isIdentifier(Tokens[i]))
                symbolTable.push_back(new symbol(Tokens[i],Tokens[i-1]));
        }
    }
    for(auto  sym:symbolTable){
        cout<<sym->name<<" : "<<sym->type<<" : "<<sym->address<<endl;
    }
}

int main(){
    //reading file 
    fstream file;
    file.open("input.c",ios::in);
    if(!file){
        cout<<"Error reading file \n";
    }else{
        string str;
        while(file>>str){
            getTokensFromString(str);
        }
        cout<<"Tokens \n";
        for(auto tok:Tokens)cout<<tok<<endl;
        //symbol table formation
        formSymbolTable();
        
    }
    return 0;
}
