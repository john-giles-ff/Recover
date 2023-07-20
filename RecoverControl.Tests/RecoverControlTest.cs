using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;

using FosterAndFreeman;
using NUnit.Framework;

namespace FosterAndFreeman.Tests
{
    [TestFixture()]
    public class RecoverControlTest
    {
        [SetUp]
        public void Setup()
        {

        }

        [Test]
        public void Init()
        {
            var recoverControl = new RecoverControl();
            recoverControl.Init();
        }

        [Test]
        public void Find()
        {
            var recoverControl = new RecoverControl();
            recoverControl.Init();

            recoverControl.FindPortName(PortType.APP_CRC);
            recoverControl.FindPortName(PortType.BL_CRC);
            recoverControl.FindPortName(PortType.BL_DFU);
        }
    }
}
