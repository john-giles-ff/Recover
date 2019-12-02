using FosterAndFreeman.RecoverCompanionApplication.Definitions.BaseClasses;
using FosterAndFreeman.RecoverCompanionApplication.Definitions.Misc;
using FosterAndFreeman.RecoverCompanionApplication.Definitions.WPF;
using FosterAndFreeman.RecoverCompanionApplication.Resources.Languages;
using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using System.Xml.Serialization;

namespace FosterAndFreeman.RecoverCompanionApplication.Definitions.DeviceData
{
    /// <summary>
    /// Sample Modes
    /// </summary>
    [Serializable]
    public enum SampleMode
    {
        SAMPLE_UNKNOWN,
        SAMPLE_INITIALISE,
        SAMPLE_PUMPDOWN,
        SAMPLE_HEAT,
        SAMPLE_COOL,
        SAMPLE_BREAKSEAL
    }

    /// <summary>
    /// Sample Data
    /// </summary>
    [Serializable]
    public struct Sample
    {
        /// <summary>
        /// Sample Mode
        /// </summary>
        public SampleMode Mode { get; set; }

        /// <summary>
        /// Sample Number as Time
        /// </summary>
        public DateTime SampleTime
        {
            get 
            {                                
                return new DateTime().AddSeconds(SampleNumber * 10);                
            }
        }

        /// <summary>
        /// Sample Number
        /// </summary>
        public int SampleNumber { get; set; }

        /// <summary>
        /// Base Temperature at sample time
        /// </summary>
        public int BaseTemperature { get; set; }

        /// <summary>
        /// Precursor Temperature at sample time
        /// </summary>
        public int PrecursorTemperature { get; set; }

        /// <summary>
        /// Pressure at sample time
        /// </summary>
        public int Pressure { get; set; }

        /// <summary>
        /// Status at sample time
        /// </summary>
        public int Status { get; set; }

        /// <summary>
        /// Parse Sample from input from LFT
        /// </summary>
        /// <param name="input"></param>
        public Sample(string input)
        {            
            var datas = input.Split('\t');
            switch (datas[0][0])
            {
                case 'I':
                    Mode = SampleMode.SAMPLE_INITIALISE;
                    break;
                case 'P':
                    Mode = SampleMode.SAMPLE_PUMPDOWN;
                    break;
                case 'H':
                    Mode = SampleMode.SAMPLE_HEAT;
                    break;
                case 'C':
                    Mode = SampleMode.SAMPLE_COOL;
                    break;
                case 'B':
                    Mode = SampleMode.SAMPLE_BREAKSEAL;
                    break;
                default:
                    Mode = SampleMode.SAMPLE_UNKNOWN;
                    break;
            }

            SampleNumber = int.Parse(datas[1]);
            BaseTemperature = int.Parse(datas[2]);
            PrecursorTemperature = int.Parse(datas[3]);
            Pressure = int.Parse(datas[4]);
            Status = int.Parse(datas[5]);
            

        }
    }

    /// <summary>
    /// Recover Log
    /// </summary>
    [Serializable]
    public class RecoverLog : PropertyChangedBase
    {
        private const string _encryptionPassPhrase = "dz2D9Q88!FT2z{9[&[vn2Nw&%:Vu>_n^j4<Rz%f.8@34`~3;SFS7g6D]?7d(!MXs";

        private string _filename;

        private int _index;
        private DateTime _startTime;
        private int _baseHeaterSetpoint;
        private int _precursorHeaterSetpoint;
        private int _vacuumSetPoint;
        private int _finalBaseHeaterTemperature;
        private int _fullPrecursorTemperature;
        private int _finalPrecusorHeaterTemperature;
        private int _finalPressureMeasurement;
        private int _leakTest1Result;
        private int _leakTest2Result;
        private int _pumpdownRunTime;
        private int _heatRunTime;
        private int _timeToReachVacuumSetpoint;
        private int _maximumLeakSetting;
        private bool _chamberSize;
        private int _filterCount;
        private int _totalRunCount;
        private int _timeForPSW2toOperate;
        private int _pressureWhenPSW2Operates;
        private int _lowVacSetting;
        private int _sampleRatePumpdown;
        private int _sampleRateDevelop;
        private int _firmwareBuild;
        private string _serialNumber;
        private int _numberOfSamples;
        private bool _samplesLoaded;
        private bool _metalType;
        private IEnumerable<Sample> _samples;
        private bool _isUSB;

        private string _caseReference;
        private string _caseOperator;
        private string _notes;


        /// <summary>
        /// Index of Log
        /// </summary>
        public int Index
        {
            get
            {
                return _index;
            }
            set
            {
                _index = value;
                RaisePropertyChanged();
            }
        }

