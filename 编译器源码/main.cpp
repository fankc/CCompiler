#include "translation.h"
#include <bits/stdc++.h>
using namespace std;

int main(){
    if(Word_analysis()){
        puts("�ʷ��������ɹ�!");
        if(grammar_analysis()){
            puts("�﷨�������ɹ�!");
            if(translation()) puts("���룺�ɹ���");
            else puts("���룺ʧ�ܣ�");
        }
        else puts("�﷨������ʧ�ܣ�");
    }
    else puts("�ʷ�������ʧ�ܣ�");

    return 0;
}