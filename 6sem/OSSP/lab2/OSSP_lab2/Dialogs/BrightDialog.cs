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
    public partial class BrightDialog : Form
    {
        public BrightDialog(MainForm mainForm)
        {
            this.mainForm = mainForm;
            InitializeComponent();
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            try
            {
                mainForm.myImage.adjustBrightness(Int32.Parse(textBoxBright.Text));
                mainForm.refreshImageBox();
                this.Close();
            }
            catch(ArgumentException ex)
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
