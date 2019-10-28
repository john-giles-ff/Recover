using FosterAndFreeman.RecoverCompanionApplication.Resources.Languages;
using Microsoft.Win32;
using System;
using System.Collections.Generic;
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
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace FosterAndFreeman.RecoverCompanionApplication.UserInterface.Views
{
    /// <summary>
    /// Interaction logic for SoftwareUpgradeControl.xaml
    /// </summary>
    public partial class SoftwareUpgradeControl : UserControl
    {
        public bool IsInterruptable { get; private set; } = true;





        public SoftwareUpgradeControl()
        {
            InitializeComponent();
            
            Definitions.DeviceCommunications.RecoverManager.OnProgress = OnProgressUpdated;
        }

        private void OnProgressUpdated(object sender, Definitions.DeviceCommunications.Progress progress)
        {
            Application.Current.Dispatcher.Invoke(() =>
            {
                switch (progress.Type)
                {
                    case Definitions.DeviceCommunications.ProgressType.Firmware:
                        PbrFirmware.Value = progress.Value;
                        PbrFirmware.Maximum = progress.Maximum;
                        if (!string.IsNullOrWhiteSpace(progress.Information))
                            TxtInformation.Text = progress.Information;
                        break;


                    case Definitions.DeviceCommunications.ProgressType.Software:
                        PbrSoftware.Value = progress.Value;
                        PbrSoftware.Maximum = progress.Maximum;
                        if (!string.IsNullOrWhiteSpace(progress.Information))
                            TxtInformation.Text = progress.Information;
                        break;

                    case Definitions.DeviceCommunications.ProgressType.Text:
                        TxtInformation.Text = progress.Information;
                        break;

                }

                var firmwarePercentage = (int)(((float)PbrFirmware.Value / PbrFirmware.Maximum) * 100);
                var softwarePercentage = (int)(((float)PbrSoftware.Value / PbrSoftware.Maximum) * 100);


                PbrTotal.Maximum = 200;
                PbrTotal.Value = firmwarePercentage + softwarePercentage;

            });

        }

        private void BrowseSoftwareFile(object sender, RoutedEventArgs e)
        {            
            var fileDialog = new OpenFileDialog
            {
                Filter = "LFT Files (*.lft)|*.lft",
                InitialDirectory = System.IO.File.Exists(TxtInputFilePath.Text) ? System.IO.Path.GetDirectoryName(TxtInputFilePath.Text) : Environment.GetFolderPath(Environment.SpecialFolder.Desktop),
                FileName = TxtInputFilePath.Text,
                Multiselect = false,                
            };

            if (fileDialog.ShowDialog() != true)
                return;

            TxtInputFilePath.Text = fileDialog.FileName;

        }

        private void ProgramDevice_Click(object sender, RoutedEventArgs e)
        {
            OnProgressUpdated(null, new Definitions.DeviceCommunications.Progress() { Information = String.Empty, Type = Definitions.DeviceCommunications.ProgressType.Text });
            var recoverFile = new Definitions.DeviceData.RecoverFile();

            //Check that the file exists
            try
            {
                recoverFile = new Definitions.DeviceData.RecoverFile(TxtInputFilePath.Text);
            }
            catch (Exception ex)
            {
                new UserInterface.Views.ErrorBox(ex.Message).ShowDialog();                
                return;                
            }

            //Confirm user wants to continue
            if (new MsgBox(Strings.ConfirmProgram, MsgBoxOptions.YesCancel).ShowDialog() == false)
                return;

            IsInterruptable = false;

            //Hide Controls            
            BtnProgramDevice.Visibility = Visibility.Hidden;
            BtnBack.Visibility = Visibility.Hidden;
            Backdrop.SetExitVisible(false);
            BtnBrowseProgramFile.IsEnabled = false;

            //Set Progress bars to 0
            PbrFirmware.Value = 0;
            PbrSoftware.Value = 0;
            PbrTotal.Value = 0;

            //Start Threading
            new System.Threading.Thread(() => {

                var firmwareData = recoverFile.GetFirmwareData();
                var softwareData = recoverFile.GetSoftwareData();
                                
                try
                {                    
                    //Start Software Flash or set software to 100%
                    if (softwareData != null)
                        Definitions.DeviceCommunications.RecoverManager.WriteSoftware(recoverFile.GetSoftwareData());
                    else
                        OnProgressUpdated(null, new Definitions.DeviceCommunications.Progress() { Information = string.Empty, Maximum = 1, Value = 1, Type = Definitions.DeviceCommunications.ProgressType.Software });


                    //Start Firmware Flash or set firmware bar to 100%
                    if (firmwareData != null)
                        Definitions.DeviceCommunications.RecoverManager.WriteFirmware(recoverFile.GetFirmwareData());
                    else
                        OnProgressUpdated(null, new Definitions.DeviceCommunications.Progress() { Information = string.Empty, Maximum = 1, Value = 1, Type = Definitions.DeviceCommunications.ProgressType.Firmware });

                    //Wait a bit for the device to setup
                    System.Threading.Thread.Sleep(2000);

                    //Check Serial Number
                    App.CheckSerial();

                    //Show Finished Message
                    OnProgressUpdated(null, new Definitions.DeviceCommunications.Progress() { Information = Strings.FinishedProgramming, Type = Definitions.DeviceCommunications.ProgressType.Text });                    
                }
                catch (Exception ex)
                {
                    Application.Current.Dispatcher.Invoke(() =>
                    {
                        new ErrorBox(ex.Message).ShowDialog();                                                
                    });                    
                }

                //Allow the application to be ran again
                Application.Current.Dispatcher.Invoke(() =>
                {                    
                    BtnProgramDevice.Visibility = Visibility.Visible;
                    BtnBack.Visibility = Visibility.Visible;
                    BtnBrowseProgramFile.IsEnabled = true;
                    Backdrop.SetExitVisible(true);
                    IsInterruptable = true;
                });


            }).Start();
        }

        private void Back_Clicked(object sender, RoutedEventArgs e)
        {
            App.MainAppWindow.GotoModeChooser();            
        }

        private void Upgrade_Drop(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
            {                
                string[] files = (string[])e.Data.GetData(DataFormats.FileDrop);
                string successfulFile = null;

                foreach (var file in files)
                {
                    try
                    {
                        var recoverFile = new Definitions.DeviceData.RecoverFile(file);
                        successfulFile = file;
                        break;
                    }
                    catch (Exception)
                    { }
                }

                if (successfulFile != null)
                    TxtInputFilePath.Text = successfulFile;
            }
        }
    }
}
