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

namespace RSSReader
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();
        }

        private string groupByDateHTML(List<Channel> channels)
        {
            List<Article> articles = new List<Article>();
            foreach (Channel channel in channels)
            {
                List<Article> ch_articles = channel.articles;
                for (int i = 0; i < ch_articles.Count; i++)
                {
                    Article a = ch_articles[i];
                    a.parent = channel;
                    ch_articles[i] = a;
                }
                articles.AddRange(channel.articles);
            }
            articles.Sort((article1, article2) =>
            {
                DateTime dt1 = DateTime.Parse(article1.pubDate); DateTime dt2 = DateTime.Parse(article2.pubDate);
                return (dt1 < dt2) ? -1 : ((dt1 == dt2) ? 0 : 1);
            });
            StringBuilder res = new StringBuilder();
            res.Append(HTML_HEAD);
            res.Append("<body>\n");
            res.Append("<font size=\"2\" face=\"Verdana\">\n");
            res.Append("<table width=\"80%\" align=\"center\" border=1>\n");
            foreach (Article article in articles)
            {
                res.Append(article.to_html(true));
            }
            res.Append("</table>");
            res.Append("</body>\n");
            res.Append("</html>");
            return res.ToString();
        }

        private string groupByChannelHTML(List<Channel> channels)
        {
            StringBuilder res = new StringBuilder();
            res.Append(HTML_HEAD);
            res.Append("<body>\n");
            foreach (Channel channel in channels)
                res.Append(channel.to_html());
            res.Append("</body>\n");
            res.Append("</html>");
            return res.ToString();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            List<Channel> channels = new List<Channel>();
            foreach (String channel_url in URLlistBox.Items)
                channels.AddRange(RssFetcher.getFeedFromURL(channel_url));
            using (StreamWriter writer = new StreamWriter("last_articles.html"))
            {
                if (groupCheckBox.Checked)
                {
                    writer.Write(groupByDateHTML(channels));
                }
                else
                {   
                    writer.Write(groupByChannelHTML(channels));
                }
            }
            webBrowser.Navigate(Environment.CurrentDirectory + "\\last_articles.html");
        }

        private void addButton_Click(object sender, EventArgs e)
        {
            if (URLtextBox.Text.Length > 0)
                URLlistBox.Items.Add(URLtextBox.Text);
        }

        private const string HTML_HEAD = "<html>\n<head>\n<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\n<style type='text/css'>\n" +
            "A{color:#483D8B; text-decoration:none; font:Verdana;}\npre{font-family:courier;color:#000000;\nbackground-color:#dfe2e5;padding-top:5pt;padding-left:5pt;\n" +
            "padding-bottom:5pt;border-top:1pt solid #87A5C3;\nborder-bottom:1pt solid #87A5C3;border-left:1pt solid #87A5C3;\nborder-right : 1pt solid #87A5C3;text-align : left;}\n</style>\n</head>\n";
    }
}
