#include "word_analysis.h"
#include <bits/stdc++.h>
using namespace std;

//LL(1)语法分析
void get_wenfa(){
    freopen("grammar.txt", "r", stdin);

    string line, s, foo;
    int cnt = 0;
    while(getline(cin, line)){
        cnt ++;
        int len = line.size();
        int i = 0;
        while(line[i] != ' ' && i < len) i ++;
        foo = line.substr(0, i);
        if(cnt == 1) Start = foo;
        i++;
        while(line[i] != ' ' && i < len) i ++;   //略去 ->
        for(; i<len; i++){
            if(line[i] != ' '){
                s = "";
                while(!(line[i] == '|' && line[i-1] == ' ' && line[i+1] == ' ') && i < len){
                    s += line[i++];
                }
                while(s[s.size()-1] == ' ') s.erase(s.size()-1, 1);
                wenfa[foo].insert(s);
            }
        }
    }
    /*
    for(map<string, Sets>::iterator it = wenfa.begin(); it != wenfa.end(); it ++){    //  输出文法
        cout<<it->first<<"->";
        for(Sets::iterator ti = (it->second).begin(); ti != (it->second).end(); ti ++){
            if(ti == it->second.begin()) cout<<*ti;
            else cout<<" | "<<*ti;
        }
        puts("");
    }
    */
    freopen("CON", "r", stdin);
}

void get_ch(){
    fstream in;
    in.open("grammar.txt", ios::in);

    string str;
    while(in >> str){
        if(str == "->") continue;
        if(str[0] == '<' && str[str.size()-1] == '>') Nterm.insert(str);
        else Term.insert(str);
    }
    Term.insert("#");
    in.close();
}

void get_first(){
    int flag = 1;
    Sets tt;
    while(flag){
        flag = 0;
        for(map<string, Sets>::iterator it = wenfa.begin(); it != wenfa.end(); it ++){    //所有非终极符
            string X = it->first;       //对于非终极符X

            if(wenfa[X].count("@") && !first[X].count("@")){   //包含@，加入FIRST
                first[X].insert("@");
                flag = 1;
            }

            for(Sets::iterator f = wenfa[X].begin(); f != wenfa[X].end(); f ++){    //遍历s所有产生式
                string foo = *f, str = "", last = "";                               //对于产生式X->Y1Y2Y3...

                int i = 0;
                while(foo[i] != ' ' && i < foo.size()) str += foo[i ++];    //找到Y1
                if(str == "") continue;

                if(!(str[0] == '<' && str[str.size()-1] == '>')){     //Y1是终极符
                    if(!first[X].count(str)){
                        first[X].insert(str);
                        flag = 1;
                    }
                    if(str != "@") continue;      //是终极符但不是@
                }
                else{     //Y1是非终极符
                    for(Sets::iterator ii = first[str].begin(); ii != first[str].end(); ii ++){
                        if(*ii != "@" && !first[X].count(*ii)){
                            first[X].insert(*ii);
                            flag = 1;
                        }
                    }
                    if(!first[str].count("@")) continue;
                }

                for( ; i<foo.size(); i++){
                    if(foo[i] != ' '){
                        last = str;
                        str = "";
                        while(foo[i] != ' ' && i < foo.size()) str += foo[i ++];

                        if(last == "@" || first[last].count("@")){
                            for(Sets::iterator ii = first[str].begin(); ii != first[str].end(); ii ++){
                                if(*ii != "@" && !first[X].count(*ii)){
                                    first[X].insert(*ii);
                                    flag = 1;
                                }
                            }
                        }
                        else break;
                    }
                }

                if(i >= foo.size() && (str == "@" || first[str].count("@"))){                          //Y1Y2Y3...Yn的FIRST都可产生@
                    if(!first[X].count("@")){
                        first[X].insert("@");
                        flag = 1;
                    }
                }
            }
        }
    }

//输出FIRST集
    fstream out;
    out.open("FIRST.txt", ios::out);       //输出保存FIRST
    for(map<string, Sets>::iterator t = first.begin(); t != first.end(); t ++){
        out<<"FIRST("<<t->first<<"): ";
        for(Sets::iterator it = t->second.begin(); it != t->second.end(); it ++)
            out<<" "<<*it;
        out<<endl;
    }
    out.close();
}

