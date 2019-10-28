using FosterAndFreeman.RecoverCompanionApplication.Definitions.BaseClasses;
using FosterAndFreeman.RecoverCompanionApplication.Definitions.DeviceData;
using FosterAndFreeman.RecoverCompanionApplication.Definitions.Misc;
using FosterAndFreeman.RecoverCompanionApplication.Resources.Languages;
using FosterAndFreeman.RecoverCompanionApplication.UserInterface.Views;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Globalization;
using System.Linq;
using System.Threading;
using System.Windows;
using System.Windows.Controls;

namespace FosterAndFreeman.RecoverCompanionApplication
{

    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        public static UserInterface.Views.MainWindow MainAppWindow;
        public static List<RecoverLog> Logs;
        public static List<RecoverLog> USBLogs;
        public static EventHandler LogsChanged;
        public static string[] Arguments;        
        private static Mutex _mutex;
        private static bool _isAdvanced = false;
        private static CONNECTION_MODE _connectionMode;
        private static AppCommunication _appCommunication;


        public static CONNECTION_MODE ConnectionMode
        {
            get
            {
                return _connectionMode;
            }
            set
            {
                _connectionMode = value;                                
            }
        }


        public static bool IsAdvanced
        {
            get
            {
                return _isAdvanced;
            }
            set
            {
                _isAdvanced = value;                
            }
        }
        

        private static LowLevelKeyboardHook _lowLevelKeyboard;
        private static List<Keys> _previousKeys;
        private static bool _isCtrlDown = false;
        private static bool _exitCommited = false;
        private static readonly Keys[] _engineerPhrase =
        {
            Keys.F,
            Keys.R,
            Keys.E,
            Keys.D,
            Keys.A
        };

        

        private void Application_Startup(object sender, StartupEventArgs e)
        {
            Arguments = e.Args;

            //Set Language
            var languageName = FosterAndFreeman.RecoverCompanionApplication.Properties.Settings.Default.Language;
            var language = new CultureInfo(languageName);
            CultureInfo.DefaultThreadCurrentCulture = language;
            CultureInfo.DefaultThreadCurrentUICulture = language;
            Thread.CurrentThread.CurrentCulture = language;
            Thread.CurrentThread.CurrentUICulture = language;
            FrameworkElement.LanguageProperty.OverrideMetadata(typeof(FrameworkElement), new FrameworkPropertyMetadata(System.Windows.Markup.XmlLanguage.GetLanguage(language.IetfLanguageTag)));

            SingleInstanceSupport();

            MainAppWindow = new UserInterface.Views.MainWindow();
            MainAppWindow.Show();

            new System.Threading.Thread(() =>
            {
                Application.Current.Dispatcher.Invoke(() => { MainAppWindow.Splash.SetText(Strings.Initialising); });                

                Definitions.DeviceCommunications.RecoverManager.Initialise();

                Current.Dispatcher.Invoke(() =>
                {
                    //Setup Low Level Keyboard Hook to enter Engineer Mode
                    _previousKeys = new List<Keys>();
                    _lowLevelKeyboard = new LowLevelKeyboardHook();
                    _lowLevelKeyboard.HookKeyboard();
                    _lowLevelKeyboard.OnKeyPressed += LowLevelKeyboard_OnKeyPressed;
                    _lowLevelKeyboard.OnKeyUnpressed += _lowLevelKeyboard_OnKeyUnpressed;

                    //Setup ToolTips on Disabled Items
                    ToolTipService.ShowOnDisabledProperty.OverrideMetadata(typeof(Control), new FrameworkPropertyMetadata(true));
                });

                //Check if any files have been passed (Will automatically determine the mode)
                var lftFileExists = e.Args.Any(a => System.IO.File.Exists(a) && System.IO.Path.GetExtension(a).ToUpper() == ".LFT");
                var recoverFileExists = e.Args.Any(a => System.IO.File.Exists(a) && System.IO.Path.GetExtension(a).ToUpper() == ".RECOVER");

                if (!lftFileExists && !recoverFileExists)
                {
                    //Determine which mode is required
                    Application.Current.Dispatcher.Invoke(() => { MainAppWindow.GotoConnectionPicker(); });
                }
                else if (lftFileExists)
                {
                    ConnectionMode = CONNECTION_MODE.USB;
                    Application.Current.Dispatcher.Invoke(() =>
                    {
                        MainAppWindow.GotoSplash();
                        MainAppWindow.Splash.ConnectViaUSb();
                    });
                }
                else
                {
                    ConnectionMode = CONNECTION_MODE.FILE;
                    Application.Current.Dispatcher.Invoke(() =>
                    {
                        MainAppWindow.GotoLogs();
                    });
                }

            }).Start();

            MainAppWindow.Closed += MainAppWindow_Closed;
        }

