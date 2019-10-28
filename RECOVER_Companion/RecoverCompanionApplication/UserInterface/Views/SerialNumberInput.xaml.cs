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
using System.Windows.Shapes;

namespace FosterAndFreeman.RecoverCompanionApplication.UserInterface.Views
{
    /// <summary>
    /// Interaction logic for SerialNumberInput.xaml
    /// </summary>
    public partial class SerialNumberInput : Window
    {
        public SerialNumberInput()
        {
            InitializeComponent();
            Owner = App.MainAppWindow;
        }

        public string SerialNumber
        {
            get
            {
                return $"1236-{TxtSerialNumber.Text}";
            }
        }


        private void Cancel_Click(object sender, RoutedEventArgs e)
        {
            this.DialogResult = false;
        }

        private void OK_Click(object sender, RoutedEventArgs e)
        {
            if (!Definitions.DeviceCommunications.RecoverManager.IsSerialNumberValid(SerialNumber))
            {
                new MsgBox(Strings.InvalidSerialNumber, MsgBoxOptions.Ok).ShowDialog();
                return;
            }

            Definitions.DeviceCommunications.RecoverManager.SerialNumber = SerialNumber;
            this.DialogResult = true;
        }        
    }
}
