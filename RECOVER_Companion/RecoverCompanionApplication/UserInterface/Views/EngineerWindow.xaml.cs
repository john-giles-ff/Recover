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
            if (!RecoverManager.IsOpen)
            {
                new MsgBox("Please Open Device first!", MsgBoxOptions.Ok, true).ShowDialog();
                return;
            }

            ServiceReport.Create(false);
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
