using FosterAndFreeman.RecoverCompanionApplication.Resources.Languages;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading;
using System.Threading.Tasks;

namespace FosterAndFreeman.RecoverCompanionApplication.Definitions.DeviceCommunications
{
    public enum ProgressType
    {
        Firmware, 
        Software,
        Text        
    }

    public struct Progress
    {
        public ProgressType Type;
        public string Information;
        public int Value;
        public int Maximum;
    }

    public struct SampleLoadedArgument
    {
        public DeviceData.Sample SampleLoaded;
        public int Value;
        public int Maximum;
    }

    

    


    public static class RecoverManager
    {                
        private static string DeviceSerial => "LFT";        
        private static RecoverControl recover;        
        private static bool _getSamplesAborted;
        private static Mutex _mutex;

        public const int NumberOfLogs = 64;
        public static EventHandler<Progress> OnProgress;
        public static EventHandler<SampleLoadedArgument> OnSampleLoaded;
        public static AutoResetEvent SampleFinishedEvent;

        public static void Initialise()
        {
            _mutex = new Mutex();
            recover = new RecoverControl();
            recover.Init();
            SampleFinishedEvent = new AutoResetEvent(false);

            recover.Bootloader.DfuManager.OnProgress += OnDFUProgress;
        }

        /// <summary>
        /// Enter PC Mode
        /// 
        /// Setting the PCMode Setting (2) to 1 will ask the LCD if PC mode is available.
        /// The LCD will then set the setting to 2, set the screen to show "PC Mode" and wait for the firmware to become unresponsive.
        /// Once the firmware is unresponsive it then waits for the firmware to start responding again
        /// Once the firmware is once again responsive, it will return to the main screen.
        /// </summary>
        public static void EnterPCMode()
        {            
            //Enter "PC Mode"
            recover.Application.EnterPCMode();
        }


        /// <summary>
        /// Exit PC mode
        /// Setting the LCD connection to active should be enough, but for tidyness I set the
        /// PC mode setting back to 0 anyway
        /// 
        /// </summary>
        public static void ExitPCMode()
        {
            recover.Application.ExitPCMode();
        }

        public static void AttemptConnection()
        {
            var portName = recover.FindPortName(PortType.APP_CRC);
            if (portName == null)
                throw new Exception("Unable to find correct device");

            //Fetch number of devices           
            recover.Application.OpenComPort(portName);
        }

        public static void WriteFirmware(byte[] hexData)
        {
            try
            {
                OnProgress?.Invoke(null, new Progress { Value = 0, Maximum = 100, Information = Strings.FirmwareInititalise, Type = ProgressType.Firmware });

                _mutex.WaitOne();

                //Switch to transparent mode first
                recover.Application.TransparentMode();
                Thread.Sleep(1000);
                

                //Get the data from the hex
                var data = System.Text.Encoding.Default.GetString(hexData).Split('\n');

                data = data.Where(a => !string.IsNullOrWhiteSpace(a)).ToArray();

                //Tell System to erase data
                Write("Bl#\n");

                OnProgress?.Invoke(null, new Progress { Value = 0, Maximum = 100, Information = Strings.FirmwareErasing, Type = ProgressType.Firmware });

                //Wait for data erased started confirmation
                if (Read(1) != "<")
                    throw new Exception(Strings.InvalidEraseStartedKey);


                //Wait for data finished erase confirmation
                if (Read(1) != ">")
                    throw new Exception(Strings.InvalidEraseFinishedKey);

                OnProgress?.Invoke(null, new Progress { Value = 0, Maximum = 100, Information = Strings.FirmwareUpdating, Type = ProgressType.Firmware });

                for (int i = 0; i < data.Length; i++)
                {
                    //Send Data
                    Write(data[i]);                    

                    //Receive Confirmation
                    var result = Read(1);
                    if (result != "\0")
                        throw new Exception(Strings.InvalidReadReceipt);
                    

                    OnProgress?.Invoke(null, new Progress { Value = i, Maximum = data.Length, Information = Strings.FirmwareUpdating, Type = ProgressType.Firmware });
                }


                //Report Success
                OnProgress?.Invoke(null, new Progress { Value = data.Length, Maximum = data.Length, Information = Strings.FirmwareUpdating, Type = ProgressType.Firmware });
            }            
            finally
            {                             
                try
                {
                    recover.Application.ExitTransparentMode();                   
                }
                catch (Exception ex)
                {
                    Console.WriteLine(ex.Message);
                }

                _mutex.ReleaseMutex();

            }
            



        }

