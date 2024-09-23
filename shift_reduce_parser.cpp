#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Production class to store LHS and RHS of grammar productions
class Production {
public:
    string LHS;
    string RHS;

    Production(string lhs, string rhs) : LHS(lhs), RHS(rhs) {}
};

// Grammar class to store the grammar rules, start symbol, terminals, and non-terminals
class Grammar {
public:
    string startSymbol;
    vector<Production> productions;
    vector<char> terminals;
    vector<char> nonTerminals;

    Grammar(string start, vector<Production> prods, vector<char> terms, vector<char> nonTerms)
        : startSymbol(start), productions(prods), terminals(terms), nonTerminals(nonTerms) {}
};

// Function to check if a string is a non-terminal
bool isNonTerminal(char ch, const vector<char>& nonTerminals) {
    return find(nonTerminals.begin(), nonTerminals.end(), ch) != nonTerminals.end();
}

// Function to perform the shift-reduce parsing
void shiftReduceParse(Grammar& grammar, string input) {
    string stack;
    string action;
    string inputBuffer = input;

    cout << "Stack\t\tInput Buffer\t\tAction" << endl;
    cout << "-----\t\t------------\t\t------" << endl;

    while (!inputBuffer.empty()) {
        // If stack is empty, shift the next token
        if (stack.empty()) {
            stack += inputBuffer[0];
            inputBuffer.erase(inputBuffer.begin());
            action = "Shift";
        } else {
            // Check if the current stack matches the RHS of any production for reduction
            bool reduced = false;
            for (const auto& prod : grammar.productions) {
                if (stack.size() >= prod.RHS.size() &&
                    stack.substr(stack.size() - prod.RHS.size()) == prod.RHS) {
                    stack.erase(stack.size() - prod.RHS.size());
                    stack += prod.LHS;
                    action = "Reduce by " + prod.LHS + " -> " + prod.RHS;
                    reduced = true;
                    break;
                }
            }
            // If no reduction was possible, shift the next token
            if (!reduced) {
                stack += inputBuffer[0];
                inputBuffer.erase(inputBuffer.begin());
                action = "Shift";
            }
        }

        // Print the current state of the stack, input buffer, and action
        cout << stack << "\t\t" << inputBuffer << "\t\t" << action << endl;
    }

    // Final check to determine if the input was accepted
    while (stack != grammar.startSymbol) {
        bool reduced = false;
        for (const auto& prod : grammar.productions) {
            if (stack.size() >= prod.RHS.size() &&
                stack.substr(stack.size() - prod.RHS.size()) == prod.RHS) {
                stack.erase(stack.size() - prod.RHS.size());
                stack += prod.LHS;
                action = "Reduce by " + prod.LHS + " -> " + prod.RHS;
                reduced = true;
                break;
            }
        }
        if (!reduced) {
            break;
        }
        cout << stack << "\t\t" << inputBuffer << "\t\t" << action << endl;
    }

    if (stack == grammar.startSymbol) {
        cout << "Input string accepted." << endl;
    } else {
        cout << "Input string rejected." << endl;
    }
}

int main() {
    string startSymbol;
    int numProductions, numTerminals, numNonTerminals;
    vector<Production> productions;
    vector<char> terminals, nonTerminals;
    string input;

    // Input start symbol
    cout << "Enter start symbol: ";
    cin >> startSymbol;

    // Input number of productions and the productions themselves
    cout << "Enter number of productions: ";
    cin >> numProductions;
    cout << "Enter productions (LHS -> RHS) (e.g., E -> E+T):" << endl;
    for (int i = 0; i < numProductions; i++) {
        string lhs, rhs, arrow;
        cin >> lhs >> arrow >> rhs;
        productions.emplace_back(lhs, rhs);
    }

    // Input terminals
    cout << "Enter number of terminals: ";
    cin >> numTerminals;
    cout << "Enter terminals (as a single character each):" << endl;
    for (int i = 0; i < numTerminals; i++) {
        char terminal;
        cin >> terminal;
        terminals.push_back(terminal);
    }

    // Input non-terminals
    cout << "Enter number of non-terminals: ";
    cin >> numNonTerminals;
    cout << "Enter non-terminals (as a single character each):" << endl;
    for (int i = 0; i < numNonTerminals; i++) {
        char nonTerminal;
        cin >> nonTerminal;
        nonTerminals.push_back(nonTerminal);
    }

    // Input the string to be parsed
    cout << "Enter the input string to be parsed: ";
    cin >> input;

    // Create grammar object
    Grammar grammar(startSymbol, productions, terminals, nonTerminals);

    // Perform shift-reduce parsing
    shiftReduceParse(grammar, input);

    return 0;
}
