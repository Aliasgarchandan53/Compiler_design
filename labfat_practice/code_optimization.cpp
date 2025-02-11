#include <bits/stdc++.h>
using namespace std;

class code{
    public:
    string lhs,rhs;
};

//deadcode elimination
vector<code> deadcodeElimination(vector<code> &threeac){
    vector<code> noDeadCode;
    for(int i=0;i<threeac.size();i++){
        bool used = false;
        for(int j=i+1;j<threeac.size();j++){
            for(int k=0;k<threeac[j].rhs.length();k++){
                if(threeac[i].lhs[0]==threeac[j].rhs[k]){
                    used=true;
                    break;
                }
            }
            if(used){
                break;
            }
        }
        if(used){
            noDeadCode.push_back(threeac[i]);
        }
    }
    noDeadCode.push_back(threeac.back());
    return noDeadCode;
}

vector<code> removeCommonSubExpr(vector<code> &codes){
    vector<code> noDeadcode=codes;
    
    for(int i=0;i<noDeadcode.size();i++){
        for(int j=i+1;j<noDeadcode.size();j++){
            if(noDeadcode[i].rhs==noDeadcode[j].rhs){
                noDeadcode[j].rhs=noDeadcode[i].lhs;
            }
        }
    }
    vector<code> withoutCommon = noDeadcode;
    vector<int> toRemove;
    for(int i=0;i<noDeadcode.size();i++){
        if(noDeadcode[i].rhs.length()==1){
            for(int j=i+1;j<noDeadcode.size();j++){
                for(int k=0;k<withoutCommon[j].rhs.length();k++){
                    if(withoutCommon[j].rhs[k]==withoutCommon[i].lhs[0])
                        withoutCommon[j].rhs[k]=withoutCommon[i].rhs[0];
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

    vector<code> finalExpressions;
    for (int i = 0; i < withoutCommon.size(); ++i) {
        if (find(toRemove.begin(), toRemove.end(), i) == toRemove.end()) {
            finalExpressions.push_back(withoutCommon[i]);
        }
    }

    return finalExpressions;
}

int main(){
    int n;
    cin>>n;
    vector<code> threeac;
    for(int i=0;i<n;i++){
        string s;
        cin>>s;
        code c;
        c.lhs=s.substr(0,1);
        c.rhs=s.substr(2);
        threeac.push_back(c);
    }
    cout<<"3-add code:\n";
    for(auto c:threeac)
        cout<<c.lhs<<" = "<<c.rhs<<endl;

    vector<code> withoutDeadCode = deadcodeElimination(threeac);
    cout<<"3-add code without dead code :\n";
    for(auto c:withoutDeadCode)
        cout<<c.lhs<<" = "<<c.rhs<<endl;
    vector<code> finalCode=removeCommonSubExpr(withoutDeadCode);
    cout<<"3-add code optimized :\n";
    for(auto c:finalCode)
        cout<<c.lhs<<" = "<<c.rhs<<endl;
    return 0;
}