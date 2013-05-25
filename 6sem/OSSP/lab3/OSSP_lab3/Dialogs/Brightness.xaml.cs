using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace OSSP_lab3.Dialogs
{
    /// <summary>
    /// Логика взаимодействия для Window1.xaml
    /// </summary>
    public partial class Brightness : Window
    {
        public Brightness()
        {
            InitializeComponent();
        }

        void okButton_Click(object sender, RoutedEventArgs e)
        {
            this.DialogResult = true;
            try
            {
                adjustBrightness = Int32.Parse(BrightnessTextBox.Text);
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine(ex.Data);
            }
        }

        private int _adjustBrightness;
        public int adjustBrightness { get { return _adjustBrightness; } set { _adjustBrightness = value; } }
    }
}
