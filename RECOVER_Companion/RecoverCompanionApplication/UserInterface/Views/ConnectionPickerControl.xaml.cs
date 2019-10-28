using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
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
    public enum CONNECTION_MODE
    {
        FILE,
        USB
    }

    /// <summary>
    /// Interaction logic for ConnectionPicker.xaml
    /// </summary>
    public partial class ConnectionPickerControl : UserControl
    {                
        public ConnectionPickerControl()
        {
            InitializeComponent();
        }

        private void FileMode_Clicked(object sender, RoutedEventArgs e)
        {
            App.ConnectionMode = CONNECTION_MODE.FILE;
            App.MainAppWindow.GotoLogs();
        }

        private void USBMode_Clicked(object sender, RoutedEventArgs e)
        {
            App.ConnectionMode = CONNECTION_MODE.USB;
            App.MainAppWindow.GotoSplash();
            App.MainAppWindow.Splash.ConnectViaUSb();
        }

        private void ChangeLanguage_Click(object sender, RoutedEventArgs e)
        {
            var languageSelector = new LanguageSelector();
            languageSelector.ShowDialog();
        }
    }
}
