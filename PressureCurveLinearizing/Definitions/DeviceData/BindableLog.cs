using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using PressureCurveLinearizing.Definitions.BaseClasses;
using RecoverLogInspector;
using PressureCurveLinearizing.ViewModels;
using System.Windows.Media;
using LiveCharts.Defaults;
using System.Diagnostics;

namespace PressureCurveLinearizing.Definitions.DeviceData
{
    public struct DeltaStruct
    {
        public double Pressure { get; set; }
        public double Delta { get; set; }
    }


    public class BindableLog : PropertyChangedBase
    {
        private LiveCharts.SeriesCollection _chartData;
        private LiveCharts.ChartValues<double> _originalSeries;        
        private LiveCharts.ChartValues<double> _minimumFixedSeries;
        private LiveCharts.ChartValues<double> _averageFixedSeries;
        private LiveCharts.ChartValues<double> _fixedSeries;
        private LiveCharts.ChartValues<ObservablePoint> _deltaSeries;
        private LiveCharts.ChartValues<ObservablePoint> _avgDeltaSeries;
        private LiveCharts.ChartValues<ObservablePoint> _cAvgDeltaSeries;
        private LiveCharts.ChartValues<ObservablePoint> _avgGlobalDeltaSeries;
        private LiveCharts.ChartValues<ObservablePoint> _minGlobalDeltaSeries;

        private double[] _sampleValues;
        private Section[] _individualSections;
        private double lastNumber;
        private ObservablePoint[] _deltaPressureValues;

        public ObservablePoint[] DeltaPressureValues => _deltaPressureValues;
      


        public LiveCharts.SeriesCollection ChartData
        {
            get
            {
                return _chartData;
            }
            set
            {
                _chartData = value;
                RaisePropertyChanged();
            }
        }


        public Section[] IndividualSections
        {
            get
            {
                return _individualSections;
            }
            set
            {
                _individualSections = value;
                RaisePropertyChanged();
            }
        }
        

        public static BindableLog Parse (RecoverLog log, int sectionCount)
        {
            //Work out sections for log

            var ret = new BindableLog();


            var relevantValues = log.SerializableSamples.Samples.Where(a => a.Mode == SampleMode.SAMPLE_PUMPDOWN && a.Pressure < 20000 && a.Pressure > 800);
            ret._sampleValues = relevantValues.Select(a => (double)a.Pressure).ToArray();
            if (!ret._sampleValues.Any())
                return null;

            ret._deltaPressureValues = 
                relevantValues
                .Zip(relevantValues.Skip(1), (current, next) =>
                {
                    return new ObservablePoint( 
                        next.Pressure,
                        Math.Abs(next.Pressure - current.Pressure)
                    );
                })
                .OrderByDescending(a => a.X)
                .ToArray();


            //Workout average delta for single log
            ret._avgDeltaSeries = new LiveCharts.ChartValues<ObservablePoint>();
            ret._cAvgDeltaSeries = new LiveCharts.ChartValues<ObservablePoint>();
            var recentValCount = 5;
            var recentVals = Enumerable.Repeat(-1.0, recentValCount).ToArray();
            var recentValIndex = 0;

            var cAvg = 0.0;
            var cAvgCount = 0.0;

            foreach (var delta in ret._deltaPressureValues.Where(a => a.X < 10000))
            {

                cAvg = (delta.Y + (cAvg * cAvgCount)) / (cAvgCount + 1);
                cAvgCount = Math.Min(recentValCount, cAvgCount + 1);
                ret._cAvgDeltaSeries.Add(new ObservablePoint(delta.X, cAvg));


                recentValIndex %= recentValCount;
                recentVals[recentValIndex++] = delta.Y;
                ret._avgDeltaSeries.Add(new ObservablePoint(delta.X, recentVals.Where(a => a > 0).Average()));
            }


            var sections = new Section[sectionCount];
            int length = ret._sampleValues.Count();
            int single = length / sections.Length;
            for (int i = 0; i < sections.Length; i++)
            {
                var xStart = single * i;
                var xEnd = Math.Min(single * (i + 1), ret._sampleValues.Length - 1);
                sections[i].ValueStart = ret._sampleValues[xStart];
                sections[i].ValueEnd = ret._sampleValues[xEnd];
                sections[i].ProgressRange = 100 / sectionCount;

                if (i == sections.Length - 1)
                    sections[i].ValueEnd = 780; //Final mTorr = 780
            }
            ret.IndividualSections = sections;

            //Define Series
            ret.lastNumber = 0;
            ret._originalSeries = new LiveCharts.ChartValues<double>(ret._sampleValues);
            ret._fixedSeries = new LiveCharts.ChartValues<double>(ret._sampleValues.Select(a => ret.TranslatePoint(a, ret.IndividualSections)));
            ret._minimumFixedSeries = new LiveCharts.ChartValues<double>();
            ret._averageFixedSeries = new LiveCharts.ChartValues<double>();
            ret._deltaSeries = new LiveCharts.ChartValues<ObservablePoint>(ret._deltaPressureValues);
            ret._avgGlobalDeltaSeries = new LiveCharts.ChartValues<ObservablePoint>();
            ret._minGlobalDeltaSeries = new LiveCharts.ChartValues<ObservablePoint>();

            //Define Chart Data
            ret.ChartData = new LiveCharts.SeriesCollection()
            {                
                /*
                new LiveCharts.Wpf.LineSeries() { Values = ret._originalSeries, ScalesXAt = 0, ScalesYAt = 0, Fill = Brushes.Transparent, Stroke = Brushes.Purple},
                new LiveCharts.Wpf.LineSeries() { Values = ret._fixedSeries, PointGeometry = null, ScalesXAt = 0, ScalesYAt= 1, Fill = Brushes.Transparent, Stroke=Brushes.Green },
                new LiveCharts.Wpf.LineSeries() { Values = ret._minimumFixedSeries, PointGeometry = null, ScalesXAt = 0, ScalesYAt = 1, Fill = Brushes.Transparent, Stroke=Brushes.Red },
                new LiveCharts.Wpf.LineSeries() { Values = ret._averageFixedSeries, PointGeometry = null, ScalesXAt = 0, ScalesYAt = 1, Fill = Brushes.Transparent, Stroke=Brushes.Blue },                                
                */
                new LiveCharts.Wpf.LineSeries() { Values = ret._deltaSeries, ScalesXAt = 1, ScalesYAt = 2, Fill = Brushes.Transparent, Stroke=Brushes.HotPink },
                new LiveCharts.Wpf.LineSeries() { Values = ret._avgDeltaSeries, PointGeometry = null, ScalesXAt = 1, ScalesYAt = 2, Fill = Brushes.Transparent, Stroke=Brushes.Blue },
                new LiveCharts.Wpf.LineSeries() { Values = ret._cAvgDeltaSeries, PointGeometry = null, ScalesXAt = 1, ScalesYAt = 2, Fill = Brushes.Transparent, Stroke=Brushes.LightSkyBlue },
                new LiveCharts.Wpf.LineSeries() { Values = ret._avgGlobalDeltaSeries, PointGeometry = null, ScalesXAt = 1, ScalesYAt = 2, Fill = Brushes.Transparent, Stroke=Brushes.Tan},
                new LiveCharts.Wpf.LineSeries() { Values = ret._minGlobalDeltaSeries, PointGeometry = null, ScalesXAt = 1, ScalesYAt = 2, Fill = Brushes.Transparent, Stroke=Brushes.Orange},               
            };

            return ret;
        }        
        
