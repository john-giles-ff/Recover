using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AutoFlash
{
    class Program
    {
        static void Main(string[] args)
        {
            //Find Executing Folder
            var executingDirectory = System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
            var directories = System.IO.Directory.GetDirectories(executingDirectory);

            //Check for ST Files
            var requiredFiles = new string[]
            {
                System.IO.Path.Combine(executingDirectory, "ST", "ST-LINK_CLI.exe"), //This must be the first file!                
                System.IO.Path.Combine(executingDirectory, "ST", "STLinkUSBDriver.dll"),                
                System.IO.Path.Combine(executingDirectory, "ST", "FlashLoader", "0x449.stldr"),
                System.IO.Path.Combine(executingDirectory, "ST", "N25Q128A_STM32F746G-EDT7_C.stldr"), //This must be the last file
            };
            var missingFiles = requiredFiles.Where(a => !System.IO.File.Exists(a));
            if (missingFiles.Any())
                Exit("Required files were missing from the PC!");



            //Find RECOVER folder
            var recoverDirectory = directories.FirstOrDefault(a => System.IO.Path.GetFileName(a).StartsWith("RECOVER_"));
            if (recoverDirectory == null)
                Exit("Unable to find RECOVER Directory");


            //Find Bootloader
            var bootloaderDirectory = System.IO.Path.Combine(recoverDirectory, "BOOTLOADER");
            if (!System.IO.Directory.Exists(bootloaderDirectory))
                Exit("Unable to find bootloader directory!");
            var bootloaderPath = System.IO.Directory.GetFiles(bootloaderDirectory).FirstOrDefault();
            if (bootloaderPath == null)
                Exit("Unable to find bootloader file!");

            //Find Application
            var applicationDirectory = System.IO.Path.Combine(recoverDirectory, "PARTS");
            if (!System.IO.Directory.Exists(applicationDirectory))
                Exit("Unable to find parts directory");
            var applicationPath = System.IO.Directory.GetFiles(applicationDirectory).FirstOrDefault(a => System.IO.Path.GetFileName(a).StartsWith("IMAGE_S"));
            if (applicationPath == null)
                Exit("Unable to find application file!");


            //Confirm with user!            
            while (true)
            {
                Console.WriteLine("Are you sure you want to flash the following files?");
                Console.WriteLine();
                Console.WriteLine($"\tBootloader:\t{bootloaderPath}");
                Console.WriteLine($"\tApplication:\t{applicationPath}");
                Console.WriteLine();
                Console.WriteLine("Y/N");
                var response = Console.ReadLine();

                if (response.ToUpper() == "Y")
                    break;

                if (response.ToUpper() == "N")
                    Exit("User indicated incorrect files found");

            }

            //Define process
            var stApp = requiredFiles.First();
            var p = new System.Diagnostics.Process()
            {
                StartInfo = new System.Diagnostics.ProcessStartInfo()
                {
                    FileName = stApp,
                    UseShellExecute = false
                }
            };            


            //Flash Bootloader            
            Console.WriteLine("Flashing Bootloader...");
            p.StartInfo.Arguments = $"-P \"{bootloaderPath}\" -Rst";
            p.Start();
            p.WaitForExit();
            if (p.ExitCode != 0)
            {
                Console.ForegroundColor = ConsoleColor.Red;
                Console.WriteLine("Error code returned not 0! Be aware!");
                Console.ForegroundColor = ConsoleColor.White;
            }
            else
            {
                Console.WriteLine("Done");
            }
            Console.WriteLine();


            //Flash Application
            p.StartInfo.Arguments = $"-P \"{applicationPath}\" -EL \"{requiredFiles.Last()}\" -Rst";
            p.Start();
            p.WaitForExit();
            if (p.ExitCode != 0)
            {
                Console.ForegroundColor = ConsoleColor.Red;
                Console.WriteLine("Error code returned not 0! Be aware!");
                Console.ForegroundColor = ConsoleColor.White;
            }
            else
            {
                Console.WriteLine("Done");
            }
            Console.WriteLine();


            Exit("Finished");

        }

        private static void Exit(string reason)
        {
            Console.WriteLine(reason);
            Console.ReadLine();
            Environment.Exit(0);
        }
    }
}
