using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using FosterAndFreeman.Internal.Misc;

namespace FosterAndFreeman.Internal
{
    public class Bootloader : ConnectionBase
    {
        public Dfu.DFU_Manager DfuManager;

        private RecoverControl _recoverControl;

        internal Bootloader(RecoverControl recoverControl)
        {
            _recoverControl = recoverControl;
            DfuManager = new Dfu.DFU_Manager();
        }

        /// <summary>
        /// Opens the bootloader port with the specified name
        /// </summary>
        /// <param name="portName">Port to open</param>
        public void OpenComPort(string portName)
        {
            //Open port specified
            _bootloaderPort.PortName = portName;
            _bootloaderPort.Open();
        }

        /// <summary>
        /// Will look for port until it finds one, then open it and ensure the bootloader doesn't timeout
        /// </summary>
        public void CatchStartup(int timeoutMs = -1)
        {
            //Look for port name until one is found            
            string portName = null;
            Console.WriteLine("Waiting for connection");

            DateTime startTime = DateTime.Now;

            do
            {
                portName = _recoverControl.FindPortName(PortType.BL_CRC);
                if (portName != null)
                    break;

                if (timeoutMs != -1)
                    if ((DateTime.Now - startTime).TotalMilliseconds > timeoutMs)
                        throw new System.TimeoutException();

            }
            while (true);
            
            //Open Port
            Console.WriteLine("Opening Port");
            OpenComPort(portName);

            //Open port and stop timeout
            Console.WriteLine("Stopping Timeout");
            StopTimeout();
        }

        public byte GetVersion()
        {
            _bootloaderPort.Write(new byte[] { (byte)BL_CMD.BL_VERSION }, 0, 1);

            var output = new byte[1];
            _bootloaderPort.Read(output, 0, 1);

            return output[0];
        }

        /// <summary>
        /// Switch to DFU mode
        /// </summary>
        public void SwitchToDFU()
        {
            _bootloaderPort.Write(new byte[] { (byte)BL_CMD.DFU_MODE }, 0, 1);
        }

        /// <summary>
        /// Stop Bootloader automatically timing out and advancing to Application
        /// </summary>
        public void StopTimeout()
        {
            _bootloaderPort.Write(new byte[] { (byte)BL_CMD.DISABLE_AUTO_BOOT }, 0, 1);
        }


        /// <summary>
        /// Exit the bootloader and start the user application
        /// </summary>
        public void BootApplication()
        {
            _bootloaderPort.Write(new byte[] { (byte)BL_CMD.BOOT_APP }, 0, 1);
        }


        public void SendCustom(byte[] data)
        {
            _applicationPort.Write(data, 0, data.Length);
        }

    }
}
