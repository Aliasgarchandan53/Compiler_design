%{
    #include <stdio.h>
    int keywords=0, single_com=0, multi_com=0;
%}

%%
(include|void|main|int|char|float|stdio.h) {keywords++;}
"//".* {single_com++;}
"/*"(.|\n)*?"*/" {multi_com++;}
.|\n ;
%%

int yywrap() { return 1; }

int main(int argc, char* argv[]) {
    if(argc == 2) {
        yyin = fopen(argv[1], "r");
    } else {
        printf("Enter the code:\n");
        yyin = stdin;
    }
    yylex();
    printf("Number of keywords: %d\n", keywords);
    printf("Number of single-line comments: %d\n", single_com);
    printf("Number of multi-line comments: %d\n", multi_com);
    return 0;
}
