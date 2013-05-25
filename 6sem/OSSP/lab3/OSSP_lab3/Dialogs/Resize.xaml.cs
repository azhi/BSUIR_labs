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
    public partial class Resize : Window
    {
        public Resize()
        {
            InitializeComponent();
        }

        void okButton_Click(object sender, RoutedEventArgs e)
        {
            this.DialogResult = true;
            try
            {
                resWidth = Int32.Parse(WidthTextBox.Text);
                resHeight = Int32.Parse(HeightTextBox.Text);
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine(ex.Data);
            }
        }

        private int _width;
        public int resWidth { get { return _width; } set { _width = value; } }
        private int _height;
        public int resHeight { get { return _height; } set { _height = value; } }
    }
}