        /// <summary>
        /// Start Time of Process
        /// </summary>
        public DateTime StartTime
        {
            get
            {
                return _startTime;
            }
            set
            {
                _startTime = value;
                RaisePropertyChanged();
            }
        }

        /// <summary>
        /// Set point of base heater
        /// </summary>
        public int BaseHeaterSetPoint
        {
            get
            {
                return _baseHeaterSetpoint;
            }
            set
            {
                _baseHeaterSetpoint = value;
                RaisePropertyChanged();
            }
        }

        /// <summary>
        /// Set point of precursor heater
        /// </summary>
        public int PrecursorHeaterSetPoint
        {
            get
            {
                return _precursorHeaterSetpoint;
            }
            set
            {
                _precursorHeaterSetpoint = value;
                RaisePropertyChanged();
            }
        }

        /// <summary>
        /// Set point of vacuum 
        /// </summary>
        public int VacuumSetPoint
        {
            get
            {
                return _vacuumSetPoint;
            }
            set
            {
                _vacuumSetPoint = value;
                RaisePropertyChanged();
            }
        }

        /// <summary>
        /// Base Heater temperature at end of procedure
        /// </summary>
        public int FinalBaseHeaterTemperature
        {
            get
            {
                return _finalBaseHeaterTemperature;
            }
            set
            {
                _finalBaseHeaterTemperature = value;
                RaisePropertyChanged();
            }
        }

        /// <summary>
        /// Precursor Temperature before it backs off
        /// </summary>
        public int FullPrecursorTemperature
        {
            get
            {
                return _fullPrecursorTemperature;
            }
            set
            {
                _fullPrecursorTemperature = value;
                RaisePropertyChanged();
            }

        }

        /// <summary>
        /// Precursor Heater temperature at end of procedure
        /// </summary>
        public int FinalPrecusorHeaterTemperature
        {
            get
            {
                return _finalPrecusorHeaterTemperature;
            }
            set
            {
                _finalPrecusorHeaterTemperature = value;
                RaisePropertyChanged();
            }
        }

        /// <summary>
        /// Pressure at end of procedure
        /// </summary>
        public int FinalPressureMeasurement
        {
            get
            {
                return _finalPressureMeasurement;
            }
            set
            {
                _finalPressureMeasurement = value;
                RaisePropertyChanged();
            }
        }

        /// <summary>
        /// If the First Leak Test Succeeded
        /// </summary>
        public int LeakTest1Result
        {
            get
            {
                return _leakTest1Result;
            }
            set
            {
                _leakTest1Result = value;
                RaisePropertyChanged();
            }
        }

        /// <summary>
        /// If the second leak test succeeded
        /// </summary>
        public int LeakTest2Result
        {
            get
            {
                return _leakTest2Result;
            }
            set
            {
                _leakTest2Result = value;
                RaisePropertyChanged();
            }
        }

        /// <summary>
        /// Time spent in pump down
        /// </summary>
        public int PumpDownRunTime
        {
            get
            {
                return _pumpdownRunTime;
            }
            set
            {
                _pumpdownRunTime = value;
                RaisePropertyChanged();
            }
        }

        /// <summary>
        /// Time spent in heat 
        /// </summary>
        public int HeatRunTime
        {
            get
            {
                return _heatRunTime;
            }
            set
            {
                _heatRunTime = value;
                RaisePropertyChanged();
            }
        }

        /// <summary>
        /// Number of samples taken
        /// </summary>
        public int NumberOfSamples
        {
            get
            {
                return _numberOfSamples;
            }
            set
            {
                _numberOfSamples = value;
                RaisePropertyChanged();
            }
        }

        /// <summary>
        /// If Base is hotter than -10% Setpoint Deviation and lower than 60C
        /// </summary>
        public bool BaseHeaterWithinTolerance
        {
            get
            {
                return BaseHeaterSetpointDeviation > -10 && FinalBaseHeaterTemperature < 60;
            }
        }

        /// <summary>
        /// Amount of deviation from baseheater setpoint to actual
        /// </summary>
        public float BaseHeaterSetpointDeviation
        {
            get
            {
                var percentage = ((float)FinalBaseHeaterTemperature / BaseHeaterSetPoint) * 100;
                return percentage - 100.0f;
            }
        }

        /// <summary>
        /// String Describing Base Tolerance
        /// </summary>
        public string BaseString
        {
            get
            {
                var state = BaseHeaterWithinTolerance ? Strings.TestSuccess : Strings.TestFail;
                //return $"{state}({BaseHeaterSetpointDeviation.ToString("+#;-#;+0")}%)";
                //Was asked to remove the deviation values, keeping here just incase
                return state;
            }
        }

        /// <summary>
        /// If Precusor Deviation is withing +- 5%
        /// </summary>
        public bool PrecursorHeaterWithinTolerance
        {
            get
            {
                return PrecusorSetpointDeviation > -5 && PrecusorSetpointDeviation < 5;
            }
        }

