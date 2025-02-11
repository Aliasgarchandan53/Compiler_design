%{
    #include <stdio.h>
    int single_comments=0;
    int multi_comments=0;    
%}

%%
"//"[^\n]* {single_comments++;}
"/*"([^*]|\*[^/])*"*/" {multi_comments++;}
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
    printf("Number of single line comments are %d and number of multiline comments are %d\n",single_comments,multi_comments);
    return 0;
}