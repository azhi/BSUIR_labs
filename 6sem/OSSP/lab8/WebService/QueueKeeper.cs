using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using WebService.RSSItems;

namespace WebService
{
    public class QueueKeeper
    {
        private static readonly QueueKeeper instance = new QueueKeeper();

        public static QueueKeeper Instance
        {
            get
            {
                return instance;
            }
        }
                
        private QueueKeeper()
        {
            url_queue = new Queue<string>();
            filter_queue = new Queue<List<Channel>>();
            send_queue = new Queue<List<Article>>();
            sw = new ThreadPools.StringWrapper();

            loader = new ThreadPools.Loader(url_queue, 2);
            filter = new ThreadPools.Filter(filter_queue, sw, 2);
            mailer = new ThreadPools.Mailer(send_queue, 2);
        }

        public Queue<string> url_queue;
        public Queue<List<Channel>> filter_queue;
        public Queue<List<Article>> send_queue;
        public ThreadPools.Loader loader;
        public ThreadPools.Filter filter;
        public ThreadPools.Mailer mailer;
        public string[] emails;
        public ThreadPools.StringWrapper sw;
    }
}