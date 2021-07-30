#include "translation.h"
#include <bits/stdc++.h>
using namespace std;

int main(){
    if(Word_analysis()){
        puts("´Ê·¨·ÖÎö£º³É¹¦!");
        if(grammar_analysis()){
            puts("Óï·¨·ÖÎö£º³É¹¦!");
            if(translation()) puts("·­Òë£º³É¹¦£¡");
            else puts("·­Òë£ºÊ§°Ü£¡");
        }
        else puts("Óï·¨·ÖÎö£ºÊ§°Ü£¡");
    }
    else puts("´Ê·¨·ÖÎö£ºÊ§°Ü£¡");

    return 0;
}