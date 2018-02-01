using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Data;

namespace Knk.GuiWPF.DynamicWindows
{
    public class DynamicWindowPropertyAttribute : Attribute
    {
        public string Label { get; set; }
        public string ToolTip { get; set; }
        public bool Ignore { get; set; }
    }

    public class SimplePropertyWindow : BaseWindow
    {
        public SimplePropertyWindow(string title, object data)
        {
            createControlsAndBindings(data);

            Title = title;
            //Height = countRows * 30;
            Width = 300;
            SizeToContent = SizeToContent.Height;
            //this.ResizeMode = ResizeMode.NoResize;
            DataContext = data;
        }

        void createControlsAndBindings(object Data)
        {
            Grid rootGrid = new Grid();
            rootGrid.Margin = new Thickness(10.0);

            rootGrid.ColumnDefinitions.Add(new ColumnDefinition() { Width = GridLength.Auto/*new GridLength(100.0)*/ });
            rootGrid.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Star) });

            int countRows = 0;
            RowDefinition rd;
            foreach (var prop in Data.GetType().GetProperties())
            {
                PropertyInfo propertyInfo = Data.GetType().GetProperty(prop.Name);
                if (propertyInfo != null)
                {
                    var propertyAttrib = propertyInfo.GetCustomAttributes<DynamicWindowPropertyAttribute>().FirstOrDefault();
                    if (propertyAttrib != null && propertyAttrib.Ignore)
                        continue;

                    rd = new RowDefinition();
                    rd.Height = GridLength.Auto;
                    rootGrid.RowDefinitions.Add(rd);

                    string label = propertyAttrib == null ? propertyInfo.Name : propertyAttrib.Label;
                    if (label != null) label += ": ";
                    var Label = new Label { Content = label };
                    Label.FontWeight = FontWeights.Bold;
                    Grid.SetRow(Label, countRows);
                    Grid.SetColumn(Label, 0);
                    rootGrid.Children.Add(Label);

                    Control control;

                    if (propertyInfo.PropertyType == typeof(bool))
                    {
                        var picker = new CheckBox();
                        var binding = new Binding(propertyInfo.Name);
                        binding.Mode = BindingMode.TwoWay;
                        picker.SetBinding(ToggleButton.IsCheckedProperty, binding);
                        control = picker;
                    }
                    else
                    {
                        var picker = new TextBox();
                        var binding = new Binding(propertyInfo.Name);
                        binding.Mode = BindingMode.TwoWay;
                        if (!propertyInfo.CanWrite)
                        {
                            picker.IsReadOnly = true;
                            binding.Mode = BindingMode.OneWay;
                        }
                        picker.SetBinding(TextBox.TextProperty, binding);
                        control = picker;
                    }

                    if (propertyAttrib != null && propertyAttrib.ToolTip != null)
                        control.ToolTip = new ToolTip {Content = propertyAttrib.ToolTip};
                    Grid.SetRow(control, countRows);
                    Grid.SetColumn(control, 1);
                    rootGrid.Children.Add(control);
                    countRows++;
                }
            }

            rd = new RowDefinition();
            rd.Height = GridLength.Auto;
            rootGrid.RowDefinitions.Add(rd);
            Button b = new Button() {Content = "Ok"};
            b.Click += (sender, args) => { DialogResult = true; };
            Grid.SetRow(b, countRows++);
            Grid.SetColumn(b, 1);
            rootGrid.Children.Add(b);
            b = new Button() {Content = "Cancel", IsCancel = true};
            Grid.SetRow(b, countRows++);
            Grid.SetColumn(b, 0);
            rootGrid.Children.Add(b);

            AddChild(rootGrid);
        }
    }
}

