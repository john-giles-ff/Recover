using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

[assembly: InternalsVisibleTo("RecoverCompanionApplication.Tests")]

namespace FosterAndFreeman.RecoverCompanionApplication.Definitions.DeviceData
{
    public class RecoverFile
    {        
        private int FirmwareLength = 0;
        private byte[] FirmwareData;
        private int SoftwareLength = 0;
        private byte[] SoftwareData;
        private byte[] HeaderData => new byte[] { 0x52, 0x45, 0x43, 0x4f, 0x56, 0x45, 0x52, 0x5f, 0x53, 0x4f, 0x46, 0x54, 0x57, 0x41, 0x52, 0x45, 0x5f, 0x46, 0x49, 0x52, 0x4d, 0x57, 0x41, 0x52, 0x45 };
        private byte FirmwareSignifier => 0x46;
        private byte SoftwareSignifier => 0x53;

        public RecoverFile() { }

        /// <summary>
        /// Create object from file
        /// </summary>
        /// <param name="input">File to use to create class</param>
        public RecoverFile(string input)
        {
            DecodeFile(input);
        }

        /// <summary>
        /// Generate File and write it to path
        /// </summary>
        /// <param name="path">Location to generate file</param>
        public void GenerateFile(string path)
        {            
            System.IO.File.WriteAllBytes(path, GetBytes());
        }

        public byte[] GetFirmwareData()
        {
            return FirmwareData;
        }

        public byte[] GetSoftwareData()
        {
            return SoftwareData;
        }


        /// <summary>
        /// Compare this Recover File to another Recover File
        /// </summary>
        /// <param name="file">File to compare with</param>
        /// <returns>If the file was the same</returns>
        public bool Compare(RecoverFile file)
        {
            var thisBytes = GetBytes();
            var otherBytes = file.GetBytes();

            return thisBytes.SequenceEqual(otherBytes);
        }

        /// <summary>
        /// Add Firmware to file
        /// </summary>
        /// <param name="firmware">Firmware to add</param>
        /// <param name="length">Length of firmware data</param>
        public void AddFirmware(IEnumerable<byte> firmware, int length)
        {
            FirmwareData = firmware.ToArray();
            FirmwareLength = length;
        }

        /// <summary>
        /// Add software to file
        /// </summary>
        /// <param name="software">Software to add</param>
        /// <param name="length">Length of software data</param>
        public void AddSoftware(IEnumerable<byte> software, int length)
        {
            SoftwareData = software.ToArray();
            SoftwareLength = length;
        }

        /// <summary>
        /// Get File in byte form
        /// </summary>
        /// <returns>File In Byte Form</returns>
        internal byte[] GetBytes()
        {
            if (FirmwareLength == 0 && SoftwareLength == 0)
                throw new InvalidOperationException("No Firmware or Software to write to file");

            //Declare object for all data to go into
            var data = new List<byte>();

            //Add Header
            data.AddRange(HeaderData);

            //Add Firmware (If exists)
            if (FirmwareLength > 0)
            {
                //Add F to signify firmware
                data.Add(FirmwareSignifier);

                //Add Length of firmware
                var firmwareLengthBytes = BitConverter.GetBytes(FirmwareLength);
                data.AddRange(firmwareLengthBytes);

                //Add Firmware Data
                data.AddRange(FirmwareData);
            }

            if (SoftwareLength > 0)
            {
                //Add S to signify software
                data.Add(SoftwareSignifier);

                //Add Length of software
                var softwareLengthBytes = BitConverter.GetBytes(SoftwareLength);
                data.AddRange(softwareLengthBytes);

                //Add Software Data
                data.AddRange(SoftwareData);
            }

            return data.ToArray();
        }

        /// <summary>
        /// Decode Data from file
        /// </summary>
        /// <param name="input">File to decode</param>
        private void DecodeFile(string input)
        {
            if (string.IsNullOrWhiteSpace(input))
                throw new InvalidCastException("No File has been Selected");

            if (!System.IO.File.Exists(input))
                throw new InvalidCastException("File does not exist");

            var inputData = System.IO.File.ReadAllBytes(input);
            int offset = 0;
            
            //Check length is longer than the header
            if (inputData.Length < HeaderData.Length)
                throw new InvalidCastException("File is not a valid Recover File - 22DA1W5");

            //Copy Data where header would be to new array
            var inputHeaderData = new byte[HeaderData.Length];
            Array.Copy(inputData, inputHeaderData, HeaderData.Length);

            //Check header data is correct
            if (HeaderData.SequenceEqual(inputData))
                throw new InvalidCastException("File is not a valid Recover File - AWWDFE3");

            //Update offset
            offset = HeaderData.Length;

            //Loop until there is no more data
            do
            {
                //Decode data block
                offset = DecodeDataBlock(offset, inputData);
                
            } while (offset < inputData.Length);            
            

        }

        /// <summary>
        /// Decode Data Block
        /// </summary>
        /// <param name="offset">How far into the data to look</param>
        /// <param name="data">Data to look at</param>
        /// <returns>New offset</returns>
        private int DecodeDataBlock(int offset, byte[] data)
        {
            //Check what data is next and decode it, if neither option throw error
            if (data[offset] == FirmwareSignifier)
                offset = DecodeFirmware(offset + 1, data);
            else if (data[offset] == SoftwareSignifier)
                offset = DecodeSoftware(offset + 1, data);
            else
                throw new InvalidCastException("File is not a valid Recover File - 54DAWCW");

            return offset;
        }


        /// <summary>
        /// Decode Firmware from data
        /// </summary>
        /// <param name="offset">How far into the data to look</param>
        /// <param name="data">Data to look at</param>
        /// <returns>New Offset</returns>
        private int DecodeFirmware(int offset, byte[] data)
        {
            //Get length value
            //var lengthData = new byte[4];
            //Array.Copy(data, offset + 1, lengthData, 0, 4);            
            FirmwareLength = BitConverter.ToInt32(data, offset);
            offset += 4;

            FirmwareData = new byte[FirmwareLength];
            Array.Copy(data, offset, FirmwareData, 0, FirmwareLength);

            offset += FirmwareLength;

            return offset;
        }

        /// <summary>
        /// Decode Software from data
        /// </summary>
        /// <param name="offset">How far into the data to look</param>
        /// <param name="data">Data to look at</param>
        /// <returns>New Offset</returns>
        private int DecodeSoftware(int offset, byte[] data)
        {
            SoftwareLength = BitConverter.ToInt32(data, offset);
            offset += 4;

            SoftwareData = new byte[SoftwareLength];
            Array.Copy(data, offset, SoftwareData, 0, SoftwareLength);

            offset += SoftwareLength;

            return offset;            
        }

    }
}
