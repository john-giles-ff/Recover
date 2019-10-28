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
using System.Windows.Interop;
using FosterAndFreeman.RecoverCompanionApplication.Definitions.Misc;

namespace FosterAndFreeman.RecoverCompanionApplication.UserInterface.Views
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public SplashControl Splash = new SplashControl();
        public ModeControl ModeChooser = new ModeControl();
        public SoftwareUpgradeControl SoftwareUpgrade = new SoftwareUpgradeControl();
        public AuditLogsControl AuditLogs = new AuditLogsControl();
        public ConnectionPickerControl ConnectionPicker = new ConnectionPickerControl();


        public MainWindow()
        {
            InitializeComponent();
            GotoSplash();
        }

        public bool IsInterruptable
        {
            get
            {
                return SoftwareUpgrade.IsInterruptable;
            }
        }


        public void GotoModeChooser()
        {
            Border.Child = ModeChooser;
        }

        public void GotoSoftwareUpgrade()
        {
            Border.Child = SoftwareUpgrade;
        }

        public void GotoLogs(bool initialise = true)
        {            
            Border.Child = AuditLogs;

            if (initialise)
                AuditLogs.Initialise();
        }

        public void GotoSplash()
        {
            Border.Child = Splash;
        }

        public void GotoConnectionPicker()
        {
            Border.Child = ConnectionPicker;
        }
        

        private void Window_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            DragMove();
        }
        
    }
}
