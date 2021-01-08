using FosterAndFreeman.RecoverCompanionApplication.Definitions.DeviceCommunications;
using FosterAndFreeman.RecoverCompanionApplication.Resources.Languages;
using FosterAndFreeman.RecoverCompanionApplication.UserInterface.Views;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Xml;
using System.Xml.Serialization;

namespace FosterAndFreeman.RecoverCompanionApplication.Definitions.Misc
{
    static class ServiceReport
    {
        public static void Create(bool encrypt = true)
        {
            var totalSamples = 0;
            foreach (var log in App.Logs)
                totalSamples += log.NumberOfSamples;

            var progressWindow = new ProgressWindow()
            {
                IsIndetermite = false,
                Text = Strings.CreatingServicePack,
                Maximum = totalSamples,
                Value = 0
            };            

            RecoverManager.OnSampleLoaded = (a, b) =>
            {
                Application.Current.Dispatcher.Invoke(() =>
                {
                    progressWindow.Value++;
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
                            var valueBefore = 0d;
                            Application.Current.Dispatcher.Invoke(() =>
                            {
                                valueBefore = progressWindow.Value;                                
                            });                            

                            //Only fetch is not previously fetched
                            if (log.Samples == null || !log.Samples.Any())
                            {
                                RecoverManager.GetLogSamples(log);
                                RecoverManager.SampleFinishedEvent.WaitOne();
                            }

                            Application.Current.Dispatcher.Invoke(() =>
                            {
                                progressWindow.Value = valueBefore + log.NumberOfSamples;
                            });

                            //Escape only on success
                            break;
                        }
                        catch (Exception) { }
                    } while (true);                    
                }

                Application.Current.Dispatcher.Invoke(() =>
                {
                    progressWindow.Value = totalSamples;
                });

                //Serialise and encrypt
                var individualData = new List<string>();
                foreach (var log in App.Logs)
                {
                    if (log == null)
                        continue;
                    using (var writer = new StringWriter())
                    {
                        var serializer = new XmlSerializer(typeof(Definitions.DeviceData.RecoverLog), new Type[] { typeof(Definitions.DeviceData.Sample[]) });
                        serializer.Serialize(writer, log);

                        individualData.Add(EncryptionManager.Encrypt(writer.ToString()));
                    }
                }

                //Combine Data
                var data = new List<byte>();
                foreach (var dat in individualData)
                {
                    data.AddRange(BitConverter.GetBytes(dat.Length));
                    data.AddRange(Encoding.UTF8.GetBytes(dat));
                }

                //Ask for location to save
                bool success = false;
                var filename = string.Empty;
                

                Application.Current.Dispatcher.Invoke(() =>
                {
                    if (encrypt)
                    {
                        var saveFileDialog = new Ookii.Dialogs.Wpf.VistaSaveFileDialog()
                        {
                            FileName = RecoverManager.SerialNumber,
                            Filter = "RECOVER Service Pack (*.LFT_SVC)|*.LFT_SVC",
                            AddExtension = true,
                        };

                        if (saveFileDialog.ShowDialog() != true)
                        {
                            Application.Current.Dispatcher.Invoke(() =>
                            {
                                progressWindow.Text = Strings.ServicePackCreatingCancelled;
                                progressWindow.IsCloseAllowed = true;
                            });

                            return;
                        }

                        filename = saveFileDialog.FileName;

                        if (Path.HasExtension(filename))
                            filename = Path.ChangeExtension(filename, "LFT_SVC");
                        else
                            filename += ".LFT_SVC";
                        success = true;
                    }
                    else
                    {
                        var saveDirectoryDialog = new Ookii.Dialogs.Wpf.VistaFolderBrowserDialog();
                        if (saveDirectoryDialog.ShowDialog() != true)
                            return;

                        SaveDataToDirectory(data.ToArray(), saveDirectoryDialog.SelectedPath);
                    }
                });

                if (!success)
                    return;

                //Save
                File.WriteAllBytes(filename, data.ToArray());

                Application.Current.Dispatcher.Invoke(() =>
                {                    
                    progressWindow.Text = Strings.ServicePackCreated;
                    progressWindow.IsCloseAllowed = true;
                });
            }).Start();

            progressWindow.ShowDialog();
        }

        public static void Decrypt()
        {
            var openFileDialog = new Ookii.Dialogs.Wpf.VistaOpenFileDialog()
            {
                Filter = "RECOVER Service Pack (*.LFT_SVC)|*.LFT_SVC",
            };

            if (openFileDialog.ShowDialog() != true)
                return;

            var saveDirectoryDialog = new Ookii.Dialogs.Wpf.VistaFolderBrowserDialog();
            if (saveDirectoryDialog.ShowDialog() != true)
                return;

            var bytes = File.ReadAllBytes(openFileDialog.FileName);
            SaveDataToDirectory(bytes, saveDirectoryDialog.SelectedPath);            
        }


        private static void SaveDataToDirectory(byte[] bytes, string directory)
        {
            int index = 0;
            do
            {
                var length = BitConverter.ToInt32(bytes, index);
                index += 4;

                var individualBytes = bytes.Skip(index).Take(length).ToArray();
                index += length;

                var decryptedString = EncryptionManager.Decrypt(Encoding.UTF8.GetString(individualBytes));

                var xmlDoc = new XmlDocument();
                xmlDoc.LoadXml(decryptedString);
                var startTime = DateTime.Parse(xmlDoc["RecoverLog"]["StartTime"].FirstChild.Value);
                var logFilename = Path.Combine(directory, $"{startTime:yyyy-MM-dd-HH-mm-ss}.XML");

                File.WriteAllText(logFilename, decryptedString);
            }
            while (index < bytes.Length);
        }
    }
}