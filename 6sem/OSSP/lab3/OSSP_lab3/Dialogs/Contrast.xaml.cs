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
    public partial class Contrast : Window
    {
        public Contrast()
        {
            InitializeComponent();
        }

        void okButton_Click(object sender, RoutedEventArgs e)
        {
            this.DialogResult = true;
            try
            {
                adjustContrast = Int32.Parse(ContrastTextBox.Text);
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine(ex.Data);
            }
        }

        private int _adjustContrast;
        public int adjustContrast { get { return _adjustContrast; } set { _adjustContrast = value; } }
    }
}
