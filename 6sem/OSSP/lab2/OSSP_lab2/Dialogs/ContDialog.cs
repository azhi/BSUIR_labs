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
    public partial class ContDialog : Form
    {
        public ContDialog(MainForm mainForm)
        {
            this.mainForm = mainForm;
            InitializeComponent();
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            try
            {
                int val = Int32.Parse(textBoxCont.Text);
                if ( val < -100 || val > 100 )
                    MessageBox.Show("Wrong number. Please enter number between -100 and 100.", "Range error");
                mainForm.myImage.adjustContrast((val + 100) * (val + 100) / 10000.0);
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
