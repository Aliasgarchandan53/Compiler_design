#include <bits/stdc++.h>
using namespace std;

int precedence(char c) {
    switch(c) {
        case '+': case '-':
            return 1;
        case '*': case '/':
            return 2;
        case '(': case ')':
            return -1;
        default:
            cout << "Unrecognised operator : " << c << "\n";
            return -1;
    }
}

string getPostfixExpression(string s) {
    stack<char> stack;
    string post = "";
    
    for (char c : s) {
        if (isalnum(c)) {
            post += c;
        } else if (c == '(') {
            stack.push(c);
        } else if (c == ')') {
            while (!stack.empty() && stack.top() != '(') {
                post += stack.top();
                stack.pop();
            }
            stack.pop(); 
        } else {
            int currPrec = precedence(c);
            while (!stack.empty() && currPrec <= precedence(stack.top())) {
                post += stack.top();
                stack.pop();
            }
            stack.push(c);
        }
    }
    while (!stack.empty()) {
        post += stack.top();
        stack.pop();
    }
    cout << "Postfix expression : " << post << endl;
    return post;
}

int getThreeAddressCode(string p, char a) {
    stack<string> stack;
    int tempIndex = 1;
    cout<<"Three address code :\n";
    for (char c : p) {
        if (isalnum(c)) {
            stack.push(string(1, c));
        } else {
            string var2 = stack.top();
            stack.pop();
            string var1 = stack.top();
            stack.pop();
            string tempVar;

            if (tempIndex <= 6) {
                tempVar = "temp" + to_string(tempIndex++);
            } else {
                tempVar = string(1, a);
            }
            stack.push(tempVar);
            cout << tempVar << " = " << var1 << " " << c << " " << var2 << "\n";
        }
    }
    cout<<a<<" = "<<"temp" + to_string(--tempIndex)<<endl;
    return tempIndex;
}

int main() {
    vector<string> inputs;
    int n;

    cout << "Enter the number of expressions: ";
    cin >> n;
    cin.ignore();

    cout << "Enter the expressions (e.g., a=b+c):\n";
    for (int i = 0; i < n; ++i) {
        string expr;
        getline(cin, expr);
        inputs.push_back(expr);
    }

    for (string input : inputs) {
        string postfix = getPostfixExpression(input.substr(2));
        int final = getThreeAddressCode(postfix, input.at(0));
    }
    
    return 0;
}
