#pragma once
#include <string>

class String {
	private:
	std::string val;
		
	public:
	String();
	String(char);
	String(const char*);
	String(std::string);
	String(bool);
	String(int);
	String(long);
	String(long long);
	String(double);
	int length();
	int indexOf(String pattern, int pos = 0);
	int indexOf(char pattern, int pos = 0);
	int lastIndexOf(String pattern);
	int lastIndexOf(char pattern);
	int lastIndexOf(String pattern, int pos);
	int lastIndexOf(char pattern, int pos);
	String subString(int start);
	String subString(int start, int end);
	String replace(String text, String replacement);
	const char* toCharArray();
	char charAt(int pos);

	String operator+(char);
	String operator+(String);
	String operator+=(char);
	String operator+=(String);
	bool operator==(String);
	bool operator!=(String);
};
