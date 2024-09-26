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
unordered_map<char,vector<char>> firstSet;
unordered_map<char,vector<char>> followSet;
char startSymbol;

//parsing table
unordered_map<char , unordered_map<char,vector<Production>>> ParsingTable;

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

unordered_set<char> visitedFollow; // To track visited non-terminals during follow computation

vector<char> computeFollow(char c) {
    // Return if follow is already computed or being computed
    if (visitedFollow.find(c) != visitedFollow.end()) {
        return followSet[c]; 
    }

    visitedFollow.insert(c); // Mark this non-terminal as being processed
    vector<char> followc;

    // Start symbol gets '$' in its follow set
    if (c == startSymbol && !contains(followc, '$')) {
        followc.push_back('$');
    }

    for (auto p : productions) {
        for (int j = 0; j < p.rhs.length(); j++) {
            if (p.rhs[j] == c) {
                // If c is not the last symbol in the production
                if (j != p.rhs.length() - 1) {
                    char nextSymbol = p.rhs[j + 1];

                    // sub case 1 : Next symbol is a terminal, add it to follow set
                    if (isTerminal(nextSymbol)) {
                        if (!contains(followc, nextSymbol)) {
                            followc.push_back(nextSymbol);
                        }
                    } 
                    // sub case 2: Next symbol is a non-terminal, add its first set to follow set
                    else if (isNonTerminal(nextSymbol)) {
                        vector<char> first = firstSet[nextSymbol];
                        for (char f : first) {
                            if (f != epsilon && !contains(followc, f)) {
                                followc.push_back(f);
                            }
                        }
                        // If epsilon is in the first , add follow of LHS to follow(c)
                        if (contains(first, epsilon)) {
                            vector<char> temp = (followSet.find(p.lhs[0]) == followSet.end()) 
                                                ? computeFollow(p.lhs[0])
                                                : followSet[p.lhs[0]];

                            for (char ch : temp) {
                                if (!contains(followc, ch)) {
                                    followc.push_back(ch);
                                }
                            }
                        }
                    }
                } 
                //If c is the last symbol, add follow of LHS to follow(c)
                else {
                    vector<char> temp = (followSet.find(p.lhs[0]) == followSet.end()) 
                                        ? computeFollow(p.lhs[0])
                                        : followSet[p.lhs[0]];

                    for (char ch : temp) {
                        if (!contains(followc, ch)) {
                            followc.push_back(ch);
                        }
                    }
                }
            }
        }
    }

    followSet[c] = followc; // add follow of NT to followSet
    return followc;
}

