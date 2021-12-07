using FosterAndFreeman.RecoverCompanionApplication.Definitions.DeviceData;
using FosterAndFreeman.RecoverCompanionApplication.Resources.Languages;
using NUnit.Framework;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace RecoverCompanionApplication.Tests
{
    [TestFixture()]

    internal class RecoverLogTests
    {
        Random rnd = null;

        [SetUp]
        public void Setup()
        {
            rnd = new Random();
        }

        [Test]
        public void Init()
        {
            var log = new RecoverLog();
        }

        [Test]
        public void Index()
        {

            var log = new RecoverLog();
            log.Index = 0;
            
            Assert.AreEqual(0, log.Index);
            
            
        }

        [Test]
        public void Time()
        {
            var log = new RecoverLog();

            log.StartTime = DateTime.MaxValue;
            log.HeatRunTime = 22;
            log.NumberOfSamples = 23;
            log.TimeForPSW2ToOperate = 24;

            Assert.AreEqual(log.StartTime, DateTime.MaxValue);
            Assert.AreEqual(log.HeatRunTime, 22);
            Assert.AreEqual(log.NumberOfSamples, 23);
            Assert.AreEqual(log.TimeForPSW2ToOperate, 24);

        }

        [Test]
        public void BaseHeater()
        {
            var log = new RecoverLog();

            log.BaseHeaterSetPoint = 12;
            log.FinalBaseHeaterTemperature = 15;
            log.TimeToReachVacuumSetpoint = 17;

            Assert.AreEqual(12, log.BaseHeaterSetPoint);
            Assert.AreEqual(15, log.FinalBaseHeaterTemperature);
            Assert.AreEqual(17, log.TimeToReachVacuumSetpoint);



            Assert.AreEqual(25.0f, log.BaseHeaterSetpointDeviation);
            Assert.IsTrue(log.BaseHeaterWithinTolerance);
            log.FinalBaseHeaterTemperature = 1000;
            Assert.IsFalse(log.BaseHeaterWithinTolerance);
            log.FinalBaseHeaterTemperature = -100;
            Assert.IsFalse(log.BaseHeaterWithinTolerance);


            log.BaseHeaterSetPoint = 50;
            log.FinalBaseHeaterTemperature = 50;
            Assert.IsTrue(log.BaseHeaterWithinTolerance);
            Assert.AreEqual(Strings.TestSuccess, log.BaseString);
            log.FinalBaseHeaterTemperature = 10;
            Assert.IsFalse(log.BaseHeaterWithinTolerance);
            Assert.AreEqual(Strings.TestFail, log.BaseString);
            log.FinalBaseHeaterTemperature = 100;
            Assert.IsFalse(log.BaseHeaterWithinTolerance);
            Assert.AreEqual(Strings.TestFail, log.BaseString);
        }

        [Test]
        public void Precursor()
        {
            var log = new RecoverLog();

            log.PrecursorHeaterSetPoint = 13;
            log.FullPrecursorTemperature = 16;
            log.FinalPrecusorHeaterTemperature = 17;

            Assert.AreEqual(13, log.PrecursorHeaterSetPoint);
            Assert.AreEqual(16, log.FullPrecursorTemperature);
            Assert.AreEqual(17, log.FinalPrecusorHeaterTemperature);

            Assert.IsFalse(log.PrecursorHeaterWithinTolerance);
            Assert.AreEqual(23.0769234f, log.PrecusorSetpointDeviation);
            Assert.IsFalse(string.IsNullOrWhiteSpace(log.PrecursorString));

            log.PrecursorHeaterSetPoint = 100;
            log.FullPrecursorTemperature = 100;
            Assert.IsTrue(log.PrecursorHeaterWithinTolerance);
            Assert.AreEqual(Strings.TestSuccess, log.PrecursorString);
            log.FullPrecursorTemperature = 10;
            Assert.IsFalse(log.PrecursorHeaterWithinTolerance);
            Assert.AreEqual(Strings.TestFail, log.PrecursorString);
            log.FullPrecursorTemperature = 190;
            Assert.IsFalse(log.PrecursorHeaterWithinTolerance);
            Assert.AreEqual(Strings.TestFail, log.PrecursorString);
        }

        [Test]
        public void Pressure()
        {
            var log = new RecoverLog();

            log.VacuumSetPoint = 14;
            log.FinalPressureMeasurement = 18;
            log.LeakTest1Result = 19;
            log.LeakTest2Result = 20;
            log.PumpDownRunTime = 21;
            log.MaximumLeakSetting = 22;
            log.PressureWhenPSW2Operates = 23;
            log.LowVacSetting = 24;


            Assert.AreEqual(14, log.VacuumSetPoint);
            Assert.AreEqual(18, log.FinalPressureMeasurement);
            Assert.AreEqual(19, log.LeakTest1Result);
            Assert.AreEqual(20, log.LeakTest2Result);
            Assert.AreEqual(21, log.PumpDownRunTime);
            Assert.AreEqual(22, log.MaximumLeakSetting);
            Assert.AreEqual(23, log.PressureWhenPSW2Operates);
            Assert.AreEqual(24, log.LowVacSetting);

            Assert.IsTrue(log.PressureDeviationWithinTolerance);
            Assert.AreEqual(28.5714283f, log.PressureSetpointDeviation);
            Assert.IsFalse(string.IsNullOrWhiteSpace(log.PressureString));


            log.VacuumSetPoint = 100;
            log.FinalPressureMeasurement = 100;
            Assert.IsTrue(log.PressureDeviationWithinTolerance);
            Assert.AreEqual(Strings.TestSuccess, log.PressureString);
            log.FinalPressureMeasurement = 1200;
            Assert.IsFalse(log.PressureDeviationWithinTolerance);
            Assert.AreEqual(Strings.TestFail, log.PressureString);
            log.FinalPressureMeasurement = 0;
            Assert.IsFalse(log.PressureDeviationWithinTolerance);
            Assert.AreEqual(Strings.TestFail, log.PressureString);

            Assert.IsTrue(log.LeakTest1);
            Assert.IsTrue(log.LeakTest2);
        }

        [Test]
        public void Misc()
        {
            var log = new RecoverLog();


            log.ChamberSize = true;
            log.FilterCount = 123;
            log.TotalRunCount = 124;
            log.SampleRatePumpdown = 12;
            log.SampleRateDevelop = 13;
            log.FirmwareBuild = 14;
            log.SerialNumber = "1234-1121";
            log.MetalType = true;
            log.CaseReference = "Test-test";
            log.CaseOperator = "McTest Test";
            log.Notes = "Notes note notes note";

            Assert.IsTrue(log.ChamberSize);
            Assert.IsFalse(string.IsNullOrWhiteSpace(log.ChamberSizeString));
            Assert.AreEqual(12, log.SampleRatePumpdown);
            Assert.AreEqual(13, log.SampleRateDevelop);
            Assert.AreEqual(14, log.FirmwareBuild);
            Assert.AreEqual("1234-1121", log.SerialNumber);
            Assert.IsTrue(log.MetalType);
            Assert.AreEqual("Test-test", log.CaseReference);
            Assert.AreEqual("McTest Test", log.CaseOperator);
            Assert.AreEqual("Notes note notes note", log.Notes);


            Assert.AreEqual(123, log.FilterCount);
            Assert.AreEqual(log.TotalRunCount, log.TotalRunCount);
            Assert.IsFalse(string.IsNullOrWhiteSpace(log.MetalTypeString));
            Assert.IsFalse(string.IsNullOrWhiteSpace(log.DevelopRType));
            Assert.IsTrue(log.HasCaseReference);
            Assert.IsTrue(log.HasCaseOperator);
            Assert.IsTrue(log.HasNotes);
            Assert.IsTrue(log.HasExtraInformation);

            log.IsUSB = true;
            log.CaseReference = "AAAA";
            Assert.IsFalse(log.HasCaseReference);
            log.CaseOperator = "AAAA";
            Assert.IsFalse(log.HasCaseOperator);
            log.Notes = "AAAA";
            Assert.IsFalse(log.HasNotes);
            Assert.IsFalse(log.HasExtraInformation);

            log.ChamberSize = false;
            Assert.AreEqual(Strings.SmallChamber, log.ChamberSizeString);
            log.ChamberSize = true;
            Assert.AreEqual(Strings.LargeChamber, log.ChamberSizeString);

            log.FileName = "CCCC";
            Assert.AreEqual("CCCC", log.FileName);
            
        }

        [Test]
        public void DevelopR()
        {
            var log = new RecoverLog();

            log.ChamberSize = false;
            log.MetalType = false;
            Assert.AreEqual(Strings.DevelopR1, log.DevelopRType);

            log.ChamberSize = true;
            log.MetalType = false;
            Assert.AreEqual(Strings.DevelopR3, log.DevelopRType);

            log.ChamberSize = false;
            log.MetalType = true;
            Assert.AreEqual(Strings.DevelopR2, log.DevelopRType);

            log.ChamberSize = true;
            log.MetalType = true;
            Assert.AreEqual(Strings.DevelopR4, log.DevelopRType);

        }

        [Test]
        public void Samples()
        {
            var log = new RecoverLog();

            Assert.AreEqual(DateTime.MinValue, log.PumpDownStart);
            Assert.AreEqual(DateTime.MinValue, log.HeatStart);
            Assert.AreEqual(DateTime.MinValue, log.CoolStart);
            Assert.AreEqual(DateTime.MinValue, log.BreakSealStart);
            Assert.AreEqual(DateTime.MinValue, log.EndTime);


            var samples = new Sample[10]
            {
                new Sample("I\t1\t31\t12\t100\t0"),
                new Sample("I\t2\t30\t23\t90\t0"),
                new Sample("P\t3\t29\t50\t40\t0"),
                new Sample("P\t4\t39\t64\t10\t0"),
                new Sample("H\t5\t31\t84\t15\t0"),
                new Sample("H\t6\t32\t180\t12\t0"),
                new Sample("H\t7\t34\t192\t14\t0"),
                new Sample("C\t8\t31\t191\t11\t0"),
                new Sample("C\t9\t30\t40\t60\t0"),
                new Sample("B\t10\t30\t42\t90\t0"),
            };
            
            log.SerializableSamples = samples;
            Assert.IsTrue(log.Samples.SequenceEqual(samples));
            Assert.IsTrue(log.SerializableSamples.SequenceEqual(samples));

            log.SamplesLoaded = true;
            Assert.IsTrue(log.SamplesLoaded);

            Assert.IsNotNull(log.PumpDownStart);
            Assert.AreEqual(30, (log.PumpDownStart - new DateTime()).TotalSeconds);

            Assert.IsNotNull(log.HeatStart);
            Assert.AreEqual(50, (log.HeatStart - new DateTime()).TotalSeconds);

            Assert.IsNotNull(log.CoolStart);
            Assert.AreEqual(80, (log.CoolStart - new DateTime()).TotalSeconds);

            Assert.IsNotNull(log.BreakSealStart);
            Assert.AreEqual(100, (log.BreakSealStart - new DateTime()).TotalSeconds);

            Assert.IsNotNull(log.EndTime);
            Assert.AreEqual(100, (log.EndTime - new DateTime()).TotalSeconds);

        }

        [Test]
        public void ParseString()
        {
            var log = new RecoverLog();
            log.Parse(@"Thu Dec 20 09:35:17 2018	35	190	750	39	189	742	6	3	20	8	255	189	15	12	1	12	11	21	123	119	10 	10 	111	123-312	000000	0	1234512	0	0	0	0	0	1231212", 0);
            log.Parse(@"", 0);
            log.Parse(@"Thu Dec 20 09:35:17 2018	35	190	750	39	189	742	6	3	20	8	1101	189	15	12	1	12	11	21	123	119	10 	10 	111	123-312	000000	0	1234512	0	0	0	0	0	1231212", 0);
            log.Parse(@"Thu Dec 20 09:35:17 2018	35	190	750	39	189	742	6	3	20	8	\	189	15	12	1	12	11	21	123	119	10 	10 	111	123-312	000000	0	1234512	0	0	0	0	0	1231212", 0);
        }

        [Test]
        public void ParseFile()
        {
            var logPathA = Extract("RecoverCompanionApplication.Tests", "test", "Resources", "2019-02-11-09-22-16.recover");
            var logPathB = Extract("RecoverCompanionApplication.Tests", "test", "Resources", "2019-02-18-14-21-22.recover");

            var log = new RecoverLog();
            log.Parse(logPathA);
            log.Parse(logPathB);

            Directory.Delete("test", true);
            
        }

        [Test]
        public void Save()
        {
            var log = new RecoverLog();
            log.Parse(@"Thu Dec 20 09:35:17 2018	35	190	750	39	189	742	6	3	20	8	255	189	15	12	1	12	11	21	123	119	10 	10 	111	123-152312	000000	0	1234512	0	0	0	0	0	1231212", 0);
            log.Save.Execute("test");

            File.Delete("test");
        }

        [Test]
        public void AddExtraInfo()
        {            
            var logPath = Extract("RecoverCompanionApplication.Tests", "test", "Resources", "2019-02-18-14-21-22.recover");
            var log = new RecoverLog();
            log.Parse(logPath);

            log.AddExtraInformation("TestCase", "TestOperator", "TestNotes");

        }


        /// <summary>
        /// Extract an embedded resource to a folder
        /// </summary>
        /// <param name="nameSpace">The name space of the project (e.g. WindowsSetup)</param>
        /// <param name="outDirectory">The directory where the resource will go</param>
        /// <param name="internalFilePath">
        /// The internal directory that holds the resource that wants to be extracted
        /// </param>
        /// <param name="resourceName">The name of the resource you want to extract</param>
        public static string Extract(string nameSpace, string outDirectory, string internalFilePath, string resourceName)
        {
            System.IO.Directory.CreateDirectory(outDirectory);

            var assembly = Assembly.GetCallingAssembly();
            var resourceLocation = nameSpace + "." + (internalFilePath == "" ? "" : internalFilePath + ".") + resourceName;
            var outPath = Path.Combine(outDirectory, resourceName);
            using (var s = assembly.GetManifestResourceStream(resourceLocation))
            {
                using (var r = new BinaryReader(s))
                {
                    using (var fs = new FileStream(outPath, FileMode.OpenOrCreate))
                    {
                        using (var w = new BinaryWriter(fs))
                        {
                            w.Write(r.ReadBytes((int)s.Length));
                            return outPath;
                        }
                    }
                }
            }
        }
    }
}
