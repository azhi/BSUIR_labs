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
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Drawing;
using Microsoft.Win32;
using System.IO;
using System.Drawing.Imaging;

namespace OSSP_lab3
{
    /// <summary>
    /// Логика взаимодействия для MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            myImage = new MyImage(400, 400);
            using (Graphics grp = Graphics.FromImage(myImage.image))
            {
                grp.FillRectangle(System.Drawing.Brushes.White, 0, 0, 400, 400);
            }

            mainImage.Source = myImage.scr_image;
        }

        void resize_Click(object sender, RoutedEventArgs e)
        {
            Dialogs.Resize dlg = new Dialogs.Resize();

            dlg.Owner = this;
            dlg.resHeight = 100;
            dlg.resWidth = 100;

            if ( dlg.ShowDialog() == true )
            {
                myImage.resize(dlg.resWidth, dlg.resHeight);
            }

            mainImage.Source = myImage.scr_image;
            mainImage.InvalidateVisual();
        }

        private void flip_Click(object sender, RoutedEventArgs e)
        {
            Dialogs.Flip dlg = new Dialogs.Flip();

            dlg.Owner = this;
            dlg.angle = RotateFlipType.RotateNoneFlipNone;

            if (dlg.ShowDialog() == true)
            {
                myImage.image.RotateFlip(dlg.angle);
            }

            mainImage.Source = myImage.scr_image;
            mainImage.InvalidateVisual();
        }

        private void mirror_Click(object sender, RoutedEventArgs e)
        {
            Dialogs.Mirror dlg = new Dialogs.Mirror();

            dlg.Owner = this;
            dlg.mirror = RotateFlipType.RotateNoneFlipNone;

            if (dlg.ShowDialog() == true)
            {
                myImage.image.RotateFlip(dlg.mirror);
            }

            mainImage.Source = myImage.scr_image;
            mainImage.InvalidateVisual();
        }

        private void brightness_Click(object sender, RoutedEventArgs e)
        {
            Dialogs.Brightness dlg = new Dialogs.Brightness();

            dlg.Owner = this;
            dlg.adjustBrightness = 0;

            if (dlg.ShowDialog() == true)
            {
                myImage.adjustBrightness(dlg.adjustBrightness);
            }

            mainImage.Source = myImage.scr_image;
            mainImage.InvalidateVisual();
        }

        private void contrast_Click(object sender, RoutedEventArgs e)
        {
            Dialogs.Contrast dlg = new Dialogs.Contrast();

            dlg.Owner = this;
            dlg.adjustContrast = 0;

            if (dlg.ShowDialog() == true)
            {
                if (dlg.adjustContrast < -100 || dlg.adjustContrast > 100)
                    MessageBox.Show("Wrong number. Please enter number between -100 and 100.", "Range error");
                myImage.adjustContrast((dlg.adjustContrast + 100) * (dlg.adjustContrast + 100) / 10000.0);
            }

            mainImage.Source = myImage.scr_image;
            mainImage.InvalidateVisual();
        }

        private void load_Click(object sender, RoutedEventArgs e)
        {
            OpenFileDialog lfd = new OpenFileDialog();
            if (lfd.ShowDialog() == true)
            {
                myImage.image = new Bitmap(lfd.FileName);
                mainImage.Source = myImage.scr_image;
                mainImage.InvalidateVisual();
            }
        }

        private void save_Click(object sender, RoutedEventArgs e)
        {
            SaveFileDialog sfd = new SaveFileDialog();
            if (sfd.ShowDialog() == true)
            {
                string extension = System.IO.Path.GetExtension(sfd.FileName);

                switch (extension.ToLower())
                {
                    case ".bmp":
                        myImage.image.Save(sfd.FileName, ImageFormat.Bmp);
                        break;
                    case ".jpg":
                        myImage.image.Save(sfd.FileName, ImageFormat.Jpeg);
                        break;
                    case ".png":
                        myImage.image.Save(sfd.FileName, ImageFormat.Png);
                        break;
                    default:
                        throw new ArgumentOutOfRangeException(extension);
                }

            }
        }

        private void mainImage_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            isDrawing = true;
            lastPoint = new System.Drawing.Point((int)Math.Round(e.GetPosition(mainImage).X), (int)Math.Round(e.GetPosition(mainImage).Y));
        }

        private void mainImage_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            isDrawing = false;
        }

        private void mainImage_MouseMove(object sender, MouseEventArgs e)
        {
            if (isDrawing)
            {
                System.Drawing.Point p = new System.Drawing.Point((int)Math.Round(e.GetPosition(mainImage).X), (int)Math.Round(e.GetPosition(mainImage).Y));
                using (Graphics grp = Graphics.FromImage(myImage.image))
                {
                    grp.DrawLine(Pens.Black, p, lastPoint);
                }
                lastPoint = p;
                mainImage.Source = myImage.scr_image;
                mainImage.InvalidateVisual();
            }
        }

        private MyImage myImage;
        private bool isDrawing;
        private System.Drawing.Point lastPoint;
    }
}
