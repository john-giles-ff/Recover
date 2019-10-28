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

namespace FosterAndFreeman.RecoverCompanionApplication.UserInterface
{
    /// <summary>
    /// Interaction logic for LanguageSelector.xaml
    /// </summary>
    public partial class LanguageSelector : Window
    {
        public LanguageSelector()
        {
            InitializeComponent();

            Owner = App.MainAppWindow;
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            //Get all languages
            var languages = Definitions.Misc.Languages.GetAvailableLanguages();

            languages = languages.OrderBy(a => a.DisplayName).ToArray();
            
            //Add all languges
            foreach (var language in languages)            
                LstLanguages.Items.Add(language);

            //Select Current language
            LstLanguages.SelectedItem = languages.First(a => a.Name == System.Globalization.CultureInfo.CurrentCulture.Name);            
        }

        private void BtnExit_Click(object sender, RoutedEventArgs e)
        {
            DialogResult = true;
        }

        private void BtnConfirm_Click(object sender, RoutedEventArgs e)
        {
            //Get selected language
            var newLanguage = (System.Globalization.CultureInfo)LstLanguages.SelectedItem;

            //Save Setting
            Properties.Settings.Default.Language = newLanguage.Name;
            Properties.Settings.Default.Save();

            //Restart 
            App.ExitApp(true);
        }
    }
}
