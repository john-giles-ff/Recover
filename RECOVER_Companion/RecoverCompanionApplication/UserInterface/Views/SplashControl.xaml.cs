using FosterAndFreeman.RecoverCompanionApplication.Definitions.DeviceData;
using FosterAndFreeman.RecoverCompanionApplication.Resources.Languages;
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
    /// Interaction logic for SplashControl.xaml
    /// </summary>
    public partial class SplashControl : UserControl
    {
        public SplashControl()
        {
            InitializeComponent();
        }

        public void SetText(string text)
        {
            TxtInformation.Text = text;
        }

        public void ConnectViaUSb()
        {
            new System.Threading.Thread(() =>
            {

                //Connect to the device
                Application.Current.Dispatcher.Invoke(() => { SetText(Strings.AttemptingConnection); });
                try
                {
                    Definitions.DeviceCommunications.RecoverManager.AttemptConnection();
                    Definitions.DeviceCommunications.RecoverManager.EnterPCMode();
                }
                catch (Exception ex)
                {
                    //If unable to connect to the device, then the application can't open
                    var message = string.Format(Strings.FailedConnection, ex.Message);
                    Application.Current.Dispatcher.Invoke(() =>
                    {
                        new UserInterface.Views.ErrorBox(message).ShowDialog();
                        App.MainAppWindow.GotoConnectionPicker();
                    });

                    return;
                }


                Application.Current.Dispatcher.Invoke(() => { SetText(Strings.ConnectionSuccesful); });

                App.CheckSerial();



                Application.Current.Dispatcher.Invoke(() => { SetText(Strings.FetchingLogHeaders); });

                //Fetch Log Headers                
                App.Logs = new List<RecoverLog>();
                Definitions.DeviceCommunications.RecoverManager.EnterTransparentMode();

                for (int i = 0; i < Definitions.DeviceCommunications.RecoverManager.NumberOfLogs; i++)
                {
                    try
                    {
                        var log = Definitions.DeviceCommunications.RecoverManager.GetLogHeader(i);
                        if (log != null)
                            App.Logs.Add(log);
                    }
                    //Ignore if logs are unable to parse
                    catch (Exception) { }
                }

                Definitions.DeviceCommunications.RecoverManager.ExitTransparentMode();



                if (App.Logs != null && App.Logs.Count() == 0)
                    App.Logs = null;

                if (App.Logs != null)
                    App.Logs = App.Logs.OrderByDescending(a => a.StartTime).ToList();

                //Copy to USB logs
                App.USBLogs = App.Logs;


                Application.Current.Dispatcher.Invoke(() =>
                {
                //Check if file with .LFT extension was passed                
                var lftFileArgument = App.Arguments.FirstOrDefault(a => System.IO.File.Exists(a) && System.IO.Path.GetExtension(a).ToUpper() == ".LFT");

                    if (lftFileArgument != null)
                    {
                        App.MainAppWindow.SoftwareUpgrade.TxtInputFilePath.Text = lftFileArgument;
                        App.MainAppWindow.GotoSoftwareUpgrade();
                    }
                    else
                        App.MainAppWindow.GotoModeChooser();

                });
            }).Start();
        }
    }
}
