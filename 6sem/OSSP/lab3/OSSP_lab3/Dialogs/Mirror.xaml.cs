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
    public partial class Mirror : Window
    {
        public Mirror()
        {
            InitializeComponent();
        }

        void okButton_Click(object sender, RoutedEventArgs e)
        {
            this.DialogResult = true;
            try
            {
                if (CBHoriz.IsChecked.HasValue && CBHoriz.IsChecked.Value &&
                    CBVert.IsChecked.HasValue && CBVert.IsChecked.Value)
                    mirror = RotateFlipType.RotateNoneFlipXY;
                else if (CBHoriz.IsChecked.HasValue && CBHoriz.IsChecked.Value)
                    mirror = RotateFlipType.RotateNoneFlipX;
                else if (CBVert.IsChecked.HasValue && CBVert.IsChecked.Value)
                    mirror = RotateFlipType.RotateNoneFlipY;
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine(ex.Data);
            }
        }

        private RotateFlipType _mirror;
        public RotateFlipType mirror { get { return _mirror; } set { _mirror = value; } }
    }
}
