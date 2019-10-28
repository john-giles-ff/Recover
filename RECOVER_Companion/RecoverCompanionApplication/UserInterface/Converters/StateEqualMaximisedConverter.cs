using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Data;

namespace FosterAndFreeman.RecoverCompanionApplication.UserInterface.Converters
{
    class StateEqualMaximisedConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            var state = value as System.Windows.WindowState?;
            return state != null && state == System.Windows.WindowState.Maximized;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}
