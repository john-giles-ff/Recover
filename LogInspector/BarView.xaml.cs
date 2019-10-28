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

namespace RecoverLogInspector
{
    /// <summary>
    /// Interaction logic for barView.xaml
    /// </summary>
    public partial class BarView : UserControl
    {
        public EventHandler<double> OnPointClicked;

        public BarView()
        {
            InitializeComponent();
        }

        private void On_DataClick(object sender, LiveCharts.ChartPoint chartPoint)
        {
            OnPointClicked?.Invoke(sender, chartPoint.X);
        }

        private void Chart_baseTemp_Loaded(object sender, RoutedEventArgs e)
        {

        }
    }
}
