using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Windows;

namespace BI_Gui
{
	/// <summary>
	/// Interaktionslogik für "App.xaml"
	/// </summary>
	public partial class App : Application
	{
		public App()
		{
			// setze culture entsprechend den Computereinstellungen...
			System.Windows.FrameworkElement.LanguageProperty.OverrideMetadata(
				typeof(System.Windows.FrameworkElement),
				new System.Windows.FrameworkPropertyMetadata(
					System.Windows.Markup.XmlLanguage.GetLanguage(System.Globalization.CultureInfo.CurrentCulture.IetfLanguageTag)));
		}
	}
}
