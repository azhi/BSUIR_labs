using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Drawing.Imaging;

namespace OSSP_lab2
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();
            _myImage = new MyImage(pictureBox.Height, pictureBox.Width);
            using (Graphics grp = Graphics.FromImage(_myImage.image))
            {
                grp.FillRectangle(Brushes.White, 0, 0, pictureBox.Height, pictureBox.Width);
            }
            pictureBox.Image = _myImage.image;
            pointsToDraw = new List<Point>();
        }

        public void refreshImageBox()
        {
            pictureBox.Image = _myImage.image;
            pictureBox.Refresh();
        }

        public MyImage myImage { get { return _myImage; } }

        private MyImage _myImage;

        private bool isDrawing;
        private List<Point> pointsToDraw;


        private void btnLoad_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();

            ofd.InitialDirectory = "c:\\";
            ofd.Filter = "image files (*.bmp, *.jpg, *png)|*.bmp;*.jpg;*.png";
            ofd.RestoreDirectory = true;
            if (ofd.ShowDialog() == DialogResult.OK)
            {
                _myImage.image = new Bitmap(ofd.FileName);
            }
            refreshImageBox();
        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            SaveFileDialog sfd = new SaveFileDialog();

            sfd.InitialDirectory = "c:\\";
            sfd.Filter = "image files (*.bmp, *.jpg, *png)|*.bmp;*.jpg;*.png";
            sfd.RestoreDirectory = true;
            if (sfd.ShowDialog() == DialogResult.OK)
            {
                string extension = Path.GetExtension(sfd.FileName);

                switch (extension.ToLower())
                {
                    case ".bmp":
                        _myImage.image.Save(sfd.FileName, ImageFormat.Bmp);
                        break;
                    case ".jpg":
                        _myImage.image.Save(sfd.FileName, ImageFormat.Jpeg);
                        break;
                    case ".png":
                        _myImage.image.Save(sfd.FileName, ImageFormat.Png);
                        break;
                    default:
                        throw new ArgumentOutOfRangeException(extension);
                }
            }
        }

        private void btnFlip_Click(object sender, EventArgs e)
        {
            Dialogs.FlipDialog fd = new Dialogs.FlipDialog(this);
            fd.ShowDialog();
        }

        private void btnMirror_Click(object sender, EventArgs e)
        {
            Dialogs.MirrorDialog md = new Dialogs.MirrorDialog(this);
            md.ShowDialog();
        }

        private void btnBright_Click(object sender, EventArgs e)
        {
            Dialogs.BrightDialog bd = new Dialogs.BrightDialog(this);
            bd.ShowDialog();
        }

        private void btnCont_Click(object sender, EventArgs e)
        {
            Dialogs.ContDialog cd = new Dialogs.ContDialog(this);
            cd.ShowDialog();
        }

        private void btnResize_Click(object sender, EventArgs e)
        {
            Dialogs.ResizeDialog rd = new Dialogs.ResizeDialog(this);
            rd.ShowDialog();
        }

        private void pictureBox_MouseDown(object sender, MouseEventArgs e)
        {
            isDrawing = true;
        }

        private void pictureBox_MouseUp(object sender, MouseEventArgs e)
        {
            isDrawing = false;
            using (Graphics grp = Graphics.FromImage(_myImage.image))
            {
                if ( pointsToDraw.Count > 1 )
                    grp.DrawLines(Pens.Black, pointsToDraw.ToArray());
            }
            pointsToDraw.Clear();
            refreshImageBox();
        }

        private void pictureBox_MouseMove(object sender, MouseEventArgs e)
        {
            if (isDrawing)
            {
                pointsToDraw.Add(e.Location);
                pictureBox.Invalidate();
            }
        }

        private void pictureBox_Paint(object sender, PaintEventArgs e)
        {
            if (pointsToDraw.Count > 1)
                e.Graphics.DrawLines(Pens.Black, pointsToDraw.ToArray());
        }
    }
}
