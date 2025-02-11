#include <bits/stdc++.h>
using namespace std;

class expression {
public:
    char lhs;
    string rhs;
    
    expression(string s) {
        lhs = s[0];
        rhs = s.substr(2, s.length() - 2);
    }
    expression(char l, string r) {
        lhs = l;
        rhs = r;
    }
};

vector<expression> eliminateDeadCode(vector<expression>& threeAddCode) {
    vector<expression> withoutDeadCode;

    for (int i = 0; i < threeAddCode.size() - 1; ++i) {
        bool used = false;

        for (int j = i + 1; j < threeAddCode.size(); ++j) {
            for (char c : threeAddCode[j].rhs) {
                if (c == threeAddCode[i].lhs) {
                    used = true;
                    break;
                }
            }
            if (used) break;
        }

        if (used) {
            withoutDeadCode.push_back(threeAddCode[i]);
        }
    }
    withoutDeadCode.push_back(threeAddCode.back());

    return withoutDeadCode;
}

vector<expression> removeCommonSubexpression(vector<expression>& withoutDeadCode) {
    vector<expression> withoutSubExp = withoutDeadCode;

    for (int i = 0; i < withoutDeadCode.size(); ++i) {
        for (int j = i + 1; j < withoutDeadCode.size(); ++j) {
            if (withoutDeadCode[i].rhs == withoutDeadCode[j].rhs) {
                withoutSubExp[j].rhs = withoutDeadCode[i].lhs;
            }
        }
    }

    // Simplify using temp variable replacements
    vector<expression> withoutCommon = withoutSubExp;
    vector<int> toRemove;

    for (int i = 0; i < withoutSubExp.size() - 1; ++i) {
        if (withoutSubExp[i].rhs.length() == 1) {
            for (int j = i + 1; j < withoutSubExp.size(); ++j) {
                for (int k = 0; k < withoutCommon[j].rhs.length(); ++k) {
                    if (withoutCommon[j].rhs.at(k) == withoutSubExp[i].lhs) {
                        withoutCommon[j].rhs.at(k) = withoutSubExp[i].rhs.at(0);
                    }
                }
            }
            toRemove.push_back(i);
        }
    }
    cout << "After Removing Common Subexpression:\n";
    for (const auto& e : withoutCommon) {
        cout << e.lhs << "=" << e.rhs << "\n";
    }
    cout << endl;

    vector<expression> finalExpressions;
    for (int i = 0; i < withoutCommon.size(); ++i) {
        if (find(toRemove.begin(), toRemove.end(), i) == toRemove.end()) {
            finalExpressions.push_back(withoutCommon[i]);
        }
    }

    return finalExpressions;
}

int main() {
    vector<expression> threeAddCode;
    cout << "Enter the number of values: ";
    int n;
    cin >> n;

    for (int i = 0; i < n; ++i) {
        char c;
        string s;
        cout << "Left: ";
        cin >> c;
        cout << "Right: ";
        cin >> s;
        cout << "\n";
        threeAddCode.push_back(expression(c, s));
    }
    // Intermediate code 
    cout<<"Intermediate code :\n";
    for(auto tac:threeAddCode){
        cout<<tac.lhs<<" = "<<tac.rhs<<endl;
    }
    // Eliminated Dead Code
    vector<expression> withoutDeadCode = eliminateDeadCode(threeAddCode);
    cout << "After Dead Code Elimination: \n";
    for (const auto& e : withoutDeadCode) {
        cout << e.lhs << "=" << e.rhs << "\n";
    }
    cout << endl;
    // Removed Common Subexpressions
    vector<expression> finalCode = removeCommonSubexpression(withoutDeadCode);

    // Final optimized code
    cout << "Final Optimized Code:\n";
    for (const auto& e : finalCode) {
        cout << e.lhs << "=" << e.rhs << "\n";
    }
    cout << endl;

    return 0;
}
