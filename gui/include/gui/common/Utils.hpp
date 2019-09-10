#ifndef GUI_UTILS_HPP
#define GUI_UTILS_HPP

#include <touchgfx/hal/HAL.hpp>
#include <codecvt>
#include <iostream>
#include <locale>
#include <touchgfx\Utils.hpp>
#include <stdlib.h>
#include <texts\TextKeysAndLanguages.hpp>
#include <gui/common/date.hpp>
#include <gui/common/String.hpp>

struct ErrorMessage
{
	TEXTS ErrorText;

	int ErrorID;

	//Is Actionable Error messages will show as specfic error messages, others will show as generic
	bool isActionable;
	//Is Ignored is infectious, if one record in a group is ignored then all of them are.
	bool isIgnored;
	//NULL Quanitifier
	bool exists;

	ErrorMessage() { exists = false; }
	ErrorMessage(int errorID)
	{
		ErrorID = errorID;

		exists = true;
		isActionable = false;
		isIgnored = false;

		switch (errorID)
		{
			//User Actionable
			//================================
		case 3:  //Large Leak
		case 4:  //Small Leak
		case 28: //Vacuum above VAC_MAX
			ErrorText = T_ERROR_LEAK;
			isActionable = true;
			break;
		case 8:  //Door not in safe state
			ErrorText = T_ERROR_DOOR;
			isActionable = true;
			break;			
		case 13: //Pump timeout
			ErrorText = T_ERROR_TIMEOUT;
			isActionable = true;
			break;

			//User inactionable
			//================================
		case 1:  //WDT RESET			
		case 2:  //Hood Fault
		case 5:  //Precursor heater out of range
		case 6:  //Base heater out or range
		case 7:  //Pre heater exceeds maximum temperature
		case 9:  //P SWITCH 1 Fault
		case 10: //P SWITCH 2 Fault
		case 11: //Base Heater timeout
		case 12: //Precursor heater timeout
		case 14: //Sensor not found
		case 15: //Sensor fault
		case 16: //Purge Valve not within range
		case 17: //Inlet Valve not within range
		case 18: //Bypass Valve not within range
		case 19: //Spare Valve fault (NOT CURRENTLY FITTED)
		case 20: //Pump current not within range
		case 21: //Precursor heater not within range
		case 22: //Base heater not within range
		case 23: //Persistent Fault on Driver
		case 24: //Leak detected on P SWITCH 1
		case 26: //Leak detected on P SWITCH 2
		case 27: //Not Initialised
		case 29: //Temperature Sensor Swapped
		default:
			ErrorText = T_ERROR_GENERIC;
			break;



			//Ignore
			//================================
		case 25: //HALT
			isIgnored = true;
			break;

		}
	}

	

};


class Utils
{
public:		
	static int numberOfDigits(int input)
	{
		int numberOfDigits = 0;
		const int base = 10;

		do {
			++numberOfDigits;
			input /= base;
		} while (input);

		return numberOfDigits;
	}		

	static String STimeToString(STime time)
	{
		return String(
			PadL('0', time.GetHour(), 2) + ":" +
			PadL('0', time.GetMinute(), 2) + ":" +
			PadL('0', time.GetSecond(), 2) + " " +
			PadL('0', time.GetDay(), 2) + " " +
			GetMonthString(time.GetMonth()) + " " +
			PadL('0', time.GetYear(), 4));		
	}

	static String GetMonthString(int month)
	{
		//const uint16_t * text = NULL;
		touchgfx::TypedText text = touchgfx::TypedText();

		switch (month)
		{
		case 0:
			text = touchgfx::TypedText(T_DATE_JAN);
			break;
		case 1:
			text = touchgfx::TypedText(T_DATE_FEB);
			break;
		case 2:
			text = touchgfx::TypedText(T_DATE_MAR);
			break;
		case 3:
			text = touchgfx::TypedText(T_DATE_APR);
			break;
		case 4:
			text = touchgfx::TypedText(T_DATE_MAY);
			break;
		case 5:
			text = touchgfx::TypedText(T_DATE_JUN);
			break;
		case 6:
			text = touchgfx::TypedText(T_DATE_JUL);
			break;
		case 7:
			text = touchgfx::TypedText(T_DATE_AUG);
			break;
		case 8:
			text = touchgfx::TypedText(T_DATE_SEP);
			break;
		case 9:
			text = touchgfx::TypedText(T_DATE_OCT);
			break;
		case 10:
			text = touchgfx::TypedText(T_DATE_NOV);
			break;
		case 11:
			text = touchgfx::TypedText(T_DATE_DEC);
			break;
		default:			
			break;
		}		
		
		if (text.hasValidId())		
		{
			const uint16_t * unicodeFormat = text.getText();


			uint8_t utf8Format[20];
			touchgfx::Unicode::toUTF8(unicodeFormat, utf8Format, 20);
			

			return String((char*)utf8Format);
		}

		return String();
	}

	static bool CompareArrays(int* array1, const int* array2, int size)
	{
		for (int i = 0; i < size; i++)
			if (array1[i] != array2[i])
				return false;

		return true;
	}

	static String PadL(char c, String input, int count)
	{
		while ((int)input.len() < count)		
			input = c + input;
		
		return input;
	}

	static TEXTS GetDevelopRString(bool chamberType, bool metalType, bool isLog)
	{
		int data = 0;
		if (metalType)
			data += 1;
		if (chamberType)
			data += 2;
		

		switch (data)
		{
		case 0:
			if (isLog)
				return T_DEVELOPR1LOG;
			else
				return T_DEVELOPR1;
		case 1:
			if (isLog)
				return T_DEVELOPR2LOG;
			else
				return T_DEVELOPR2;
		case 2:
			if (isLog)
				return T_DEVELOPR3LOG;
			else
				return T_DEVELOPR3;
		case 3:
			if (isLog)
				return T_DEVELOPR4LOG;
			else
				return T_DEVELOPR4;
		}

		return TEXTS::T_ALPHAMODE;


	}
	
	static String GetTypedTextsString(TEXTS textID)
	{
		touchgfx::TypedText text(textID);
		const uint16_t * unicodeFormat = text.getText();
		uint8_t utf8Format[52];
		touchgfx::Unicode::toUTF8(unicodeFormat, utf8Format, 52);
		return String((const char *)utf8Format);
	}

	static String GetTypedTextsString(TEXTS textID, String(wildcard))
	{
		String text = GetTypedTextsString(textID);

		

		//Get position of in and out of wildcard
		int inIndex = text.index(2);		

		//Get text before and after wildcard
		String beforeWildcard = text.substr(0, inIndex);
		String afterWildcard = text.substr(inIndex + 1, text.len() - inIndex + 1);

		//Combine wildcard result
		String result(beforeWildcard + wildcard + afterWildcard);						
		
		return result;
	}

	static bool IsPointWithinRectagle(int rectangleX, int rectangleY, int rectangleWidth, int rectangleHeight, int pointX, int pointY)
	{
		int x1 = rectangleX;
		int y1 = rectangleY;
		int x2 = rectangleX + rectangleWidth;
		int y2 = rectangleY + rectangleHeight;

		int x = pointX;
		int y = pointY;

		if (x > x1 && x < x2 && y > y1 && y < y2)
			return true;

		return false;

	}
};


#endif /* GUI_UTILS_HPP */
