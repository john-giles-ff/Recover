namespace FosterAndFreeman.RecoverCompanionApplication.Definitions.BaseClasses
{
    using System;
    using System.ComponentModel;
    using System.Linq.Expressions;
    using System.Runtime.CompilerServices;

    /// <summary>
    /// The base class for any class that wants to provide property changing notifiers
    /// </summary>
    [Serializable]
    public abstract class PropertyChangedBase : INotifyPropertyChanged
    {
        #region Public Events

        /// <summary>
        /// Fires when a property changes
        /// </summary>
        [field: NonSerialized]
        public event PropertyChangedEventHandler PropertyChanged;

        #endregion Public Events

        #region Protected Methods

        /// <summary>
        /// Raises the property changed event
        /// </summary>
        protected void RaisePropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        protected void RaisePropertyChanged<T>(T property)
        {
            var propertyName = GetPropertyName(() => property);

            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        #endregion Protected Methods

        #region Private Methods

        private string GetPropertyName<T>(Expression<Func<T>> propertyLambda)
        {
            var me = propertyLambda.Body as MemberExpression;

            if (me == null)
            {
                throw new ArgumentException("You must pass a lambda of the form: '() => Class.Property' or '() => object.Property'");
            }

            return me.Member.Name;
        }

        #endregion Private Methods
    }
}