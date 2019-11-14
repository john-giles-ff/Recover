using FosterAndFreeman.RecoverCompanionApplication.Definitions.DeviceCommunications;
using FosterAndFreeman.RecoverCompanionApplication.Definitions.Misc;
using FosterAndFreeman.RecoverCompanionApplication.Resources.Languages;
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
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Xml.Serialization;

namespace FosterAndFreeman.RecoverCompanionApplication.UserInterface.Views
{
    /// <summary>
    /// Interaction logic for ModeControl.xaml
    /// </summary>
    public partial class ModeControl : UserControl
    {
        public ModeControl()
        {
            InitializeComponent();
        }

        private void AuditLogs_Clicked(object sender, RoutedEventArgs e)
        {
            App.MainAppWindow.GotoLogs();
        }

        private void UpdateSoftware_Clicked(object sender, RoutedEventArgs e)
        {
            App.MainAppWindow.GotoSoftwareUpgrade();
        }

        private void UserControl_Loaded(object sender, RoutedEventArgs e)
        {            
        }

        private void ChangeLanguage_Click(object sender, RoutedEventArgs e)
        {
            var languageSelector = new LanguageSelector();
            languageSelector.ShowDialog();
        }

        private void CreateServicePack_Click(object sender, RoutedEventArgs e)
        {
            ServiceReport.Create();
        }
    }
}
