#ifndef GUI_CIPHER_HPP
#define GUI_CIPHER_HPP

#include <touchgfx/hal/HAL.hpp>
#include <codecvt>
#include <iostream>
#include <locale>
#include <gui/common/String.hpp>

class Cipher
{
public:
	long GetCipher(double pressure, double baseTemp, double precursorTemp)
	{
#ifdef SIMULATOR
		pressure = 15.043;
		baseTemp = 36;
		precursorTemp = 178;
#endif

		uint16_t uPressure = (uint16_t)(pressure * 100);
		uint8_t uBaseTemp = (uint8_t)baseTemp;
		uint8_t uPrecursorTemp = (uint8_t)precursorTemp;


		bool bitArray[32];
		uint8_t pressurePos = 15;
		uint8_t tempPos = 7;
		for (int i = 0; i < 32; i++)
		{
			if (i % 2 == 0)
			{
				bitArray[i] = (uPressure & (1u << pressurePos)) != 0;
				pressurePos--;
			}
			else if (i % 4 == 1)
			{
				bitArray[i] = (uBaseTemp & (1u << tempPos)) != 0;

			}
			else
			{
				bitArray[i] = (uPrecursorTemp & (1u << tempPos)) != 0;
				tempPos--;
			}
		}

		return BitArrayToInt32(bitArray, 32);		
	}

private:
	String BitArrayToBinary(bool arr[], int count)
	{
		String r;
		for (int i = 0; i < count; i++)		
			r += arr[i] ? "1" : "0";
		

		return r;		
	}

	long BitArrayToInt32(bool arr[], int count)
	{
		long ret = 0;
		long tmp;
		for (int i = 0; i < count; i++) 
		{
			tmp = arr[i];
			ret |= tmp << (count - i - 1);
		}
		return ret;
	}
};


#endif /* GUI_UTILS_HPP */
