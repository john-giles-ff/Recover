using FosterAndFreeman.RecoverCompanionApplication.Definitions.DeviceCommunications;
using FosterAndFreeman.RecoverCompanionApplication.Definitions.Misc;
using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.Xml.Serialization;

namespace FosterAndFreeman.RecoverCompanionApplication.UserInterface.Views
{
    /// <summary>
    /// Interaction logic for EngineerWindow.xaml
    /// </summary>
    public partial class EngineerWindow : Window
    {
        public EngineerWindow()
        {
            InitializeComponent();
        }

        private void TextBox_PreviewKeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key != Key.Enter)
            {
                return;
            }

            e.Handled = true;
                        
            TxtResult.Text = RecoverManager.SendCustom(TxtInput.Text);
        }

        private void ExportLogs_Clicked(object sender, RoutedEventArgs e)
        {
            var totalSamples = 0;
            foreach (var log in App.Logs)
                totalSamples += log.NumberOfSamples;
            
            PbrExportAllProgress.Maximum = totalSamples;
            BtnExportAllLogs.IsEnabled = false;

            int progress = 0;
            RecoverManager.OnSampleLoaded = (a, b) => 
            {
                Application.Current.Dispatcher.Invoke(() => 
                {
                    progress++;
                    PbrExportAllProgress.Value = progress;
                });
            };

            new System.Threading.Thread(() =>
            {
                //Fetch All Data
                foreach (var log in App.Logs)
                {                   
                    do
                    {
                        try
                        {
                            if (log.Samples == null || !log.Samples.Any())
                            {
                                RecoverManager.GetLogSamples(log);
                                RecoverManager.SampleFinishedEvent.WaitOne();
                            }
                            else
                                progress += log.NumberOfSamples;

                            break;
                        }
                        catch (Exception)
                        {

                        }
                    }
                    while (true);

                    Application.Current.Dispatcher.Invoke(() =>
                    {
                        PbrExportAllProgress.Value = progress;
                    });                    
                }

                bool threwError = true;
                do
                {
                    try
                    {
                        string path = null;
                        Application.Current.Dispatcher.Invoke(() =>
                        {
                            var dialog = new Ookii.Dialogs.Wpf.VistaFolderBrowserDialog()
                            {
                                ShowNewFolderButton = true,
                                Description = "Select folder to export logs to...",
                            };

                            if (dialog.ShowDialog() != true)
                                return;

                            path = dialog.SelectedPath;
                        });

                        if (path == null)
                            return;

                        //Save as seperate files
                        foreach (var log in App.Logs)
                        {
                            if (log == null)
                                continue;

                            var logFilename = System.IO.Path.Combine(path, $"{log.StartTime.ToString("yyyy-MM-dd-HH-mm-ss")}.XML");

                            using (var writer = new StreamWriter(logFilename))
                            {
                                var serializer = new XmlSerializer(typeof(Definitions.DeviceData.RecoverLog), new Type[] { typeof(Definitions.DeviceData.Sample[]) });
                                serializer.Serialize(writer, log);

                                writer.Close();
                            }

                            log.Export.Execute(System.IO.Path.ChangeExtension(logFilename, ".pdf"));
                        }
                    }
                    catch (Exception)
                    {
                        Application.Current.Dispatcher.Invoke(() =>
                        {
                            new MsgBox("An error occurred whilst saving the logs. Please try again...", MsgBoxOptions.Ok, true).ShowDialog();                            
                        });

                        continue;
                    }

                    Application.Current.Dispatcher.Invoke(() =>
                    {
                        PbrExportAllProgress.Value = totalSamples;
                        BtnExportAllLogs.IsEnabled = true;
                    });
                    threwError = false;
                }
                while (threwError);
            }).Start();


        }

        private void TxtResult_PreviewKeyDown(object sender, KeyEventArgs e)
        {
            e.Handled = true;
        }

        private void DecryptServiceReport_Click(object sender, RoutedEventArgs e)
        {
            ServiceReport.Decrypt();
        }
    }
}
