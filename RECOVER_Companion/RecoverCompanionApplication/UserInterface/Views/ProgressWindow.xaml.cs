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

        public bool IsCloseAllowed
        {
            get
            {
                return BtnExit.Visibility == Visibility.Visible;
            }
            set
            {
                if (value)
                    BtnExit.Visibility = Visibility.Visible;
                else
                    BtnExit.Visibility = Visibility.Collapsed;
            }
        }

        public bool IsIndetermite
        {
            get
            {
                return PbrProgress.IsIndeterminate;
            }
            set
            {
                PbrProgress.IsIndeterminate = value;
                if (!value)
                {
                    PbrProgress.Maximum = 1;
                    PbrProgress.Value = 1;
                }
            }
        }

        public double Maximum
        {
            get
            {
                return PbrProgress.Maximum;
            }
            set
            {
                PbrProgress.Maximum = value;
            }
        }

        public double Value
        { 
            get
            {
                return PbrProgress.Value;
            }
            set
            {
                PbrProgress.Value = value;
            }
        }

        public string Text
        {
            set 
            {
                TxtContent.Text = value;
            }
        }



        private void Exit_Clicked(object sender, RoutedEventArgs e)
        {
            Close();
        }
    }
}
