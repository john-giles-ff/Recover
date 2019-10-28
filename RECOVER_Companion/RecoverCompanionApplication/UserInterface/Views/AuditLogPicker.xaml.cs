using FosterAndFreeman.RecoverCompanionApplication.Definitions.BaseClasses;
using FosterAndFreeman.RecoverCompanionApplication.Definitions.DeviceData;
using FosterAndFreeman.RecoverCompanionApplication.Definitions.WPF;
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
using System.Windows.Shapes;

namespace FosterAndFreeman.RecoverCompanionApplication.UserInterface.Views
{
    public class ObservableSelectedAuditLog : PropertyChangedBase
    {
        private bool _isChecked;
        private string _title;
        private RecoverLog _log;


        public bool IsChecked
        {
            get
            {
                return _isChecked;
            }
            set
            {
                _isChecked = value;
                RaisePropertyChanged();
            }
        }

        public string Title
        {
            get
            {
                return _title;
            }
            set
            {
                _title = value;
                RaisePropertyChanged();
            }
        }

        public RecoverLog Log
        {
            get
            {
                return _log;
            }
            set
            {
                _log = value;
                RaisePropertyChanged();
            }
        }

        public ICommand ToggleChecked
        {
            get
            {
                return new ButtonCommand(p => {
                    IsChecked = !IsChecked;
                });
            }


        }

        public ObservableSelectedAuditLog(RecoverLog log)
        {
            Log = log;
            Title = log.StartTime.ToString("G");
            IsChecked = false;
        }
    }



    /// <summary>
    /// Interaction logic for AuditLogPicker.xaml
    /// </summary>
    public partial class AuditLogPicker : Window
    {
        private ObservableSelectedAuditLog[] logs;

        public RecoverLog[] SelectedLogs;
        public bool UseSingleLogPerPage = false;


        public AuditLogPicker()
        {
            InitializeComponent();
            Owner = App.MainAppWindow;
        }

        private void Exit_Clicked(object sender, RoutedEventArgs e)
        {
            this.DialogResult = false;
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            logs = App.Logs.Select(a => new ObservableSelectedAuditLog(a)).ToArray();
            LstLogs.ItemsSource = logs;
            TglUseSingleLogPerPage.IsChecked = UseSingleLogPerPage;
        }

        private void Confirm_Clicked(object sender, RoutedEventArgs e)
        {
            UseSingleLogPerPage = (bool)TglUseSingleLogPerPage.IsChecked;


            SelectedLogs = logs.Where(a => a.IsChecked).Select(a => a.Log).ToArray();
            if (!SelectedLogs.Any())
                return;

            

            this.DialogResult = true;
        }        

        private void CheckAll_Clicked(object sender, RoutedEventArgs e)
        {
            for (int i = 0; i < logs.Count(); i++)
                logs[i].IsChecked = true;

            //LstLogs.ItemsSource = logs;
        }

        private void UncheckAll_Clicked(object sender, RoutedEventArgs e)
        {
            for (int i = 0; i < logs.Count(); i++)
                logs[i].IsChecked = false;

            //LstLogs.ItemsSource = logs;
        }


    }
}
