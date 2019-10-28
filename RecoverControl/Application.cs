using FosterAndFreeman.Internal.Misc;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FosterAndFreeman.Internal
{
    public class Application : ConnectionBase
    {
        private RecoverControl _recoverControl;

        const int ESCAPE_SEQUENCE_COUNT = 32;
        private readonly byte[] EscapeSequence = new byte[ESCAPE_SEQUENCE_COUNT]
        {
            0xEF, 0x86, 0x5F, 0x11, 0x10, 0x74, 0x55, 0xDD, 0x9E, 0x8D, 0x60, 0x6E, 0x07, 0x17, 0xC5, 0x6A, 0x5D, 0x62, 0x05, 0x40, 0xDD, 0xCD, 0xCD, 0xE7, 0x09, 0xA9, 0xD2, 0x56, 0xDA, 0xE6, 0x8B, 0x71
        };



        internal Application(RecoverControl recoverControl)
        {
            _recoverControl = recoverControl;
        }

        public void OpenComPort(string portName)
        {
            //Open port specified
            _applicationPort.PortName = portName;
            _applicationPort.Open();

            //No command should take longer than 250 ms
            _applicationPort.ReadTimeout = 10000;
        }

        public void Close()
        {
            _applicationPort.Close();
        }
        
        public void Test()
        {
            _applicationPort.Write(new byte[] { (byte)APP_CMD.TEST }, 0, 1);
        }

        public void EnterPCMode()
        {
            _applicationPort.Write(new byte[] { (byte)APP_CMD.ENTER_PC_MODE }, 0, 1);

            //Wait for system to respond (2 second)
            System.Threading.Thread.Sleep(2000);

            //Ask if PC mode is enabled            
            var ret = new byte[1];
            do
            {                
                try
                {
                    _applicationPort.Write(new byte[] { (byte)APP_CMD.PRINT_PC_MODE }, 0, 1);
                    _applicationPort.Read(ret, 0, 1);
                }
                catch (TimeoutException) { throw new Exception("Recover is not ready to enter PC mode"); };
                                    
            } while (ret[0] != 2);
            
        }

        public void ExitPCMode()
        {
            _applicationPort.Write(new byte[] { (byte)APP_CMD.EXIT_PC_MODE }, 0, 1);
        }

        public void GotoBootloader()
        {
            _applicationPort.Write(new byte[] { (byte)APP_CMD.GOTO_BOOTLOADER }, 0, 1);
        }

        public void TransparentMode()
        {
            _applicationPort.Write(new byte[] { (byte)APP_CMD.TRANSPARENT_MODE }, 0, 1);
        }

        public void ExitTransparentMode()
        {
            _applicationPort.Write(EscapeSequence, 0, ESCAPE_SEQUENCE_COUNT);


            byte[] bytes;
            do
            {
                bytes = Read(1);
            }
            while (bytes[0] != '\n');

        }

        public void SendCustom(byte[] data)
        {
            _applicationPort.Write(data, 0, data.Length);
        }

        public byte[] Read (int length)
        {
            var buffer = new byte[256];

            _applicationPort.Read(buffer, 0, length);

            return buffer.Take(length).ToArray();
        }


    }
}
