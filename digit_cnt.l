%{
    int positive_nums=0;
    int negative_nums=0;    
%}

%%
^[-][0-9]+$ {negative_nums++;}
^[0-9]+$ {positive_nums++;}
%%

int yywrap(){return 1;}

int main(){
    yylex();
    printf("Number of positive numbers are %d and negative numbers are %d\n",positive_nums,negative_nums);
    return 0;
}
