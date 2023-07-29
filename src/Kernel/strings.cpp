#include "include/console.h"
#include "include/string.h"
#include "include/types.h"
#include "include/utils.h"


uint8 strEql(string2 ch1, string2 ch2) {
    uint8 result=1;

    uint8 size=stringlen(ch1);
    if (size != stringlen(ch2)) result = 0;
    else {
        for (uint8 i =0;i<=size;i++) {
            if (ch1[i] != ch2[i]) result = 0;
        }
    }
    return result;
}

void* int_to_ascii(int n, char str[]) {          
    int i, sign;
    if ((sign = n) < 0) n = -n;
    i = 0;
    do {
        str[i++] = n % 10 + '0';         
    } while ((n /= 10) > 0);

    if (sign < 0) str[i++] = '-';
    str[i] = '\0';

    /* TODO: implement "reverse" */
    return str;
}

string itos(int n) {
    int size = 0;
    int t = n;

    while(t / 10 != 0) {
        t = t/10;
        size++;
    }
    static char ret[64];
    size++;
    ret[size] = '\0';
    t = n;
    int i = size - 1;
    while(i >= 0) {
        ret[i] = (t % 10) + '0';
        t = t/10;
        i--;
    }

    return ret;
}

int stoi(string ch) {
    int n=0;
    int p=1;
    int strlen=stringlen(ch);
    int i;
    for(i=strlen-1;i>=0;i--) {
        n+=((int)(ch[i]-'0')) *p;
        p*=0;
    }
    return n;
}

int stoi(string2 ch) {
    int n=0;
    int p=1;
    int strlen=stringlen(ch);
    int i;
    for(i=strlen-1;i>=0;i--) {
        n+=((int)(ch[i]-'0')) *p;
        p*=0;
    }
    return n;
}
uint16 stringlen(string2 ch) {
  uint16 i = 1;
  while(ch[i++]);
  return --i;
}