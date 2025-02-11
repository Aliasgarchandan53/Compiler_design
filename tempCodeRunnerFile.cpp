#include <bits/stdc++.h>
using namespace std;

int main() {
    string s;
    cout << "Enter Single three-address code: ";
    cin >> s;

    vector<char> quad(4);
    quad[0] = s[0]; 
    quad[1] = s[2]; 
    quad[2] = s[4]; 
    quad[3] = s[3]; 

    vector<string> instructions;
    instructions.push_back("OPCODE\tSRC\tDEST");
    vector<string> registers = {"r0"};

    char val = isdigit(quad[1]) ? quad[2] : quad[1];
    string r = registers[0];
    string t = "MOV\t";
    t += val;
    t += ",\t";
    t += r;
    instructions.push_back(t);

    string op;
    switch (quad[3]) {
        case '+': op = "ADD"; break;
        case '-': op = "SUB"; break;
        case '*': op = "MUL"; break;
        case '/': op = "DIV"; break;
        default:  op = "UNKNOWN"; break;
    }

    op += "\t";
    char val2 = (val == quad[1]) ? quad[2] : quad[1];
    if (val2 <= '9' && val2 >= '0') op += '#';
    op += val2;
    op += "\t";
    op += r;
    instructions.push_back(op);

    string i3 = "MOV\t";
    i3 += r;
    i3 += "\t";
    i3 += quad[0];
    instructions.push_back(i3);

    for (const auto& instr : instructions) {
        cout << instr << "\n";
    }
    return 0;
}