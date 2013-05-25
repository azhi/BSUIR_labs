using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace GUIPlugin1
{
    [ModuleGuiAttribute.Position(true)]
    public partial class GUIPlugin1Form : Form
    {
        public GUIPlugin1Form()
        {
            InitializeComponent();
        }

        private void btnHW_Click(object sender, EventArgs e)
        {
            MessageBox.Show("HELLO WORLD!");
        }
    }
}
