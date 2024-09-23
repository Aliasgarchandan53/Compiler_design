#include <bits/stdc++.h>
using namespace std;

class Tag {
public:
    string tagname;
    unordered_map<string, string> attributes;
    unordered_map<string, Tag*> children;

    void parseTag(string s) {
        if(s[1]=='/') return;  // Ignore closing tags
        int i = 1; // start from index 1, since index 0 is '<'
        // Extracting the tag name
        while (i < s.length() && s[i] != ' ' && s[i] != '>') {
            tagname += s[i++];
        }
        // Parsing attributes if present
        while (i < s.length()) {
            if (s[i] == ' ') {
                i++;
                string attr, val;
                // Parsing attribute name
                while (i < s.length() && s[i] != ' ') {
                    attr += s[i++];
                }
                // Skipping the ' = "' part
                i += 4; // this skips ' = "'
                // Parsing attribute value
                while (i < s.length() && s[i] != '"') {
                    val += s[i++];
                }
                i++; // skip the closing '"'
                attributes[attr] = val;
            } else {
                i++;
            }
        }
    }
};

int main() {
    int a, q;
    cout << "Enter number of tags:\n";
    cin >> a;
    cout << "Enter number of queries:\n";
    cin >> q;
    cin.ignore(); // To ignore newline character after integer input
    vector<Tag*> tags(a);
    unordered_map<string, Tag*> tagMap;
    stack<Tag*> tagStack;
    
    cout << "Enter tags:\n";
    for (int i = 0; i < a; i++) {
        string t;
        getline(cin, t); // Use getline to handle spaces correctly

        Tag* newTag = new Tag();
        newTag->parseTag(t);

        if (t[1] != '/') {  // Not a closing tag
            if (!tagStack.empty()) {
                // Link the current tag to its parent
                Tag* parent = tagStack.top();
                parent->children[newTag->tagname] = newTag;
            }
            tagStack.push(newTag);
            tagMap[newTag->tagname] = newTag;  // Map tagname to the Tag object
        } else {  // Closing tag
            tagStack.pop();
        }

        tags[i] = newTag;
    }

    cout << "Enter queries:\n";
    for (int i = 0; i < q; i++) {
        string query;
        cin >> query;
        
        stringstream ss(query);
        string tagPath, attr;
        
        // Extracting tag path and attribute name
        getline(ss, tagPath, '~');
        getline(ss, attr);

        stringstream tagStream(tagPath);
        string tagName;
        Tag* currentTag = nullptr;
        bool found = true;

        while (getline(tagStream, tagName, '.')) {
            if (currentTag == nullptr) {
                currentTag = tagMap[tagName];
            } else {
                currentTag = currentTag->children[tagName];
            }
            if (currentTag == nullptr) {
                found = false;
                break;
            }
        }

        if (found && currentTag->attributes.find(attr) != currentTag->attributes.end()) {
            cout << currentTag->attributes[attr] << endl;
        } else {
            cout << "Not Found!" << endl;
        }
    }
    
    // Free allocated memory
    for(auto tag: tags) {
        delete tag;
    }

    return 0;
}

