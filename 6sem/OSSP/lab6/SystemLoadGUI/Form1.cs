using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace SystemLoadGUI
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();
        }

        private void btnClose_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void tmrUpdate_Tick(object sender, EventArgs e)
        {
            long cpuUsage = SystemLoadMeasurer.getCpuUsage();
            double memUsage = Math.Round(SystemLoadMeasurer.getRamUsage() / 1000.0 / 1000.0, 2);
            cpuUsageText.Text = cpuUsage.ToString();
            memUsageText.Text = memUsage.ToString();
            notifyIcon.Text = "CPU: " + cpuUsage.ToString() +
                              ", MEM: " + memUsage.ToString();
        }

        private void MainForm_Resize(object sender, EventArgs e)
        {
            if (FormWindowState.Minimized == this.WindowState)
            {
                notifyIcon.Visible = true;
                this.Hide();
            }

            else if (FormWindowState.Normal == this.WindowState)
            {
                notifyIcon.Visible = false;
            }
        }

        private void notifyIcon_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            notifyIcon.Visible = false;
            this.Show();
            this.WindowState = FormWindowState.Normal;
        }
    }
}
