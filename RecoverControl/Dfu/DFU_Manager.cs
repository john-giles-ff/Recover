using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using static FosterAndFreeman.Internal.Win32Usb;

namespace FosterAndFreeman.Internal.Dfu
{
    /// <summary>
    /// Class to encapsulate information about a programmable sector of memory within the micro-controller
    /// </summary>
    public class MappingSector
    {
        public enum SectorType { InternalFLASH, OptionBytes, OTP, DeviceFeature, Other };
        public SectorType Type;
        public String Name;
        public UInt32 dwStartAddress;
        public UInt32 dwSectorIndex;
        public UInt32 dwSectorSize;

        public MappingSector(String Name, SectorType SType, UInt32 StartAddress, UInt32 Size, UInt16 SectorIndex)
        {
            this.Name = Name;
            this.Type = SType;
            this.dwStartAddress = StartAddress;
            this.dwSectorSize = Size;
            this.dwSectorIndex = SectorIndex;
        }
    }

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Auto)]
    public struct DFU_FunctionalDescriptor
    {
        public byte bLength;
        public byte bDescriptorType; // Should be 0x21
        public byte bmAttributes;
        public UInt16 wDetachTimeOut;
        public UInt16 wTransfertSize;
        public UInt16 bcdDFUVersion;
    };

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Auto)]
    public struct USB_InterfaceDescriptor
    {
        public byte bLength;
        public byte bDescriptorType;
        public byte bInterfaceNumber;
        public byte bAlternateSetting;
        public byte bNumEndpoints;
        public byte bInterfaceClass;
        public byte bInterfaceSubClass;
        public byte bInterfaceProtocol;
        public byte iInterface;
    };

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Auto)]
    public struct USB_DeviceDescriptor // Taken from usb100.h
    {
        public byte bLength;
        public byte bDescriptorType;
        public ushort bcdUSB;
        public byte bDeviceClass;
        public byte bDeviceSubClass;
        public byte bDeviceProtocol;
        public byte bMaxPacketSize0;
        public ushort idVendor;
        public ushort idProduct;
        public ushort bcdDevice;
        public byte iManufacturer;
        public byte iProduct;
        public byte iSerialNumber;
        public byte bNumConfigurations;
    };

    public struct DFU_Areas
    {
        public long StartAddress;
        public List<byte> Data;

        public DFU_Areas(int startAddress)
        {
            Data = new List<byte>();
            StartAddress = startAddress;
        }
    }

    public enum DFU_ProgressType
    {
        Parsing,
        Erasing,
        Writing,
    }

    public struct DFU_Progress
    {
        public DFU_Progress(DFU_ProgressType type, int maximum, int value)
        {
            Type = type;
            Maximum = maximum;
            Value = value;
        }

        public DFU_ProgressType Type;
        public int Maximum;
        public int Value;
    }


    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Auto)]
    public struct DFU_Status
    {
        public byte bStatus;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
        public byte[] bwPollTimeout;
        public byte bState;
        public byte iString;
    };

    public class DFU_Manager : Win32Usb
    {

        public EventHandler<DFU_Progress> OnProgress;


        private UInt16 MaxWriteBlockSize = 1024;
        const UInt16 HID_VID = 0x1234;
        const UInt16 HID_PID = 0x5678;
        const byte HID_DETACH_REPORT_ID = 0x80;
        const byte USAGE_DETACH = 0x55;
        const uint STDFU_ERROR_OFFSET = 0x12340000;
        const uint STDFU_NOERROR = STDFU_ERROR_OFFSET + 0;

        const byte STATE_IDLE = 0x00;
        const byte STATE_DETACH = 0x01;
        const byte STATE_DFU_IDLE = 0x02;
        const byte STATE_DFU_DOWNLOAD_SYNC = 0x03;
        const byte STATE_DFU_DOWNLOAD_BUS = 0x04;
        const byte STATE_DFU_DOWNLOAD_IDLE = 0x05;
        const byte STATE_DFU_MANIFEST_SYNC = 0x06;
        const byte STATE_DFU_MANIFEST = 0x07;
        const byte STATE_DFU_MANIFEST_WAIT_RESET = 0x08;
        const byte STATE_DFU_UPLOAD_IDLE = 0x09;
        const byte STATE_DFU_ERROR = 0x0A;
        const byte STATE_DFU_UPLOAD_SYNC = 0x91;
        const byte STATE_DFU_UPLOAD_BUSY = 0x92;

        IntPtr INVALID_HANDLE_VALUE = (System.IntPtr)(-1);


        private Guid GUID_DFU = new Guid(0x3fe809ab, 0xfb91, 0x4cb5, 0xa6, 0x43, 0x69, 0x67, 0x0d, 0x52, 0x36, 0x6e);

        private IntPtr OpenDFU_Device(out List<MappingSector> Sectors, out UInt16 MaxWriteBlockSize, out UInt32 interfaceNumber)        
        {
            int i = 30;
            uint Index = 0;
            interfaceNumber = 0;
            Guid GUID = GUID_DFU;
            DeviceInterfaceData ifData = new DeviceInterfaceData();
            ifData.Size = Marshal.SizeOf(ifData);
            DeviceInterfaceDetailData ifDetail = new DeviceInterfaceDetailData();
            UInt32 Size = 0;
            IntPtr hInfoSet = SetupDiGetClassDevs(ref GUID, null, IntPtr.Zero, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);  // this gets a list of all DFU devices currently connected to the computer (InfoSet)
            IntPtr hDevice = IntPtr.Zero;
            string DFU_DevicePath;

            Sectors = null;
            MaxWriteBlockSize = 1024;

            try
            {
                if (hInfoSet == INVALID_HANDLE_VALUE)
                {
                    throw new Exception("SetupDiGetClassDevs returned error=" + Marshal.GetLastWin32Error().ToString());
                }

                // Loop 30 times hoping to find exactly one DFU device
                while (i-- > 0)
                {
                    Index = 0;
                    while (SetupDiEnumDeviceInterfaces(hInfoSet, 0, ref GUID, Index, ref ifData))
                    {
                        Index++;
                    }
                    if (0 == Index)
                    {
                        System.Threading.Thread.Sleep(500);
                    }
                    else
                    {
                        break;
                    }
                }

                if (1 == Index)
                {
                    SetupDiEnumDeviceInterfaces(hInfoSet, 0, ref GUID, 0, ref ifData);
                    SetupDiGetDeviceInterfaceDetail(hInfoSet, ref ifData, IntPtr.Zero, 0, ref Size, IntPtr.Zero);
                    //					ifDetail.Size = (int)Size;
                    if (IntPtr.Size == 8)   // If we are compiled as 64bit
                    {
                        ifDetail.Size = 8;
                    }
                    else if (IntPtr.Size == 4) // If we are compiled as 32 bit
                    {
                        ifDetail.Size = 5;
                    }
                    else
                    {
                        throw new Exception("Operating system is neither 32 nor 64 bits!");
                    }
                    if (Marshal.SizeOf(ifDetail) < Size)
                    {
                        throw new Exception("ifDetail too small");
                    }
                    if (true == SetupDiGetDeviceInterfaceDetail(hInfoSet, ref ifData, ref ifDetail, Size, ref Size, IntPtr.Zero))
                    {
                        DFU_DevicePath = ifDetail.DevicePath.ToUpper();
                        if (STDFU_NOERROR == STDFU_Open(DFU_DevicePath, out hDevice))
                        {
                            //if (OnFirmwareUpdateProgress != null) OnFirmwareUpdateProgress(this, new FirmwareUpdateProgressEventArgs(10, "DFU device opened OK", false));                            
                            USB_DeviceDescriptor Descriptor = new USB_DeviceDescriptor();
                            if (STDFU_NOERROR == STDFU_GetDeviceDescriptor(ref hDevice, ref Descriptor))
                            {
                                switch (Descriptor.bcdDevice)
                                {
                                    case 0x011A:
                                    case 0x0200:
                                        MaxWriteBlockSize = 1024;
                                        break;
                                    case 0x02100:
                                        MaxWriteBlockSize = 2048;
                                        break;
                                    default:
                                        throw new Exception("Unsupported bootloader version=" + Descriptor.bcdDevice.ToString("X4"));                                        
                                }
                                
                                UInt32 Dummy2 = 0;
                                DFU_FunctionalDescriptor CurrentDeviceDescriptor = new DFU_FunctionalDescriptor();
                                if (STDFU_NOERROR == STDFU_GetDFUDescriptor(ref hDevice, ref interfaceNumber, ref Dummy2, ref CurrentDeviceDescriptor))
                                {
                                    //if (OnFirmwareUpdateProgress != null) OnFirmwareUpdateProgress(this, new FirmwareUpdateProgressEventArgs(14, "Got DFU Descriptor " + CurrentDeviceDescriptor.bcdDFUVersion.ToString(), false));
                                    Sectors = CreateMappingFromDevice(hDevice);
                                }
                                else
                                {
                                    throw new Exception("STDFU_GetDFUDescriptor failed, error code=" + Marshal.GetLastWin32Error().ToString());
                                }
                            }
                            else
                            {
                                throw new Exception("STDFU_GetDeviceDescriptor failed, error code=" + Marshal.GetLastWin32Error().ToString());
                            }
                        }
                        else
                        {
                            throw new Exception("STDFU_Open failed, error code=" + Marshal.GetLastWin32Error().ToString());
                        }
                    }
                }
                else
                {
                    throw new Exception("There must be exactly one DFU device attached to the computer");
                }
            }
            catch (Exception ex)
            {
                throw new Exception(ex.Message);
                //				del(100, ex.Message, true);
            }
            finally
            {
                // Before we go, we have to free up the InfoSet memory reserved by SetupDiGetClassDevs
                SetupDiDestroyDeviceInfoList(hInfoSet);
            }
            return (hDevice);


        }

        public void UpgradeSoftware(string[] data)
        {
            Console.WriteLine("Starting Upgrade:");

            //Get Data (Assumed Intel HEX Format)
            var parsedData = ParseIntelHex(data, out int totalBytes);


            //Open Device           
            List<MappingSector> mappingSectors;
            ushort maxBlockSize;
            IntPtr device;

            int dfuConnectionAttempts = 10;
            while (true)
            {
                try
                {
                    device = OpenDFU_Device(out mappingSectors, out maxBlockSize, out uint interfaceNumber);
                    break;
                }
                catch (Exception)
                {
                    dfuConnectionAttempts++;
                    if (dfuConnectionAttempts > 10)
                        throw new System.TimeoutException("Unable to open the DFU Connection in time");

                    //Wait a bit and look again for connection
                    System.Threading.Thread.Sleep(1500);                    
                }
            }

            //Erase all sectors                        
            float eraseOnePercent = mappingSectors.Count() / 100.0f;
            int erasePercentage = 0;
            float eraseNextPercent = eraseOnePercent;
            for (int i = 0; i < mappingSectors.Count(); i++)
            {                
                var sector = mappingSectors[i];                
                EraseSector(device, sector.dwStartAddress);
                

                if (eraseNextPercent < i)
                {
                    erasePercentage++;
                    eraseNextPercent = (erasePercentage + 1) * eraseOnePercent;


                    Console.Write($"\rErasing:\t{erasePercentage.ToString().PadLeft(3, '0')}%");
                    OnProgress?.Invoke(this, new DFU_Progress(DFU_ProgressType.Erasing, mappingSectors.Count(), erasePercentage));
                }
            }
            Console.Write("\rErasing:\t100%");
            Console.WriteLine();
                       
            //Write Data            
            int writeOnePercent = totalBytes / 100;
            int writePercentage = 0;
            int writeNextPercentage = writeOnePercent;
            int totalBytesSent = 0;                   
            foreach (var dfuArea in parsedData)
            {
                for (int i = 0; i <= dfuArea.Data.Count / maxBlockSize; i++)
                {
                    byte[] block = dfuArea.Data.Skip((int)(MaxWriteBlockSize * i)).Take((int)MaxWriteBlockSize).ToArray();
                    totalBytesSent += block.Count();

                    if (block.Length < MaxWriteBlockSize)
                    {
                        int blockI = block.Length;                        
                        Array.Resize(ref block, (int)MaxWriteBlockSize);
                        // Pad with 0xFF so our CRC matches the ST Bootloader and the ULink's CRC
                        for (; blockI < MaxWriteBlockSize; blockI++)
                        {
                            block[blockI] = 0xFF;
                        }
                    }
                    

                    WriteBlock(device, (uint)(dfuArea.StartAddress + (i * maxBlockSize)), block, (uint)0);


                    //Report Progress
                    if (writeNextPercentage < totalBytesSent)
                    {
                        writePercentage++;                                                
                        writeNextPercentage = (writePercentage + 1) * writeOnePercent;

                        Console.Write($"\rWriting:\t{writePercentage.ToString().PadLeft(3, '0')}%");
                        OnProgress?.Invoke(this, new DFU_Progress(DFU_ProgressType.Writing, 100, writePercentage));
                    }
                }                                
            }
            Console.Write("\rWriting:\t100%");
            Console.WriteLine();
            

            //Leave DFU mode
            Console.WriteLine("Leaving DFU Mode");
            LeaveDFUMode(device);


            Console.WriteLine("Finished Upgrade!");
        }

        private DFU_Areas[] ParseIntelHex(string[] dataHex, out int totalBytes)
        {            
            var extendedAddress = new byte[2];
            var onePercent = dataHex.Length / 100;
            var nextPercent = onePercent;
            int percentage = 0;

            var dfuAreas = new List<DFU_Areas>();
            

            totalBytes = 0;
            //Loop lines figuring out where they belong                
            for (int i = 0; i < dataHex.Length; i++)
            {
                var line = dataHex[i];
                line = line.Replace("\r", "");

                if (string.IsNullOrWhiteSpace(line))
                    continue;

                var record = IntelHexFormatReader.HexFileLineParser.ParseLine(line);

                //If is extended address, keep in memory as extended address
                if (record.RecordType == IntelHexFormatReader.Model.RecordType.ExtendedLinearAddress)
                {
                    extendedAddress[0] = record.Bytes[0];
                    extendedAddress[1] = record.Bytes[1];
                    
                }
                else if (record.RecordType == IntelHexFormatReader.Model.RecordType.Data)
                {
                    var addressBytes = BitConverter.GetBytes(record.Address);
                    var entireAddressBytes = new byte[4]
                    {
                    addressBytes[0],
                    addressBytes[1],
                    extendedAddress[1],
                    extendedAddress[0],
                    };

                    //If data has skipped
                    int entireAddress = BitConverter.ToInt32(entireAddressBytes, 0);
                    if (dfuAreas.Count() == 0 || dfuAreas.Last().StartAddress != entireAddress - dfuAreas.Last().Data.Count)                    
                        dfuAreas.Add(new DFU_Areas(entireAddress));
                    


                    dfuAreas.Last().Data.AddRange(record.Bytes);


                    totalBytes += record.ByteCount;
                }
                

                if (i > nextPercent)
                {
                    percentage++;
                    nextPercent = (percentage + 1) * onePercent;

                    Console.Write($"\rParsing:\t{percentage.ToString().PadLeft(3, '0')}%");
                    OnProgress?.Invoke(this, new DFU_Progress(DFU_ProgressType.Parsing, 100, percentage));                    
                }
            }

            Console.Write($"\rParsing:\t100%");
            Console.WriteLine();



            return dfuAreas.ToArray();

        }






        /// <summary>
		/// Create a list of the programmable sectors within the micro-controller, based on the string(s) which we read from the device
		/// </summary>
		/// <param name="hDevice">Handle to the USB connection to the micro-controller</param>
		/// <returns>List of sector classes</returns>
		private List<MappingSector> CreateMappingFromDevice(IntPtr hDevice)
        {
            List<MappingSector> Sectors = new List<MappingSector>();
            UInt32 Result = 0;
            UInt32 InterfaceIndex = 0;
            UInt32 NumberOfAlternates = 0;
            DFU_FunctionalDescriptor dfuDescriptor = new DFU_FunctionalDescriptor();
            USB_InterfaceDescriptor usbDescriptor = new USB_InterfaceDescriptor();
            uint i = 0;
            IntPtr StringBuffer = Marshal.AllocHGlobal(512);
            String MapDesc;
        
            if (STDFU_NOERROR == (Result = STDFU_GetDFUDescriptor(ref hDevice, ref InterfaceIndex, ref NumberOfAlternates, ref dfuDescriptor)))
            {
                // Loop thru Internal FLASH, Option bytes, OTP and Device Feature
                for (i = 0; i < NumberOfAlternates; i++)
                {
                    if (STDFU_NOERROR == (Result = STDFU_GetInterfaceDescriptor(ref hDevice, 0, InterfaceIndex, i, ref usbDescriptor)))
                    {
                        if (0 == usbDescriptor.iInterface)
                        {
                            throw new Exception("STDFU_GetInterfaceDescriptor bad value in iInterface");
                        }
                        if (STDFU_NOERROR == (Result = STDFU_GetStringDescriptor(ref hDevice, usbDescriptor.iInterface, StringBuffer, 512)))
                        {
                            //	ByteArray = Marshal.ReadByte(
                            UInt32 StartAddress;
                            UInt16 NumberOfSectors = 0;
                            UInt32 SectorSize = 0;
                            UInt16 j = 0;
                            MappingSector.SectorType SType = MappingSector.SectorType.Other;
                            String SectorName;
                            String SectorDescription;
                            String SectorN;

                            MapDesc = Marshal.PtrToStringAnsi(StringBuffer);
                            if ('@' != MapDesc[0])
                            {
                                throw new Exception("STDFU_GetStringDescriptor bad value in MapDesc, i=" + i.ToString());
                            }
                            SectorName = MapDesc.Substring(1, MapDesc.IndexOf('/') - 1);
                            SectorName = SectorName.TrimEnd(' ');
                            if (SectorName.Equals("Internal Flash"))
                            {
                                SType = MappingSector.SectorType.InternalFLASH;
                            }
                            else if (SectorName.Equals("Option Bytes"))
                            {
                                SType = MappingSector.SectorType.OptionBytes;
                            }
                            else if (SectorName.Equals("OTP Memory"))
                            {
                                SType = MappingSector.SectorType.OTP;
                            }
                            else if (SectorName.Equals("Device Feature"))
                            {
                                SType = MappingSector.SectorType.DeviceFeature;
                            }
                            else
                            {
                                SType = MappingSector.SectorType.Other;
                            }
                            StartAddress = UInt32.Parse(MapDesc.Substring(MapDesc.IndexOf('/') + 3, 8), System.Globalization.NumberStyles.HexNumber);
                            SectorDescription = MapDesc;
                            while (SectorDescription.IndexOf('*') >= 0)
                            {
                                
                                int startOfSector = SectorDescription.IndexOf('*') - 3;
                                SectorN = SectorDescription.Substring(startOfSector, 3);
                                if (char.IsDigit(SectorN[0]))
                                {
                                    NumberOfSectors = UInt16.Parse(SectorN);
                                }
                                else
                                {
                                    NumberOfSectors = UInt16.Parse(SectorN.Substring(1));
                                }

                                //Check for new Start Address
                                int startOfStartAddress = SectorDescription.IndexOf('/');
                                if (startOfStartAddress > 0 && startOfStartAddress < startOfSector)
                                {
                                    var startAddressString = SectorDescription.Substring(SectorDescription.IndexOf('/') + 3, 8);
                                    StartAddress = UInt32.Parse(startAddressString, System.Globalization.NumberStyles.HexNumber);
                                }


                                var SectorS = SectorDescription.Substring(SectorDescription.IndexOf('*') + 1, 3);
                                SectorS = new string(SectorS.Where(a => char.IsDigit(a)).ToArray());
                                SectorSize = UInt16.Parse(SectorS);
                                if ('k' == char.ToLower(SectorDescription[SectorDescription.IndexOf('*') + 4]))
                                {
                                    SectorSize *= 1024;
                                }
                                for (j = 0; j < NumberOfSectors; j++)
                                {
                                    Sectors.Add(new MappingSector(SectorName, SType, StartAddress, SectorSize, j));
                                    StartAddress += SectorSize;
                                }
                                SectorDescription = SectorDescription.Substring(SectorDescription.IndexOf('*') + 1);
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                    else
                    {
                        throw new Exception("STDFU_GetInterfaceDescriptor returned " + Result.ToString());
                    }
                }
            }
            else
            {
                throw new Exception("STDFU_GetDFUDescriptor returned " + Result.ToString());
            }

            return (Sectors);
        }

        /// <summary>
        /// Write a block of data to the micro-controller
        /// </summary>
        /// <param name="hDevice">Handle to the USB connection to the micro-controller</param>
        /// <param name="Address">Address to write the data to</param>
        /// <param name="Data">Data for write</param>
        /// <param name="BlockNumber">Data block number</param>
        public void WriteBlock(IntPtr hDevice, UInt32 Address, byte[] Data, UInt32 BlockNumber)
        {
            byte[] Command = new byte[5];
            DFU_Status dfuStatus = new DFU_Status();

            if (Data.Length > MaxWriteBlockSize)
            {
                throw new Exception("Block size too big (" + Data.Length.ToString() + ")");
            }

            if (0 == BlockNumber)
            {
                SetAddressPointer(hDevice, Address);
            }

            STDFU_GetStatus(ref hDevice, ref dfuStatus);
            while (dfuStatus.bState != STATE_DFU_IDLE)
            {
                STDFU_ClrStatus(ref hDevice);
                STDFU_GetStatus(ref hDevice, ref dfuStatus);
            }

            STDFU_Dnload(ref hDevice, Data, (UInt32)Data.Length, (UInt16)(BlockNumber + 2));

            STDFU_GetStatus(ref hDevice, ref dfuStatus);
            while (dfuStatus.bState != STATE_DFU_IDLE)
            {
                STDFU_ClrStatus(ref hDevice);
                STDFU_GetStatus(ref hDevice, ref dfuStatus);
            }
        }


        /// <summary>
        /// Erase a sector
        /// </summary>
        /// <param name="hDevice">Device handle</param>
        /// <param name="Address">Start address of sector for erase</param>
        public void EraseSector(IntPtr hDevice, UInt32 Address)
        {
            DFU_Status dfuStatus = new DFU_Status();
            UInt32 Result = 0;
            byte[] Command = { 0x41, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

            Command[1] = (byte)(Address & 0xFF);
            Command[2] = (byte)((Address >> 8) & 0xFF);
            Command[3] = (byte)((Address >> 16) & 0xFF);
            Command[4] = (byte)((Address >> 24) & 0xFF);

            if (STDFU_NOERROR == (Result = STDFU_SelectCurrentConfiguration(ref hDevice, 0, 0, 0)))
            {
                STDFU_GetStatus(ref hDevice, ref dfuStatus);
                while (dfuStatus.bState != STATE_DFU_IDLE)
                {
                    STDFU_ClrStatus(ref hDevice);
                    STDFU_GetStatus(ref hDevice, ref dfuStatus);
                }
                if (STDFU_NOERROR == (Result = STDFU_Dnload(ref hDevice, Command, 5, 0)))
                {
                    STDFU_GetStatus(ref hDevice, ref dfuStatus);
                    while (dfuStatus.bState != STATE_DFU_IDLE)
                    {
                        STDFU_ClrStatus(ref hDevice);
                        STDFU_GetStatus(ref hDevice, ref dfuStatus);
                    }
                }
                else
                {
                    throw new Exception("STDFU_Dnload returned " + Result.ToString("X8"));
                }
            }
            else
            {
                throw new Exception("STDFU_SelectCurrentConfiguration returned " + Result.ToString("X8"));
            }
        }


        /// <summary>
        /// Set the address pointer used in the DFU module within the micro-controller
        /// </summary>
        /// <param name="hDevice">Handle to the USB connection to the micro-controller</param>
        /// <param name="Address">Value to set address pointer to</param>
        private void SetAddressPointer(IntPtr hDevice, UInt32 Address)
        {
            byte[] Command = new byte[5];
            DFU_Status dfuStatus = new DFU_Status();

            STDFU_GetStatus(ref hDevice, ref dfuStatus);
            while (dfuStatus.bState != STATE_DFU_IDLE)
            {
                STDFU_ClrStatus(ref hDevice);
                STDFU_GetStatus(ref hDevice, ref dfuStatus);
            }
            Command[0] = 0x21;
            Command[1] = (byte)(Address & 0xFF);
            Command[2] = (byte)((Address >> 8) & 0xFF);
            Command[3] = (byte)((Address >> 16) & 0xFF);
            Command[4] = (byte)((Address >> 24) & 0xFF);

            STDFU_Dnload(ref hDevice, Command, 5, 0);

            STDFU_GetStatus(ref hDevice, ref dfuStatus);
            while (dfuStatus.bState != STATE_DFU_IDLE)
            {
                STDFU_ClrStatus(ref hDevice);
                STDFU_GetStatus(ref hDevice, ref dfuStatus);
            }
        }

        private void LeaveDFUMode(IntPtr hDevice)
        {
            var dfuStatus = new DFU_Status();

            STDFU_Dnload(ref hDevice, new byte[0], 0, 0);
            STDFU_GetStatus(ref hDevice, ref dfuStatus);
            STDFU_ClrStatus(ref hDevice);
            STDFU_GetStatus(ref hDevice, ref dfuStatus);
        }


        /// <summary>
        /// Open the DFU driver, get the handle
        /// </summary>
        /// <param name="szDevicePath">Device path string</param>
        /// <param name="hDevice">Device handle, populated by this call</param>
        /// <returns>STDFU_NOERROR if successful, else error codes</returns>
        [DllImport("STDFU.dll", EntryPoint = "STDFU_Open", CharSet = CharSet.Ansi)]
        private static extern UInt32 STDFU_Open([MarshalAs(UnmanagedType.LPStr)]String szDevicePath, out IntPtr hDevice);

        [DllImport("STDFU.dll", EntryPoint = "STDFU_SelectCurrentConfiguration", CharSet = CharSet.Ansi)]
        private static extern UInt32 STDFU_SelectCurrentConfiguration(ref IntPtr hDevice, UInt32 ConfigIndex, UInt32 InterfaceIndex, UInt32 AlternateSetIndex);

        [DllImport("STDFU.dll", EntryPoint = "STDFU_GetDeviceDescriptor", CharSet = CharSet.Auto)]
        private static extern UInt32 STDFU_GetDeviceDescriptor(ref IntPtr handle, ref USB_DeviceDescriptor descriptor);

        [DllImport("STDFU.dll", EntryPoint = "STDFU_GetDFUDescriptor", CharSet = CharSet.Auto)]
        private static extern UInt32 STDFU_GetDFUDescriptor(ref IntPtr handle, ref uint DFUInterfaceNum, ref uint NBOfAlternates, ref DFU_FunctionalDescriptor dfuDescriptor);

        [DllImport("STDFU.dll", EntryPoint = "STDFU_GetInterfaceDescriptor", CharSet = CharSet.Auto)]
        private static extern UInt32 STDFU_GetInterfaceDescriptor(ref IntPtr handle, UInt32 ConfigIndex, UInt32 InterfaceIndex, UInt32 AlternateIndex, ref USB_InterfaceDescriptor usbDescriptor);

        [DllImport("STDFU.dll", EntryPoint = "STDFU_Detach", CharSet = CharSet.Auto)]
        private static extern UInt32 STDFU_Detach(ref IntPtr phDevice, byte DFUInterfaceNumber);

        /// <summary>
        /// Get a string descriptor from the DFU driver
        /// </summary>
        /// <param name="handle">Handle to DFU device</param>
        /// <param name="Index">Index of desired string, if this is not valid the function will return an error</param>
        /// <param name="StringBuffer">Buffer for the string to be copied to</param>
        /// <param name="BufferSize">Size of buffer</param>
        /// <returns>STDEVICE_NOERROR or error code</returns>
        [DllImport("STDFU.dll", EntryPoint = "STDFU_GetStringDescriptor", CharSet = CharSet.Auto)]
        private static extern UInt32 STDFU_GetStringDescriptor(ref IntPtr handle, UInt32 Index, IntPtr StringBuffer, UInt32 BufferSize);

        [DllImport("STDFU.dll", EntryPoint = "STDFU_Dnload", CharSet = CharSet.Ansi)]
        private static extern UInt32 STDFU_Dnload(ref IntPtr hDevice, [MarshalAs(UnmanagedType.LPArray)]byte[] pBuffer, UInt32 nBytes, UInt16 nBlocks);

        [DllImport("STDFU.dll", EntryPoint = "STDFU_Getstatus", CharSet = CharSet.Ansi)]
        private static extern UInt32 STDFU_GetStatus(ref IntPtr hDevice, ref DFU_Status dfuStatus);

        [DllImport("STDFU.dll", EntryPoint = "STDFU_Clrstatus", CharSet = CharSet.Ansi)]
        private static extern UInt32 STDFU_ClrStatus(ref IntPtr hDevice);


        //[DllImport("STDFUFiles.dll", EntryPoint = "STDFUFILES_OpenExistingDFUFile", CharSet = CharSet.Ansi)]
        //public static extern UInt32 STDFUFILES_OpenExistingDFUFile(byte[] pPathFile, ref IntPtr phFile, ushort pVid, ushort pPid, ushort pBcd, ref byte pNbImages);
        //[DllImport("STDFUFiles.dll", EntryPoint = "STDFUFILES_CloseDFUFile", CharSet = CharSet.Ansi)]
        //public static extern UInt32 STDFUFILES_CloseDFUFile(IntPtr hFile);
    }
}
