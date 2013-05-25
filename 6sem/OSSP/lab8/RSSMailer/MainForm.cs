using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Threading;
using System.Net;

namespace RSSReader
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();

            emails = new List<string>();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            var ser = new System.Runtime.Serialization.Json.DataContractJsonSerializer(typeof(string[]));
            MemoryStream ms = new MemoryStream();
            ser.WriteObject(ms, emails.ToArray());
            string email_str = Encoding.UTF8.GetString(ms.ToArray());
            ms.Close();

            WebRequest paramsWebRequest = WebRequest.Create("http://localhost:49714/WebService.svc/set_params?emails=" +
                    Uri.EscapeDataString(email_str) + "&filter_regex=" + Uri.EscapeDataString(regexTextBox.Text));
            paramsWebRequest.GetResponse();

            foreach (String channel_url in URLlistBox.Items)
            {
                WebRequest urlWebRequest = WebRequest.Create("http://localhost:49714/WebService.svc/enqueue_url?url=" + 
                    Uri.EscapeDataString(channel_url));
                urlWebRequest.GetResponse();
            }
        }

        private void addButton_Click(object sender, EventArgs e)
        {
            if (URLtextBox.Text.Length > 0)
                URLlistBox.Items.Add(URLtextBox.Text);
        }

        private void addEmailButton_Click(object sender, EventArgs e)
        {
            if (emailTextBox.Text.Length > 0)
            {
                emailsListBox.Items.Add(emailTextBox.Text);
                emails.Add(emailTextBox.Text);
            }
        }

        private void MainForm_FormClosed(object sender, FormClosedEventArgs e)
        {
        }

        public List<String> emails;
    }
}
