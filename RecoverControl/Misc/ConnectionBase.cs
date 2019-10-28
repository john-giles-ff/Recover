using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FosterAndFreeman.Internal.Misc
{
    public class ConnectionBase
    {
        protected static SerialPort _bootloaderPort;
        protected static SerialPort _applicationPort;


        protected const string BL_CRC_SERIAL    = "0000BL1236-CRC";
        protected const string BL_DFU_SERIAL    = "0000BL1236-DFU";
        protected const string APP_CRC_SERIAL   = "000APP1236-CRC";
    }
}
