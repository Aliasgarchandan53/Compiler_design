#include <bits/stdc++.h>
using namespace std;

#define epsilon 'e'
#define start "S1"
class Production
{
public:
    int id;
    string lhs, rhs;
    Production(int id = 0)
    {
        this->id = id;
    }
};
class LR0_prod
{
public:
    Production prod;
    int dIndex; // position of dot
    LR0_prod(Production prod)
    {
        this->prod = prod;
        dIndex = 0;
    }
    void printProd()
    {
        cout << prod.lhs << "->";
        for (int i = 0; i < prod.rhs.length(); i++)
        {
            if (i == dIndex)
                cout << ".";
            cout << prod.rhs[i];
        }
        if (dIndex == prod.rhs.length())
            cout << ".";
        cout << endl;
    }
    void next()
    {
        dIndex++;
    }
    char getNext()
    {
        if (dIndex == prod.rhs.length())
            return NULL;
        return prod.rhs[dIndex];
    }
    bool operator==(LR0_prod other)
    {
        return (dIndex == other.dIndex && prod.lhs == other.prod.lhs && prod.rhs == other.prod.rhs);
    }
};

class state
{
public:
    int sr;
    vector<LR0_prod> currentState;
    state(vector<LR0_prod> currentState, int sr)
    {
        this->currentState = currentState;
        this->sr = sr;
    }
    void printState()
    {
        cout << "I" << sr << ":\n";
        for (auto p : currentState)
            p.printProd();
    }
    bool operator==(state other)
    {
        if (currentState.size() != other.currentState.size())
            return false;
        for (auto &p1 : currentState)
        {
            bool found = false;
            for (auto &p2 : other.currentState)
            {
                if (p1 == p2)
                {
                    found = true;
                    break;
                }
            }
            if (!found)
                return false;
        }
        return true;
    }
};
class action
{
public:
    string type;
    int id;
    action(string type = "", int id = -1)
    {
        this->id = id;
        this->type = type;
    }
};
vector<Production> productions;
vector<char> NT;
vector<char> T;
vector<char> symbols;
unordered_map<char, vector<char>> firstSet;
unordered_map<char, vector<char>> followSet;
vector<state> States;
char startSymbol;
// parsing tables
unordered_map<int, unordered_map<char, action>> Actions; // stores shift and reduce actions
unordered_map<int, unordered_map<char, int>> GotoTable;  // stores goto table

bool contains(vector<char> arr, char c)
{
    for (auto ch : arr)
    {
        if (ch == c)
            return true;
    }
    return false;
}
void remove(vector<char> arr, char c)
{
    auto it = find(arr.begin(), arr.end(), c);
    arr.erase(it);
}

bool isTerminal(char c)
{
    for (auto t : T)
    {
        if (t == c)
            return true;
    }
    return false;
}

bool isNonTerminal(char c)
{
    for (auto nt : NT)
    {
        if (nt == c)
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
        Production p(i + 1);
        if (i == 0)
            startSymbol = s[0];
        p.lhs = s[0];
        p.rhs = s.substr(3);
        productions.push_back(p);
        for (int j = 0; j < s.length(); j++)
        {
            if (s[j] == '-' || s[j] == '>')
                continue;
            else
            {
                if (isupper(s[j]))
                {
                    if (!contains(NT, s[j]))
                        NT.push_back(s[j]);
                }
                else
                {
                    if (!contains(T, s[j]))
                        T.push_back(s[j]);
                }
                if (!contains(symbols, s[j]))
                    symbols.push_back(s[j]);
            }
        }
    }
}

