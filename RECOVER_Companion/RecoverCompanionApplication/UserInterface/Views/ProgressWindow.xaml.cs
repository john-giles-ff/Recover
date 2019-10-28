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
    /// Interaction logic for ProgressWindow.xaml
    /// </summary>
    public partial class ProgressWindow : Window
    {
        public ProgressWindow()
        {
            InitializeComponent();
            Owner = App.MainAppWindow;
        }

        public void SetAllowClose(bool state)
        {
            if (state)
                BtnExit.Visibility = Visibility.Visible;
            else
                BtnExit.Visibility = Visibility.Collapsed;
        }

        public void SetIsIndetermite(bool state)
        {
            PbrProgress.IsIndeterminate = state;

            if (!state)
            {
                PbrProgress.Maximum = 1;
                PbrProgress.Value = 1;
            }
                   
        }

        public void SetText(string text)
        {
            TxtContent.Text = text;
        }



        private void Exit_Clicked(object sender, RoutedEventArgs e)
        {
            Close();
        }
    }
}
