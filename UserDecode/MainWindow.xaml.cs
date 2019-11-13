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
        public MainWindow()
        {
            InitializeComponent();

            var pressure = 12.51;
            var baseTemp = 38;
            var preTemp = 189;

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
            if (!IsLoaded)
                return;

            var cipher = new UserCipher(SldPressure.Value, SldBase.Value, SldPrecursor.Value);
            TxtCode.Text = cipher.Cipher;
        }

        private void TxtCode_TextChanged(object sender, TextChangedEventArgs e)
        {
            new UserCipher(TxtCode.Text);
        }
    }
}
