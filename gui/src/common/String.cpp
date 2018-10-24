#include "gui\common\String.hpp"

String::String () 
{
	clear();
    length = 0;    
}

String::String (char c) 
{
	clear();

    length = 1;
	data[0] = c;    
}

String::String(const char* c, int len)
{	
	clear();

	if (c)
	{
		for (int i = 0; i < len; i++)
			data[i] = c[i];

		length = len;
	}
	else
		length = 0;
}

String::String (const char* c) 
{
	clear();

    if (c) 
    {
        unsigned n = 0;
        while (c[n] != '\0') n++;
        length = n;        
        for (unsigned j=0; j < n; j++)
            data[j] = c[j];
    } else 
    {
        length = 0;        
    }
}

String::String (const String& s) 
{
	clear();

    length = s.len();   
    for (unsigned j=0; j < length; j++)
        data[j] = s[j];
}

String::String(int number)
{
	clear();
		

	int numberDescructive = number;
	int digits = 0;
	if (numberDescructive < 0)
		digits = 1;

	while (numberDescructive) {
		numberDescructive /= 10;
		digits++;
	}

	sprintf(data, "%d", number);	
	length = digits;		
}

String::~String ( ) 
{    
}

unsigned String::len ( ) const 
{
    return length;
}

char* String::c_str()
{
	return data;
}

int String::index (char c) const 
{
    for (unsigned j=0; j < length; j++)
        if (data[j] == c) return (int)j;
    return -1;
}

int String::toInt()
{
	return atoi(data);
}

float String::toFloat()
{
	return atof(data);
}

void String::clear()
{
	for (int i = 0; i < 255; i++)
		data[i] = 0;
}

char String::back()
{
	return data[length];
}

DateTime String::toDateTime()
{
	//Define Variables
	String yearString;
	String monthString;
	String dayString;
	String timeString;
	String hourString;
	String minuteString;
	String secondString;
	DateTime dateTime;
	int pos = 0;
	int i = 0;

	//Assume the input is in the following format:
	//Thu Sep 27 12:40:53 2018			

	//Skip to first space
	while (data[pos] != 0x20) { pos++; }
	pos++;

	//Store until next space in month string
	while (data[pos + i] != 0x20) { i++; }
	monthString = substr(pos, i);	
	pos += i + 1;
	i = 0;	

	//Store until next space in day string
	while (data[pos + i] != 0x20) { i++; }
	dayString = String(reinterpret_cast<char const*>(data + pos), i);
	pos += i + 1;
	i = 0;	

	//Store until next space into time string
	while (data[pos + i] != 0x20) { i++; }
	timeString = String(reinterpret_cast<char const*>(data + pos), i);
	pos += i + 1;
	i = 0;	

	//Store rest into year string
	yearString = String(reinterpret_cast<char const*>(data + pos), length - pos);
	pos += i + 1;
	i = 0;	

	//Parse Year		
	dateTime.incYear(yearString.toInt());

	//Parse Month		
	if (monthString == "Jan") dateTime.incMonth(1);
	else if (monthString == "Feb") dateTime.incMonth(2);
	else if (monthString == "Mar") dateTime.incMonth(3);
	else if (monthString == "Apr") dateTime.incMonth(4);
	else if (monthString == "May") dateTime.incMonth(5);
	else if (monthString == "Jun") dateTime.incMonth(6);
	else if (monthString == "Jul") dateTime.incMonth(7);
	else if (monthString == "Aug") dateTime.incMonth(8);
	else if (monthString == "Sep") dateTime.incMonth(9);
	else if (monthString == "Oct") dateTime.incMonth(10);
	else if (monthString == "Nov") dateTime.incMonth(11);
	else if (monthString == "Dec") dateTime.incMonth(12);

	//Parse Day				
	dateTime.incDay(dayString.toInt());

	//Parse Time
	//Hour				
	pos = timeString.index(':');
	dateTime.incHour(timeString.substr(0, pos).toInt());
	timeString.erase(0, pos + 1);

	//Minute		
	pos = timeString.index(':');
	dateTime.incMinute(timeString.substr(0, pos).toInt());
	timeString.erase(0, pos + 1);

	//Seconds		
	dateTime.incSecond(timeString.toInt());

	STime time(dateTime.getRaw());		

	return dateTime;
}

String String::substr(int startIndex, int len)
{
	String result;

	for (int i = 0; i < len; i++)
		result += data[startIndex + i];		

	return result;
}

void String::erase(int offset, int len)
{
	if (len == 0)
		return;

	int newLength = length - len;

	//Replace Buffer after offset with data after erased data
	for (int i = 0; i < newLength; i++)
		data[offset + i] = data[offset + len + i];

	//Clear rest of buffer
	for (int i = newLength; i < 255 - newLength; i++)
		data[i] = 0;

	length = newLength;
}

void String::upcase (unsigned first, unsigned last) 
{
    if ((first >= last) || (last > length)) return;

    for (unsigned j=first; j < last; j++)
        if ('a' <= data[j] && data[j] <= 'z')
            data[j] -= ('a' - 'A');
}

void String::togglecase (unsigned first, unsigned last) 
{
    if ((first >= last) || (last > length)) return;

    for (unsigned j=first; j < last; j++)
        if ('A' <= data[j] && data[j] <= 'Z')
            data[j] += ('a' - 'A');
        else if ('a' <= data[j] && data[j] <= 'z')
            data[j] -= ('a' - 'A');
}



