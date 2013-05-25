using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace GUIPlugin3
{
    public partial class GuiPlugin3From : Form
    {
        public GuiPlugin3From()
        {
            InitializeComponent();
        }

        private void btnSleep_Click(object sender, EventArgs e)
        {
            CustomFunctionality.sleep();
        }
    }
}
