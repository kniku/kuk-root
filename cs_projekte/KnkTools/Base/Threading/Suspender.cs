using System;
using System.Collections.Generic;
using System.Text;

namespace Knk.Base.Threading
{
    public class Suspender : IDisposable
    {
        /// <summary>
        /// Usage:
        /// using (var suspender = new Suspender(suspend))
        /// {
        ///     if (suspender.Suspended) { ... }
        /// }
        /// </summary>
        /// <param name="suspend">initial suspend</param>
        public Suspender(bool suspend = true)
        {
            Suspended = suspend;
        }

        public bool Suspended { get; set; }

        void IDisposable.Dispose()
        {
            Suspended = false;
        }
    }
}