        /// <summary>
        /// Amount of deviation from precusor setpoint to actual
        /// </summary>
        public float PrecusorSetpointDeviation
        {
            get
            {
                var percentage = ((float)FullPrecursorTemperature / PrecursorHeaterSetPoint) * 100;
                return percentage - 100.0f;
            }
        }

        /// <summary>
        /// String describing precursor tolerance
        /// </summary>
        public string PrecursorString
        {
            get
            {
                var state = PrecursorHeaterWithinTolerance ? Strings.TestSuccess : Strings.TestFail;
                //return $"{state}({PrecusorSetpointDeviation.ToString("+#;-#;+0")}%)";
                //Was asked to remove the deviation values, keeping here just incase
                return state;
            }
        }

        /// <summary>
        /// Check that pressure is below 1000millTorr after run
        /// </summary>
        public bool PressureDeviationWithinTolerance
        {
            get
            {
                return FinalPressureMeasurement < 1000 && FinalPressureMeasurement != 0;
            }
        }

        /// <summary>
        /// Amount of deviation from pressure setpoint to actual
        /// </summary>
        public float PressureSetpointDeviation
        {
            get
            {
                var percentage = ((float)FinalPressureMeasurement / VacuumSetPoint) * 100;
                return percentage - 100.0f;
            }
        }

        /// <summary>
        /// String describing Pressure tolerance
        /// </summary>
        public string PressureString
        {
            get
            {
                var state = PressureDeviationWithinTolerance ? Strings.TestSuccess : Strings.TestFail;
                //return $"{state}({PressureSetpointDeviation.ToString("+#;-#;+0")}%)";
                //Was asked to remove the deviation values, keeping here just incase
                return state;
            }
        }

        /// <summary>
        /// If the First Leak test passed
        /// </summary>
        public bool LeakTest1
        {
            get
            {
                return LeakTest1Result < 75;
            }
        }

        /// <summary>
        /// If the second leak test passed
        /// </summary>
        public bool LeakTest2
        {
            get
            {
                return LeakTest2Result < 75;
            }
        }

        /// <summary>
        /// Time to reach Vacuum Setpoint
        /// </summary>
        public int TimeToReachVacuumSetpoint
        {
            get
            {
                return _timeToReachVacuumSetpoint;
            }
            set
            {
                _timeToReachVacuumSetpoint = value;
                RaisePropertyChanged();
            }
        }

        /// <summary>
        /// Maximum Allowed Leak Setting
        /// </summary>
        public int MaximumLeakSetting
        {
            get
            {
                return _maximumLeakSetting;
            }
            set
            {
                _maximumLeakSetting = value;
                RaisePropertyChanged();
            }
        }

        /// <summary>
        /// Chamber size at time of run
        /// false = small, true = large
        /// </summary>
        public bool ChamberSize
        {
            get
            {
                return _chamberSize;
            }
            set
            {
                _chamberSize = value;
                RaisePropertyChanged();
                RaisePropertyChanged("ChamberSizeString");
                RaisePropertyChanged("DevelopRType");
            }
        }

        /// <summary>
        /// Chamber size as string
        /// </summary>
        public string ChamberSizeString
        {
            get
            {
                if (_chamberSize)
                    return Strings.LargeChamber;
                else
                    return Strings.SmallChamber;
            }
        }

        /// <summary>
        /// Number of cycles since process filter counter was reset.
        /// </summary>
        public int FilterCount
        {
            get
            {
                return _filterCount;
            }
            set
            {
                _filterCount = value;
                RaisePropertyChanged();
            }
        }

        /// <summary>
        /// Total Number of cycles on the machine
        /// </summary>
        public int TotalRunCount
        {
            get
            {
                return _totalRunCount;
            }
            set
            {
                _totalRunCount = value;
                RaisePropertyChanged();
            }
        }

        /// <summary>
        /// Time until low pressure switch clicked
        /// </summary>
        public int TimeForPSW2ToOperate
        {
            get
            {
                return _timeForPSW2toOperate;
            }
            set
            {
                _timeForPSW2toOperate = value;
                RaisePropertyChanged();
            }
        }

        /// <summary>
        /// Pressure when low pressure switch operates
        /// </summary>
        public int PressureWhenPSW2Operates
        {
            get
            {
                return _pressureWhenPSW2Operates;
            }
            set
            {
                _pressureWhenPSW2Operates = value;
                RaisePropertyChanged();
            }
        }

        /// <summary>
        /// Sets maximum value pressure can be before an error
        /// </summary>
        public int LowVacSetting
        {
            get
            {
                return _lowVacSetting;
            }
            set
            {
                _lowVacSetting = value;
                RaisePropertyChanged();
            }
        }

