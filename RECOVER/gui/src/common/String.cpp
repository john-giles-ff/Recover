#include "gui\common\String.hpp"

bool String::isDigit(char input, bool allowDecimal, bool isFirstCharacter)
{
	switch (input)
	{
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		return true;					
	case '-':
		return isFirstCharacter;
	case '.':
		return allowDecimal;
	}

	return false;
}

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
	if (numberDescructive <= 0)
		digits = 1;

	while (numberDescructive) {
		numberDescructive /= 10;
		digits++;
	}

	//Visual studio needs this to be sprintf_s, touchgfx needs it to be sprintf
#ifdef _MSC_VER
	sprintf_s(data, "%d", number);	
#else
	sprintf(data, "%d", number);	
#endif
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
int String::index(int i) const
{
	for (unsigned j = 0; j < length; j++)
		if (((int)data[j]) == i) return (int)j;
	return -1;
}

int String::toInt()
{
	if (length == 0u)
		return -1;

	for (unsigned i = 0; i < length; i++)
		if (!isDigit(data[i], false, i == 0))
			return -1;


	return atoi(data);
}

float String::toFloat()
{
	for (unsigned i = 0; i < length; i++)
		if (!isDigit(data[i], true, i == 0))
			return 0;

	return (float)atof(data);
}

void String::clear()
{
	for (int i = 0; i < DATA_SIZE; i++)
		data[i] = 0;
}

char String::back()
{
	return data[length];
}

DateTime String::toDateTime()
{
	DateTime dateTime;

	if (length == 0)
		return dateTime;

	//Make sure string has 4 spaces in it and 2 colons
	//Otherwise return blank
	int spaceCount = 0;
	int colonCount = 0;
	for (unsigned i = 0; i < length; i++)	
	{
		if (data[i] == ' ')
			spaceCount++;

		if (data[i] == ':')
			colonCount++;
	}
	if (spaceCount != 4 || colonCount != 2)
		return dateTime;



	//Define Variables
	String yearString;
	String monthString;
	String dayString;
	String timeString;
	String hourString;
	String minuteString;
	String secondString;
	int i = 0;
	int pos = 0;


	//Assume the input is in the following format:
	//Thu Sep 27 12:40:53 2018				

	//Skip to first space
	while (data[pos] != 0x20) { pos++; }		

	pos++;

	//Check format
	if (length - pos < 20)
		return dateTime;

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
	if (monthString == "Jan") dateTime.incMonth(0);
	else if (monthString == "Feb") dateTime.incMonth(1);
	else if (monthString == "Mar") dateTime.incMonth(2);
	else if (monthString == "Apr") dateTime.incMonth(3);
	else if (monthString == "May") dateTime.incMonth(4);
	else if (monthString == "Jun") dateTime.incMonth(5);
	else if (monthString == "Jul") dateTime.incMonth(6);
	else if (monthString == "Aug") dateTime.incMonth(7);
	else if (monthString == "Sep") dateTime.incMonth(8);
	else if (monthString == "Oct") dateTime.incMonth(9);
	else if (monthString == "Nov") dateTime.incMonth(10);
	else if (monthString == "Dec") dateTime.incMonth(11);

	//Parse Day				
	dateTime.incDay(dayString.toInt() - 2);

	//Parse Time
	//Hour				
	pos = timeString.index(':');
	hourString = timeString.substr(0, pos);
	dateTime.incHour(hourString.toInt());
	timeString.erase(0, pos + 1);

	//Minute		
	pos = timeString.index(':');
	minuteString = timeString.substr(0, pos);
	dateTime.incMinute(minuteString.toInt());
	timeString.erase(0, pos + 1);

	//Seconds		
	dateTime.incSecond(timeString.toInt());

	return dateTime;
}

bool String::toBool()
{
	if (length == 0)
		return false;

	return data[0] == '1';
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
	for (int i = newLength; i < DATA_SIZE - newLength; i++)
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

String String::combine(char seperator, String * parameters, int length)
{
	String result;	

	for (int i = 0; i < length; i++)
	{
		if (i != 0)
			result += seperator;					

		result += parameters[i];
	}	

	return result;
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




