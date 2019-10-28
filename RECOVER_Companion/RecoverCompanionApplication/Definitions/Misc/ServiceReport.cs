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
using System.Xml.Serialization;

namespace FosterAndFreeman.RecoverCompanionApplication.Definitions.Misc
{
    static class ServiceReport
    {
        public static void Create()
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
            progressWindow.Show();

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
                            //Only fetch is not previously fetched
                            if (log.Samples == null || !log.Samples.Any())
                            {
                                RecoverManager.GetLogSamples(log);
                                RecoverManager.SampleFinishedEvent.WaitOne();
                            }
                            else
                                progressWindow.Value += log.NumberOfSamples;

                            //Escape only on success
                            break;
                        }
                        catch (Exception) { }
                    } while (true);

                    Application.Current.Dispatcher.Invoke(() =>
                    {
                        progressWindow.Value = totalSamples;
                    });
                }

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
                    var saveFileDialog = new Ookii.Dialogs.Wpf.VistaSaveFileDialog()
                    {
                        Filter = "*.LFT_SVC|*.LFT_SVC",
                    };

                    if (saveFileDialog.ShowDialog() != true)
                        return;

                    filename = saveFileDialog.FileName;
                    success = true;
                });

                if (!success)
                    return;

                //Save
                File.WriteAllBytes(filename, data.ToArray());
            });


        }

        public static void Decrypt()
        {
            var openFileDialog = new Ookii.Dialogs.Wpf.VistaOpenFileDialog()
            {
                Filter = "*.LFT_SVC|*.LFT_SVC",
            };

            if (openFileDialog.ShowDialog() != true)
                return;

            var saveDirectoryDialog = new Ookii.Dialogs.Wpf.VistaFolderBrowserDialog();
            if (saveDirectoryDialog.ShowDialog() != true)
                return;

            var bytes = File.ReadAllBytes(openFileDialog.FileName);
            int filename = 0;
            int index = 0;
            do
            {
                var length = BitConverter.ToInt32(bytes, index);
                index += 4;

                var individualBytes = bytes.Skip(index).Take(length).ToArray();
                index += length;

                var decryptedString = EncryptionManager.Decrypt(Encoding.UTF8.GetString(individualBytes));

                File.WriteAllText(Path.Combine(saveDirectoryDialog.SelectedPath, filename.ToString()), decryptedString);
                filename++;
            }
            while (index < bytes.Length);
        }
    }
}