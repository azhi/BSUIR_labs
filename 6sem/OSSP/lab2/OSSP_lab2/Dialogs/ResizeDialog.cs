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
    public partial class ResizeDialog : Form
    {
        public ResizeDialog(MainForm mainForm)
        {
            this.mainForm = mainForm;
            InitializeComponent();
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            try
            {
                int width = Int32.Parse(textBoxWidth.Text);
                int height = Int32.Parse(textBoxHeigth.Text);
                mainForm.myImage.resize(width, height);
                mainForm.refreshImageBox();
                this.Close();
            }
            catch (ArgumentException ex)
            {
                MessageBox.Show("Wrong format. Please enter valid number.", "Format error");
            }
        }

        private MainForm mainForm;

        private void btnCancel_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}