void get_follow(){
    int flag = 1;
    int cnt;
    string str, foo, s, last, tr;
    Sets tt;

    follow[Start].insert("#");   //把#加入follow(Start)

    while(flag){
        flag = 0;
        for(map<string, Sets>::iterator it = wenfa.begin(); it != wenfa.end(); it ++){
            str = it->first;                                           //对于FOLLOW(str)

            for(Sets::iterator ti = it->second.begin(); ti != it->second.end(); ti ++){
                foo = *ti;                                      //对于产生式str->foo即str->ABC

                int len = foo.size(), i = 0;
                s = "";
                while(foo[i] != ' ' && i < len){ s += foo[i ++]; }
                if(s == "") continue;

                for( ; i<len; i++){
                    if(foo[i] != ' '){
                        last = s;
                        s = "";
                        while(foo[i] != ' ' && i < len){ s += foo[i ++]; }
                        if(last[0] != '<' || last[last.size()-1] != '>') continue;  //B是终结符，往后串

                        if(s[0] != '<' || s[s.size()-1] != '>'){   //C是终结符
                            if(s != "@" && !follow[last].count(s)){
                                follow[last].insert(s);
                                flag = 1;
                            }
                            if(s != "@") continue;
                        }
                        else{    //C是非终结符，先把first(C) - {@}加入
                            for(Sets::iterator ii = first[s].begin(); ii != first[s].end(); ii ++){
                                if(*ii != "@" && !follow[last].count(*ii)){
                                    follow[last].insert(*ii);
                                    flag = 1;
                                }
                            }
                        }

                        if(first[s].count("@") || s == "@"){   //C->*@,就看后面的
                            string next = s;
                            int k = i;
                            for( ; k < len; k++){
                                if(foo[k] != ' '){
                                    next = "";
                                    while(foo[k] != ' ' && k < len){ next += foo[k ++]; }

                                    if(next[0] != '<' || next[next.size()-1] != '>'){    //next是终结符
                                        if(next != "@" && !follow[last].count(next)){
                                            follow[last].insert(next);
                                            flag = 1;
                                        }
                                        if(next != "@") break;
                                    }
                                    else{    //next是非终结符
                                        for(Sets::iterator ii = first[next].begin(); ii != first[next].end(); ii ++){
                                            if(*ii != "@" && !follow[last].count(*ii)){
                                                follow[last].insert(*ii);
                                                flag = 1;
                                            }
                                        }
                                        if(!first[next].count("@")) break;
                                    }
                                }
                            }
                            if(k >= len && (next == "@" || first[next].count("@"))){    //对于str->ABC,C->*@,则将follow(str) - {@}加到follow(B)中
                                for(Sets::iterator ii = follow[str].begin(); ii != follow[str].end(); ii ++){
                                    if(!follow[last].count(*ii)){
                                        follow[last].insert(*ii);
                                        flag = 1;
                                    }
                                }
                            }
                        }
                    }
                }
                if(i >= len && (s[0] == '<' && s[s.size()-1] == '>')){    //对于str->ABC,则将follow(str) - {@}加到follow(C)中
                    for(Sets::iterator ii = follow[str].begin(); ii != follow[str].end(); ii ++){
                        if(!follow[s].count(*ii)){
                            follow[s].insert(*ii);
                            flag = 1;
                        }
                    }
                }
            }
        }
    }

//输出FOLLOW集
    fstream out;
    out.open("FOLLOW.txt", ios::out);
    for(map<string, Sets>::iterator t = follow.begin(); t != follow.end(); t ++){
        out<<"FOLLOW("<<t->first<<"): ";
        for(Sets::iterator it = t->second.begin(); it != t->second.end(); it ++)
            out<<" "<<*it;
        out<<endl;
    }
    out.close();
}

