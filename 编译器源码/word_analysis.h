#include "struct.h"
#include <bits/stdc++.h>
using namespace std;

//词法分析
bool is_oper(string s){
    for(int i=0; i<n_oper; i++) if(s == oper[i]) return true;
    return false;
}

bool is_bound(char c){
    for(int i=0; i<sizeof(bound); i++) if(c == bound[i]) return true;
    return false;
}

bool is_key(string s){
    for(int i=0; i<n_key; i++) if(s == key_word[i]) return true;
    return false;
}

int my_stoi(string s){                         //get int
    int ans = 0;
    for(int i=0; i<s.size(); i++) ans = ans * 10 + s[i] - '0';
    return ans;
}

double my_stof(string s){                      //get double
    long long ans = 0;
    int fd = -1, fe = -1;
    for(int i=0; i<s.size(); i++){
        if(s[i] == '.'){
            fd = i;
            continue;
        }
        if(s[i] == 'e'){
            fe = i;
            continue;
        }
        ans = ans * 10 + s[i] - '0';
    }
    if(fd != -1 && fe == -1) return double(ans)/(pow(10, s.size() - fd - 1));
    else if(fd == -1 && fe != -1){
        long long temp = ans % (long long)pow(10, s.size() - fe - 1);     //得到e后的数字
        ans /= pow(10, s.size() - fe - 1);                          //得到e前的数字
        return double(ans*pow(10, temp));
    }
    else{
        long long temp = ans % (long long)pow(10, s.size() - fe - 1);     //得到e后的数字
        cout<<ans<<" "<<s.size() - fe - 1<<" "<<temp<<endl;
        ans /= pow(10, s.size() - fe - 1);                                //得到e前的数字
        cout<<ans<<endl;
        long long tt = (s.size() - fd - 1) - (s.size() - fe - 1) - 1;     //得到.后的数字
        cout<<tt<<endl;
        return (double)ans/pow(10, tt) * (pow(10, temp));
    }
}

void getword(){
    freopen("programe.txt", "r", stdin);              //重定向

    string str = "";
    int flag, is_end;
    is_end = scanf("%c", &c);
    while(is_end != EOF){
        flag = 0;
        if(isspace(c)){
            while(isspace(c) && is_end != EOF){   //滤空格
                is_end = scanf("%c", &c);
            }
        }
        if(isalpha(c) || c == '_'){                         //以字母或下划线开头
            while(isalnum(c) || c == '_' || isalpha(c)){
                str += c;
                is_end = scanf("%c", &c);
            }
            w[++f].value = str;
            if(is_key(str)){
                w[f].id = 1;
                if(str == "int" || str == "float" || str == "long" || str == "double" || str == "char" || str == "bool")
                    watest.push_back(pro{str, "type", 1});
                else watest.push_back(pro{str, str, 1});
            }         //保留字
            else{
                w[f].id = 2;                     //标识符
                m[str] ++;
                watest.push_back(pro{str, "id", 2});
            }
            str = "";
            flag = 1;
        }
        if(isdigit(c)){                          //数字
            int fd = 0, fe = 0, fflag = 0;
            while(isdigit(c) || c == '.' || c == 'e'){
                if(c == '.') fd ++;
                if(c == 'e') fe ++;
                if(c == '.' && fe) fflag = 1;
                str += c;
                is_end = scanf("%c", &c);
            }
            if(str[str.size()-1] == '.' || str[str.size()-1] == 'e') fflag = 1;
            if(fflag){
                cout<<"错误-->不合法数字："<<str<<endl;    //忽略不合法输入
            }
            else{
                watest.push_back(pro{str, "number", 3});
                if(!fd && !fe){
                    n[++ff].vi = my_stoi(str);
                    n[ff].id = 1;
                }
                else{
                    n[++ff].vd = my_stof(str);
                    n[ff].id = 2;
                }
                w[++f].id = 3;  w[f].value = str;
            }
            str = "";
            flag = 1;
        }
        if(is_bound(c)){            //界符
            str += c;
            w[++f].value = str;
            w[f].id = 4;

            watest.push_back(pro{str, str, 4});

            is_end = scanf("%c", &c);
            str = "";
            flag = 1;
        }
        string ss = "";
        ss += c;
        if(is_oper(ss)){
            while(is_oper(ss)){
                str += c;
                is_end = scanf("%c", &c);
                ss += c;
            }
            if(is_oper(str)){
                w[++f].value = str;
                w[f].id = 5;

                watest.push_back(pro{str, str, 5});
            }
            str = "";
            flag = 1;
        }
        if(!flag && is_end != EOF){
            str += c;
            w[++f].value = str;
            w[f].id = 6;

            watest.push_back(pro{str, str, 6});

            is_end = scanf("%c", &c);
            str = "";
        }
    }
    freopen("CON", "r", stdin);           //关闭重定向,恢复标准
}

void to_file1(){                        //主表
//    char filename[20];
//    puts("请输入词法分析主表要保存到的地址：");
//    scanf("%s", filename);
    fstream out;
    out.open("main_table.txt", ios::out);
    out<<"1.保留字   2.标识符   3.数字   4.界符   5.操作符   6.其他"<<endl<<endl;
    for(int i=1; i<=f; i++) out<<w[i].id<<"  "<<w[i].value<<endl;
    out.close();
}

void to_file2(){                        //标识符表
//    char filename[20];
//    puts("请输入词法分析标识符表要保存到的地址：");
//    scanf("%s", filename);
    ofstream out;
    out.open("id_table.txt", ios::out);
    map<string, int>::iterator it;
    for(it=m.begin(); it!=m.end(); it++) out<<it->first<<endl;
    out.close();
}

void to_file3(){                        //数字表
//    char filename[20];
//    puts("请输入词法分析数字表要保存到的地址：");
//    scanf("%s", filename);
    ofstream out;
    out.open("number_table.txt", ios::out);
    out<<"1.int   2.double"<<endl<<endl;
    for(int i=1; i<=ff; i++) out<<n[i].id<<"  "<<(n[i].id == 1 ? n[i].vi : n[i].vd)<<endl;
    out.close();
}

void to_file4(){
    ofstream out;
    out.open("twox.txt", ios::out);   //词法分析结果（三元组）
    int len = watest.size();
    for(int i=0; i<len-1; i++) out<<watest[i].value<<"  "<<watest[i].type<<"  "<<watest[i].number<<endl;
    out<<watest[len-1].value<<"  "<<watest[len-1].type<<"  "<<watest[len-1].number<<endl;
    out.close();
}

bool Word_analysis(){
//    char filename[20];
//    puts("请输入您要打开的文件地址：");
//    scanf("%s", filename);

    getword();

    to_file1();
    to_file2();
    to_file3();
    to_file4();

    return true;
}