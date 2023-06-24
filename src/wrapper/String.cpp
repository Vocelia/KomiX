#include <cmath>
#include <cstring>
#include <sstream>

#include "../include/wrapper/String.h"

String::String(): String("") {}
String::String(char c): String(new char[2] {c, '\0'}) {}
String::String(const char* str): String(std::string(str)) {}
String::String(std::string str) {
	val = str;
}

String::String(bool val): String(val ? "true" : "false") {}

String::String(int num): String((long long) num) {}
String::String(long num): String((long long) num) {}
String::String(long long num) {
	std::stringstream ss;
	ss << num;
	val = ss.str();
}

String::String(double num) {
	std::stringstream ss;
	ss << num;
	val = ss.str();
}

int String::length() {
	return val.length();
}

int String::indexOf(String pattern, int pos) {
	const char* cstr = toCharArray();
	const char* ptr = strstr(&cstr[pos], pattern.toCharArray());
	return ptr == NULL ? -1 : (ptr - cstr);
}

int String::indexOf(char pattern, int pos) {
	const char* cstr = toCharArray();
	const char* ptr = strchr(&cstr[pos], pattern);
	return ptr == NULL ? -1 : (ptr - cstr);
}

int String::lastIndexOf(String pattern) {
	return lastIndexOf(pattern, length() - 1);
}

int String::lastIndexOf(char pattern) {
	return lastIndexOf(pattern, length() - 1);
}

int String::lastIndexOf(String pattern, int pos) {
	const char* cstr = toCharArray();
	for(int i = pos; i >= 0; i--)
		if(strstr(&cstr[i], pattern.toCharArray()) == &cstr[i])
			return i;
	return -1;
}

int String::lastIndexOf(char pattern, int pos) {
	const char* cstr = toCharArray();
	for(int i = pos; i >= 0; i--)
		if(cstr[i] == pattern)
			return i;
	return -1;
}

String String::subString(int start) {
	return subString(start, length());
}

String String::subString(int start, int end) {
	return String(val.substr(start, end - start));
}

String String::replace(String text, String replacement) {
	String result = String(toCharArray());
	int pos = lastIndexOf(text);
	while(pos != -1) {
		result = result.subString(0, pos) + replacement + result.subString(pos + text.length());
		pos = lastIndexOf(text, pos - 1);
	}
	return result;
}

const char* String::toCharArray() {
	return val.c_str();
}

char String::charAt(int pos) {
	return val.c_str()[pos];
}

// OPERATORS
String String::operator+(char chr) {
	String result = String(*this);
	result += chr;
	return result;
}

String String::operator+(String str) {
	String result = String(*this);
	result += str;
	return result;
}

String String::operator+=(char chr) {
	val.append(1, chr);
	return *this;
}

String String::operator+=(String str) {
	val.append(str.toCharArray());
	return *this;
}

bool String::operator==(String str) {
	return strcmp(this->toCharArray(), str.toCharArray()) == 0;
}

bool String::operator!=(String str) {
	return !operator==(str);
}
