#ifndef CUSTOM_STRING_H
#define CUSTOM_STRING_H

#include <iostream>
//#include <touchgfx\Utils.hpp>
#include <stdlib.h>
#include "gui/common/date.hpp"

class String
{
private:
    char  data[256];
    unsigned length;

public:    
    String ( );    
    String (char c);
	String(const char* c, int length);    
    String (const char* c);    
    String (const String& s);
	String(int number);    

    ~String ( );
    
    unsigned len ( ) const;
	char* c_str();
    	
	char back();

	int toInt();
	float toFloat();
	DateTime toDateTime();

	void clear();
	int index(char c) const;	
	String substr(int startIndex, int len);
	void erase(int offset, int len);
    
    void upcase (unsigned first, unsigned last);
    void togglecase (unsigned first, unsigned last);


    friend std::ostream& operator<< (std::ostream& so, const String& s);
    friend std::istream& operator>> (std::istream& so,       String& s);
    char  operator[] (unsigned j) const;
    char& operator[] (unsigned j);
    String& operator= (const String& s);
    String& operator+= (const String& s);
    friend String operator+ (const String& lhs, const String& rhs);
    friend String operator+ (const String& lhs, char          rhs);
    friend String operator+ (const String& lhs, const char*   rhs);
    friend String operator+ (char          lhs, const String& rhs);
    friend String operator+ (const char*   lhs, const String& rhs);    
    friend bool operator== (const String& lhs, const String& rhs);
    friend bool operator== (const String& lhs, char          rhs);
    friend bool operator== (const String& lhs, const char*   rhs);
    friend bool operator== (char          lhs, const String& rhs);
    friend bool operator== (const char*   lhs, const String& rhs);    
    friend bool operator> (const String& lhs, const String& rhs);
    friend bool operator> (const String& lhs, char          rhs);
    friend bool operator> (const String& lhs, const char*   rhs);
    friend bool operator> (char          lhs, const String& rhs);
    friend bool operator> (const char*   lhs, const String& rhs);    
    friend bool operator!= (const String& lhs, const String& rhs);
    friend bool operator!= (const String& lhs, char          rhs);
    friend bool operator!= (const String& lhs, const char*   rhs);
    friend bool operator!= (char          lhs, const String& rhs);
    friend bool operator!= (const char*   lhs, const String& rhs);    
    friend bool operator< (const String& lhs, const String& rhs);
    friend bool operator< (const String& lhs, char          rhs);
    friend bool operator< (const String& lhs, const char*   rhs);
    friend bool operator< (char          lhs, const String& rhs);
    friend bool operator< (const char*   lhs, const String& rhs);    
    friend bool operator<= (const String& lhs, const String& rhs);
    friend bool operator<= (const String& lhs, char          rhs);
    friend bool operator<= (const String& lhs, const char*   rhs);
    friend bool operator<= (char          lhs, const String& rhs);
    friend bool operator<= (const char*   lhs, const String& rhs);    
    friend bool operator>= (const String& lhs, const String& rhs);
    friend bool operator>= (const String& lhs, char          rhs);
    friend bool operator>= (const String& lhs, const char*   rhs);
    friend bool operator>= (char          lhs, const String& rhs);
    friend bool operator>= (const char*   lhs, const String& rhs);    
};

#endif // CUSTOM_STRING_H
