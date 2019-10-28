namespace FosterAndFreeman.RecoverCompanionApplication.Definitions.WPF
{
    using System;
    using System.Windows.Input;

    /// <summary>
    /// Represents a command that a button can bind to
    /// </summary>
    public sealed class ButtonCommand : ICommand
    {
        #region Private Fields

        /// <summary>
        /// The action to execute
        /// </summary>
        private readonly Action<object> _action;

        /// <summary>
        /// Whether we are allowed to execute the action
        /// </summary>
        private readonly bool _canExecute;

        #endregion Private Fields

        #region Public Constructors

        /// <summary>
        /// Create a new command handler
        /// </summary>
        /// <param name="action"> The action to execute </param>
        public ButtonCommand(Action<object> action)
        {
            _action = action;
            _canExecute = true;
            CanExecuteChanged?.Invoke(null, null);
        }

        #endregion Public Constructors

        #region Public Events

        /// <summary>
        /// Fires when can execute changes
        /// </summary>
        public event EventHandler CanExecuteChanged;

        #endregion Public Events

        #region Public Methods

        /// <summary>
        /// Gets whether we are allowed to execute the command
        /// </summary>
        /// <param name="parameter"> The parameters </param>
        /// <returns> A Boolean </returns>
        public bool CanExecute(object parameter)
        {
            return _canExecute;
        }

        /// <summary>
        /// Executes the action
        /// </summary>
        /// <param name="parameter"> Any parameters </param>
        public void Execute(object parameter = null)
        {
            _action(parameter);
        }

        #endregion Public Methods
    }
}