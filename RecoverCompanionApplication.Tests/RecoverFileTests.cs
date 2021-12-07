using FosterAndFreeman.RecoverCompanionApplication.Definitions.DeviceData;
using NUnit.Framework;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RecoverCompanionApplication.Tests
{
    [TestFixture()]
    internal class RecoverFileTests
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
            var recoverFile = new RecoverFile();
        }

        [Test]
        public void AddFirmware()
        {
            var a = new byte[512];
            rnd.NextBytes(a);

            var recoverFile = new RecoverFile();
            recoverFile.AddFirmware(a, a.Length);


            var input = string.Join(",", a);
            var result = string.Join(",", recoverFile.GetBytes());
            Assert.IsTrue(result.Contains(input));
            Assert.IsTrue(a.SequenceEqual(recoverFile.GetFirmwareData()));

        }

        [Test]
        public void AddSoftware()
        {
            var a = new byte[512];
            rnd.NextBytes(a);

            var recoverFile = new RecoverFile();
            recoverFile.AddSoftware(a, a.Length);

            var input = string.Join(",", a);
            var result = string.Join(",", recoverFile.GetBytes());
            Assert.IsTrue(result.Contains(input));
            Assert.IsTrue(a.SequenceEqual(recoverFile.GetSoftwareData()));
        }

        [Test]
        public void AddFirmwareSoftware()
        {
            var a = new byte[512];
            var b = new byte[512];
            rnd.NextBytes(a);
            rnd.NextBytes(b);

            var recoverFile = new RecoverFile();
            recoverFile.AddFirmware(a, a.Length);
            recoverFile.AddSoftware(b, b.Length);


            var inputA = string.Join(",", a);
            var inputB = string.Join(",", b);
            var result = string.Join(",", recoverFile.GetBytes());
            Assert.IsTrue(result.Contains(inputA));
            Assert.IsTrue(result.Contains(inputB));

        }

        [Test]
        public void WriteReadTest()
        {
            var a = new byte[512];
            var b = new byte[512];
            rnd.NextBytes(a);
            rnd.NextBytes(b);

            var recoverFile = new RecoverFile();
            recoverFile.AddFirmware(a, a.Length);
            recoverFile.AddSoftware(b, b.Length);

            recoverFile.GenerateFile("Test.tst");

            var readFile = new RecoverFile("Test.tst");

            Assert.IsTrue(recoverFile.GetBytes().SequenceEqual(readFile.GetBytes()));
            Assert.IsTrue(readFile.Compare(recoverFile));
        }

        [Test]
        public void GetBytesInvalid()
        {
            var a = new RecoverFile();
            Assert.Throws<InvalidOperationException>(() => { a.GetBytes(); });
        }

        [Test]
        public void InvalidDecode()
        {
            Assert.Throws<InvalidCastException>(() => { new RecoverFile("   "); });
            Assert.Throws<InvalidCastException>(() => { new RecoverFile("NO SUCH FILE"); });
        }

    }
}