        /// <summary>
        /// How often a sample is taken when pumping down(Normally 10s)
        /// </summary>
        public int SampleRatePumpdown
        {
            get
            {
                return _sampleRatePumpdown;
            }
            set
            {
                _sampleRatePumpdown = value;
                RaisePropertyChanged();
            }
        }

        /// <summary>
        /// How often a sample is taken when developing (Normally 10s)
        /// </summary>
        public int SampleRateDevelop
        {
            get
            {
                return _sampleRateDevelop;
            }
            set
            {
                _sampleRateDevelop = value;
                RaisePropertyChanged();
            }
        }

        /// <summary>
        /// Firmware Build of machine
        /// </summary>
        public int FirmwareBuild
        {
            get
            {
                return _firmwareBuild;
            }
            set
            {
                _firmwareBuild = value;
                RaisePropertyChanged();
            }
        }

        /// <summary>
        /// Serial Number of machine
        /// </summary>
        public string SerialNumber
        {
            get
            {
                return _serialNumber;
            }
            set
            {
                _serialNumber = value;
                RaisePropertyChanged();
            }
        }

        /// <summary>
        /// Metal Type
        /// false = copper, true = silver
        /// </summary>
        public bool MetalType
        {
            get
            {
                return _metalType;
            }
            set
            {
                _metalType = value;
                RaisePropertyChanged();
                RaisePropertyChanged("MetalTypeString");
                RaisePropertyChanged("DevelopRType");
            }
        }

        /// <summary>
        /// Metal Type as String
        /// </summary>
        public string MetalTypeString
        {
            get
            {
                if (_metalType)
                    return Strings.SilverColoured;
                else
                    return Strings.CopperBased;
            }
        }

        /// <summary>
        /// DevelopR type based off of MetalType and Chamber Size
        /// </summary>
        public string DevelopRType
        {
            get
            {
                int val = 0;
                if (ChamberSize)
                    val += 2;
                if (MetalType)
                    val += 1;

                switch (val)
                {
                    case 0:
                        return Strings.DevelopR1;
                    case 1:
                        return Strings.DevelopR2;
                    case 2:
                        return Strings.DevelopR3;
                    case 3:
                        return Strings.DevelopR4;
                }

                return string.Empty;
            }
        }

        [XmlIgnore()]
        public bool HasCaseReference
        {
            get
            {
                return !string.IsNullOrWhiteSpace(CaseReference);
            }
        }

        [XmlIgnore()]
        /// <summary>
        /// Case Reference Name        
        /// </summary>
        /// <remarks>
        /// Only available if IsUSB == false
        /// </remarks>
        public string CaseReference
        {
            get
            {
                if (IsUSB)
                    return null;

                return _caseReference;
            }
            set
            {
                if (IsUSB)
                    return;

                _caseReference = value;
                RaisePropertyChanged();
                RaisePropertyChanged("HasExtraInformation");
                RaisePropertyChanged("HasCaseReference");
            }
        }

        [XmlIgnore()]
        public bool HasCaseOperator
        {
            get
            {
                return !string.IsNullOrWhiteSpace(CaseOperator);
            }
        }

        [XmlIgnore()]
        /// <summary>
        /// Operator Name
        /// </summary>
        /// <remarks>
        /// Only available if IsUSB == false
        /// </remarks>
        public string CaseOperator
        {
            get
            {
                if (IsUSB)
                    return null;

                return _caseOperator;
            }
            set
            {
                if (IsUSB)
                    return;

                _caseOperator = value;
                RaisePropertyChanged();
                RaisePropertyChanged("HasExtraInformation");
                RaisePropertyChanged("HasCaseOperator");
            }
        }

        public bool HasNotes
        {
            get
            {
                return !string.IsNullOrWhiteSpace(Notes);
            }
        }

        public string Notes
        {
            get
            {
                if (IsUSB)
                    return null;

                return _notes;
            }
            set
            {
                if (IsUSB)
                    return;

                _notes = value;
                RaisePropertyChanged();
                RaisePropertyChanged("HasNotes");
                RaisePropertyChanged("HasExtraInformation");

            }
        }

        [XmlIgnore()]
        public bool HasExtraInformation
        {
            get
            {
                var result = !(string.IsNullOrWhiteSpace(CaseReference) && string.IsNullOrWhiteSpace(CaseOperator) && string.IsNullOrWhiteSpace(Notes));

                return result;
            }
        }

        /// <summary>
        /// If the samples have finished loading
        /// </summary>
        [XmlIgnore()]
        public bool SamplesLoaded
        {
            get
            {
                return _samplesLoaded;
            }
            set
            {
                _samplesLoaded = value;
                RaisePropertyChanged();
            }
        }

        /// <summary>
        /// Samples
        /// </summary>
        [XmlIgnore()]
        public IEnumerable<Sample> Samples
        {
            get
            {
                return _samples;
            }
            set
            {
                _samples = value;
                RaisePropertyChanged();
                RaisePropertyChanged("ObservableSamples");
            }
        }

