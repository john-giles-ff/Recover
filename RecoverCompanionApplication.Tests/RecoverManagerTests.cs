using FosterAndFreeman.RecoverCompanionApplication.Definitions.DeviceCommunications;
using NUnit.Framework;
using System;

namespace RecoverCompanionApplication.Tests
{
    [TestFixture()]
    public class RecoverManagerTests
    {
        [SetUp]
        public void Setup()
        {

        }

        [Test]
        public void Init()
        {
            RecoverManager.Initialise();
        }
    }
}
