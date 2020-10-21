using LiveCharts;
using LiveCharts.Defaults;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Forms;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using Microsoft.WindowsAPICodePack.Dialogs;
using System.IO;
using System.Text.RegularExpressions;

namespace RecoverLogInspector
{
    public struct UnitDirectory
    {
        public string Path { get; set; }
        public List<RecoverLog> Logs { get; set; }
        public Stats UnitStats { get; set; }

    }

    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private List<RecoverLog> logs;
        private List<UnitDirectory> cachedDirectories;
        private string prevDir = "C:\\Users";
        private string root;

        private SettingsManager settings;


        public MainWindow()
        {
            InitializeComponent();

            settings = SettingsManager.Load();

            barView.OnPointClicked += (sender, e) =>
            {
                var directory = cachedDirectories[(int)e];

                ShowLog(directory.UnitStats);

                foreach (System.Windows.Controls.MenuItem item in logSelectTab.Items)
                    item.IsChecked = item.Header.ToString() == System.IO.Path.GetFileName(directory.Path);


                logView.Visibility = Visibility.Visible;
                barView.Visibility = Visibility.Collapsed;
            };
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            LoadSettings();
            OpenCommand_Executed(null, null);
        }

        private void OpenCommand_CanExecute(object sender, CanExecuteRoutedEventArgs e)
        {
            e.CanExecute = true;
        }

        private void OpenCommand_Executed(object sender, ExecutedRoutedEventArgs e)
        {
            string[] files;
            
            using (var dialog = new CommonOpenFileDialog())
            {
                do
                {
                    dialog.InitialDirectory = prevDir;
                    dialog.IsFolderPicker = true;

                    if (dialog.ShowDialog() != CommonFileDialogResult.Ok)
                        return;

                    // Get all subfolders that match regex: xxxx-xxxxx where each x is a single digit
                    var reg = new Regex(@"1236-[0-9]{5}");
                    files = Directory.GetDirectories(dialog.FileName)
                                        .Where(path => reg.IsMatch(path))
                                        .ToArray();

                } while (files.Length == 0);

                prevDir = System.IO.Path.GetDirectoryName(dialog.FileName);
                root = dialog.FileName;
            }

            // Remove any options in the dropdown menu
            logSelectTab.Items.Clear();

            // Add the new logs to the dropdown menu
            foreach (var file in files)
            {
                var newMenuItem = new System.Windows.Controls.MenuItem
                {
                    Header = System.IO.Path.GetFileName(file),
                    IsCheckable = true,
                };
                newMenuItem.Click += NewMenuItem_Click;
                logSelectTab.Items.Add(newMenuItem);
            }

            // Show summary view
            barView.Visibility = Visibility.Visible;
            logView.Visibility = Visibility.Collapsed;

            // Show data
            ShowXCharts(root);
        }

        private void NewMenuItem_Click(object sender, RoutedEventArgs e)
        {
            var menuItem = (System.Windows.Controls.MenuItem)sender;            

            //Load log
            var searchPath = System.IO.Path.Combine(root, menuItem.Header.ToString());            
            ShowLog(cachedDirectories.First(a => a.Path == searchPath).UnitStats);

            foreach (System.Windows.Controls.MenuItem item in logSelectTab.Items)
            {
                item.IsChecked = false;
            }
            menuItem.IsChecked = true;

            // Make log view visible
            logView.Visibility = Visibility.Visible;
            barView.Visibility = Visibility.Collapsed;
        }

