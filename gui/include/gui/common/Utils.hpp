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

class Utils
{
public:	
	static void Utf16ToUtf8 (unsigned char* input, unsigned char* output, int size)
	{		
		for (int i = 0; i < size; i++)
			output[i] = (unsigned char)input[i * 2];
	}

	static void Utf8ToUtf16(unsigned char* input, unsigned char* output, int size)
	{		
		for (int i = 0; i < size; i++)
		{			
			output[(i * 2) + 0] = (unsigned char)input[i];
			output[(i * 2) + 1] = 0x00;
		}		
	}

	static int intToUtf8(int input, unsigned char* output)
	{		
		int size = 1;

		switch (input)
		{
		case 0:
			output[0] = 0x30;
			break;
		case 1:
			output[0] = 0x31;
			break;
		case 2:
			output[0] = 0x32;
			break;
		case 3:
			output[0] = 0x33;
			break;
		case 4:
			output[0] = 0x34;
			break;
		case 5:
			output[0] = 0x35;
			break;
		case 6:		
			output[0] = 0x36;
			break;
		case 7:
			output[0] = 0x37;
			break;
		case 8:
			output[0] = 0x38;
			break;
		case 9:		
			output[0] = 0x39;
			break;
		default:
			size = 0;
			break;
		}

		return size;
		
	}

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
	
	static TEXTS TranslateError(int errorID)
	{
		//TODO: Update these to translate to the relevant errors
		switch (errorID)
		{		 		
		case 1:
			return T_ERROR_WDTRESET;
		case 2:
			return T_ERROR_FUMEHOOD;
		case 3:
			return T_ERROR_LARGELEAK;
		case 4:
			return T_ERROR_SMALLLEAK;
		case 5:
			return T_ERROR_PREHEATRANGE;
		case 6: 
			return T_ERROR_BASEHEATRANGE;
		case 7: 
			return T_ERROR_PREHEATALARM;
		case 8:
			return T_ERROR_DOORSAFE;
		case 9:
			return T_ERROR_PSWITCH1SAFE;
		case 10:
			return T_ERROR_PSWITCH2SAFE;
		case 11:
			return T_ERROR_BASEHEATTIMEOUT;
		case 12:
			return T_ERROR_PREHEATTIMEOUT;
		case 13:
			return T_ERROR_PUMPTIMEOUT;
		case 14:
			return T_ERROR_SENSORNOTFOUND;
		case 15:
			return T_ERROR_SENSORFAULT;
		case 16:
			return T_ERROR_PURGEVALVEFAULT;
		case 17:
			return T_ERROR_INLETVALVEFAULT;
		case 18:
			return T_ERROR_BYPASSVALVEFAULT;
		case 19:
			return T_ERROR_SPAREVALVEFAULT;
		case 20:
			return T_ERROR_PUMPFAULT;
		case 21:
			return T_ERROR_PREHEATCURRENT;
		case 22:
			return T_ERROR_BASECURRENT;
		case 23:
			return T_ERROR_FAULTFAULT;
		case 24:
			return T_ERROR_PSWITCH1LEAK;
		case 25:
			return T_ERROR_PROCESSHALT;


		default:
			return T_ERROR_UNKNOWN;		
		}		
	}			
};


#endif /* GUI_UTILS_HPP */