void get_select(){
    int flag = 1;
    string str, foo, s, fxx;
    Sets tt;
    for(map<string, Sets>::iterator it = wenfa.begin(); it != wenfa.end(); it ++){
        str = it->first;                                           //对于str

        for(Sets::iterator ti = it->second.begin(); ti != it->second.end(); ti ++){
            foo = *ti;
            fxx = str + " " + foo;            //对于select(str->foo)

            tt.clear();
            int len = foo.size();
            for(int i=0; i<len; i++){
                if(foo[i] != ' '){
                    s = "";
                    while(foo[i] != ' ' && i < len){ s += foo[i ++]; }
                    if(s[0] != '<' || s[s.size()-1] != '>'){  //s是终结符
                        if(s != "@"){
                            tt.insert(s);
                            break;
                        }
                    }else{
                        for(Sets::iterator ii = first[s].begin(); ii != first[s].end(); ii ++){
                            if(*ii != "@") tt.insert(*ii);
                        }
                        if(!first[s].count("@")) break;
                    }
                }
            }
            if(s == "@" || first[s].count("@")){
                set_union(select[fxx].begin(), select[fxx].end(), follow[str].begin(), follow[str].end(), inserter(select[fxx], select[fxx].begin()));
                set_union(select[fxx].begin(), select[fxx].end(), tt.begin(), tt.end(), inserter(select[fxx], select[fxx].begin()));
            }
            else set_union(tt.begin(), tt.end(), select[fxx].begin(), select[fxx].end(), inserter(select[fxx], select[fxx].begin()));
        }
    }

//输出SELECT集
    fstream out;
    out.open("SELECT.txt", ios::out);
    for(map<string, Sets>::iterator t = select.begin(); t != select.end(); t ++){
        out<<"SELECT("<<t->first<<"): ";
        for(Sets::iterator it = t->second.begin(); it != t->second.end(); it ++)
            out<<" "<<*it;
        out<<endl;
    }
    out.close();
}

void get_pre_list(){
    get_ch();
    pre_list.clear();
    string str, foo;
    for(map<string, Sets>::iterator it = select.begin(); it != select.end(); it ++){
        int i = 0;
        str = foo = "";
        while(it->first[i] != ' ' && i < it->first.size()) str += it->first[i ++];
        foo = it->first.substr(i+1, it->first.size() - i - 1);

        for(Sets::iterator ti = it->second.begin(); ti != it->second.end(); ti ++){
            pre_list[P(str ,*ti)] = foo;
        }
    }
    fstream out;
    out.open("pre_list.txt", ios::out);

    out<<"        ";
    for(Sets::iterator ti = Term.begin(); ti != Term.end(); ti ++){
        out<<setw(8)<<*ti;
    }
    out<<endl;
    for(Sets::iterator it = Nterm.begin(); it != Nterm.end(); it ++){
        out<<setw(8)<<*it;
        for(Sets::iterator ti = Term.begin(); ti != Term.end(); ti ++){
            out<<setw(8)<<(pre_list[P(*it, *ti)] == "" ? "$" : pre_list[P(*it, *ti)]);
        }
        out<<endl;
    }
    out.close();
}

bool analysis(ofstream &out){         //分析过程
    vector<string> p;
    string t, foo, str, fxx;

    p.push_back("#");       //预处理
    p.push_back(Start);
    watest.push_back(pro{"#", "#", 0});

    out<<"------------------------------------------------------------------------------\n";
    out<<"  步骤 |    当前栈顶    |     当前串首     |   推导所用产生式                 \n";
    out<<"------------------------------------------------------------------------------\n";
    int step = 1, pos = 0;
    while(!p.empty() && pos < watest.size()){
        out<<setw(6)<<(step ++)<<setw(18)<<p.back()<<setw(19)<<watest[pos].type;
        if(p.back() == "#" && watest[pos].type == "#"){
            out<<"    "<<"接受"<<endl;
            return true;
        }

        if(p.back() == watest[pos].type){
            out<<"    "<<"“"<<watest[pos].type<<"”"<<"匹配"<<endl;
            p.pop_back();
            pos ++;
        }
        else{
            t = pre_list[P(p.back(), watest[pos].type)];
            if(t == "") return false;
            out<<"    "<<p.back()<<" -> "<<t<<endl;
            if(t == "@") t = "";

            p.pop_back();

            int i = t.size()-1;
            for( ; i >= 0; i--){
                if(t[i] != ' '){
                    foo = "";
                    while(t[i] != ' ' && i >= 0) foo += t[i --];
                    reverse(foo.rbegin(), foo.rend());
                    p.push_back(foo);
                }
            }
        }
    }
    return false;
}

bool grammar_analysis(){
    get_wenfa();
    get_first();
    get_follow();
    get_select();
    get_pre_list();

//输出分析过程
    ofstream out;
    out.open("process.txt", ios::out);
    bool flag = analysis(out);
    out<<(flag ? "\n\n-->LL(1)合法句子\n\n" : "\n\n-->LL(1)非法句子\n\n");
    out.close();
    return flag;
}