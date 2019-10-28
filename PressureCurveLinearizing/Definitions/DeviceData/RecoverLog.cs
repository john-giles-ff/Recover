using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Serialization;
using PressureCurveLinearizing.Definitions.BaseClasses;

namespace RecoverLogInspector
{
    [XmlRoot("RecoverLog")]
    public class RecoverLog : PropertyChangedBase
    {
        [XmlElement("Index")]
        public int Index { get; set; }
        
        [XmlElement("StartTime")]
        public DateTime StartTime { get; set; }

        [XmlElement("BaseHeaterSetPoint")]
        public int BaseHeaterSetPoint { get; set; }

        [XmlElement("PrecursorHeaterSetPoint")]
        public int PrecursorHeaterSetPoint { get; set; }

        [XmlElement("VacuumSetPoint")]
        public int VacuumSetPoint { get; set; }

        [XmlElement("FinalBaseHeaterTemperature")]
        public int FinalBaseHeaterTemperature { get; set; }

        [XmlElement("FullPrecursorTemperature")]
        public int FullPrecursorTemperature { get; set; }

        [XmlElement("FinalPrecusorHeaterTemperature")]
        public int FinalPrecusorHeaterTemperature { get; set; }

        [XmlElement("FinalPressureMeasurement")]
        public int FinalPressureMeasurement { get; set; }

        [XmlElement("LeakTest1Result")]
        public int LeakTest1Result { get; set; }

        [XmlElement("LeakTest2Result")]
        public int LeakTest2Result { get; set; }

        [XmlElement("PumpDownRunTime")]
        public int PumpDownRunTime { get; set; }

        [XmlElement("HeatRunTime")]
        public int HeatRunTime { get; set; }

        [XmlElement("NumberOfSamples")]
        public int NumberOfSamples { get; set; }

        [XmlElement("TimeToReachVacuumSetpoint")]
        public int TimeToReachVacuumSetpoint { get; set; }

        [XmlElement("MaximumLeakSetting")]
        public int MaximumLeakSetting { get; set; }

        [XmlElement("ChamberSize")]
        public bool ChamberSize { get; set; }

        [XmlElement("FilterCount")]
        public int FilterCount { get; set; }

        [XmlElement("TotalRunCount")]
        public int TotalRunCount { get; set; }

        [XmlElement("TimeForPSW2ToOperate")]
        public int TimeForPSW2ToOperate { get; set; }

        [XmlElement("PressureWhenPSW2Operates")]
        public int PressureWhenPSW2Operates { get; set; }

        [XmlElement("LowVacSetting")]
        public int LowVacSetting { get; set; }

        [XmlElement("SampleRatePumpdown")]
        public int SampleRatePumpdown { get; set; }

        [XmlElement("SampleRateDevelop")]
        public int SampleRateDevelop { get; set; }

        [XmlElement("FirmwareBuild")]
        public int FirmwareBuild { get; set; }

        [XmlElement("SerialNumber")]
        public String SerialNumber { get; set; }

        [XmlElement("MetalType")]
        public bool MetalType { get; set; }

        [XmlElement("SerializableSamples")]
        public SerializableSamples SerializableSamples { get; set; }

        [XmlElement("IsUSB")]
        public bool IsUSB { get; set; }

        [XmlIgnore]
        public string Path { get; set; }






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

    [XmlRoot("SerializableSamples")]
    public struct SerializableSamples
    {
        [XmlElement("Sample")]
        public List<Sample> Samples;
    }

    public enum SampleMode
    {
        SAMPLE_UNKNOWN,
        SAMPLE_INITIALISE,
        SAMPLE_PUMPDOWN,
        SAMPLE_HEAT,
        SAMPLE_COOL,
        SAMPLE_BREAKSEAL
    }

    [XmlRoot("Sample")]
    public class Sample
    {
        [XmlElement("Mode")]
        public SampleMode Mode { get; set; }

        [XmlElement("SampleNumber")]
        public int SampleNumber { get; set; }

        [XmlElement("BaseTemperature")]
        public int BaseTemperature { get; set; }

        [XmlElement("PrecursorTemperature")]
        public int PrecursorTemperature { get; set; }

        [XmlElement("Pressure")]
        public int Pressure { get; set; }

        [XmlElement("Status")]
        public int Status { get; set; }
    }
}