        public Sample[] SerializableSamples
        {
            get
            {
                if (Samples == null)
                    return null;

                return Samples.ToArray();
            }
            set
            {
                Samples = value;
            }
        }


        /// <summary>
        /// Time when Pumpdown was started
        /// </summary>
        public DateTime PumpDownStart
        {
            get
            {
                if (Samples == null)
                    return new DateTime();

                var firstPumpdownSample = Samples.FirstOrDefault(a => a.Mode == SampleMode.SAMPLE_PUMPDOWN);
                return firstPumpdownSample.SampleTime;
            }
        }

        /// <summary>
        /// Time when heat was started
        /// </summary>
        public DateTime HeatStart
        {
            get
            {
                if (Samples == null)
                    return new DateTime();

                var firstHeatSample = Samples.FirstOrDefault(a => a.Mode == SampleMode.SAMPLE_HEAT);
                return firstHeatSample.SampleTime;
            }
        }

        /// <summary>
        /// Time when cool was started
        /// </summary>
        public DateTime CoolStart
        {
            get
            {
                if (Samples == null)
                    return new DateTime();

                var firstCoolSample = Samples.FirstOrDefault(a => a.Mode == SampleMode.SAMPLE_COOL);
                return firstCoolSample.SampleTime;
            }
        }

        /// <summary>
        /// Time whe breakseal was started
        /// </summary>
        public DateTime BreakSealStart
        {
            get
            {
                if (Samples == null)
                    return new DateTime();

                var firstBreakSealSample = Samples.FirstOrDefault(a => a.Mode == SampleMode.SAMPLE_BREAKSEAL);
                return firstBreakSealSample.SampleTime;
            }
        }

        /// <summary>
        /// End time 
        /// </summary>
        public DateTime EndTime
        {
            get
            {
                if (Samples == null)
                    return new DateTime();

                return Samples.Last().SampleTime;
            }
        }

        /// <summary>
        /// If log is USB log or not
        /// </summary>
        public bool IsUSB
        {
            get
            {
                return _isUSB;
            }
            set
            {
                _isUSB = value;
                RaisePropertyChanged();
            }
        }

        public string FileName
        {
            get
            {
                return _filename;
            }
            set
            {
                _filename = value;
                RaisePropertyChanged();
            }
        }

        /// <summary>
        /// Parse from format returned from device
        /// </summary>
        /// <param name="input"></param>
        public void Parse(string input, int index)
        {
            IsUSB = true;

            Index = index;
            input = input.Replace("\n", "");

            //Input will be in the following data with \t seperating values
            //Thu Dec 20 09:35:17 2018	35	190	750	39	189	742	6	3	20	8	255	189	15//            

            //Because there are multiple versions of the firmware with different size headers, this should get as much as it can and then leave.
            try
            {
                var datas = input.Split('\t');
                StartTime = DateTime.ParseExact(datas[0], "ddd MMM dd HH:mm:ss yyyy", System.Globalization.CultureInfo.InvariantCulture);
                BaseHeaterSetPoint = int.Parse(datas[1]);
                PrecursorHeaterSetPoint = int.Parse(datas[2]);
                VacuumSetPoint = int.Parse(datas[3]);
                FinalBaseHeaterTemperature = int.Parse(datas[4]);
                FullPrecursorTemperature = int.Parse(datas[5]);
                FinalPressureMeasurement = int.Parse(datas[6]);
                LeakTest1Result = int.Parse(datas[7]);
                LeakTest2Result = int.Parse(datas[8]);
                PumpDownRunTime = int.Parse(datas[9]); //Old 8 bit value, no longer used
                //HeatRunTime = int.Parse(datas[10]);

                //Samples will return \ if there are no samples (Happens when the process fails)
                if (int.TryParse(datas[11], out int samples))
                    NumberOfSamples = samples;


                FinalPrecusorHeaterTemperature = int.Parse(datas[12]);
                TimeToReachVacuumSetpoint = int.Parse(datas[13]);
                MaximumLeakSetting = int.Parse(datas[14]);
                ChamberSize = int.Parse(datas[15]) == 1;
                FilterCount = int.Parse(datas[16]);
                TotalRunCount = int.Parse(datas[17]);
                TimeForPSW2ToOperate = int.Parse(datas[18]);
                PressureWhenPSW2Operates = int.Parse(datas[19]);
                LowVacSetting = int.Parse(datas[20]);
                SampleRatePumpdown = int.Parse(datas[21]);
                SampleRateDevelop = int.Parse(datas[22]);
                FirmwareBuild = int.Parse(datas[23]);
                SerialNumber = datas[24];
                //There is a skipped case number variable here
                MetalType = int.Parse(datas[26]) == 1;

                //New 16 bit value
                HeatRunTime = int.Parse(datas[33]);

            }
            catch (Exception) { };

            if (NumberOfSamples > 1000)
                NumberOfSamples = 1000;

        }

