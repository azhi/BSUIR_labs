using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace OSSP_lab2.Dialogs
{
    public partial class MirrorDialog : Form
    {
        public MirrorDialog(MainForm mainForm)
        {
            this.mainForm = mainForm;
            InitializeComponent();
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            RotateFlipType flip = RotateFlipType.RotateNoneFlipNone;
            if ( checkBoxHoriz.Checked && checkBoxVert.Checked )
                flip = RotateFlipType.RotateNoneFlipXY;
            else if ( checkBoxHoriz.Checked )
                flip = RotateFlipType.RotateNoneFlipX;
            else if ( checkBoxVert.Checked )
                flip = RotateFlipType.RotateNoneFlipY;
            mainForm.myImage.image.RotateFlip(flip);
            mainForm.refreshImageBox();
            this.Close();
        }

        private MainForm mainForm;

        private void btnCancel_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}
