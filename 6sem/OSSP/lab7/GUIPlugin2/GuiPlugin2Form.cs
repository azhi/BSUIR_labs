using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace GUIPlugin2
{
    [ModuleGuiAttribute.Position(true)]
    public partial class GuiPlugin2Form : Form
    {
        public GuiPlugin2Form()
        {
            InitializeComponent();
        }

        private void btnShout_Click(object sender, EventArgs e)
        {
            MessageBox.Show(textBox.Text);
        }
    }
}
