using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using PressureCurveLinearizing.Definitions.BaseClasses;
using PressureCurveLinearizing.Definitions.WPF;
using RecoverLogInspector;
using System.Windows.Input;
using PressureCurveLinearizing.Definitions.DeviceData;
using LiveCharts.Defaults;

namespace PressureCurveLinearizing.ViewModels
{
    public struct Section
    {
        public double ValueStart { get; set; }
        public double ValueEnd { get; set; }
        public double ProgressRange { get; set; }

        public override string ToString()
        {
            return $"Value Start - {ValueStart}, Value End - {ValueEnd}, Progress Range = {ProgressRange}";
        }

        public static bool operator ==(Section section1, Section section2)
        {
            return section1.Equals(section2);
        }
        public static bool operator !=(Section section1, Section section2)
        {
            return !section1.Equals(section2);
        }
    }

    public class MainWindowViewModel : PropertyChangedBase
    {        
        private string _logLocation;
        private List<RecoverLog> _logs = new List<RecoverLog>();
        private List<BindableLog> _bindableLogs = new List<BindableLog>();

        Section[] AverageSections;
        Section[] MinimumSections;        
        private int _sectionCount = 10;

        ObservablePoint[] _averageDeltaPressurePoints;

        public int SectionCount
        {
            get
            {
                return _sectionCount;
            }
            set
            {
                _sectionCount = value;
                RaisePropertyChanged();
            }
        }

        public void Init()
        {
            var path = @"F:\Users\Alex\Desktop\Logs\RAW\Recover LFT\1236-00007";
            if (System.IO.Directory.Exists(path))                            
                LoadLogs.Execute(path);
            
        }

        public string LogLocation
        {
            get => _logLocation;            
            set
            {
                _logLocation = value;
                RaisePropertyChanged();
            }
        }

        public List<RecoverLog> Logs
        {
            get => _logs;
            set
            {
                _logs = value;
                RaisePropertyChanged();                
            }
        }

        public List<BindableLog> BindableLogs
        {
            get => _bindableLogs;
            set
            {
                _bindableLogs = value;
                RaisePropertyChanged();
                RaisePropertyChanged("FirstLog");
            }

        }

        public ICommand BrowseCommand
        {
            get
            {
                return new ButtonCommand((a) =>
                {               
                    var dialog = new Ookii.Dialogs.Wpf.VistaFolderBrowserDialog()
                    {
                        ShowNewFolderButton = false,
                        Description = "Select folder with logs in",
                    };

                    if (dialog.ShowDialog() != true)
                        return;

                    LogLocation = dialog.SelectedPath;                    
                });
            }
        }

        public ICommand LoadLogs
        {
            get
            {
                return new ButtonCommand(p =>
                {
                    string path = p as string;
                    if (string.IsNullOrWhiteSpace(path))
                        path = LogLocation;
                    else
                        LogLocation = path;                    
                    BindableLogs.Clear();
                    GC.Collect();

                    //Load logs (fetches every *.xml under the provided directory)
                    var files = System.IO.Directory.EnumerateFiles(path, "*.xml", System.IO.SearchOption.AllDirectories);
                    var newLogs = new List<RecoverLog>();
                    var newBindableLogs = new List<BindableLog>();
                    foreach (var file in files)
                    {                                                
                        //Attempt to load
                        var bindableLog = BindableLog.Parse(RecoverLog.Deserialize(file), SectionCount);
                        
                        //On success add to list
                        if (bindableLog != null)
                            newBindableLogs.Add(bindableLog);
                    }

                    //Get all delta as single object                    
                    var allDeltasPressures = newBindableLogs.SelectMany(a => a.DeltaPressureValues).ToArray();

                    //Fetch only the relevant delta values
                    int xAvgValue = 500;                                                                //Used to create average between very distinct pressures
                    _averageDeltaPressurePoints = allDeltasPressures
                        .Where(a => a.X < 10_000)                                                       //Only looking at mTorr below 10,000
                        .GroupBy(a => ((int)(a.X / xAvgValue)) * xAvgValue)                             //Group to floored mTorr by xAvgValue
                        .Select(a => new ObservablePoint(a.Key, Math.Round(a.Average(b => b.Y))))       //Average of each xAvgValue to observable point
                        .OrderByDescending(a => a.X)                                                    //Go down in mTorr
                        .ToArray(); 



                    //Sections for all logs
                    MinimumSections = new Section[SectionCount];
                    AverageSections = new Section[SectionCount];
                    for (int i = 0; i < SectionCount; i++)
                    {
                        MinimumSections[i] = new Section()
                        {
                            ValueStart = Math.Round(newBindableLogs.Select(a => a.IndividualSections[i].ValueStart).Min()),
                            ValueEnd = Math.Round(newBindableLogs.Select(a => a.IndividualSections[i].ValueEnd).Min()),
                            ProgressRange = Math.Round(newBindableLogs.First().IndividualSections.First().ProgressRange)
                        };
                        AverageSections[i] = new Section()
                        {
                            ValueStart = Math.Round(newBindableLogs.Select(a => a.IndividualSections[i].ValueStart).Average()),
                            ValueEnd = Math.Round(newBindableLogs.Select(a => a.IndividualSections[i].ValueEnd).Average()),
                            ProgressRange = Math.Round(newBindableLogs.First().IndividualSections.First().ProgressRange)
                        };
                    }

                    //Set averages for all logs onto individual log graphs
                    foreach (var log in newBindableLogs)
                    {
                        log.SetGenericSections(MinimumSections, AverageSections);
                        log.SetAvgDelta(_averageDeltaPressurePoints, 0.5f);
                    }

                    
                    BindableLogs = newBindableLogs;

                });
            }            
        }

        public ICommand ExportLogs
        {
            get
            {
                return new ButtonCommand(a =>
                {
                    var dialog = new Ookii.Dialogs.Wpf.VistaSaveFileDialog();

                    if (dialog.ShowDialog() != true)
                        return;

                    var sectionData = string.Join("\n", MinimumSections);
                    var averageData = string.Join("\n", _averageDeltaPressurePoints.Select(b => $"{b.X}, {b.Y}"));


                    System.IO.File.WriteAllText(dialog.FileName, averageData);

                    

                });
            }        
        }



    }
}
