

using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Management;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using FosterAndFreeman.Internal;
using FosterAndFreeman.Internal.Misc;


namespace FosterAndFreeman
{

    public enum PortType
    {
        BL_CRC,
        BL_DFU,
        APP_CRC
    }

    public class RecoverControl : ConnectionBase
    {
        public Bootloader Bootloader;
        public Application Application;



        /// <summary>
        /// Initialises the device
        /// </summary>
        public void Init()
        {
            Bootloader = new Bootloader(this);
            Application = new Application(this);


            //Define ports
            _bootloaderPort = new SerialPort()
            {                
                BaudRate = 115200,
                Parity = Parity.None,
                DataBits = 8,
                StopBits = StopBits.One
            };

            _applicationPort = new SerialPort()
            {
                BaudRate = 9600,
                Parity = Parity.None,
                DataBits = 8,
                StopBits = StopBits.One
            };
        }


        /// <summary>
        /// Find port name with serial number specified
        /// </summary>
        /// <returns></returns>
        public string FindPortName(PortType portType)
        {
            if (portType == PortType.APP_CRC || portType == PortType.BL_CRC)
            {
                string serial = portType == PortType.BL_CRC ? BL_CRC_SERIAL : APP_CRC_SERIAL;


                using (var searcher = new ManagementObjectSearcher("SELECT * FROM Win32_PnPEntity WHERE Caption like '%(COM%'"))
                {
                    var portnames = SerialPort.GetPortNames();
                    var portsAll = searcher.Get().Cast<ManagementBaseObject>().ToList();
                    foreach (var port in portsAll)
                    {
                        if (!port["PNPDeviceID"].ToString().Contains(serial))
                            continue;


                        var name = port["Name"].ToString();

                        //Find "COM#" from name
                        name = Regex.Match(name, @"COM\d*").Value;



                        return name;
                    }


                    return null;
                }
            }
            else
            {
                //Figure out finding DFU Connection
                return null;
            }
        }


    }
}
