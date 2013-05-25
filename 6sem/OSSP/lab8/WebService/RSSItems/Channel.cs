using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace WebService.RSSItems
{
    public struct ChannelInfo
    {
        public string title;
        public string description;
        public string link;
        public string copyright;

        public string to_html()
        {
            String res = "";
            if (title != null && title.Length > 0 &&
                link != null && link.Length > 0)
                res = "<span style=\"border: 1px dashed black; width: 100%\"> Channel <a href=\"" + link + "\"><b>" + title + "</b></a> </span>\n";
            return res;
        }
    }

    public struct ChannelImage
    {
        public string title;
        public string link;
        public string URL;
    }

    public struct Article
    {
        public string title;
        public string link;
        public string description;
        public string pubDate;
        public Channel parent;

        public string to_html(bool withCI = false)
        {
            StringBuilder res = new StringBuilder();
            res.Append("<tr>\n");
            res.Append("  <td>\n");
            if (withCI && parent != null)
                res.Append(parent.info.to_html());
            res.Append("    <br> <a href=\"" + link + "\"><b>" + title + "</b></a>\n");
            res.Append("    <br>(" + pubDate + ")<br>\n");
            res.Append("    <table width=\"95%\" align=\"center\" border=0>\n");
            res.Append("      <tr><td>");
            res.Append(description);
            res.Append("      </td></tr>");
            res.Append("    </table>\n");
            res.Append("  <br> <a href=\"" + link + "\">\n");
            res.Append("  <font size=\"1\">читать дальше</font></a><br><br>\n");
            res.Append("  </td>\n");
            res.Append("</tr>\n");
            return res.ToString();
        }
    }

    public class Channel
    {
        public Channel()
        {
            info = new ChannelInfo();
            image = new ChannelImage();
            articles = new List<Article>();
        }

        public void addArticle(Article article)
        {
            articles.Add(article);
        }

        public string to_html()
        {
            StringBuilder res = new StringBuilder();
            res.Append("<font size=\"2\" face=\"Verdana\">\n");
            if (image.link != null && image.link.Length > 0 &&
                image.URL != null && image.URL.Length > 0)
            {
                res.Append("  <a href=\"" + image.link + "\">\n");
                res.Append("    <img src=\"" + image.URL + "\" border=0>\n");
                res.Append("  </a>\n"); 
            }

            res.Append("<h3>" + info.title + "</h3>\n");
            res.Append("<table width=\"80%\" align=\"center\" border=1>\n");
            foreach (Article article in articles)
            {
                res.Append(article.to_html());
            }
            res.Append("</table><br>");

            res.Append("<p align=\"center\">");
            res.Append("<a href=\"" + info.link + "\">" + info.copyright + "</a></p>\n");

            res.Append("</font>\n");
            return res.ToString();
        }

        public ChannelInfo info;
        public ChannelImage image;
        public List<Article> articles;
    }
}
