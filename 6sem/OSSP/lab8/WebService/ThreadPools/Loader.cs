using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using WebService.RSSItems;
using System.Threading;
using System.IO;
using System.Net;

namespace WebService.ThreadPools
{
    public class Loader
    {
        public Loader(Queue<string> in_queue, int threads_count)
        {
            _in_queue = in_queue;
            doExit = false;
            for (int i = 0; i < threads_count; ++i)
            {
                Thread thr = new Thread(doWork);
                thr.Start();
            }
        }

        public void exit()
        {
            doExit = true;
            lock (_in_queue)
                Monitor.PulseAll(_in_queue);
        }

        private void doWork()
        {
            while (true)
            {
                try
                {
                    string url = "";
                    lock (_in_queue)
                    {
                        while (_in_queue.Count == 0 && !doExit)
                            Monitor.Wait(_in_queue);
                        if (doExit)
                            break;
                        url = _in_queue.Dequeue();
                    }
                   
                    List<Channel> res = RssFetcher.getFeedFromURL(url);

                    var ser = new System.Runtime.Serialization.Json.DataContractJsonSerializer(typeof(List<Channel>));
                    MemoryStream ms = new MemoryStream();
                    ser.WriteObject(ms, res);
                    string res_str = Encoding.UTF8.GetString(ms.ToArray());
                    ms.Close();

                    HttpWebRequest request = WebRequest.Create("http://localhost:49714/WebService.svc/enqueue_filter") as HttpWebRequest;
                    request.Method = "POST";
                    request.ContentType = "application/json";
                    request.ContentLength = Encoding.UTF8.GetByteCount(res_str);

                    StreamWriter sw = new StreamWriter(request.GetRequestStream());
                    sw.Write(res_str);
                    sw.Close();
                    request.GetResponse();
                }
                catch (Exception ex)
                {
                    Console.Error.WriteLine(ex.Message);
                }
            }
        }

        private Queue<string> _in_queue;
        private bool doExit;
    }
}