        public void SetGenericSections(Section[] minimumSections, Section[] averageSections)
        {
            lastNumber = 0;
            _minimumFixedSeries.AddRange(_sampleValues.Select(a => TranslatePoint(a, minimumSections)).ToArray());
            lastNumber = 0;
            _averageFixedSeries.AddRange(_sampleValues.Select(a => TranslatePoint(a, averageSections)).ToArray());
        }

        public void SetAvgDelta(ObservablePoint[] values, float minPercentage)
        {
            //Set avg value
            _avgGlobalDeltaSeries.AddRange(values);

            //Find and set min values
            var minValues = values.Select(a => new ObservablePoint(a.X, a.Y * minPercentage));
            _minGlobalDeltaSeries.AddRange(minValues);


            //Check if any of this delta is below min
            foreach (var deltaPressure in _avgDeltaSeries)
            {
                var pressure = deltaPressure.X;
                var delta = deltaPressure.Y;

                var minDelta = FindMinAtPressure(pressure);
                if (minDelta == -1)
                    continue;

                if (minDelta > delta)
                {                    
                    Trace.WriteLine($"Pressure {pressure}: MinDelta({minDelta}) > Delta({delta})");
                }

            }

        }

        private double FindMinAtPressure(double pressure)
        {
            for (int i = 1; i < _minGlobalDeltaSeries.Count; i++)
            {
                //Check if pressure is between this and pressure item
                if (_minGlobalDeltaSeries[i].X > pressure)
                    continue;

                //Fetch min + max
                var smallPoint = _minGlobalDeltaSeries[i - 1];
                var largePoint = _minGlobalDeltaSeries[i];

                //Find percentage between relevant points
                var pressureDistance = Math.Abs(largePoint.X - smallPoint.X);
                var pressurePercentage = Math.Abs(pressure - smallPoint.X) / pressureDistance;

                //Get value
                var deltaDistance = Math.Abs(largePoint.Y - smallPoint.Y);
                return smallPoint.Y - (pressurePercentage * deltaDistance);
            }

            return -1;
        }

        

        private double TranslatePoint(double input, Section[] sections)
        {
            double offset = 0;
            var last = sections.Last();
            foreach (var section in sections)
            {
                if (input > section.ValueEnd || section == last)
                {
                    var basedValue = section.ValueStart - input;
                    var decValue = basedValue / (section.ValueStart - section.ValueEnd);
                    var adjustedPercentage = offset + (section.ProgressRange * decValue);

                    if (lastNumber > adjustedPercentage)
                        adjustedPercentage = lastNumber;
                    lastNumber = adjustedPercentage;

                    return adjustedPercentage;
                }

                offset += section.ProgressRange;
            }

            return 0;
        }
    }
}
