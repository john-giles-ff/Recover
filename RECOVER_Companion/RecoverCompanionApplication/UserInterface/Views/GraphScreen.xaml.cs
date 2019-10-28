using FosterAndFreeman.RecoverCompanionApplication.Resources.Languages;
using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
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
    /// Interaction logic for GraphScreen.xaml
    /// </summary>
    public partial class GraphScreen : Window
    {

        public Definitions.DeviceData.RecoverLog _log { get; set; }

        public GraphScreen(Definitions.DeviceData.RecoverLog log)
        {
            InitializeComponent();

            _log = log;
            DataChart.DataContext = _log;
            DataContext = _log;

            Owner = App.MainAppWindow;
        }        
        


        private void Window_Loaded(object sender, RoutedEventArgs e)
        {       
            TxtTitle.Text = _log.StartTime.ToString();

            //If there are no samples, start thread loading them
            if (_log.Samples == null)
            {
                Definitions.DeviceCommunications.RecoverManager.OnSampleLoaded = OnSampleLoaded;
                Definitions.DeviceCommunications.RecoverManager.GetLogSamples(_log);
            }

            UpdateStrips();
        }

        private void OnSampleLoaded(object sender, Definitions.DeviceCommunications.SampleLoadedArgument e)
        {            
            Application.Current.Dispatcher.Invoke(() => 
            {
                UpdateStrips();
            });            
        }

        private void UpdateStrips()
        {
            InitStrip.StartDate = default(DateTime);

            if (_log.PumpDownStart != default(DateTime))
            {
                InitStrip.EndDate = _log.PumpDownStart;
                PumpDownStrip.StartDate = _log.PumpDownStart;

                if (_log.HeatStart != default(DateTime))
                {
                    PumpDownStrip.EndDate = _log.HeatStart;
                    HeatStrip.StartDate = _log.HeatStart;

                    if (_log.CoolStart != default(DateTime))
                    {
                        HeatStrip.EndDate = _log.CoolStart;
                        CoolStrip.StartDate = _log.CoolStart;

                        if (_log.BreakSealStart != default(DateTime))
                        {
                            CoolStrip.EndDate = _log.BreakSealStart;
                            BreaksealStrip.StartDate = _log.BreakSealStart;
                            BreaksealStrip.EndDate = _log.EndTime;
                        }
                        else
                            CoolStrip.EndDate = _log.EndTime;
                    }
                    else
                        HeatStrip.EndDate = _log.EndTime;
                }
                else
                    PumpDownStrip.EndDate = _log.EndTime;
            }
            else
                InitStrip.EndDate = _log.EndTime;


            Strips.RefreshStrips();
        }

        

        private void Window_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            DragMove();            
        }

        private void Exit_Clicked(object sender, RoutedEventArgs e)
        {
            Definitions.DeviceCommunications.RecoverManager.AbortGetLogSamples();
            Close();
        }

        private void ToggleWindowState_Clicked(object sender, RoutedEventArgs e)
        {
            if (this.WindowState == WindowState.Maximized)
                this.WindowState = WindowState.Normal;
            else
                this.WindowState = WindowState.Maximized;
        }

        private void WndGraph_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            if (this.WindowState == WindowState.Maximized)
            {
                BtnNormaliseState.Visibility = Visibility.Visible;
                BtnMaximiseState.Visibility = Visibility.Collapsed;
            }
            else
            {
                BtnNormaliseState.Visibility = Visibility.Collapsed;
                BtnMaximiseState.Visibility = Visibility.Visible;
            }
                    
        }

        private void Export_Clicked(object sender, RoutedEventArgs e)
        {
            //Ask for path
            var saveFileDialog = new SaveFileDialog()
            {
                FileName = _log.StartTime.ToString("yyyy-MM-dd-HH-mm-ss"),
                Filter = "PDF (*.PDF)|*.PDF|RTF (*.RTF)|*.RTF",
                AddExtension = true
            };
            if (saveFileDialog.ShowDialog() != true)
                return;

            //Before taking an image of the control, we need to resize it to get it to be a nice resolution regardless of current window size
            var targetSize = new Size(1920, 1080);
            var actualSize = new Size(DataChart.Width, DataChart.Height);
            var fullsize = new Size(targetSize.Width, targetSize.Height + LegendRenderBorder.ActualHeight);

            //Resize DataChart and update
            DataChart.Width = targetSize.Width;
            DataChart.Height = targetSize.Height;
            DataChart.Measure(targetSize);
            DataChart.Arrange(new Rect(targetSize));            
            DataChart.UpdateLayout();                        
            DataChart.Dispatcher.Invoke(() => { }, System.Windows.Threading.DispatcherPriority.Render);

            //Remove from current position and place into a viewbox (Required to render full image)
            //Also adds the legend in a stack panel
            RenderBorder.Child = null;
            LegendRenderBorder.Child = null;
            var content = new StackPanel() { Orientation = Orientation.Vertical, Background = Brushes.Black };
            content.Children.Add(Legend);
            content.Children.Add(DataChart);

            var box = new Viewbox
            {
                Child = content
            };
            content.Measure(fullsize);
            content.Arrange(new Rect(fullsize));
            content.UpdateLayout();            

            //Get Graph Render
            var graphBitmap = new RenderTargetBitmap((int)fullsize.Width, (int)fullsize.Height, 96, 96, PixelFormats.Pbgra32);
            graphBitmap.Render(box);


            //Return to original size and location
            content.Children.Remove(DataChart);
            content.Children.Remove(Legend);
            
            DataChart.Width = actualSize.Width;
            DataChart.Height = actualSize.Height;
            RenderBorder.Child = DataChart;
            LegendRenderBorder.Child = Legend;

            //Convert Graph image to byte array            
            var graphPath = System.IO.Path.Combine(System.IO.Path.GetDirectoryName(saveFileDialog.FileName), "temp.png");
            using (var stream = new System.IO.FileStream(graphPath, System.IO.FileMode.Create))
            {
                PngBitmapEncoder encoder = new PngBitmapEncoder();
                encoder.Frames.Add(BitmapFrame.Create(graphBitmap));
                encoder.Save(stream);
            }


            //Generate Report
            var success = false;
            var exception = new Exception();
            try
            {
                Definitions.Misc.Reporting.GenerateReport(saveFileDialog.FileName, _log, graphPath);
                success = true;
            }
            catch (Exception ex){ exception = ex; }

            //Clean up afterwards
            System.IO.File.Delete(graphPath);

            if (success)
                new MsgBox(Strings.ReportGenerated, MsgBoxOptions.Ok).ShowDialog();            
            else                
                new ErrorBox(Strings.UnableToGenerateReport).ShowDialog();


        }
    }
}