vector<char> computeFirst(char c)
{
    vector<char> firstc;
    // if c is a terminal
    if (isTerminal(c))
    {
        firstc.push_back(c);
        return firstc;
    }
    for (auto p : productions)
    {
        if (p.lhs[0] == c)
        {
            for (int j = 0; j < p.rhs.length(); j++)
            {
                if ((p.rhs[j] == epsilon && p.rhs.length() == 1) || isTerminal(p.rhs[j]))
                {
                    if (!contains(firstc, p.rhs[j]))
                        firstc.push_back(p.rhs[j]);
                    break;
                }
                else
                {
                    if (p.rhs[j] == c)
                        break;
                    vector<char> temp;
                    if (firstSet.find(p.rhs[j]) == firstSet.end())
                    {
                        temp = computeFirst(p.rhs[j]);
                    }
                    else
                    {
                        temp = firstSet.at(p.rhs[j]);
                    }
                    if (!contains(temp, epsilon))
                    {
                        for (auto ch : temp)
                        {
                            if (!contains(firstc, ch))
                                firstc.push_back(ch);
                        }
                        break;
                    }
                    else
                    {
                        for (auto ch : temp)
                        {
                            if (!contains(firstc, ch) && ch != epsilon)
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

vector<char> computeFollow(char c)
{
    // Return if follow is already computed or being computed
    if (visitedFollow.find(c) != visitedFollow.end())
    {
        return followSet[c];
    }

    visitedFollow.insert(c); // Mark this non-terminal as being processed
    vector<char> followc;

    // Start symbol gets '$' in its follow set
    if (c == startSymbol && !contains(followc, '$'))
    {
        followc.push_back('$');
    }

    for (auto p : productions)
    {
        for (int j = 0; j < p.rhs.length(); j++)
        {
            if (p.rhs[j] == c)
            {
                // If c is not the last symbol in the production
                if (j != p.rhs.length() - 1)
                {
                    char nextSymbol = p.rhs[j + 1];

                    // sub case 1 : Next symbol is a terminal, add it to follow set
                    if (isTerminal(nextSymbol))
                    {
                        if (!contains(followc, nextSymbol))
                        {
                            followc.push_back(nextSymbol);
                        }
                    }
                    // sub case 2: Next symbol is a non-terminal, add its first set to follow set
                    else if (isNonTerminal(nextSymbol))
                    {
                        vector<char> first = firstSet[nextSymbol];
                        for (char f : first)
                        {
                            if (f != epsilon && !contains(followc, f))
                            {
                                followc.push_back(f);
                            }
                        }
                        // If epsilon is in the first , add follow of LHS to follow(c)
                        if (contains(first, epsilon))
                        {
                            vector<char> temp = (followSet.find(p.lhs[0]) == followSet.end())
                                                    ? computeFollow(p.lhs[0])
                                                    : followSet[p.lhs[0]];

                            for (char ch : temp)
                            {
                                if (!contains(followc, ch))
                                {
                                    followc.push_back(ch);
                                }
                            }
                        }
                    }
                }
                // If c is the last symbol, add follow of LHS to follow(c)
                else
                {
                    vector<char> temp = (followSet.find(p.lhs[0]) == followSet.end())
                                            ? computeFollow(p.lhs[0])
                                            : followSet[p.lhs[0]];

                    for (char ch : temp)
                    {
                        if (!contains(followc, ch))
                        {
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

// SLR parser
void AugmentedGrammar()
{
    Production p;
    p.lhs = start;
    p.rhs = startSymbol;
    productions.insert(productions.begin(), p);
}

void getInitialState()
{
    vector<LR0_prod> prods;
    for (auto prod : productions)
    {
        LR0_prod p(prod);
        prods.push_back(p);
    }
    state initialState(prods, 0);
    States.push_back(initialState);
}

// finding goto for a given state and a character
bool findGoto(state s, char c, int from = -1)
{
    vector<LR0_prod> gotoSet;
    for (auto p : s.currentState)
    {
        if (p.getNext() == c)
        {
            LR0_prod p1 = p;
            p1.next();
            gotoSet.push_back(p1);
        }
    }
    if (!gotoSet.size())
        return false;

    // closure : if dot is before a non-terminal
    vector<LR0_prod> closure = gotoSet;
    for (int i = 0; i < closure.size(); i++)
    {
        char nextSymbol = closure[i].getNext();
        if (nextSymbol && isNonTerminal(nextSymbol))
        {
            for (auto prod : productions)
            {
                if (prod.lhs[0] == nextSymbol)
                {
                    LR0_prod newProd(prod);
                    bool present = false;
                    for (auto existing : closure)
                    {
                        if (newProd == existing)
                        {
                            present = true;
                            break;
                        }
                    }
                    if (!present)
                        closure.push_back(newProd);
                }
            }
        }
    }

    state newState(closure, States.size());
    for (auto &st : States)
    {
        if (newState == st)
        {
            if (isNonTerminal(c))
            {
                GotoTable[from][c] = st.sr;
            }
            else if (isTerminal(c))
            {
                action A("shift", st.sr);
                Actions[from][c] = A;
            }
            return false;
        }
    }
    States.push_back(newState);
    newState.printState();
    if (isNonTerminal(c))
    {
        GotoTable[from][c] = States.size() - 1;
    }
    else if (isTerminal(c))
    {
        action A("shift", States.size() - 1);
        Actions[from][c] = A;
    }
    return true;
}

void findAllGotos()
{
    // Queue of states to process
    queue<int> toProcess;
    toProcess.push(0);

    while (!toProcess.empty())
    {
        int st = toProcess.front();
        toProcess.pop();
        for (auto ch : symbols)
        {
            if (findGoto(States[st], ch, st))
            {
                // Adding new state to the queue to process its transitions
                toProcess.push(States.size() - 1);
            }
        }
    }
}
// construction of parsing table

void findReduceActions()
{
    for (auto st : States)
    {
        for (auto prod : st.currentState)
        {

            if (!prod.getNext())
            {
                // skipping S1->S
                if (st.sr == 1 && prod.prod.id == 0)
                    continue;

                char lhs = prod.prod.lhs[0];

                for (char ch : followSet[lhs])
                {
                    action A("reduce", prod.prod.id);
                    if (Actions[st.sr][ch].type == "")
                    {
                        Actions[st.sr][ch] = A;
                    }
                    else
                    {
                        // If a shift/reduce conflict arises
                        cerr << "Conflict detected in state " << st.sr << " on symbol " << ch << endl;
                    }
                }
            }
        }
    }
}

void printParsingStep(stack<pair<char, int>> stk, string inputBuffer, action A)
{
    string stackContents;
    stack<pair<char, int>> temp1 = stk;
    stack<pair<char, int>> temp2;
    while (!temp1.empty())
    {
        temp2.push(temp1.top());
        temp1.pop();
    }
    while (!temp2.empty())
    {
        stackContents += temp2.top().first;             // first is a char
        stackContents += to_string(temp2.top().second); // convert second (int) to string
        stackContents += " ";                           // separate stack elements with space
        temp2.pop();
    }
    cout << setw(20) << left << stackContents
         << setw(20) << left << inputBuffer
         << setw(30) << left << A.type << " " << A.id << endl;
}

// parsing the string :
void SLR_Parser()
{
    string inputBuffer;
    cout << "Enter the string to be parsed (-1 for exit):\n";
    cin >> inputBuffer;
    if (inputBuffer == "-1")
        return;

    // Replace non-terminal symbols with 'i'
    for (int i = 0; i < inputBuffer.length(); i++)
    {
        if (!isTerminal(inputBuffer[i]))
            inputBuffer[i] = 'i';
    }
    inputBuffer += '$'; // Append end marker

    cout << "String passed : " << inputBuffer << endl;

    stack<pair<char, int>> stk;
    stk.push({'$', 0}); // initial state 0 with $
    cout << setw(20) << left << "Stack"
         << setw(20) << left << "Input Buffer"
         << setw(30) << left << "Action" << endl;
    cout << string(70, '-') << endl;

    while (true)
    {
        int state = stk.top().second;              // Current state from stack
        action A = Actions[state][inputBuffer[0]]; // action from Action table

        printParsingStep(stk, inputBuffer, A);

        if (A.type == "accept")
        {
            cout << "String is accepted" << endl;
            break;
        }
        else if (A.type == "shift")
        {
            // Shift: Push the symbol and new state onto the stack
            stk.push({inputBuffer[0], A.id});
            inputBuffer.erase(0, 1);
        }
        else if (A.type == "reduce")
        {
            // Reduce: Find the production by ID and pop rhs length from stack
            for (auto p : productions)
            {
                if (p.id == A.id)
                {
                    // Pop the length of rhs from stack
                    for (int i = 0; i < p.rhs.length(); i++)
                        stk.pop();

                    // Get the state after the reduction
                    int gotoState = GotoTable[stk.top().second][p.lhs[0]];
                    stk.push({p.lhs[0], gotoState});
                }
            }
        }
        else
        {
            cerr << "No action exists for I" << state << "," << inputBuffer[0] << endl;
            break;
        }
    }

    // Recursively parse more strings
    SLR_Parser();
}

void result()
{
    cout << "Terminals :\n";
    for (auto t : T)
        cout << t << ",";
    cout << endl;
    cout << "Non-terminals :\n";
    for (auto nt : NT)
        cout << nt << ",";
    cout << endl;
    cout << "Productions :\n";
    for (auto p : productions)
        cout << p.lhs << " : " << p.rhs << endl;
    cout << "First sets :\n";
    for (auto fset : firstSet)
    {
        cout << fset.first << " : {";
        for (auto ch : fset.second)
        {
            cout << ch << ",";
        }
        cout << "}\n";
    }
    cout << "Follow sets :\n";
    for (auto fset : followSet)
    {
        cout << fset.first << " : {";
        for (auto ch : fset.second)
        {
            cout << ch << ",";
        }
        cout << "}\n";
    }
    cout << "States :\n";
    for (auto st : States)
    {
        st.printState();
        cout << endl;
    }
    cout << "Goto table :\n";
    for (auto e1 : GotoTable)
    {
        cout << "I" << e1.first << ":\n";
        for (auto e2 : e1.second)
        {
            cout << e2.first << " : " << e2.second << endl;
        }
    }
    cout << endl;
    cout << "Actions table :\n";
    for (auto e1 : Actions)
    {
        cout << "I" << e1.first << ":\n";
        for (auto e2 : e1.second)
        {
            cout << e2.first << " : " << e2.second.type << " " << e2.second.id << endl;
        }
    }
}

int main()
{
    ReadGrammar();
    AugmentedGrammar();
    getInitialState();
    for (int i = 0; i < NT.size(); i++)
    {
        firstSet[NT[i]] = computeFirst(NT[i]);
    }
    for (int i = 0; i < NT.size(); i++)
    {
        if (followSet.find(NT[i]) == followSet.end())
            computeFollow(NT[i]);
    }
    action A("accept");
    Actions[1]['$'] = A;
    findAllGotos();
    findReduceActions();
    result();
    SLR_Parser();
    return 0;
}