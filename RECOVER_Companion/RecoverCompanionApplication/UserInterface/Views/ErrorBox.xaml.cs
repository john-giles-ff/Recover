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
    /// Interaction logic for ErrorBox.xaml
    /// </summary>
    public partial class ErrorBox : Window
    {
        public ErrorBox()
        {
            InitializeComponent();
        }

        public ErrorBox(string message)
        {
            InitializeComponent();

            Owner = App.MainAppWindow;
            TxtMessage.Text = message;

        }

        private void Dismiss_Clicked(object sender, RoutedEventArgs e)
        {
            this.DialogResult = true;
        }
    }
}
