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

namespace FosterAndFreeman.RecoverCompanionApplication.UserInterface.Common
{
    /// <summary>
    /// Interaction logic for Background.xaml
    /// </summary>
    public partial class Background : UserControl
    {
        public Background()
        {
            InitializeComponent();
        }

        public void SetExitVisible(bool state)
        {
            if (state)
                BtnExit.Visibility = Visibility.Visible;
            else
                BtnExit.Visibility = Visibility.Hidden;
        }

        private void Exit_Clicked(object sender, RoutedEventArgs e)
        {
            if (BtnExit.Visibility != Visibility.Visible)
                return;

            App.ExitApp();
        }
    }
}
