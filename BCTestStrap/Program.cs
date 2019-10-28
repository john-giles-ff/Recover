
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BCTestStrap
{
    class Program
    {
        static void Main(string[] args)
        {
            string result = string.Empty;
            var recoverController = new FosterAndFreeman.RecoverControl();            

            recoverController.Init();

            if (args.Count() > 0)
            {
                for(int i = 0; i < args.Count(); i++)
                {
                    if (args[i].ToUpper() == "-FLASH")
                    {
                        i++;
                        if (args[i].ToUpper() == "-SOFTWARE")
                        {


                        }
                        else if (args[i].ToUpper() == "-FIRMWARE")
                        {



                        }
                        

                        
                    }




                }


                Exit(recoverController, null);
            }






            while (true)
            {
                Console.WriteLine("Open as:");
                Console.WriteLine("\t0) Bootloader");
                Console.WriteLine("\t1) Application");
                Console.WriteLine("\t2) Exit");
                var response = ReadOption(new int[] { 0, 1, 2 });

                switch (response)
                {
                    case 0:
                        OpenAsBootloader(recoverController, true);
                        break;
                    case 1:
                        OpenAsApplication(recoverController);
                        break;
                    case 2:
                        Exit(recoverController, null);
                        break;
                }
                                
            }
        }


        static void OpenAuto(FosterAndFreeman.RecoverControl recoverController)
        {
            var appName = recoverController.FindPortName(FosterAndFreeman.PortType.APP_CRC);


            if (appName != null)
                OpenAsApplication(recoverController);
            else
                OpenAsBootloader(recoverController, true);


        }

        static void OpenAsBootloader(FosterAndFreeman.RecoverControl recoverController, bool requiresPowerCycle)
        {
            if (requiresPowerCycle)
                Console.WriteLine("Please Power cycle the RECOVER device now");

            try
            {
                recoverController.Bootloader.CatchStartup();
            }
            catch (Exception)
            {
                Console.WriteLine("Unable to open bootloader! Attempting to open as Application");
                OpenAsApplication(recoverController);
            }

            var version = recoverController.Bootloader.GetVersion();
            Console.WriteLine($"Bootloader Version: 0x{version.ToString("X2")}");


            while (true)
            {
                Console.WriteLine("What would you like to do?");
                Console.WriteLine("\t0) Upgrade Software");
                Console.WriteLine("\t1) Go to Application");
                Console.WriteLine("\t2) Exit");

                var response = ReadOption(new int[] { 0, 1, 2, 3 });
                switch (response)
                {
                    case 0:
                        UpgradeSoftware(recoverController);
                        break;
                    case 1:
                        recoverController.Bootloader.BootApplication();                        
                        OpenAsApplication(recoverController);
                        break;
                    case 2:
                        Exit(recoverController, false);
                        break;
                    case 3:
                        recoverController.Bootloader.SwitchToDFU();
                        break;
                }

            }

            

        }

        static void OpenAsApplication(FosterAndFreeman.RecoverControl recoverController)
        {
            string portName;
            do
            {
                portName = recoverController.FindPortName(FosterAndFreeman.PortType.APP_CRC);
            }
            while (string.IsNullOrWhiteSpace(portName));
            
            recoverController.Application.OpenComPort(portName);
            Console.WriteLine($"Port Opened - {portName}");

            recoverController.Application.EnterPCMode();
            Console.WriteLine("PC Mode Entered");


            while (true)
            {
                Console.WriteLine("What would you like to do?");
                Console.WriteLine("\t0) Go to Bootloader");
                Console.WriteLine("\t1) Upgrade Firmware");
                Console.WriteLine("\t2) Send Custom Command");
                Console.WriteLine("\t3) Exit");


                var response = ReadOption(new int[] { 0, 1, 2, 3 });

                switch (response)
                {
                    case 0: //Go to bootloader
                        var thread = new System.Threading.Thread(() => { OpenAsBootloader(recoverController, false); });
                        thread.Start();
                        recoverController.Application.GotoBootloader();
                        thread.Join();
                        break;
                    case 1: //Upgrade firmware
                        UpgradeFirmware(recoverController);
                        break;
                    case 2: //Send Custom
                        SendCustom(recoverController);
                        break;
                    case 3:
                        Exit(recoverController, true);
                        break;
                }                
            }            
        }

        static void UpgradeFirmware(FosterAndFreeman.RecoverControl recoverController, string path = null)
        {
            string hexFilePath = null;

            if (path == null)
            {
                do
                {
                    Console.WriteLine("Please Enter the Path to a valid .HEX file to load as firmware");

                    hexFilePath = Console.ReadLine();
                    hexFilePath = hexFilePath.Replace("\"", "");
                }
                while (string.IsNullOrWhiteSpace(hexFilePath));


                Console.WriteLine("Are you certain that you want to continue with the upgrade described below?:");
                Console.WriteLine("\t-Firmware Upgrade");
                Console.WriteLine($"\t-Path: {hexFilePath}");
                Console.WriteLine("Confirm?");
                Console.WriteLine("\t0) No");
                Console.WriteLine("\t1) Yes");

                var response = ReadOption(new int[] { 0, 1 });
                if (response == 0)
                    return;
            }
            else            
                hexFilePath = path;

            var startTime = DateTime.Now;
            var data = System.IO.File.ReadAllLines(hexFilePath);


            Console.WriteLine();
            Console.WriteLine("Firmware Upgrade Started:");

            //Turn on Transparent Mode
            recoverController.Application.TransparentMode();


            //Tell System to erase data
            Console.Write("\rErasing:");
            recoverController.Application.SendCustom(Encoding.UTF8.GetBytes("Bl#\n"));


            //Wait for data erased started confirmation            
            if (Encoding.UTF8.GetString(recoverController.Application.Read(1)) != "<")
                throw new Exception("Invalid Erase Start Key");


            //Wait for data finished erase confirmation
            if (Encoding.UTF8.GetString(recoverController.Application.Read(1)) != ">")
                throw new Exception("Invalid Erase Finished Key");

            Console.Write("\rErasing:\tOK");
            Console.WriteLine();


            int onePercent = data.Length / 100;
            int nextPercent = onePercent;
            int percent = 0;
            for (int i = 0; i < data.Length; i++)
            {
                //Send Data
                recoverController.Application.SendCustom(Encoding.UTF8.GetBytes(data[i]));                

                //Receive Confirmation
                var result = Encoding.UTF8.GetString(recoverController.Application.Read(1));
                if (result != "\0")
                    throw new Exception("Invalid Read Receipt");                
                
                //Report Progress
                if (i > nextPercent)
                {
                    percent++;
                    nextPercent += onePercent;
                    
                    Console.Write($"\rWriting:\t{percent.ToString().PadLeft(3, '0')}%");
                }

            }

            Console.Write($"\rWriting:\t100%");
            Console.WriteLine();

            //Exit Transparent Mode
            recoverController.Application.ExitTransparentMode();

            var elapsedTime = DateTime.Now - startTime;
            Console.WriteLine("Firmware Upgrade Finished");
            Console.WriteLine($"Elapsed Time - {elapsedTime.TotalSeconds}s");
            Console.WriteLine();

        }

        static void UpgradeSoftware(FosterAndFreeman.RecoverControl recoverController, string path = null)
        {
            string hexFilePath = null;
            do
            {
                Console.WriteLine("Please Enter the Path to a valid .HEX file to load as software");

                hexFilePath = Console.ReadLine();
                hexFilePath = hexFilePath.Replace("\"", "");
            }
            while (string.IsNullOrWhiteSpace(hexFilePath));

            Console.WriteLine("Are you certain that you want to continue with the upgrade described below?:");
            Console.WriteLine("\t-Software Upgrade");
            Console.WriteLine($"\t-Path: {hexFilePath}");
            Console.WriteLine("Confirm?");
            Console.WriteLine("\t0) No");
            Console.WriteLine("\t1) Yes");

            var response = ReadOption(new int[] { 0, 1 });
            if (response == 0)
                return;

            Console.WriteLine();

            recoverController.Bootloader.SwitchToDFU();

            var startTime = DateTime.Now;

            var data = System.IO.File.ReadAllLines(hexFilePath);
            recoverController.Bootloader.DfuManager.UpgradeSoftware(data);


            recoverController.Bootloader.CatchStartup();

            var elapsedTime = DateTime.Now - startTime;
            Console.WriteLine($"Elapsed Time - {elapsedTime.TotalSeconds}s");
        }

        static void SendCustom(FosterAndFreeman.RecoverControl recoverController)
        {
            Console.WriteLine("Send Custom Command mode entered!");
            Console.WriteLine("To Exit type \"\\x\" and hit return");
            Console.WriteLine();

            recoverController.Application.TransparentMode();

            while (true)
            {
                var customCommand = Console.ReadLine();

                if (customCommand.ToUpper() == "\\X")
                    break;


                if (!customCommand.EndsWith("\n"))
                    customCommand += "\n";

                recoverController.Application.SendCustom(Encoding.UTF8.GetBytes(customCommand));

                var firmwareResponse = string.Empty;
                do
                {
                    firmwareResponse += Encoding.UTF8.GetString(recoverController.Application.Read(1));
                }
                while (!firmwareResponse.EndsWith("\n"));

                Console.WriteLine(firmwareResponse);
            }

            recoverController.Application.ExitTransparentMode();
        }

        static void Exit(FosterAndFreeman.RecoverControl recoverController, bool? isApplication)
        {
            if (isApplication == true)
            {
                recoverController.Application.ExitPCMode();
                recoverController.Application.Close();
            }
            else if (isApplication == false)
            {
                recoverController.Bootloader.BootApplication();                
            }

            Environment.Exit(0);
            
        }


        static int ReadOption(int[] options)
        {
            string response = null;            

            do
            {
                response = Console.ReadLine();
            }
            while (!options.Any(a => a.ToString() == response));

            return options.First(a => a.ToString() == response);
        }

    }
}
