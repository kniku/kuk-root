using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media;

namespace Knk.GuiWPF
{
    /// <summary>
    /// Base class of KnkWPF windows...
    /// </summary>
    public abstract class BaseWindow : Window
    {
        protected BaseWindow()
        {
            Background = Brushes.AliceBlue;
        }
    }
}
