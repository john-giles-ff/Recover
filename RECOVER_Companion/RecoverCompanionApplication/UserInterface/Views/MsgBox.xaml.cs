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
    public enum MsgBoxOptions
    {
        YesNo,
        YesCancel,
        Ok,
        OkCancel
    }


    /// <summary>
    /// Interaction logic for MsgBox.xaml
    /// </summary>
    public partial class MsgBox : Window
    {
        public MsgBox()
        {
            InitializeComponent();
        }

        public MsgBox(string message, MsgBoxOptions options, bool isWarning = false)
        {
            InitializeComponent();

            Owner = App.MainAppWindow;
            TxtMessage.Text = message;

            if (isWarning)
            {
                ImgIcon.Visibility = Visibility.Collapsed;
                ImgIcon_Red.Visibility = Visibility.Visible;
                TxtMessage.Foreground = Brushes.Red;
            }


            SetState(options);
        }

        private void Window_MouseDown(object sender, MouseButtonEventArgs e)
        {
            DragMove();
        }

        private void Negative_Click(object sender, RoutedEventArgs e)
        {
            this.DialogResult = false;
        }

        private void Positive_Click(object sender, RoutedEventArgs e)
        {
            this.DialogResult = true;
        }

        private void SetState(MsgBoxOptions options)
        {
            switch (options)
            {
                case MsgBoxOptions.Ok:
                    BtnPositive.Visibility = Visibility.Visible;
                    BtnNegative.Visibility = Visibility.Collapsed;
                    BtnPositive.Content = Strings.OK;
                    BtnNegative.Content = string.Empty;
                    break;
                case MsgBoxOptions.OkCancel:
                    BtnPositive.Visibility = Visibility.Visible;
                    BtnNegative.Visibility = Visibility.Visible;
                    BtnPositive.Content = Strings.OK;
                    BtnNegative.Content = Strings.Cancel;
                    break;
                case MsgBoxOptions.YesCancel:
                    BtnPositive.Visibility = Visibility.Visible;
                    BtnNegative.Visibility = Visibility.Visible;
                    BtnPositive.Content = Strings.Yes;
                    BtnNegative.Content = Strings.Cancel;
                    break;
                case MsgBoxOptions.YesNo:
                    BtnPositive.Visibility = Visibility.Visible;
                    BtnNegative.Visibility = Visibility.Visible;
                    BtnPositive.Content = Strings.Yes;
                    BtnNegative.Content = Strings.No;
                    break;
            }

        }        
    }
}
