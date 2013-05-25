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
    public partial class FlipDialog : Form
    {
        public FlipDialog(MainForm mainForm)
        {
            this.mainForm = mainForm;
            InitializeComponent();
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            RotateFlipType flip = RotateFlipType.RotateNoneFlipNone;
            if ( radioBtn90.Checked )
                flip = RotateFlipType.Rotate90FlipNone;
            else if ( radioBtn180.Checked )
                flip = RotateFlipType.Rotate180FlipNone;
            else if ( radioBtn270.Checked )
                flip = RotateFlipType.Rotate270FlipNone;
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
