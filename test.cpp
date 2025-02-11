#include <iostream>
#include <vector>

int getPrecedance(char c) {
    switch(c) {
        case '+': case '-':
            return 1;
        case '*': case '/':
            return 2;
        case '(': case ')':
            return -1;
        default:
            std::cout << "What " << c << "\n";
            return -1;
        
    }
}

std::vector<char> tempStack = {'p', 'q', 'r', 's', 't', 'u'};



std::string convertToPostfix(std::string s) {
    std::vector<char> stack;
    std::string post = "";
    
    for (char c: s) {
        if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')) {
            post += c;
        } else if (c == '(') {
            stack.push_back(c);
        } else if (c == ')') {
            while (stack[stack.size()-1] != '(') {
                post += stack[stack.size()-1];
                stack.pop_back();
            }
            stack.pop_back();
        } else {
            int topPrec = -2;
            if (stack.size() != 0)
                topPrec = getPrecedance(stack[stack.size()-1]);
            int currPrec = getPrecedance(c);
            if (currPrec > topPrec) {
                stack.push_back(c);
            } else {
                while (currPrec <= topPrec) {
                    post += stack[stack.size()-1];
                    stack.pop_back();
                    if (stack.size() != 0)
                        topPrec = getPrecedance(stack[stack.size()-1]);
                    else break;
                }
                stack.push_back(c);
            }
        }
    }
    while (stack.size() != 0) {
        post += stack[stack.size()-1];
        stack.pop_back();
    }
    std::cout << "Postfix: " << post << std::endl;
    return post;
}

int printThreeAddress(std::string p, char a) {
    std::vector<std::string> stack;
    int varCount = 1;

    for (int i = 0; i<p.length(); ++i) {
        if ((p.at(i) >= 'a' && p.at(i) <= 'z') || (p.at(i) >= '0' && p.at(i) <= '9')) {
            stack.push_back(std::string(1, p.at(i)));
        } else {
            char op = p.at(i);
            std::string var2 = stack[stack.size()-1];
            stack.pop_back();
            std::string var1 = stack[stack.size()-1];
            stack.pop_back();
            // std::string out = "Temp";
            // out += (char)(varCount + '0');
            // varCount++;
            std::string out = "";
            if (i >= p.length()-2) {
                out += a;
            } else {
                out += tempStack[tempStack.size()-1];
                tempStack.pop_back();
            }
            stack.push_back(out);  
            std::cout << out << " = " << var1 << " " << op << " " << var2 << "\n";
        }
    }
    return varCount;
}

int main() {
    std::vector<std::string> inputs = {"a=b*c+9", "d=b*c*e"};
    for (std::string input: inputs) {

        std::string postfix = convertToPostfix(input.substr(2, input.length()-2));

        //std::cout << "Postfix: " << postfix << "\n";

        int final = printThreeAddress(postfix, input.at(0));
        //std::cout << input.substr(0,2) << tempStack[tempStack.size()] << "\n";
    }
    return 0;
}
