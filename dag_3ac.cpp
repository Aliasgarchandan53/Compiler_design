#include <bits/stdc++.h>
using namespace std;

class Node {
public:
    int id;
    char val;
    string left;
    string right;
};

static int ID = 1;

int calcPrec(char c) {
    int prec = 9;
    switch (c) {
        case '+':
            prec = 1;
            break;
        case '*':
            prec = 2;
            break;
        default:
            break;
    }
    return prec;
}

string getPrefixExpression(string s) {
    vector<char> stack;
    string prefix = "";
    for (char c : s) {
        if (c == '(') {
            stack.push_back(c);
        } else if (c == ')') {
            while (stack.back() != '(') {
                prefix += stack.back();
                stack.pop_back();
            }
            stack.pop_back();
        } else if (c <= 'z' && c >= 'Z') {
            prefix += c;
        } else {
            if (stack.empty()) {
                stack.push_back(c);
            } else {
                int topPrec = calcPrec(stack.back());
                int currPrec = (c == '+') ? 1 : 2;
                if (currPrec < topPrec) {
                    stack.push_back(c);
                } else {
                    while (!stack.empty() && currPrec >= topPrec) {
                        prefix += stack.back();
                        stack.pop_back();
                        topPrec = stack.empty() ? -1 : calcPrec(stack.back());
                    }
                    stack.push_back(c);
                }
            }
        }
    }
    while (!stack.empty()) {
        prefix += stack.back();
        stack.pop_back();
    }
    return prefix;
}

int main() {
    string expr;
    cout<<"Enter the expression :\n";
    cin>>expr;
    string prefix = getPrefixExpression(expr);

    vector<Node> dagTable;
    vector<Node> stack;

    for (char c : prefix) {
        if (c <= 'z' && c >= 'a') {
            bool exists = false;
            for (Node node : dagTable) {
                if (node.val == c) {
                    stack.push_back(node);
                    exists = true;
                    break;
                }
            }
            if (!exists) {
                Node v = {ID++, c, "NULL", "NULL"};
                dagTable.push_back(v);
                stack.push_back(v);
            }
        } else {
            Node right = stack.back();
            stack.pop_back();
            Node left = stack.back();
            stack.pop_back();
            bool repeat = false;
            for (Node node : dagTable) {
                if (node.left.at(0) == left.val && node.right.at(0) == right.val) {
                    stack.push_back(node);
                    repeat = true;
                    break;
                }
            }
            if (!repeat) {
                string Left = string(1, left.val) + "(" + to_string(left.id) + ")";
                string Right = string(1, right.val) + "(" + to_string(right.id) + ")";
                Node v = {ID++, c, Left, Right};
                dagTable.push_back(v);
                stack.push_back(v);
            }
        }
    }
    cout<<"DAG Table :\n";
    cout << left << setw(5) << "ID" << setw(5) << "Char" << setw(10) << "Left" << setw(10) << "Right" << endl;
    cout << "----------------------------------" << endl;

    for (Node node : dagTable) {
        cout << left << setw(5) << node.id
             << setw(5) << node.val
             << setw(10) << node.left
             << setw(10) << node.right << endl;
    }
}
