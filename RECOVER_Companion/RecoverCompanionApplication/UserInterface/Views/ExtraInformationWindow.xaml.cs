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
    /// <summary>
    /// Interaction logic for ExtraInformationWindow.xaml
    /// </summary>
    public partial class ExtraInformationWindow : Window
    {
        public string CaseReference;
        public string CaseOperator;
        public string Notes;        

        public ExtraInformationWindow()
        {
            InitializeComponent();

            Owner = App.MainAppWindow;
        }

        private void Window_MouseDown(object sender, MouseButtonEventArgs e)
        {
            DragMove();
        }

        private void Positive_Click(object sender, RoutedEventArgs e)
        {
            if (string.IsNullOrWhiteSpace(txtCaseReference.Text) && string.IsNullOrWhiteSpace(txtCaseOperator.Text) && string.IsNullOrWhiteSpace(txtNotes.Text))
                return;


            //Confirm that the user knows they aren't allowed to go back from this            
            if (new MsgBox(Strings.ConfirmWriteExtraData, MsgBoxOptions.OkCancel, true).ShowDialog() != true)
                return;


            CaseReference = txtCaseReference.Text;
            CaseOperator = txtCaseOperator.Text;
            Notes = txtNotes.Text;  

            this.DialogResult = true;
        }

        private void Negative_Click(object sender, RoutedEventArgs e)
        {
            this.DialogResult = false;
        }        
    }
}

