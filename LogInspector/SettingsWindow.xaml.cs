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

namespace RecoverLogInspector
{
    /// <summary>
    /// Interaction logic for SettingsWindow.xaml
    /// </summary>
    public partial class SettingsWindow : Window
    {
        public SettingsWindow()
        {
            InitializeComponent();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            var settings = SettingsManager.Load();

            TxtAvgPeakBaseTempLCL.Text = settings.AveragePeakBaseTemperatureLCL.ToString();
            TxtAvgPeakBaseTempHCL.Text = settings.AveragePeakBaseTemperatureHCL.ToString();
            TxtAvgPeakBaseTempAVG.Text = settings.AveragePeakBaseTemperatureAVG.ToString();

            TxtAvgTimeToReachPeakTemperatureLCL.Text = settings.AverageBaseTimeLCL.ToString();
            TxtAvgTimeToReachPeakTemperatureHCL.Text = settings.AverageBaseTimeHCL.ToString();
            TxtAvgTimeToReachPeakTemperatureAVG.Text = settings.AverageBaseTimeAVG.ToString();

            TxtAvgPumpdownTimeLCL.Text = settings.AveragePumpdownTimeLCL.ToString();
            TxtAvgPumpdownTimeHCL.Text = settings.AveragePumpdownTimeHCL.ToString();
            TxtAvgPumpdownTimeAVG.Text = settings.AveragePumpdownTimeAVG.ToString();

            TxtAvgTimeToReachPrecursorTempLCL.Text = settings.AveragePrecursorTimeLCL.ToString();
            TxtAvgTimeToReachPrecursorTempHCL.Text = settings.AveragePrecursorTimeHCL.ToString();
            TxtAvgTimeToReachPrecursorTempAVG.Text = settings.AveragePrecursorTimeAVG.ToString();



            TxtIndividualPeakBaseTempLCL.Text = settings.IndividualPeakBaseTemperatureLCL.ToString();
            TxtIndividualPeakBaseTempHCL.Text = settings.IndividualPeakBaseTemperatureHCL.ToString();
            TxtIndividualPeakBaseTempAVG.Text = settings.IndividualPeakBaseTemperatureAVG.ToString();

            TxtIndividualTimeToReachPeakBaseTemperatureLCL.Text = settings.IndividualBaseTimeLCL.ToString();
            TxtIndividualTimeToReachPeakBaseTemperatureHCL.Text = settings.IndividualBaseTimeHCL.ToString();
            TxtIndividualTimeToReachPeakBaseTemperatureAVG.Text = settings.IndividualBaseTimeAVG.ToString();

            TxtIndividualPumpdownTimeLCL.Text = settings.IndividualPumpdownTimeLCL.ToString();
            TxtIndividualPumpdownTimeHCL.Text = settings.IndividualPumpdownTimeHCL.ToString();
            TxtIndividualPumpdownTimeAVG.Text = settings.IndividualPumpdownTimeAVG.ToString();

            TxtIndividualTimeToReachPrecursorTempLCL.Text = settings.IndividualPrecursorTimeLCL.ToString();
            TxtIndividualTimeToReachPrecursorTempHCL.Text = settings.IndividualPrecursorTimeHCL.ToString();
            TxtIndividualTimeToReachPrecursorTempAVG.Text = settings.IndividualPrecursorTimeAVG.ToString();

        }

        private void BtnExit_Clicked(object sender, RoutedEventArgs e)
        {
            Close();
        }

        private void BtnSave_Clicked(object sender, RoutedEventArgs e)
        {
            try
            {
                var settings = new SettingsManager();


                settings.AveragePeakBaseTemperatureLCL = int.Parse(TxtAvgPeakBaseTempLCL.Text);
                settings.AveragePeakBaseTemperatureHCL = int.Parse(TxtAvgPeakBaseTempHCL.Text);
                settings.AveragePeakBaseTemperatureAVG = int.Parse(TxtAvgPeakBaseTempAVG.Text);

                settings.AverageBaseTimeLCL = int.Parse(TxtAvgTimeToReachPeakTemperatureLCL.Text);
                settings.AverageBaseTimeHCL = int.Parse(TxtAvgTimeToReachPeakTemperatureHCL.Text);
                settings.AverageBaseTimeAVG = int.Parse(TxtAvgTimeToReachPeakTemperatureAVG.Text);

                settings.AveragePumpdownTimeLCL = int.Parse(TxtAvgPumpdownTimeLCL.Text);
                settings.AveragePumpdownTimeHCL = int.Parse(TxtAvgPumpdownTimeHCL.Text);
                settings.AveragePumpdownTimeAVG = int.Parse(TxtAvgPumpdownTimeAVG.Text);

                settings.AveragePrecursorTimeLCL = int.Parse(TxtAvgTimeToReachPrecursorTempLCL.Text);
                settings.AveragePrecursorTimeHCL = int.Parse(TxtAvgTimeToReachPrecursorTempHCL.Text);
                settings.AveragePrecursorTimeAVG = int.Parse(TxtAvgTimeToReachPrecursorTempAVG.Text);


                settings.IndividualPeakBaseTemperatureLCL = int.Parse(TxtIndividualPeakBaseTempLCL.Text);
                settings.IndividualPeakBaseTemperatureHCL = int.Parse(TxtIndividualPeakBaseTempHCL.Text);
                settings.IndividualPeakBaseTemperatureAVG = int.Parse(TxtIndividualPeakBaseTempAVG.Text);

                settings.IndividualBaseTimeLCL = int.Parse(TxtIndividualTimeToReachPeakBaseTemperatureLCL.Text);
                settings.IndividualBaseTimeHCL = int.Parse(TxtIndividualTimeToReachPeakBaseTemperatureHCL.Text);
                settings.IndividualBaseTimeAVG = int.Parse(TxtIndividualTimeToReachPeakBaseTemperatureAVG.Text);

                settings.IndividualPumpdownTimeLCL = int.Parse(TxtIndividualPumpdownTimeLCL.Text);
                settings.IndividualPumpdownTimeHCL = int.Parse(TxtIndividualPumpdownTimeHCL.Text);
                settings.IndividualPumpdownTimeAVG = int.Parse(TxtIndividualPumpdownTimeAVG.Text);

                settings.IndividualPrecursorTimeLCL = int.Parse(TxtIndividualTimeToReachPrecursorTempLCL.Text);
                settings.IndividualPrecursorTimeHCL = int.Parse(TxtIndividualTimeToReachPrecursorTempHCL.Text);
                settings.IndividualPrecursorTimeAVG = int.Parse(TxtIndividualTimeToReachPrecursorTempAVG.Text);


                settings.Save();

            }
            catch (Exception)
            {
                MessageBox.Show("Unable to save settings!", "Error", MessageBoxButton.OK, MessageBoxImage.Error);                
            }
        }
    }
}
