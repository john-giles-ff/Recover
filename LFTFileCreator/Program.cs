using FosterAndFreeman.RecoverCompanionApplication.Definitions.DeviceData;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace LFTFileCreator
{
    class Program
    {
        static string SoftwareBuildFileName => @"F:\Users\Alex\Documents\Projects\Recover\LFT\gui\include\gui\common\Build.hpp";

        static void Main(string[] args)
        {
            var recoverFile = new RecoverFile();

            //Add Firmware Data
            Console.WriteLine("Set Firmware File (Leave blank for none):");
            var firmwarePath = Console.ReadLine();
            firmwarePath = firmwarePath.Replace("\"", "");
            if (!string.IsNullOrWhiteSpace(firmwarePath) && System.IO.File.Exists(firmwarePath))
            {
                var firmwareData = System.IO.File.ReadAllBytes(firmwarePath);
                recoverFile.AddFirmware(firmwareData, firmwareData.Length);
            }
            else
                Console.WriteLine("Firmware Skipped");

            //Add Software Data
            Console.WriteLine("Set Software File (Leave blank for none):");
            var softwarePath = Console.ReadLine();
            softwarePath = softwarePath.Replace("\"", "");
            if (!string.IsNullOrWhiteSpace(softwarePath) || System.IO.File.Exists(softwarePath))
            {
                var softwareData = System.IO.File.ReadAllBytes(softwarePath);
                recoverFile.AddSoftware(softwareData, softwareData.Length);
            }
            else
                Console.WriteLine("Software Skipped");


            //Write to location
            Console.WriteLine("Enter Save Location (Leave blank for on the desktop)");
            var savePath = Console.ReadLine();
            if (string.IsNullOrWhiteSpace(savePath))
            {
                //Figure out name                
                var firmwareBuild = Regex.Match(System.IO.Path.GetFileName(firmwarePath), @"\d{2,}").Value;
                var softwareBuild = Regex.Match(System.IO.File.ReadAllText(SoftwareBuildFileName), @"\d{2,}").Value;
                var fileName = $"S{softwareBuild}-F{firmwareBuild}.LFT";

                savePath = System.IO.Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.Desktop), fileName);
            }
            else
                savePath = System.IO.Path.ChangeExtension(savePath, ".LFT");

            recoverFile.GenerateFile(savePath);



            //Don't just close, show message and wait for newline
            Console.WriteLine("Finished!");
            Console.ReadLine();
            




        }
    }
}
