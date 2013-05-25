using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;
using System.ServiceModel.Web;
using System.Threading;
using System.IO;
using WebService.RSSItems;

namespace WebService
{
    // NOTE: You can use the "Rename" command on the "Refactor" menu to change the class name "WebService" in code, svc and config file together.
    [ServiceContract]
    public class WebService
    {
        //http://localhost:49714/WebService.svc/set_params?emails=astzhe&filter_regex=123
        [WebGet(UriTemplate = "/set_params?emails={emails}&filter_regex={filter_regex}", ResponseFormat = WebMessageFormat.Json)]
        public void setParams(string emails, string filter_regex)
        {
            var emails_arr = new string[0];
            try
            {
                var ser = new System.Runtime.Serialization.Json.DataContractJsonSerializer(typeof(string[]));
                MemoryStream ms = new MemoryStream(Encoding.UTF8.GetBytes(emails));
                emails_arr = (string[])ser.ReadObject(ms);
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }

            QueueKeeper.Instance.sw.val = filter_regex;
            QueueKeeper.Instance.emails = emails_arr;
        }

        [WebGet(UriTemplate = "/enqueue_url?url={url}", RequestFormat = WebMessageFormat.Json, ResponseFormat = WebMessageFormat.Json)]
        public void enqueue_url(string url)
        {
            lock (QueueKeeper.Instance.url_queue)
            {
                QueueKeeper.Instance.url_queue.Enqueue(url);
                Monitor.Pulse(QueueKeeper.Instance.url_queue);
            }
        }

        [WebInvoke(UriTemplate = "/enqueue_filter", Method = "POST", RequestFormat = WebMessageFormat.Json, ResponseFormat = WebMessageFormat.Json)]
        public void enqueue_filter(Stream input)
        {
            StreamReader reader = new StreamReader(input);
            string channels = reader.ReadToEnd();
            var channels_arr = new List<Channel>();
            try
            {
                var ser = new System.Runtime.Serialization.Json.DataContractJsonSerializer(typeof(List<Channel>));
                MemoryStream ms = new MemoryStream(Encoding.UTF8.GetBytes(channels));
                channels_arr = (List<Channel>)ser.ReadObject(ms);
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }

            lock (QueueKeeper.Instance.filter_queue)
            {
                QueueKeeper.Instance.filter_queue.Enqueue(channels_arr);
                Monitor.Pulse(QueueKeeper.Instance.filter_queue);
            }
        }

        [WebInvoke(UriTemplate = "/enqueue_mailer", Method = "POST", RequestFormat = WebMessageFormat.Json, ResponseFormat = WebMessageFormat.Json)]
        public void enqueue_mailer(Stream input)
        {
            StreamReader reader = new StreamReader(input);
            string articles = reader.ReadToEnd();
            var articles_arr = new List<Article>();
            try
            {
                var ser = new System.Runtime.Serialization.Json.DataContractJsonSerializer(typeof(List<Article>));
                MemoryStream ms = new MemoryStream(Encoding.UTF8.GetBytes(articles));
                articles_arr = (List<Article>)ser.ReadObject(ms);
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }

            lock (QueueKeeper.Instance.send_queue)
            {
                QueueKeeper.Instance.send_queue.Enqueue(articles_arr);
                Monitor.Pulse(QueueKeeper.Instance.send_queue);
            }
        }
    }
}
