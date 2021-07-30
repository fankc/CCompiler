#include <bits/stdc++.h>
using namespace std;

const int maxn = 1002;
const int n_key = 41;           //保留字数目
const int n_oper = 21;          //操作符数目
char c;

//保留字
string key_word[n_key] = {"auto", "enum", "restrict", "unsigned", "break",
                    "extern", "return", "void", "case", "float",
                    "short", "volatile", "char", "for", "signed",
                    "while", "const", "goto", "sizeof", "_Bool",
                    "continue", "if", "static", "_Complex", "default",
                    "inline", "struct", "_Imaginary", "do", "int",
                    "switch", "double", "long", "typedef", "else",
                    "register", "union", "scanf", "printf", "cin",
                    "cout"
};

//操作符
string oper[] = {"+", "-", "*", "/", "^",
                "<", ">", "++", "--", "==",
                "*=", "/=", ">=", "<=", "<<",
                ">>", ">>=", "<<=", "%", "&",
                "^"
};

char bound[] = {',', ';', '(', ')', '[', ']', '{', '}'};

//词结构体
struct Word{
    int id;
    string value;
};

//数字结构体
struct Num{
    int id;
    int vi;
    double vd;
};

Num n[maxn];            //数字
Word w[maxn];           //词
map<string, int> m;     //标识符
int f = 0, ff = 0;

//词法
typedef struct node{
    string value;
    string type;
    int number;
}pro;
vector<pro> watest;

//语法
typedef pair<string, string> P;
map<P, string> pre_list;
typedef set<string> Sets;
map<string, Sets> first, follow, wenfa, select;      //first, follow
Sets Term, Nterm;                //终极符，非终极符
string Start;

//翻译
int cnt = 0;
pro now, last;
vector<string> var;
struct Four_exp{   //四元式结构体
    string op;
    string r1, r2;
    string left;
};
vector<Four_exp> four_exp, assemble;   //四元式表，汇编指令表
int cnt_temp = 0;
map<string, int> pri;   //优先级数组