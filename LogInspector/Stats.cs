using System;
using System.Collections.Generic;
using System.Linq;

namespace RecoverLogInspector
{
    public class Stats
    {        
        /// <summary>
        /// Takes log data and assigns stats to properties
        /// </summary>
        /// <param name="logs"></param>
        public Stats(List<RecoverLog> logs)
        {
            if (logs.Count() == 0)
                return;

            PumpdownTimes = new List<int>();
            PrecursorTimes = new List<int>();
            BaseTimes = new List<int>();
            BaseTemps = new List<int>();

            foreach (var log in logs)
            {
                // Skip failed logs
                if (log.FinalBaseHeaterTemperature == 0 || log.FullPrecursorTemperature == 0)
                    continue;

                // Just to shorten the code
                var samples = log.SerializableSamples.Samples;

                // Get peak temperatures
                int peakPrecursorTemp = samples.Max(a => a.PrecursorTemperature);
                int peakBaseTemp = samples.Where(a => a.Mode != SampleMode.SAMPLE_INITIALISE).Max(a => a.BaseTemperature);

                // Get peak samples
                var peakPrecursorSample = samples.FirstOrDefault(a => a.PrecursorTemperature == peakPrecursorTemp);
                var peakBaseSample = samples.Where(a => a.Mode != SampleMode.SAMPLE_INITIALISE).First(a => a.BaseTemperature == peakBaseTemp);

                // Get reference points
                var firstPumpSample = samples.FirstOrDefault(a => a.Mode == SampleMode.SAMPLE_PUMPDOWN);
                var firstHeatSample = samples.FirstOrDefault(a => a.Mode == SampleMode.SAMPLE_HEAT);

                // Get times
                int pumpdownTime = (firstHeatSample.SampleNumber - firstPumpSample.SampleNumber) * log.SampleRatePumpdown;
                int precursorTime = (peakPrecursorSample.SampleNumber - firstHeatSample.SampleNumber) * log.SampleRateDevelop;
                int baseTime = (peakBaseSample.SampleNumber - firstPumpSample.SampleNumber) * log.SampleRateDevelop;

                // Store values
                PumpdownTimes.Add(pumpdownTime);
                PrecursorTimes.Add(precursorTime);
                BaseTimes.Add(baseTime);
                BaseTemps.Add(peakBaseTemp);
            }

            StartTimes = logs.Select(a => a.StartTime).ToList();

            // Calculate stats for pumpdown time
            AvgTime_Pumpdown = Math.Round(PumpdownTimes.Average(), 2);
            SdTime_Pumpdown = CalculateStdDev(PumpdownTimes);
            PumpdownQs = GetQuartiles(PumpdownTimes);

            // Calculate stats for base temperature reached
            AvgTemp_PeakBase = Math.Round(BaseTemps.Average(), 2);
            SdTemp_PeakBase = CalculateStdDev(BaseTemps);
            BaseTempQs = GetQuartiles(BaseTemps);

            // Calculate stats for time taken to get to peak base temperature
            AvgTime_PeakBase = Math.Round(BaseTimes.Average(), 2);
            SdTime_PeakBase = CalculateStdDev(BaseTimes);
            BaseTimeQs = GetQuartiles(BaseTimes);

            // Calculate stats for time taken to get to peak precursor temperature
            AvgTime_Precursor = Math.Round(PrecursorTimes.Average(), 2);
            SdTime_Precursor = CalculateStdDev(PrecursorTimes);
            PrecursorQs = GetQuartiles(PrecursorTimes);
        }

        #region public properties
        public List<int> PumpdownTimes { get; }
        public List<int> PrecursorTimes { get; }
        public List<int> BaseTimes { get; }
        public List<int> BaseTemps { get; }
        
        public List<DateTime> StartTimes { get; }

        public double SdTime_Pumpdown   { get; }
        public Quartiles PumpdownQs     { get; }
        public double AvgTime_Pumpdown  { get; }

        public double AvgTemp_PeakBase  { get; }
        public double SdTemp_PeakBase   { get; }
        public Quartiles BaseTempQs     { get; }

        public double AvgTime_PeakBase  { get; }
        public double SdTime_PeakBase   { get; }
        public Quartiles BaseTimeQs     { get; }

        public double AvgTime_Precursor { get; }
        public double SdTime_Precursor  { get; }
        public Quartiles PrecursorQs    { get; }
        #endregion

        /// <summary>
        /// Calculates the population standard deviation from a set of values
        /// </summary>
        /// <param name="values">The set of values</param>
        /// <returns>The population standard deviation of the values</returns>
        private double CalculateStdDev(IEnumerable<int> values)
        {
            double avg = values.Average();
            return Math.Sqrt(values.Average(v => (v - avg) * (v - avg)));
        }


        /// <summary>
        /// Calculates the minimum, median, maximum, and quartiles for a set of data
        /// </summary>
        /// <param name="Values">The data to be represented</param>
        /// <returns>Quartiles struct with calculated values</returns>
        private Quartiles GetQuartiles(List<int> Values)
        {
            var sorted = Values.OrderBy(value => value).ToArray();

            return new Quartiles
            {
                Min = sorted[0],
                 Q1 = sorted[(sorted.Length / 4) - 1],
             Median = sorted[(sorted.Length / 2) - 1],
                 Q3 = sorted[(int) Math.Round(sorted.Length * 0.75) - 1],
                Max = sorted[sorted.Length - 1]
            };
        }


        /// <summary>
        /// Stores the minimum, lower quartile, median, upper quartile, and maximum of a set of values.
        /// </summary>
        public struct Quartiles
        {
            public int Min;
            public int Q1;
            public int Median;
            public int Q3;
            public int Max;
        }
    }
}