        /// <summary>
        /// Parse from USB file
        /// </summary>
        /// <param name="input"></param>
        public void Parse(string filename)
        {
            IsUSB = false;
            _filename = filename;

            //Read Data
            var readData = ReadData(filename);

            //Start Time
            StartTime = new DateTime
                (
                    readData[0x00] + 2000, //Year
                    readData[0x01] + 1, //Month (+1 since it's handled as 0-11 on the other side)
                    readData[0x02], //Day
                    readData[0x03], //Hour
                    readData[0x04], //Minute
                    readData[0x05] //Second
                );

            //Stage 1 Heater Deviation
            FinalBaseHeaterTemperature = readData[0x06];
            BaseHeaterSetPoint = readData[0x07];

            //Stage 2 Heater Deviation
            FinalPrecusorHeaterTemperature = readData[0x08];
            FullPrecursorTemperature = readData[0x09];
            PrecursorHeaterSetPoint = readData[0x0A];

            //Pressure Deviation
            FinalPressureMeasurement = readData[0x0B];
            VacuumSetPoint = readData[0x0C];

            //Leak Test 1
            LeakTest1Result = readData[0x0D];

            //Leak Test 2
            LeakTest2Result = readData[0x0E];

            //Conditioning Time
            TimeToReachVacuumSetpoint = readData[0x0F];

            //Fuming Time
            HeatRunTime = readData[0x10];

            //Firmware Build
            FirmwareBuild = readData[0x11];

            //Serial Number
            var serialBytes = new byte[]
            {
                (byte)readData[0x12],
                (byte)readData[0x13],
                (byte)readData[0x14],
                (byte)readData[0x15],
                (byte)readData[0x16]
            };
            SerialNumber = $"1236-{Encoding.ASCII.GetString(serialBytes)}";

            //Metal Type
            MetalType = readData[0x17] == 1;

            //Chamber Size
            ChamberSize = readData[0x18] == 1;

            //Check for Case Reference and Operator (These values are added in companion application not in the main unit)
            int index = 0x19;
            if (readData.Count() <= index)
                return;

            //Case Reference
            int referenceLength = readData[index];
            index++;
            var referenceArray = new ushort[referenceLength];
            Array.Copy(readData, index, referenceArray, 0, referenceLength);
            var encryptedReference = Encoding.ASCII.GetString(referenceArray.Select(a => (byte)a).ToArray());
            var decryptedReference = Misc.EncryptionManager.Decrypt(encryptedReference, _encryptionPassPhrase);
            CaseReference = decryptedReference;

            index += referenceLength;
            if (readData.Count() <= index)
                return;

            //Case Operator            
            int operatorLength = readData[index];
            index++;
            var operatorArray = new ushort[operatorLength];
            Array.Copy(readData, index, operatorArray, 0, operatorLength);
            var encryptedOperator = Encoding.ASCII.GetString(operatorArray.Select(a => (byte)a).ToArray());
            var decryptedOperator = Misc.EncryptionManager.Decrypt(encryptedOperator, _encryptionPassPhrase);
            CaseOperator = decryptedOperator;

            index += operatorLength;
            if (readData.Count() <= index)
                return;

            //Notes
            int notesLength = readData[index];
            index++;
            var notesArray = new ushort[notesLength];
            Array.Copy(readData, index, notesArray, 0, notesLength);
            var encryptedNotes = Encoding.ASCII.GetString(notesArray.Select(a => (byte)a).ToArray());
            var decryptedNotes = Misc.EncryptionManager.Decrypt(encryptedNotes, _encryptionPassPhrase);
            Notes = decryptedNotes;

            index += notesLength;
            if (readData.Count() <= index)
                return;

        }

