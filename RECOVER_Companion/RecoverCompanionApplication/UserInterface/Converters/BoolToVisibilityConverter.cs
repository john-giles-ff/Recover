using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Data;

namespace FosterAndFreeman.RecoverCompanionApplication.UserInterface.Converters
{
    class BoolToVisibilityConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            var boolean = value as bool?;
            if (boolean != null)
            {
                if (boolean == true)
                    return Visibility.Visible;
                else
                    return Visibility.Collapsed;
            }

            var integer = value as int?;
            if (integer != null)
            {
                if (integer > 0)
                    return Visibility.Visible;
                else
                    return Visibility.Collapsed;
            }



            return Visibility.Visible;



        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}
