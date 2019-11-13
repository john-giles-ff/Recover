using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace UserDecode
{
    public class UserCipher
    {
        public string Cipher { get; set; }
        public double Pressure { get; set; }
        public double BaseTemp { get; set; }
        public double PreTemp { get; set; }

        public UserCipher(double pressure, double baseTemp, double preTemp)
        {
            Pressure = pressure;
            BaseTemp = baseTemp;
            PreTemp = preTemp;

            UInt16 uPressure = (UInt16)(Pressure * 100);
            byte uBase = (byte)BaseTemp;
            byte uPre = (byte)PreTemp;


            var bitArray = new bool[32];
            byte pressurePos = 15;
            byte tempPos = 7;
            string debug;
            for (int i = 0; i < 32; i++)
            {
                if (i % 2 == 0)
                {                    
                    bitArray[i] = (uPressure & (1u << pressurePos)) != 0;
                    pressurePos--;
                }
                else if (i % 4 == 1)
                {
                    bitArray[i] = (uBase & (1u << tempPos)) != 0;
                }
                else
                {
                    bitArray[i] = (uPre & (1u << tempPos)) != 0;
                    tempPos--;
                }

                debug = BinaryToString(bitArray);
                Console.Write(debug);

            }

            debug = BinaryToString(bitArray);
            Console.Write(debug);

            Cipher = BoolArrayToInt(bitArray).ToString("X");
        }

        public UserCipher(string cipher)
        {            
            var number = Convert.ToUInt32(cipher, 16);

            var array = IntToBoolArray(number);
            UInt16 uPressure = 0;
            byte uBase = 0;
            byte uPre = 0;

            int pressureShift = 15;
            int baseShift = 7;
            int preShift = 7;

            for (int i = 0; i < 32; i++)
            {
                if (i % 2 == 0)
                {
                    uPressure += (UInt16)((array[i] ? 1u : 0u) << pressureShift);
                    pressureShift--;                    
                }
                else if (i % 4 == 1)
                {
                    uBase += (byte)((array[i] ? 1u : 0u) << baseShift);
                    baseShift--;                    
                }
                else
                {
                    uPre += (byte)((array[i] ? 1u : 0u) << preShift);
                    preShift--;                    
                }
            }


            Pressure = uPressure / 100.0;
            BaseTemp = uBase;
            PreTemp = uPre;
        }

        private UInt32 BoolArrayToInt(bool[] bits)
        {
            var r = new int[1];
            new BitArray(bits).CopyTo(r, 0);
            return (UInt32)r[0];
        } 

        private bool[] IntToBoolArray(UInt32 number)
        {
            var r = new bool[32];
            new BitArray(BitConverter.GetBytes(number)).CopyTo(r, 0);
            return r;
        }

        
        string BinaryToString(bool[] a)
        {
            string r = "";
            for (int i = 0; i < a.Length; i++)            
                r += a[i] ? "1" : "0";

            return r;            
        }
    }
}
