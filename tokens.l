%{
    #include <stdio.h>
    int words=0;
    int numbers=0;
    int keywords=0;
    int operators =0;
%}

%%
[ \t\n] {;}
"+"|"-"|"*"|"/"|"=" {operators++;}
"if"|"else"|"int"|"char"|"main"|"return" {keywords++;words++;}
[0-9]+|[-][0-9]+ {numbers++;}
^[A-Za-z][_A-Za-z0-9]*$ {words++;}
%%

int yywrap(){return 1;}

int main(int argc,char* argv[]){
    if(argc==2){
        yyin=fopen(argv[1],"r");
    }else{
        printf("Enter the input :\n");
        yyin=stdin;
    }
    yylex();
    printf("Number of keywords : %d\n",keywords);
    printf("Number of words : %d\n",words);
    printf("Number of operators : %d\n",operators);
    printf("Number of numbers : %d\n",numbers);
}