        /// <summary>
        /// Save data to .RECOVER file
        /// </summary>
        /// <param name="filename"></param>
        public ICommand Save
        {
            get
            {
                return new ButtonCommand((p) =>
                {
                    var data = new UInt16[]
                    {
                        //Start Time
                        (UInt16)(StartTime.Year - 2000),
                        (UInt16)(StartTime.Month - 1),
                        (UInt16)StartTime.Day,
                        (UInt16)StartTime.Hour,
                        (UInt16)StartTime.Minute,
                        (UInt16)StartTime.Second,

                        //Stage 1 Heater Deviation
                        (UInt16)FinalBaseHeaterTemperature,
                        (UInt16)BaseHeaterSetPoint,

                        //Stage 2 Heater Deviation
                        (UInt16)FinalPrecusorHeaterTemperature,
                        (UInt16)FullPrecursorTemperature,
                        (UInt16)PrecursorHeaterSetPoint,

                        //Pressure Deviation
                        (UInt16)FinalPressureMeasurement,
                        (UInt16)VacuumSetPoint,

                        //Leak Test 1
                        (UInt16)LeakTest1Result,

                        //Leak Test 2
                        (UInt16)LeakTest2Result,

                        //Conditioning Time
                        (UInt16)TimeToReachVacuumSetpoint,

                        //Fuming Time
                        (UInt16)HeatRunTime,

                        //Firmware Version
                        (UInt16)FirmwareBuild,

                        //Serial Number
                        (UInt16)SerialNumber[5],
                        (UInt16)SerialNumber[6],
                        (UInt16)SerialNumber[7],
                        (UInt16)SerialNumber[8],
                        (UInt16)SerialNumber[9],

                        //Metal Type
                        (UInt16)(MetalType ? 1 : 0),

                        //Chamber Type
                        (UInt16)(ChamberSize ? 1 : 0)
                    };

                    string filename = "";

                    if (p == null)                                            
                    {
                        var saveFileDialog = new SaveFileDialog()
                        {
                            FileName = StartTime.ToString("yyyy-MM-dd-HH-mm-ss.recover"),
                            Filter = "RECOVER (*.recover)|*.recover",
                            AddExtension = true
                        };
                        if (saveFileDialog.ShowDialog() != true)
                            return;

                        filename = saveFileDialog.FileName;
                    }
                    else
                        filename = p as string;


                    //Convert to bytes
                    var outputBytes = new List<byte>();
                    foreach(var dat in data)                    
                        outputBytes.AddRange(BitConverter.GetBytes(dat));
                    

                    //Write bytes
                    System.IO.File.WriteAllBytes(filename, outputBytes.ToArray());
                });
            }
        }

        public void AddExtraInformation(string caseReference, string caseOperator, string notes)
        {
            if (IsUSB)
                throw new Exception("Unable to add extra information to direct log");

            CaseReference = caseReference;
            CaseOperator = caseOperator;
            Notes = notes;

            //Parse information provided into correct format
            var encryptedReference = Misc.EncryptionManager.Encrypt(caseReference, _encryptionPassPhrase);
            var referenceArray = Encoding.ASCII.GetBytes(encryptedReference);

            var encryptedOperator = Misc.EncryptionManager.Encrypt(caseOperator, _encryptionPassPhrase);
            var operatorArray = Encoding.ASCII.GetBytes(encryptedOperator);

            var encryptedNotes = Misc.EncryptionManager.Encrypt(notes, _encryptionPassPhrase);
            var notesArray = Encoding.ASCII.GetBytes(encryptedNotes);

            //Read Data from file
            var readData = ReadData(_filename).ToList();
            int index = 0x19;

            //If extra information exists, then remove data
            if (readData.Count() > index)
            {
                //Remove all bytes and set length to 0 for reference and operator
                int referenceLength = readData[index];
                readData[index] = 0;
                readData.RemoveRange(index + 1, referenceLength);

                int operatorLength = readData[index + 1];
                readData[index + 1] = 0;
                readData.RemoveRange(index + 2, operatorLength);
            }
            else
            {
                //Add 0 values for lengths of operator and reference
                readData.AddRange(new ushort[] { 0, 0, 0 });
            }


            //Add Reference
            readData[index] = (ushort)referenceArray.Length;
            index++;
            readData.InsertRange(index, referenceArray.Select(a => (ushort)a).ToArray());
            index += referenceArray.Length;

            //Add Operator
            readData[index] = (ushort)operatorArray.Length;
            index++;
            readData.InsertRange(index, operatorArray.Select(a => (ushort)a).ToArray());
            index += operatorArray.Length;

            //Add Notes
            readData[index] = (ushort)notesArray.Length;
            index++;
            readData.InsertRange(index, notesArray.Select(a => (ushort)a).ToArray());
            index += notesArray.Length;

            //Write Data
            WriteData(_filename, readData.ToArray());
        }



        /// <summary>
        /// Open Graph of this log
        /// </summary>
        public ICommand OpenGraph
        {
            get
            {
                return new ButtonCommand((p) => {
                    var graph = new UserInterface.Views.GraphScreen(this);
                    graph.ShowDialog();
                });
            }
        }

        /// <summary>
        /// Export this graph to file
        /// </summary>
        public ICommand Export
        {
            get
            {
                return new ButtonCommand((p) => {
                    string path = string.Empty;

                    if (p as string == null)
                    {                        
                        var saveFileDialog = new SaveFileDialog()
                        {
                            FileName = StartTime.ToString("yyyy-MM-dd-HH-mm-ss"),
                            Filter = "PDF (*.PDF)|*.PDF",
                            AddExtension = true
                        };
                        if (saveFileDialog.ShowDialog() != true)
                            return;

                        path = saveFileDialog.FileName;
                    }
                    else
                        path = p as string;

                    Misc.Reporting.GenerateReport(path, this, null);
                });
            }
        }