        public static void WriteSoftware(byte[] hexData)
        {
            //Go to Bootloader and catch
            var gotoBootloaderAttempts = 0;
            Thread.CurrentThread.Priority = ThreadPriority.Highest;
            while (true)
            {
                try
                {
                    var thread = new System.Threading.Thread(() => { recover.Bootloader.CatchStartup(15000); });
                    thread.Start();
                    recover.Application.GotoBootloader();
                    thread.Join();
                    break;
                }
                catch (Exception)
                {
                    //Exit after 10 failiures
                    gotoBootloaderAttempts++;
                    if (gotoBootloaderAttempts > 10)
                        throw new System.TimeoutException("Unable to go to bootloader in time");

                    Thread.Sleep(2000);

                    //Opening Bootloader will fail when using a new PC due to drivers being installed on the device
                    //We will try again so we re-open as application
                    string appPortName = null;
                    while (appPortName == null)                    
                        appPortName = recover.FindPortName(PortType.APP_CRC);

                    recover.Application.OpenComPort(appPortName);
                    
                }
            }

            Thread.CurrentThread.Priority = ThreadPriority.Normal;

            Thread.Sleep(500);


            //Switch to DFU mode
            recover.Bootloader.SwitchToDFU();
            Thread.Sleep(500);

            //Upgrade Software
            var intelHexLines = Encoding.UTF8.GetString(hexData).Split('\n');
            recover.Bootloader.DfuManager.UpgradeSoftware(intelHexLines);            

            string portName = null;
            int i = 0;
            do
            {
                portName = recover.FindPortName(PortType.APP_CRC);

                Thread.Sleep(100);
                i++;
                if (i > 25)
                    throw new Exception("Unable to re-open port");

            }
            while (portName == null);

            recover.Application.OpenComPort(portName);
            recover.Application.EnterPCMode();

        }

        public static void EnterTransparentMode()
        {
            recover.Application.TransparentMode();
        }

        public static void ExitTransparentMode()
        {
            recover.Application.ExitTransparentMode();
        }
        
        public static DeviceData.RecoverLog GetLogHeader(int index)
        {
            

            _mutex.WaitOne();
            Write($"READHEADER {index}\n");
            var headerRaw = Read();
            _mutex.ReleaseMutex();

            DeviceData.RecoverLog log = null;
            if (!headerRaw.StartsWith("-1") && !headerRaw.Contains("\0"))
            {
                log = new DeviceData.RecoverLog();
                log.Parse(headerRaw, index);
            }
            

            return log;
        }

        public static void GetLogSamples(DeviceData.RecoverLog recoverLog)
        {            
            int index = recoverLog.Index;

            _getSamplesAborted = false;
            int numberOfSamples = recoverLog.NumberOfSamples;
            var samples = new List<DeviceData.Sample>();
            SampleFinishedEvent.Reset();

            new System.Threading.Thread(() =>
            {
                var startTime = DateTime.UtcNow;

                EnterTransparentMode();

                for (int i = 0; i <= numberOfSamples; i++)
                {
                    if (_getSamplesAborted)
                        return;

                    _mutex.WaitOne();
                    Write($"READSAMPLE {index} {i}");
                    var rawSampleData = Read();
                    _mutex.ReleaseMutex();

                    if (rawSampleData.StartsWith("-1"))
                        continue;

                    var sample = new DeviceData.Sample(rawSampleData);

                    OnSampleLoaded?.Invoke(null, new SampleLoadedArgument() { SampleLoaded = sample, Value = i, Maximum = recoverLog.NumberOfSamples });

                    samples.Add(sample);
                    recoverLog.Samples = samples.ToArray();
                }

                ExitTransparentMode();

                recoverLog.SamplesLoaded = true;
                SampleFinishedEvent.Set();

                var elapsedTime = DateTime.UtcNow - startTime; 
                Console.WriteLine($"LOG LOAD TOOK {elapsedTime.Seconds}.{elapsedTime.Milliseconds}s");

            }).Start();
        }

