using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using PressureCurveLinearizing.Definitions.BaseClasses;
using RecoverLogInspector;
using PressureCurveLinearizing.ViewModels;
using System.Windows.Media;

namespace PressureCurveLinearizing.Definitions.DeviceData
{


    public class BindableLog : PropertyChangedBase
    {
        private LiveCharts.SeriesCollection _chartData;
        private LiveCharts.ChartValues<double> _originalSeries;        
        private LiveCharts.ChartValues<double> _minimumFixedSeries;
        private LiveCharts.ChartValues<double> _averageFixedSeries;
        private LiveCharts.ChartValues<double> _fixedSeries;

        private double[] _sampleValues;
        private Section[] _individualSections;
        private double lastNumber;
        

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
        

        public BindableLog(RecoverLog log, int sectionCount)
        {
            //Work out sections for log            
            _sampleValues = log.SerializableSamples.Samples.Where(a => a.Mode == SampleMode.SAMPLE_PUMPDOWN && a.Pressure < 20000 && a.Pressure > 800).Select(a => (double)a.Pressure).ToArray();

            var sections = new Section[sectionCount];
            int length = _sampleValues.Count();
            int single = length / sections.Length;
            for (int i = 0; i < sections.Length; i++)
            {
                var xStart = single * i;
                var xEnd = single * (i + 1);
                sections[i].ValueStart = _sampleValues[xStart];
                sections[i].ValueEnd = _sampleValues[xEnd];
                sections[i].ProgressRange = 100 / sectionCount;

                if (i == sections.Length - 1)
                    sections[i].ValueEnd = 780;
            }
            IndividualSections = sections;

            //Define Series
            lastNumber = 0;
            _originalSeries = new LiveCharts.ChartValues<double>(_sampleValues);            
            _fixedSeries = new LiveCharts.ChartValues<double>(_sampleValues.Select(a => TranslatePoint(a, IndividualSections)));
            _minimumFixedSeries = new LiveCharts.ChartValues<double>();
            _averageFixedSeries = new LiveCharts.ChartValues<double>();


            //Define Chart Data
            ChartData = new LiveCharts.SeriesCollection()
            {
                new LiveCharts.Wpf.LineSeries() { Values = _originalSeries, PointGeometry = null, ScalesYAt = 0, Fill = Brushes.Transparent, Stroke = Brushes.Purple},
                new LiveCharts.Wpf.LineSeries() { Values = _fixedSeries, PointGeometry = null, ScalesYAt= 1, Fill = Brushes.Transparent, Stroke=Brushes.Green },
                new LiveCharts.Wpf.LineSeries() { Values = _minimumFixedSeries, PointGeometry = null, ScalesYAt = 1, Fill = Brushes.Transparent, Stroke=Brushes.Red },
                new LiveCharts.Wpf.LineSeries() { Values = _averageFixedSeries, PointGeometry = null, ScalesYAt = 1, Fill = Brushes.Transparent, Stroke=Brushes.Blue },
            };
        }        
        
        public void SetGenericSections(Section[] minimumSections, Section[] averageSections)
        {
            lastNumber = 0;
            _minimumFixedSeries.AddRange(_sampleValues.Select(a => TranslatePoint(a, minimumSections)).ToArray());
            lastNumber = 0;
            _averageFixedSeries.AddRange(_sampleValues.Select(a => TranslatePoint(a, averageSections)).ToArray());
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
