﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Serialization;

namespace RecoverLogInspector
{
    public class SettingsManager
    {
        public void Save()
        {
            if (System.IO.File.Exists(PathLocation))
                System.IO.File.Delete(PathLocation);

            using (var writer = System.IO.File.Create(PathLocation))
            {
                var serializer = new XmlSerializer(this.GetType());
                serializer.Serialize(writer, this);
            }
        }

        public static SettingsManager Load()
        {
            if (!System.IO.File.Exists(PathLocation))
                return new SettingsManager();

            using (var reader = System.IO.File.OpenRead(PathLocation))
            {
                var serializer = new XmlSerializer(typeof(SettingsManager));
                return serializer.Deserialize(reader) as SettingsManager;
            }


        }        


        //Average Settings
        public int AveragePeakBaseTemperatureLCL { get; set; }
        public int AveragePeakBaseTemperatureHCL { get; set; }
        public int AveragePeakBaseTemperatureAVG { get; set; }
                   
                   
        public int AverageBaseTimeLCL { get; set; }
        public int AverageBaseTimeHCL { get; set; }
        public int AverageBaseTimeAVG { get; set; }
                   
                   
        public int AveragePumpdownTimeLCL { get; set; }
        public int AveragePumpdownTimeHCL { get; set; }
        public int AveragePumpdownTimeAVG { get; set; }
                   
                   
        public int AveragePrecursorTimeLCL { get; set; }
        public int AveragePrecursorTimeHCL { get; set; }
        public int AveragePrecursorTimeAVG { get; set; }
                   
                   
        //Individual
        public int IndividualPeakBaseTemperatureLCL { get; set; }
        public int IndividualPeakBaseTemperatureHCL { get; set; }
        public int IndividualPeakBaseTemperatureAVG { get; set; }
                   
                   
        public int IndividualBaseTimeLCL { get; set; }
        public int IndividualBaseTimeHCL { get; set; }
        public int IndividualBaseTimeAVG { get; set; }
                   
        public int IndividualPumpdownTimeLCL { get; set; }
        public int IndividualPumpdownTimeHCL { get; set; }
        public int IndividualPumpdownTimeAVG { get; set; }
                   
        public int IndividualPrecursorTimeLCL { get; set; }
        public int IndividualPrecursorTimeHCL { get; set; }
        public int IndividualPrecursorTimeAVG { get; set; }


        private static string PathLocation
        {
            get
            {
                return System.IO.Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "Settings.ini");
            }
        }
    }
}