        private void ShowLog(Stats stats)
        {
            // Peak base temperature
            logView.peakBaseXAxis.Labels = stats.StartTimes.Select(a => a.ToString()).ToList();            
            logView.peakBaseLine.Values = new ChartValues<int>(stats.BaseTemps);
            logView.peakBaseAverageLine.Values = new ChartValues<double>(Enumerable.Repeat(stats.AvgTemp_PeakBase, stats.BaseTemps.Count()));
            logView.peakBaseMaxLine.Values = new ChartValues<int>(Enumerable.Repeat(stats.BaseTemps.Max(), stats.BaseTemps.Count()));
            logView.peakBaseMinLine.Values = new ChartValues<int>(Enumerable.Repeat(stats.BaseTemps.Min(), stats.BaseTemps.Count()));

            // Peak base time
            logView.peakBaseTimeXAxis.Labels = stats.StartTimes.Select(a => a.ToString()).ToList();
            logView.peakBaseTimeLine.Values = new ChartValues<int>(stats.BaseTimes);
            logView.peakBaseTimeAverageLine.Values = new ChartValues<double>(Enumerable.Repeat(stats.AvgTime_PeakBase, stats.BaseTimes.Count()));
            logView.peakBaseTimeMaxLine.Values = new ChartValues<int>(Enumerable.Repeat(stats.BaseTimes.Max(), stats.BaseTimes.Count()));
            logView.peakBaseTimeMinLine.Values = new ChartValues<int>(Enumerable.Repeat(stats.BaseTimes.Min(), stats.BaseTimes.Count()));

            // Pumpdown time
            logView.pumpdownXAxis.Labels = stats.StartTimes.Select(a => a.ToString()).ToList();
            logView.pumpdownLine.Values = new ChartValues<int>(stats.PumpdownTimes);
            logView.pumpdownAverageLine.Values = new ChartValues<double>(Enumerable.Repeat(stats.AvgTime_Pumpdown, stats.PumpdownTimes.Count()));
            logView.pumpdownMaxLine.Values = new ChartValues<int>(Enumerable.Repeat(stats.PumpdownTimes.Max(), stats.PumpdownTimes.Count()));
            logView.pumpdownMinLine.Values = new ChartValues<int>(Enumerable.Repeat(stats.PumpdownTimes.Min(), stats.PumpdownTimes.Count()));

            // Precursor time
            logView.precursorXAxis.Labels = stats.StartTimes.Select(a => a.ToString()).ToList();
            logView.precursorLine.Values = new ChartValues<int>(stats.PrecursorTimes);
            logView.precursorAverageLine.Values = new ChartValues<double>(Enumerable.Repeat(stats.AvgTime_Precursor, stats.PrecursorTimes.Count()));
            logView.precursorMaxLine.Values = new ChartValues<int>(Enumerable.Repeat(stats.PrecursorTimes.Max(), stats.PrecursorTimes.Count()));
            logView.precursorMinLine.Values = new ChartValues<int>(Enumerable.Repeat(stats.PrecursorTimes.Min(), stats.PrecursorTimes.Count()));
        }

        private void ShowXCharts(string root)
        {
            var progressWindow = new ProgressWindow();            

            new System.Threading.Thread(() =>
            {
                var sampleAvgs_baseTemp = new List<double>();
                var sampleAvgs_baseTime = new List<double>();
                var sampleAvgs_pumpdown = new List<double>();
                var sampleAvgs_precursor = new List<double>();
                var sampleNames = new List<string>();
                cachedDirectories = new List<UnitDirectory>();

                // Get all compatible folders
                var reg = new Regex(@"1236-[0-9]{5}");
                var directories = Directory.GetDirectories(root)
                                     .Where(path => reg.IsMatch(path))
                                     .ToArray();

                App.Current.Dispatcher.Invoke(() =>
                {
                    progressWindow.PgrMajor.Maximum = directories.Count();
                });

                // Read all logs
                int index = 0;
                foreach (string directory in directories)
                {
                    App.Current.Dispatcher.Invoke(() =>
                    {
                        progressWindow.PgrMajor.Value = index;
                        progressWindow.TxtDescription.Text = System.IO.Path.GetFileName(directory);
                    });

                    var reader = new LogReader();
                    reader.OnProgress += (sender, e) =>
                    {
                        App.Current.Dispatcher.Invoke(() =>
                        {                            
                            progressWindow.PgrMinor.Maximum = e.Maximum;
                            progressWindow.PgrMinor.Value = e.Value;
                        });
                    };

                    logs = reader.OpenLogs(directory);
                    if (logs.Count() == 0)
                        continue;

                    try
                    {                    
                        var stats = new Stats(logs);

                        // Get log averages
                        sampleAvgs_baseTemp.Add(stats.AvgTemp_PeakBase);
                        sampleAvgs_baseTime.Add(stats.AvgTime_PeakBase);
                        sampleAvgs_pumpdown.Add(stats.AvgTime_Pumpdown);
                        sampleAvgs_precursor.Add(stats.AvgTime_Precursor);
                        sampleNames.Add(logs.First().SerialNumber);

                        cachedDirectories.Add(new UnitDirectory { Path = directory, Logs = logs, UnitStats = stats });
                        index++;
                    }
                    catch (Exception ex)
                    {
                        System.Windows.MessageBox.Show($"Unable to parse files for directory:\n{Path.GetFileName(directory)}\n{ex.Message}");
                    }

                }

                // Plot peak base temp x-bar chart
                App.Current.Dispatcher.Invoke(() => { 
                    barView.peakBaseXAxis.Labels = sampleNames;
                    barView.peakBaseLine.Values = new ChartValues<double>(sampleAvgs_baseTemp);

                    // Plot peak base time x-bar chart
                    barView.peakBaseTimeXAxis.Labels = sampleNames;
                    barView.peakBaseTimeLine.Values = new ChartValues<double>(sampleAvgs_baseTime);

                    // Plot pumpdown time x-bar chart
                    barView.pumpdownXAxis.Labels = sampleNames;
                    barView.pumpdownTimeLine.Values = new ChartValues<double>(sampleAvgs_pumpdown);

                    // Plot precursor time x-bar chart
                    barView.precursorTimeXAxis.Labels = sampleNames;
                    barView.precursorTimeLine.Values = new ChartValues<double>(sampleAvgs_precursor);

                    progressWindow.Close();
                });
            }).Start();

            progressWindow.ShowDialog();
        }

        

