using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using RSSReader.RSSItems;
using System.Xml;
using System.Windows.Forms;
using System.IO;
using System.Text.RegularExpressions;

namespace RSSReader
{
    public class RssFetcher
    {
        public static List<Channel> getFeedFromURL(string URL)
        {
            try
            {
                if (!Directory.Exists("files"))
                    Directory.CreateDirectory("files"); 
                System.Net.WebClient client = new System.Net.WebClient();
                string XML_FileName = @"files\"+EscapeUrl(URL)+".xml";
                client.DownloadFile(URL, XML_FileName);
                string sXML = File.ReadAllText(XML_FileName, Encoding.GetEncoding(1251));
                sXML = Regex.Replace(sXML, @"<\?xml-stylesheet.*\?>", "");
                sXML = Regex.Replace(sXML, @"<\w+\sxmlns:xsi.*\.xsd.?\s*>", "");
                sXML = Regex.Replace(sXML, @"<!DOCTYPE.*>", "");
                File.WriteAllText(XML_FileName, sXML, Encoding.GetEncoding(1251));
                return getFeedFromFile(XML_FileName);
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message, " Ошибка загрузки или обработки XML");
                return null;
            }
        }

        public static List<Channel> getFeedFromFile(string fileSource)
        {
            List<Channel> channelList = new List<Channel>();
            try
            {
                XmlDocument doc = new XmlDocument();
                doc.Load(fileSource);

                XmlNodeList nodeList;
                XmlNode root = doc.DocumentElement;
                nodeList = root.ChildNodes;

                foreach (XmlNode chanel in nodeList)
                {
                    Channel channel = new Channel();
                    foreach (XmlNode chanel_item in chanel)
                    {
                        if (chanel_item.Name == "title")
                        {
                            channel.info.title = chanel_item.InnerText;
                        }
                        if (chanel_item.Name == "description")
                        {
                            channel.info.description = chanel_item.InnerText;
                        }
                        if (chanel_item.Name == "copyright")
                        {
                            channel.info.copyright = chanel_item.InnerText;
                        }
                        if (chanel_item.Name == "link")
                        {
                            channel.info.link = chanel_item.InnerText;
                        }

                        if (chanel_item.Name == "img")
                        {
                            XmlNodeList imgList = chanel_item.ChildNodes;
                            foreach (XmlNode img_item in imgList)
                            {
                                if (img_item.Name == "url")
                                {
                                    channel.image.URL = img_item.InnerText;
                                }
                                if (img_item.Name == "link")
                                {
                                    channel.image.link = img_item.InnerText;
                                }
                                if (img_item.Name == "title")
                                {
                                    channel.image.title = img_item.InnerText;
                                }
                            }
                        }

                        if (chanel_item.Name == "item")
                        {
                            XmlNodeList itemsList = chanel_item.ChildNodes;
                            Article article = new Article();

                            foreach (XmlNode item in itemsList)
                            {
                                if (item.Name == "title")
                                {
                                    article.title = item.InnerText;
                                }
                                if (item.Name == "link")
                                {
                                    article.link = item.InnerText;
                                }
                                if (item.Name == "description")
                                {
                                    article.description = item.InnerText;
                                }
                                if (item.Name == "pubDate")
                                {
                                    article.pubDate = item.InnerText;
                                }
                            }
                            channel.addArticle(article);
                        }
                    }
                    channelList.Add(channel);
                }
                return channelList;
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, " Ошибка загрузки или обработки XML"); 
                return channelList;
            }
        }

        private static string EscapeUrl(string s)
        {
            return s.Replace('/', '_').Replace(".xml", "").Replace("http:__", "").Replace("www.", "");
        } 
    }
}
