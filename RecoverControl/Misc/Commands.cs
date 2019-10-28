using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FosterAndFreeman.Internal.Misc
{
    /// <summary>
    /// Bootloader Commands   
    /// </summary>
    internal enum BL_CMD
    {
        BL_VERSION = 0x00,
        DFU_MODE = 0x01,
        BOOT_APP = 0x03,
        DISABLE_AUTO_BOOT = 0x04
    };

    /// <summary>
    /// Application Commands
    /// </summary>
    internal enum APP_CMD
    {
        TEST = 0x00,
        ENTER_PC_MODE = 0x01,
        EXIT_PC_MODE = 0x02,
        TRANSPARENT_MODE = 0x03,
        GOTO_BOOTLOADER = 0x04,
        PRINT_PC_MODE = 0x05
    }
}
