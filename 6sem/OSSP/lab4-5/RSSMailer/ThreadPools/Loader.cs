using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using RSSReader.RSSItems;
using System.Threading;
using System.Windows.Forms;

namespace RSSReader.ThreadPools
{
    public class Loader
    {
        public Loader(Queue<string> in_queue, Queue<List<Channel>> out_queue, int threads_count)
        {
            _in_queue = in_queue; _out_queue = out_queue;
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

        private Queue<string> _in_queue;
        private Queue<List<Channel>> _out_queue;
        private bool doExit;
    }
}