        public ICommand Delete
        {
            get
            {
                return new ButtonCommand((p) => {

                    if (p == null)
                        if (new UserInterface.Views.MsgBox(Strings.ConfirmDeleteLog, UserInterface.Views.MsgBoxOptions.YesNo).ShowDialog() != true)
                            return;


                    if (IsUSB)
                        DeviceCommunications.RecoverManager.DeleteLog(Index);

                    App.Logs.Remove(this);
                    App.LogsChanged?.Invoke(null, null);
                });
            }
        }

        public ICommand Print
        {
            get
            {
                return new ButtonCommand((p) =>
                {
                    var printManager = new Misc.PrintManager();
                    printManager.PrintRecoverLog(this);
                });
            }
        }

        public PrintObject[] PrintObjects
        {
            get
            {
                return new PrintObject[]
                {
                    new PrintObject() { Description = Strings.BaseHeaterDeviation,      Value = BaseString,                                         IsPositive = BaseHeaterWithinTolerance },
                    new PrintObject() { Description = Strings.PrecursorHeaterDeviation, Value = PrecursorString,                                    IsPositive = PrecursorHeaterWithinTolerance},
                    new PrintObject() { Description = Strings.PressureDeviation,        Value = PressureString,                                     IsPositive = PressureDeviationWithinTolerance},
                    new PrintObject() { Description = Strings.LeakTest1Result,          Value = LeakTest1 ? Strings.TestSuccess : Strings.TestFail, IsPositive = LeakTest1},
                    new PrintObject() { Description = Strings.LeakTest2Result,          Value = LeakTest2 ? Strings.TestSuccess : Strings.TestFail, IsPositive = LeakTest2},
                    new PrintObject() { Description = Strings.PumpdownMinutes,          Value = TimeToReachVacuumSetpoint.ToString(),               IsPositive = null },
                    new PrintObject() { Description = Strings.HeatMinutes,              Value = HeatRunTime.ToString(),                             IsPositive = null },
                    new PrintObject() { Description = Strings.FirmwareVersion,          Value = FirmwareBuild.ToString(),                           IsPositive = null },
                    new PrintObject() { Description = Strings.SerialNumber,             Value = SerialNumber,                                       IsPositive = null },
                    new PrintObject() { Description = Strings.MetalType,                Value = MetalTypeString,                                    IsPositive = null },
                    new PrintObject() { Description = Strings.ChamberType,              Value = ChamberSizeString,                                  IsPositive = null },
                    new PrintObject() { Description = Strings.DevelopRType,             Value = DevelopRType,                                       IsPositive = null },
                };
            }
        }

        public ICommand Edit
        {
            get
            {
                return new ButtonCommand((p) =>
                {
                    var extraInformationWindow = new UserInterface.Views.ExtraInformationWindow();
                    if (extraInformationWindow.ShowDialog() != true)
                        return;

                    AddExtraInformation(
                        extraInformationWindow.CaseReference,
                        extraInformationWindow.CaseOperator,
                        extraInformationWindow.Notes);
                });
            }
        }


        private UInt16[] ReadData(string filename)
        {
            //Read Data
            byte[] data;
            int size;
            using (var stream = new System.IO.FileStream(filename, System.IO.FileMode.Open, System.IO.FileAccess.Read))
            {
                size = (int)stream.Length;
                data = new byte[size];
                stream.Read(data, 0, size);
            }


            //Copy array to 16 bit array
            var readData = new UInt16[size / 2];
            using (var memoryStream = new System.IO.MemoryStream(data))
            {
                using (var reader = new System.IO.BinaryReader(memoryStream))
                {
                    for (int i = 0; i < size / 2; i++)
                        readData[i] = reader.ReadUInt16();
                }
            }

            return readData;
        }

        private void WriteData(string filename, UInt16[] data16)
        {
            //Copy array to 8 bit array
            var data = new byte[data16.Length * 2];
            for (int i = 0; i < data.Length; i+=2)
            {
                data[i + 0] = (byte)(data16[i / 2] & 0xFF);
                data[i + 1] = (byte)(data16[i / 2] >> 8);
            }


            //Write Data
            using (var stream = new System.IO.FileStream(filename, System.IO.FileMode.Create, System.IO.FileAccess.ReadWrite))            
                stream.Write(data, 0, data.Length);            
        }

        public static RecoverLog Deserialize(string path)
        {
            RecoverLog result;

            var bytes = System.IO.File.ReadAllBytes(path);


            var serializer = new XmlSerializer(typeof(RecoverLog));
            using (var filestream = new MemoryStream(bytes))
            {
                var test = serializer.Deserialize(filestream);
                result = (RecoverLog)test;
            }

            return result;
        }

    }

}
