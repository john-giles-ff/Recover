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
    }
}
