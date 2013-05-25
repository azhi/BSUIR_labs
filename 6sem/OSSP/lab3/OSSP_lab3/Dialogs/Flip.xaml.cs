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
using System.Drawing;

namespace OSSP_lab3.Dialogs
{
    /// <summary>
    /// Логика взаимодействия для Window1.xaml
    /// </summary>
    public partial class Flip : Window
    {
        public Flip()
        {
            InitializeComponent();
        }

        void okButton_Click(object sender, RoutedEventArgs e)
        {
            this.DialogResult = true;
            try
            {
                if (RB90.IsChecked.HasValue && RB90.IsChecked.Value)
                    angle = RotateFlipType.Rotate90FlipNone;
                else if (RB180.IsChecked.HasValue && RB180.IsChecked.Value)
                    angle = RotateFlipType.Rotate180FlipNone;
                else if (RB270.IsChecked.HasValue && RB270.IsChecked.Value)
                    angle = RotateFlipType.Rotate270FlipNone;
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine(ex.Data);
            }
        }

        private RotateFlipType _angle;
        public RotateFlipType angle { get { return _angle; } set { _angle = value; } }
    }
}