        private void MainAppWindow_Closed(object sender, EventArgs e)
        {
            App.ExitApp();
        }

        /// <summary>
        /// Happens when a key is released
        /// </summary>        
        private void _lowLevelKeyboard_OnKeyUnpressed(object sender, Keys e)
        {
            //If the Ctrl Key is lifted, clear progress
            if (e != Keys.LControlKey)            
                return;
            
            _isCtrlDown = false;
            _previousKeys.Clear();
        }

        /// <summary>
        /// Happens when a key is pressed
        /// </summary>        
        private void LowLevelKeyboard_OnKeyPressed(object sender, Keys e)
        {
            //If the key is ctrl, setup and ignore
            if (e == Keys.LControlKey)
            {
                _isCtrlDown = true;
                return;
            }

            //Only care about key presses if ctrl is down
            if (!_isCtrlDown)
                return;

            //Update array to be last key's pressed
            _previousKeys.Add(e);
            if (_previousKeys.Count() > _engineerPhrase.Length)
                _previousKeys.RemoveAt(0);

            //Check if sequence is equal
            if (!_previousKeys.SequenceEqual(_engineerPhrase))
                return;

            //Open Engineer Window
            var engineer = new UserInterface.Views.EngineerWindow();
            engineer.Show();            
        }

        /// <summary>
        /// Safely Exit the Application 
        /// </summary>
        public static void ExitApp(bool restart = false)
        {
            Application.Current.Dispatcher.Invoke(() =>
            {
                try
                {
                    _mutex.ReleaseMutex();                    
                    MainAppWindow.Hide();
                    _lowLevelKeyboard.UnHookKeyboard();

                    if (ConnectionMode == CONNECTION_MODE.USB)
                    {
                        Definitions.DeviceCommunications.RecoverManager.ExitPCMode();
                        Definitions.DeviceCommunications.RecoverManager.Disconnect();
                    }
                }
                catch { }
                _exitCommited = true;


                if (restart)                
                    System.Diagnostics.Process.Start(ResourceAssembly.Location);

                Environment.Exit(0);
                
            });
        }

        public static void CheckSerial()
        {
            if (Definitions.DeviceCommunications.RecoverManager.SerialNumber != null)
                return;

            Application.Current.Dispatcher.Invoke(() =>
            {
                if (new SerialNumberInput().ShowDialog() != true)
                    ExitApp();
            });
        }

        /// <summary>
        /// Handles the single instance support
        /// </summary>
        private static void SingleInstanceSupport()
        {       
            //single instance support
            _mutex = new Mutex(true, "Recover Companion", out bool ok);

            if (ok)
            {
                _appCommunication = new AppCommunication();
                _appCommunication.OnCommunicationRecieved += On_ArgumentRecieved;

                new Thread(() =>
                {
                    _appCommunication.StartListening();
                }).Start();

                return;
            }

            try
            {                
                foreach (var arg in Arguments)                
                    AppCommunication.SendMessage(arg);
                
            }
            catch (Exception)
            {
                //show an error message
                Application.Current.Dispatcher.Invoke(() =>
                {
                    new ErrorBox(Strings.SingleInstanceError).ShowDialog();
                });
            }

            //exit the application
            ExitApp();
        }

        private static void On_ArgumentRecieved(object sender, string argument)
        {
            App.Current.Dispatcher.Invoke(() =>
            {
                //Check app is in a good state to suceed
                if (MainAppWindow == null || !MainAppWindow.IsInterruptable)
                {
                    new MsgBox(Strings.UnableToOpenFileRightNow, MsgBoxOptions.Ok, true).ShowDialog();
                    return;
                }

                //Check file exists
                if (!System.IO.File.Exists(argument))
                    return;


                //Check the file path ends with .RECOVER
                if (System.IO.Path.GetExtension(argument).ToUpper() != ".RECOVER")
                    return;

                //Attempt to decode it
                var log = new RecoverLog();
                log.Parse(argument);

                //Add it to currently loaded logs
                if (App.Logs == null)
                    App.Logs = new List<RecoverLog>();                

                //Only add if not already added
                if (!App.Logs.Any(a => a.FileName == log.FileName))
                    App.Logs.Add(log);

                //Goto Audit screen                
                App.MainAppWindow.Topmost = true;
                App.MainAppWindow.Topmost = false;

                MainAppWindow.GotoLogs(false);
                MainAppWindow.AuditLogs.ReloadLogs();
            });
        }

        private void Application_Exit(object sender, ExitEventArgs e)
        {
            if (!_exitCommited)
                ExitApp();

        }
    }
}
