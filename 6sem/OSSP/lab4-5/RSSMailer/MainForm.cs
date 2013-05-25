using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using RSSReader.RSSItems;
using System.IO;
using System.Threading;

namespace RSSReader
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();

            url_queue = new Queue<string>();
            filter_queue = new Queue<List<Channel>>();
            send_queue = new Queue<List<Article>>();
            sw = new ThreadPools.StringWrapper();
            emails = new List<string>();

            loader = new ThreadPools.Loader(url_queue, filter_queue, 2);
            filter = new ThreadPools.Filter(filter_queue, send_queue, sw, 2);
            mailer = new ThreadPools.Mailer(send_queue, emails, 2);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            sw.val = regexTextBox.Text;
            foreach (String channel_url in URLlistBox.Items)
                lock (url_queue)
                {
                    url_queue.Enqueue(channel_url);
                    Monitor.Pulse(url_queue);
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
            loader.exit();
            filter.exit();
            mailer.exit();
        }

        private Queue<string> url_queue;
        private Queue<List<Channel>> filter_queue;
        private Queue<List<Article>> send_queue;
        private ThreadPools.Loader loader;
        private ThreadPools.Filter filter;
        private ThreadPools.Mailer mailer;
        private List<String> emails;
        private ThreadPools.StringWrapper sw;
    }
}
