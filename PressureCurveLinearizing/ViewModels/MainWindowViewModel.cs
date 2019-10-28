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
        private BindableLog _bindableLog;
        private int _sectionCount = 10;

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
                    Logs.Clear();
                    BindableLogs.Clear();
                    GC.Collect();

                    var files = System.IO.Directory.EnumerateFiles(path, "*.xml", System.IO.SearchOption.AllDirectories);
                    var newLogs = new List<RecoverLog>();
                    var newBindableLogs = new List<BindableLog>();                    
                    foreach (var file in files)
                    {
                        try
                        {
                            var log = RecoverLog.Deserialize(file);

                            //Add Logs
                            newLogs.Add(log);
                            newBindableLogs.Add(new BindableLog(log, SectionCount));

                        }
                        catch (Exception ex) { Console.WriteLine(ex.Message); }
                    }

                    var minimumSections = new Section[SectionCount];
                    var averageSections = new Section[SectionCount];
                    for (int i = 0; i < averageSections.Length; i++)
                    {
                        minimumSections[i] = new Section()
                        {
                            ValueStart = Math.Round(newBindableLogs.Select(a => a.IndividualSections[i].ValueStart).Min()),
                            ValueEnd = Math.Round(newBindableLogs.Select(a => a.IndividualSections[i].ValueEnd).Min()),
                            ProgressRange = Math.Round(newBindableLogs.First().IndividualSections.First().ProgressRange)
                        };
                        averageSections[i] = new Section()
                        {
                            ValueStart = Math.Round(newBindableLogs.Select(a => a.IndividualSections[i].ValueStart).Average()),
                            ValueEnd = Math.Round(newBindableLogs.Select(a => a.IndividualSections[i].ValueEnd).Average()),
                            ProgressRange = Math.Round(newBindableLogs.First().IndividualSections.First().ProgressRange)
                        };

                    }

                    foreach (var log in newBindableLogs)
                        log.SetGenericSections(minimumSections, averageSections);


                    Logs = newLogs;
                    BindableLogs = newBindableLogs;

                });
            }            
        }




    }
}
