using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
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

            var number = BoolArrayToInt(bitArray);
            Trace.WriteLine("Generated:");
            Trace.WriteLine($"Number - {number}");
            Trace.WriteLine($"Binary - {BinaryToString(bitArray)}");
            Trace.WriteLine($"Cipher - {number.ToString("X")}");
            Trace.WriteLine("");


            Cipher = number.ToString("X");
        }

        public UserCipher(string cipher)
        {            
            var number = Convert.ToUInt32(cipher, 16);
            var array = IntToBoolArray(number);

            var debug = BinaryToString(array);
            var pressureBits = new List<bool>();
            var baseBits = new List<bool>();
            var precursorBits = new List<bool>();
            

            for (int i = 0; i < 32; i++)
            {
                if (i % 2 == 0)
                    pressureBits.Add(array[i]);
                else if (i % 4 == 1)
                    baseBits.Add(array[i]);
                else
                    precursorBits.Add(array[i]);
            }
            

            Trace.WriteLine("Read:");
            Trace.WriteLine($"Number - {number}");
            Trace.WriteLine($"Binary - {BinaryToString(array)}");
            Trace.WriteLine($"Cipher - {number.ToString("X")}");
            Trace.WriteLine("");


            Pressure = ((double)BoolArrayToInt(pressureBits.ToArray())) / 100.0;
            BaseTemp = BoolArrayToInt(baseBits.ToArray());
            PreTemp = BoolArrayToInt(precursorBits.ToArray());
        }

        private UInt32 BoolArrayToInt(bool[] arr)
        {
            UInt32 ret = 0;
            UInt32 tmp;
            UInt32 count = (UInt32)arr.Length;            
            for (int i = 0; i < count; i++)
            {
                tmp = arr[i] ? 1u : 0u;
                ret |= tmp << (int)(count - i - 1);
            }
            return (UInt32)ret;
        } 

        private bool[] IntToBoolArray(UInt32 n)
        {
            var bytes = BitConverter.GetBytes(n);
            Array.Reverse(bytes);
            var bits = new List<bool>();
            foreach (var b in bytes)
            {
                var eightBits = new bool[8];
                for (int i = 0; i < 8; i++)
                    eightBits[i] = (b & (1 << i)) == 0 ? false : true;

                Array.Reverse(eightBits);                
                var a = BinaryToString(eightBits);
                bits.AddRange(eightBits);

            }


            return bits.ToArray();
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