        public static void AbortGetLogSamples()
        {
            _getSamplesAborted = true;
        }

        public static void DeleteLog(int index)
        {
            EnterTransparentMode();

            Write($"DELETELOG {index}");
            Read();

            ExitTransparentMode();
        }
        
        public static string SerialNumber
        {           
            get
            {
                EnterTransparentMode();
                Thread.Sleep(50);

                Write("READSERIAL");
                var result = Read();
                result = result.Substring(result.Length - 11, 11).Replace("\n", "");

                ExitTransparentMode();


                if (!IsSerialNumberValid(result))
                    return null;

                return result;
            }
            set
            {
                EnterTransparentMode();

                //Check that string starts with 1236- and the has 5 digits, then ends
                if (!IsSerialNumberValid(value))
                    throw new ArgumentException("Serial does not follow correct format");

                Write($"SETSERIAL {value}");
                Read();

                ExitTransparentMode();
            }
        }

        public static bool IsSerialNumberValid(string input)
        {
            return Regex.IsMatch(input, @"^1236-\d{5}$");
        }

        public static void Disconnect()
        {
            recover.Application.Close();
        }


        public static string SendCustom(string command)
        {
            EnterTransparentMode();

            Write(command);
            var result = Read();

            ExitTransparentMode();

            return result;
        }

        private static void Write(string command)
        {
            //Add newline
            if (!command.EndsWith("\n"))
                command += "\n";
            

            var commandArray = Encoding.UTF8.GetBytes(command);
            recover.Application.SendCustom(commandArray);
        }        

        private static string Read(string escape = "\n")
        {
            var result = string.Empty;
            do
            {
                var output = Read(1);                
                result += output;
            }
            while (!result.EndsWith(escape));

            return result;
        }

        private static string Read(int numberToRead)
        {
            var responseBytes = recover.Application.Read(numberToRead).Take(numberToRead).ToArray();
            return Encoding.UTF8.GetString(responseBytes);           
        }

        public static T[] SubArray<T>(this T[] data, int index, int length)
        {
            T[] result = new T[length];
            Array.Copy(data, index, result, 0, length);
            return result;
        }


        private static void OnDFUProgress(object sender, Internal.Dfu.DFU_Progress e)
        {
            //Parsing = 000% - 010%
            //Erasing = 010% - 020%
            //Writing = 020% - 100%

            var percentage = (int)(((float)e.Value / e.Maximum) * 100);


            switch (e.Type)
            {
                case Internal.Dfu.DFU_ProgressType.Parsing:
                    OnProgress?.Invoke(null, new Progress() { Type = ProgressType.Software, Maximum = 100, Value = 0 + (int)(e.Value * 0.1) });
                    break;

                case Internal.Dfu.DFU_ProgressType.Erasing:
                    OnProgress?.Invoke(null, new Progress() { Type = ProgressType.Software, Maximum = 100, Value = 10 + (int)(e.Value * 0.1) });
                    break;

                case Internal.Dfu.DFU_ProgressType.Writing:
                    OnProgress?.Invoke(null, new Progress() { Type = ProgressType.Software, Maximum = 100, Value = 20 + (int)(e.Value * 0.8) });
                    break;
            }
        }

    }
}