//construction of parsing table
void constructParsingTable(){
    for(auto p : productions){
        char nt=p.lhs[0];
        //compute first of rhs[0];
        vector<char> firstRhs;
        if(firstSet.find(p.rhs[0])==firstSet.end())
            firstRhs=computeFirst(p.rhs[0]);
        else firstRhs=firstSet.at(p.rhs[0]);
        //push productions in first of rhs[0]
        for(char ch: firstRhs){
            if(ch!=epsilon)
                ParsingTable[nt][ch].push_back(p);
        }
        if(contains(firstRhs,epsilon)){
            vector<char> followNt= followSet.at(nt);   // Use cached follow set
            for(char ch: followNt){
                if(ch!=epsilon)
                    ParsingTable[nt][ch].push_back(p);
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
    cout<<"Follow sets :\n";
    for(auto fset:followSet){
        cout<<fset.first<<" : {";
        for(auto ch:fset.second){
            cout<<ch<<",";
        }
        cout<<"}\n";
    }
}

//function to print table
void printParsingTableAsTable() {
    const int cellWidth = 12; 
    set<char> terminalsSet;  

    // Collect all terminals in a set
    for (const auto& ntEntry : ParsingTable) {
        for (const auto& tEntry : ntEntry.second) {
            terminalsSet.insert(tEntry.first);
        }
    }

    vector<char> terminals(terminalsSet.begin(), terminalsSet.end());  
    // top border
    cout << "+" << string(cellWidth - 1, '-') ;
    for (const char& terminal : terminals) {
        cout << "+" << string(cellWidth - 1, '-') ; 
    }
    cout << "+" << endl;

    //table headers (terminals)
    cout << "|" << setw(cellWidth) << " NT/T ";  
    for (const char& terminal : terminals) {
        cout << "|" << setw(cellWidth) << terminal;  
    }
    cout << "|" << endl;

    cout << "+" << string(cellWidth - 1, '-') ;
    for (const char& terminal : terminals) {
        cout << "+" << string(cellWidth - 1, '-') ;  
    }
    cout << "+" << endl;

    //rows (non-terminals)
    for (const auto& ntEntry : ParsingTable) {
        char nonTerminal = ntEntry.first;
        cout << "|" << setw(cellWidth) << nonTerminal;  

        // For each terminal, print the corresponding production
        for (const char& terminal : terminals) {
            cout << "|";  // Start a new cell
            if (ntEntry.second.find(terminal) != ntEntry.second.end()) {
                const auto& prods = ntEntry.second.at(terminal);                
                string productionList;
                for (const Production& prod : prods) {
                    productionList += prod.lhs + "->" + prod.rhs + " | ";  // Join productions
                }
                if (!productionList.empty()) {
                    productionList = productionList.substr(0, productionList.size() - 3); 
                }
                cout << setw(cellWidth) << productionList;  // Print production
            } else {
                cout << setw(cellWidth) << "-";  //if there's no production
            }
        }
        cout << "|" << endl;

        cout << "+" << string(cellWidth - 1, '-') ;
        for (const char& terminal : terminals) {
            cout << "+" << string(cellWidth - 1, '-') ;  
        }
        cout << "+" << endl;
    }
}

// Helper function to print the current state of the stack, input buffer, and action
void printParsingStep(stack<char> stk, string inputBuffer, string action) {
    string stackContents;
    stack<char> temp = stk;
    while (!temp.empty()) {
        stackContents +=temp.top();
        temp.pop();
    }
    cout << setw(20) << left << stackContents
         << setw(20) << left << inputBuffer
         << setw(30) << left << action << endl;
}

// Function to parse the string with step-by-step logging
void ll1_parser() {
    string inputBuffer;
    cout << "Enter string to parse ('exit' to exit):\n";
    cin >> inputBuffer;
    if(inputBuffer == "exit") exit(0);
    for(int i=0;i<inputBuffer.length();i++){
        if(!isTerminal(inputBuffer[i]))
            inputBuffer[i]='i';
    }
    inputBuffer += '$';  //$ at end

    stack<char> stk;
    stk.push('$');
    stk.push(startSymbol);

    bool parsed = false;

    cout << setw(20) << left << "Stack"
         << setw(20) << left << "Input Buffer"
         << setw(30) << left << "Action" << endl;
    cout << string(70, '-') << endl;

    while (!parsed) {
        char topch = stk.top();

        // Case 1: String gets parsed successfully
        if (topch == '$' && inputBuffer[0] == '$') {
            printParsingStep(stk, inputBuffer, "String is accepted");
            parsed = true;
            break;
        }

        // Case 2: Stack top is a terminal and matches the input
        if (topch == inputBuffer[0]) {
            printParsingStep(stk, inputBuffer, "pop( " + string(1, topch)+")");
            stk.pop();  // Match and pop
            inputBuffer.erase(0, 1);  // Remove matched character from input
        } else {
            // Case 3: Stack top is a non-terminal, apply production
            try {
                vector<Production> prods = ParsingTable.at(topch).at(inputBuffer[0]);

                if (prods.size() > 1) {
                    printParsingStep(stk, inputBuffer, "Ambiguity detected!");
                    cerr << "Ambiguity: More than one production for " 
                         << topch << "/" << inputBuffer[0] << endl;
                    return;
                }

                // Push the right-hand side of the production onto the stack in reverse order
                stk.pop();  // Pop the non-terminal from the stack
                printParsingStep(stk, inputBuffer, "Apply " + prods[0].lhs + " -> " + prods[0].rhs);

                for (int j = prods[0].rhs.length() - 1; j >= 0; j--) {
                    if (prods[0].rhs[j] != epsilon) {  // Do not push epsilon
                        stk.push(prods[0].rhs[j]);
                    }
                }
            } catch (const out_of_range& e) {
                printParsingStep(stk, inputBuffer, "Error: No valid production");
                cerr << "Error: No production found for " << topch << "/" << inputBuffer[0] << endl;
                return;
            }
        }

        // Case 4: If inputBuffer is empty and the top of the stack is epsilon
        if (inputBuffer.empty() && topch != '$') {
            try {
                vector<Production> prods = ParsingTable.at(topch).at(epsilon);
                if (!prods.empty()) {
                    stk.pop();  // Pop the non-terminal from the stack
                    printParsingStep(stk, inputBuffer, "Apply " + prods[0].lhs + " -> " + prods[0].rhs);
                } else {
                    printParsingStep(stk, inputBuffer, "Error: No valid production");
                    cerr << "Error: No production found for " << topch << "/ε" << endl;
                    return;
                }
            } catch (const out_of_range& e) {
                printParsingStep(stk, inputBuffer, "Error: No valid production");
                cerr << "Error: No production found for " << topch << "/ε" << endl;
                return;
            }
        }
    }
    ll1_parser();
}

int main(){
    ReadGrammar();
    for(int i=0;i<NT.size();i++){
        firstSet[NT[i]] = computeFirst(NT[i]); 
    }
    for(int i=0;i<NT.size();i++){
        if(followSet.find(NT[i])==followSet.end())
        computeFollow(NT[i]); 
    }
    constructParsingTable();
    result();
    printParsingTableAsTable();
    ll1_parser();
    return 0;
}

