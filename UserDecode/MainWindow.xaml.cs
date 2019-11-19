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

namespace UserDecode
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        bool blockStackOverflow = false;

        public MainWindow()
        {
            InitializeComponent();
            
            var pressure = 15.04;
            var baseTemp = 36;
            var preTemp = 178;

            var first = new UserCipher(pressure, baseTemp, preTemp);
            var second = new UserCipher(first.Cipher);

            if (second.Pressure != pressure)
                throw new Exception("BAD PRESSURE!\n");
            if (second.BaseTemp != baseTemp)
                throw new Exception("BAD TEMP\n");
            if (second.PreTemp != preTemp)
                throw new Exception("BAD PRETEMP!\n");


        }

        private void ValuesChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (blockStackOverflow)
                return;

            if (!IsLoaded)
                return;
            
                blockStackOverflow = true;
                var cipher = new UserCipher(SldPressure.Value, SldBase.Value, SldPrecursor.Value);
                TxtCipherResult.Text = cipher.Cipher;
                blockStackOverflow = false;


        }

        private void TxtCode_TextChanged(object sender, TextChangedEventArgs e)
        {
            if (blockStackOverflow)
                return;

            try
            {
                if (!Int32.TryParse(TxtCode.Text, System.Globalization.NumberStyles.HexNumber, System.Globalization.CultureInfo.InvariantCulture, out int result))
                    throw new Exception("Invalid HEX Code!");

                blockStackOverflow = true;
                var decodedCipher = new UserCipher(TxtCode.Text);
                SldPressure.Value = decodedCipher.Pressure;
                SldBase.Value = decodedCipher.BaseTemp;
                blockStackOverflow = false;
                SldPrecursor.Value = decodedCipher.PreTemp;

                TxtInvalidCipher.Visibility = Visibility.Collapsed;
            }
            catch
            {
                TxtInvalidCipher.Visibility = Visibility.Visible;
            }
        }
    }
}
