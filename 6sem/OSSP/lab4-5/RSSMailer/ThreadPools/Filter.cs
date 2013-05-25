using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using RSSReader.RSSItems;
using System.Threading;
using System.Windows.Forms;
using System.Text.RegularExpressions;

namespace RSSReader.ThreadPools
{
    public class StringWrapper
    {
        public string val { get { return _str; } set { _str = value; } }
        private string _str;
    }

    public class Filter
    {
        public Filter(Queue<List<Channel>> in_queue, Queue<List<Article>> out_queue, StringWrapper sw, int threads_count)
        {
            _in_queue = in_queue; _out_queue = out_queue; _sw = sw;
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
                    List<Channel> channels;
                    lock (_in_queue)
                    {
                        while (_in_queue.Count == 0 && !doExit)
                            Monitor.Wait(_in_queue);
                        if (doExit)
                            break;
                        channels = _in_queue.Dequeue();
                    }

                    Regex regex;
                    lock (_sw)
                    {
                        regex = new Regex(_sw.val, RegexOptions.IgnoreCase);
                    }

                    List<Article> res = new List<Article>();
                    foreach (Channel channel in channels)
                        foreach (Article article in channel.articles)
                        {
                            if (regex.IsMatch(article.title) || regex.IsMatch(article.description))
                                res.Add(article);
                        }

                    lock (_out_queue)
                    {
                        _out_queue.Enqueue(res);
                        Monitor.Pulse(_out_queue);
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
            }
        }

        private Queue<List<Channel>> _in_queue;
        private Queue<List<Article>> _out_queue;
        private StringWrapper _sw;
        private bool doExit;
    }
}
