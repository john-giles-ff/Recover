using FosterAndFreeman.RecoverCompanionApplication.Definitions.DeviceData;
using FosterAndFreeman.RecoverCompanionApplication.Definitions.Misc;
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
    /// Interaction logic for AuditLogsControl.xaml
    /// </summary>
    public partial class AuditLogsControl : UserControl
    {
        private bool isFileLogsLoaded;        


        public AuditLogsControl()
        {
            InitializeComponent();
        }

        public void ReloadLogs()
        {
            LoadLogs(null, null);
        }
        
        public void Initialise()
        {
            isFileLogsLoaded = false;
            App.Logs = App.USBLogs;

            //Check for Argument passed logs
            var argumentLogs = App.Arguments.Where(a => System.IO.File.Exists(a) && System.IO.Path.GetExtension(a).ToUpper() == ".RECOVER");
            if (argumentLogs.Any())
            {
                App.Logs = new List<Definitions.DeviceData.RecoverLog>();
                foreach (var file in argumentLogs)
                {
                    var fileLog = new Definitions.DeviceData.RecoverLog();
                    fileLog.Parse(file);
                    App.Logs.Add(fileLog);                    
                }

                App.Arguments = new string[0];
                isFileLogsLoaded = true;
                LoadLogs(null, null);
            }            
                        

            UpdateFileButtons();
        }

        private void Back_Clicked(object sender, RoutedEventArgs e)
        {
            if (App.ConnectionMode == CONNECTION_MODE.USB)
                App.MainAppWindow.GotoModeChooser();
            else
                App.MainAppWindow.GotoConnectionPicker();
        }

        private void UserControl_Loaded(object sender, RoutedEventArgs e)
        {
            App.LogsChanged += LoadLogs;

            LoadLogs(null, null);
        }
        private void LoadLogs(object sender, EventArgs e)
        {
            if (App.Logs != null && App.Logs.Any())
            {
                LstLogs.ItemsSource = new ListCollectionView(App.Logs);
                TxtLogsNotFound.Visibility = Visibility.Collapsed;
            }
            else
            {
                LstLogs.ItemsSource = null;
                TxtLogsNotFound.Visibility = Visibility.Visible;
            }
        }

        

        private void Open_Clicked(object sender, RoutedEventArgs e)
        {
            //Ask where file to open is
            var filePicker = new Microsoft.Win32.OpenFileDialog()
            {
                Filter = "Recover Files|*.recover",
                Multiselect = true,

            };


            if (filePicker.ShowDialog() != true)
                return;

            //Parse File
            App.Logs = new List<Definitions.DeviceData.RecoverLog>();
            foreach (var file in filePicker.FileNames)
            {
                var fileLog = new Definitions.DeviceData.RecoverLog();
                fileLog.Parse(file);
                App.Logs.Add(fileLog);
            }


            isFileLogsLoaded = true;
            //Load USB logs again (In file mode this will be null)


            LoadLogs(null, null);
            UpdateFileButtons();
        }

        private void UpdateFileButtons()
        {            
            BtnCloseFiles.IsEnabled = isFileLogsLoaded;
            BtnPrintMultiple.IsEnabled = App.Logs != null && App.Logs.Any();

            BtnExportAll.IsEnabled = App.Logs != null && App.Logs.Any();
            BtnSaveAll.IsEnabled = App.Logs != null && App.Logs.Any();
        }

        private void CloseFiles_Clicked(object sender, RoutedEventArgs e)
        {
            isFileLogsLoaded = false;
            App.Logs = App.USBLogs;
            LoadLogs(null, null);

            UpdateFileButtons();
        }

        private void BtnPrintMultiple_Click(object sender, RoutedEventArgs e)
        {
            if (App.Logs == null || !App.Logs.Any())
                return;


            RecoverLog[] printLogs;
            bool useSinglePagePerLog = false; 

            //Decide which logs to print, if there is only 1 log, then don't bother showing log picker
            if (App.Logs.Count() > 1)
            {
                var auditLogPicker = new AuditLogPicker();                

                if (auditLogPicker.ShowDialog() == false)
                    return;

                useSinglePagePerLog = auditLogPicker.UseSingleLogPerPage;
                printLogs = auditLogPicker.SelectedLogs;
            }
            else
                printLogs = App.Logs.ToArray();


            //Start printing
            var printManager = new PrintManager();
            printManager.PrintRecoverLog(printLogs, useSinglePagePerLog);


        }

        private void BtnExportAll_Clicked(object sender, RoutedEventArgs e)
        {
            if (App.Logs == null)
                return;

            //Ask user where to save
            string path = null;
            using (var dialog = new System.Windows.Forms.FolderBrowserDialog())
            {
                if (dialog.ShowDialog() != System.Windows.Forms.DialogResult.OK)
                    return;

                path = dialog.SelectedPath;
            }

            //Export All logs
            foreach (var log in App.Logs)
            {
                var logPath = System.IO.Path.Combine(path, $"{log.StartTime.ToString("yyyy-MM-dd-HH-mm-ss")}.pdf");
                log.Export.Execute(logPath);
            }


            //Report Success
            new MsgBox(Strings.SuccesfullyExportedAll, MsgBoxOptions.Ok).ShowDialog();
        }

        private void BtnSaveAll_Clicked(object sender, RoutedEventArgs e)
        {
            if (App.Logs == null)
                return;


            string path = null;
            using (var dialog = new System.Windows.Forms.FolderBrowserDialog())
            {
                if (dialog.ShowDialog() != System.Windows.Forms.DialogResult.OK)
                    return;

                path = dialog.SelectedPath;
            }

            //Export All logs
            foreach (var log in App.Logs)
            {
                var logPath = System.IO.Path.Combine(path, $"{log.StartTime.ToString("yyyy-MM-dd-HH-mm-ss")}.recover");
                log.Save.Execute(logPath);
            }

            //Report Success
            new MsgBox(Strings.SuccesfullyExportedAll, MsgBoxOptions.Ok).ShowDialog();

        }
    }
}
