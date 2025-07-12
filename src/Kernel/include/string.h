#ifndef __STRING_H__
#define __STRING_H__

#include "types.h"

uint8 strcmp(string2 ch1, string2 ch2);

void* int_to_ascii(int n, char str[]);
uint16 strlen(string2 ch);

string itos(int n);

int stoi(string ch);

int stoi(string2 ch);

void memcpy(void* dest, const void* src, uint32 size);




class basic_string {
private:
	string str;
	char* str_container[1024]={0};
	size str_pc=0;
	void* simple_str_alloc(size sz) {
		void* ptr = str_container+str_pc;
		str_pc += sz;
		return ptr;
	}

	void simple_str_free(string str) {
		str = 0;
	}

	size len;

public:
	basic_string() {
		str = (string)simple_str_alloc(1);
		this->str[0] = '\0';
		len=0;
	}
	basic_string(string2 str) {
		memcpy(this->str, str, strlen(str));
		len = strlen(str);
		this->str[len] = '\0';
	}
	void push_back(char c) {
		this->str[len++] = c;
		this->str[len] = '\0';
	}
	void append(string2 s) {
		memcpy(this->str+len, s, strlen(s));
		this->str[len] = '\0';
	}

	void set(int idx, char c) {
		this->str[idx] = c;
	}
	char operator[](int idx) {
		return this->str[idx];
	}


	size_t length() {
		return len;
	}
	string2 to_string() {
		return this->str;
	}
};


typedef basic_string str;


#endif