        private void Summary_Click(object sender, RoutedEventArgs e)
        {
            // Make bar view visible
            barView.Visibility = Visibility.Visible;
            logView.Visibility = Visibility.Collapsed;
        }

        private void LoadSettings()
        {
            barView.baseTempHCL.Value = settings.AveragePeakBaseTemperatureHCL;
            barView.baseTempAVG.Value = settings.AveragePeakBaseTemperatureAVG;
            barView.baseTempLCL.Value = settings.AveragePeakBaseTemperatureLCL;

            barView.baseTimeHCL.Value = settings.AverageBaseTimeHCL;
            barView.baseTimeAVG.Value = settings.AverageBaseTimeAVG;
            barView.baseTimeLCL.Value = settings.AverageBaseTimeLCL;

            barView.pumpdownHCL.Value = settings.AveragePumpdownTimeHCL;
            barView.pumpdownAVG.Value = settings.AveragePumpdownTimeAVG;
            barView.pumpdownLCL.Value = settings.AveragePumpdownTimeLCL;

            barView.precursorHCL.Value = settings.AveragePrecursorTimeHCL;
            barView.precursorAVG.Value = settings.AveragePrecursorTimeAVG;
            barView.precursorLCL.Value = settings.AveragePrecursorTimeLCL;



            logView.baseTempHCL.Value = settings.IndividualPeakBaseTemperatureHCL;
            logView.baseTempAVG.Value = settings.IndividualPeakBaseTemperatureAVG;
            logView.baseTempLCL.Value = settings.IndividualPeakBaseTemperatureLCL;

            logView.baseTimeHCL.Value = settings.IndividualBaseTimeHCL;
            logView.baseTimeAVG.Value = settings.IndividualBaseTimeAVG;
            logView.baseTimeLCL.Value = settings.IndividualBaseTimeLCL;

            logView.pumpdownHCL.Value = settings.IndividualPumpdownTimeHCL;
            logView.pumpdownAVG.Value = settings.IndividualPumpdownTimeAVG;
            logView.pumpdownLCL.Value = settings.IndividualPumpdownTimeLCL;

            logView.precursorHCL.Value = settings.IndividualPrecursorTimeHCL;
            logView.precursorAVG.Value = settings.IndividualPrecursorTimeAVG;
            logView.precursorLCL.Value = settings.IndividualPrecursorTimeLCL;
        }

        private void OpenSettings(object sender, RoutedEventArgs e)
        {
            var settingsWindow = new SettingsWindow();
            settingsWindow.ShowDialog();

            settings = SettingsManager.Load();
            LoadSettings();
        }

        private void MenuOpenLogs_Click(object sender, RoutedEventArgs e)
        {
            OpenCommand_Executed(null, null);
        }

        private void MenuExit_Click(object sender, RoutedEventArgs e)
        {
            Environment.Exit(0);
        }
    }
}