std::ostream& operator<< (std::ostream& os, const String& s) 
{
    if (s.len() > 0) 
    {
        for (unsigned j=0; j < s.len(); j++)
            os << s[j];
    } else os << "";

    return os;
}

std::istream& operator>> (std::istream& is, String& s) 
{
    char c[256];
    is >> c;
    s = String(c);    
    
    return is;
}



char String::operator[] (unsigned j) const 
{
    if (j >= length) return data[0];
    return data[j];
}

char& String::operator[] (unsigned j) 
{
    if (j >= length) return data[0];
    return data[j];
}



String& String::operator= (const String& s) 
{
    if(this == &s) return *this;

	length = s.len();

    for (unsigned j=0; j < length; j++)
        data[j] = s[j];
    return *this;
}



String& String::operator+= (const String& s) 
{
    unsigned len = length + s.len();        

    for (unsigned i=0; i < s.len(); i++)
        data[length+i] = s[i];
    
    length = len;    
    return *this;    
}



String operator+ (const String& lhs, const String& rhs) 
{
    return String(lhs) += rhs;
}

String operator+ (const String& lhs, char rhs) 
{
    return String(lhs) += String(rhs);
}

String operator+ (const String& lhs, const char* rhs) 
{
    return String(lhs) += String(rhs);
}

String operator+ (char lhs, const String& rhs) 
{
    return String(lhs) += rhs;
}
String operator+ (const char* lhs, const String& rhs) 
{
    return String(lhs) += rhs;
}



bool operator== (const String& lhs, const String& rhs) 
{
    if (lhs.len() != rhs.len()) return false;

    unsigned cap = lhs.len();
    unsigned   n = 0;
    while ((n < cap) && (lhs[n] == rhs[n])) n++;
    return (n == cap);
}

bool operator== (const String& lhs, char rhs) 
{
    return (lhs == String(rhs));
}

bool operator== (const String& lhs, const char* rhs) 
{
    return (lhs == String(rhs));
}

bool operator== (char lhs, const String& rhs) 
{
    return (String(lhs) == rhs);
}

bool operator== (const char* lhs, const String& rhs) 
{
    return (String(lhs) == rhs);
}



bool operator> (const String& lhs, const String& rhs) 
{
    unsigned cap = (lhs.len() < rhs.len()) ? lhs.len() : rhs.len();
    unsigned n = 0;
    while ((n < cap) && (lhs[n] == rhs[n])) n++;
    if (n == cap) return (lhs.len() > rhs.len());

    if ( (('A'<=lhs[n]&&lhs[n]<='Z') || ('a'<=lhs[n]&&lhs[n]<='z')) &&
         (('A'<=rhs[n]&&rhs[n]<='Z') || ('a'<=rhs[n]&&rhs[n]<='z')) )
    {
        char A = (lhs[n] & ~32);
        char B = (rhs[n] & ~32);
        if (A != B) return (A > B);
    }
    return lhs[n] > rhs[n];
}

bool operator> (const String& lhs, char rhs) 
{
    return (lhs > String(rhs));
}

bool operator> (const String& lhs, const char* rhs) 
{
    return (lhs > String(rhs));
}

bool operator> (char lhs, const String& rhs) 
{
    return (String(lhs) > rhs);
}

bool operator> (const char* lhs, const String& rhs) 
{
    return (String(lhs) > rhs);
}



bool operator!= (const String& lhs, const String& rhs) 
{
    return !(lhs == rhs);
}

bool operator!= (const String& lhs, char rhs) 
{
    return !(lhs == rhs);
}

bool operator!= (const String& lhs, const char* rhs) 
{
    return !(lhs == rhs);
}

bool operator!= (char lhs, const String& rhs) 
{
    return !(lhs == rhs);
}

bool operator!= (const char* lhs, const String& rhs) 
{
    return !(lhs == rhs);
}



bool operator< (const String& lhs, const String& rhs) 
{
    return !(lhs == rhs) && !(lhs > rhs);
}

bool operator< (const String& lhs, char rhs) 
{
    return !(lhs == rhs) && !(lhs > rhs);
}

bool operator< (const String& lhs, const char* rhs) 
{
    return !(lhs == rhs) && !(lhs > rhs);
}

bool operator< (char lhs, const String& rhs) 
{
    return !(lhs == rhs) && !(lhs > rhs);
}

bool operator< (const char* lhs, const String& rhs) 
{
    return !(lhs == rhs) && !(lhs > rhs);
}



bool operator<= (const String& lhs, const String& rhs) 
{
    return !(lhs > rhs);
}

bool operator<= (const String& lhs, char rhs) 
{
    return !(lhs > rhs);
}

bool operator<= (const String& lhs, const char* rhs) 
{
    return !(lhs > rhs);
}

bool operator<= (char lhs, const String& rhs) 
{
    return !(lhs > rhs);
}

bool operator<= (const char* lhs, const String& rhs) 
{
    return !(lhs > rhs);
}



bool operator>= (const String& lhs, const String& rhs) 
{
    return (lhs == rhs) || (lhs > rhs);
}

bool operator>= (const String& lhs, char rhs) 
{
    return (lhs == rhs) || (lhs > rhs);
}

bool operator>= (const String& lhs, const char* rhs) 
{
    return (lhs == rhs) || (lhs > rhs);
}

bool operator>= (char lhs, const String& rhs) 
{
    return (lhs == rhs) || (lhs > rhs);
}

bool operator>= (const char* lhs, const String& rhs) 
{
    return (lhs == rhs) || (lhs > rhs);
}




