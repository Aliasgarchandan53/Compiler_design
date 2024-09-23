#include <bits/stdc++.h>
using namespace std;

#define epsilon 'e' 
#define start "S1"
class Production{
    public:
        string lhs,rhs;
};
class LR0_prod{
    public:
    Production prod;
    int dIndex;//position of dot
    LR0_prod(Production prod){
        this->prod=prod;
        dIndex=0;
    }
    void printProd(){
        cout<<prod.lhs<<"->";
        for(int i=0;i<prod.rhs.length();i++){
            if(i==dIndex)
                cout<<".";
            cout<<prod.rhs[i];
        }
        if(dIndex==prod.rhs.length())cout<<".";
        cout<<endl;
    }
    void next(){
        dIndex++;
    }
    char getNext(){
        if(dIndex==prod.rhs.length())return NULL;
        return prod.rhs[dIndex];
    }
    bool operator==(LR0_prod other){
         return (dIndex == other.dIndex && prod.lhs == other.prod.lhs && prod.rhs == other.prod.rhs);
    }
};

class state{
    public:
    int sr;
    vector<LR0_prod> currentState;
    state(vector<LR0_prod> currentState,int sr){
        this->currentState=currentState;
        this->sr=sr;
    }
    void printState(){
        cout<<"I"<<sr<<":\n";
        for(auto p:currentState)
            p.printProd();
    }
    bool operator==(state other){
    if (currentState.size() != other.currentState.size()) return false;
    for (auto &p1 : currentState) {
        bool found = false;
        for (auto &p2 : other.currentState) {
            if (p1 == p2) {
                found = true;
                break;
            }
        }
        if (!found) return false;
    }
    return true;
}
};
vector<Production> productions;
vector<char> NT;
vector<char> T;
vector<char> symbols;
unordered_map<char,vector<char>> firstSet;
vector<state> States;
vector<pair<int,int>> gotos;
char startSymbol;
//parsing tables


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
            else{
                 if(isupper(s[j])){
                if(!contains(NT,s[j]))
                    NT.push_back(s[j]);
                }else{
                if(!contains(T,s[j]))
                    T.push_back(s[j]);
                }
                if(!contains(symbols,s[j]))
                    symbols.push_back(s[j]);
            }
        }
    }
}

vector<char> computeFirst(char c){
    vector<char> firstc;
    //if c is a terminal
    if(isTerminal(c)){
        firstc.push_back(c);
        return firstc;
    }
    for(auto p:productions){
        if(p.lhs[0]==c){
            for(int j=0;j<p.rhs.length();j++){
                if((p.rhs[j]==epsilon && p.rhs.length()==1)||isTerminal(p.rhs[j])){
                    if(!contains(firstc,p.rhs[j]))
                        firstc.push_back(p.rhs[j]);
                        break;
                }else{
                    if(p.rhs[j]==c)
                        break;
                    vector<char> temp;
                    if(firstSet.find(p.rhs[j])==firstSet.end()){
                        temp=computeFirst(p.rhs[j]);
                    }else{
                        temp=firstSet.at(p.rhs[j]);
                    }
                    if(!contains(temp,epsilon)){
                        for(auto ch:temp){
                            if(!contains(firstc,ch))
                                firstc.push_back(ch);
                        }
                        break;
                    }else{
                        for(auto ch:temp){
                            if(!contains(firstc,ch) && ch!=epsilon)
                                firstc.push_back(ch);
                        }
                    }
                }
            }
        }
    }
    return firstc;
}
//SLR parser
void AugmentedGrammar(){
    Production p;
    p.lhs=start;
    p.rhs=startSymbol;
    productions.insert(productions.begin(),p);
}

void getInitialState(){
    vector<LR0_prod> prods;
    for(auto prod:productions){
        LR0_prod p(prod);
        prods.push_back(p);
    }
    state initialState(prods,0);
    States.push_back(initialState);
}

//finding goto for a given state and a character
bool findGoto(state s,char c,int from=-1){
    vector<LR0_prod> gotoSet;
    for(auto p:s.currentState){
        if(p.getNext()==c){
            LR0_prod p1=p;
            p1.next();
            gotoSet.push_back(p1);
        }
    }
    if(!gotoSet.size())return false;

    //closure : if dot is before a non-terminal
    vector<LR0_prod> closure=gotoSet;
    for(int i=0;i<closure.size();i++){
        char nextSymbol = closure[i].getNext();
        if(nextSymbol && isNonTerminal(nextSymbol)){
            for(auto prod:productions){
                if(prod.lhs[0]==nextSymbol){
                    LR0_prod newProd(prod);
                    bool present=false;
                    for(auto existing:closure){
                        if(newProd==existing){
                            present=true;
                            break;
                        }
                    }
                    if(!present)closure.push_back(newProd);
                }
            }
        }
    }

    state newState(closure,States.size());
    for (auto &st : States) {
        if (newState == st) {
            gotos.push_back({from, st.sr});
            return false; 
        }
    }
    States.push_back(newState);
    newState.printState();
    gotos.push_back({from,States.size()-1});
    return true;
}


void findAllGotos() {
    // Queue of states to process
    queue<int> toProcess;
    toProcess.push(0);  

    while (!toProcess.empty()) {
        int st = toProcess.front();
        toProcess.pop();  
        for (auto ch : symbols) {
            if (findGoto(States[st], ch, st)) {
                // Adding new state to the queue to process its transitions
                toProcess.push(States.size() - 1);
            }
        }
    }
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
    cout<<"First sets :\n";
    for(auto fset:firstSet){
        cout<<fset.first<<" : {";
        for(auto ch:fset.second){
            cout<<ch<<",";
        }
        cout<<"}\n";
    }
    cout<<"States :\n";
    for(auto st:States){
        st.printState();
        cout<<endl;
    }
    cout<<"Goto pairs :\n";
    for(auto p:gotos)
        cout<<"("<<p.first<<","<<p.second<<")"<<endl;
}
//construction of parsing table 

int main(){
    ReadGrammar();
    AugmentedGrammar();
    getInitialState();
    findAllGotos();
    for(int i=0;i<NT.size();i++){
        firstSet[NT[i]] = computeFirst(NT[i]); 
    }
    result();
    return 0;
}

