using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using WebService.RSSItems;
using System.Net;
using System.Net.Mail;

namespace WebService.ThreadPools
{
    public class Mailer
    {
        public Mailer(Queue<List<Article>> in_queue, int threads_count)
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
                    List<Article> articles;
                    lock (_in_queue)
                    {
                        while (_in_queue.Count == 0 && !doExit)
                            Monitor.Wait(_in_queue);
                        if (doExit)
                            break;
                        articles = _in_queue.Dequeue();
                    }

                    var fromAddress = new MailAddress("stub@example.com", "RSS MAILER");
                    const string fromPassword = "";
                    var smtp = new SmtpClient
                               {
                                   Host = "smtp.gmail.com",
                                   Port = 587,
                                   EnableSsl = true,
                                   DeliveryMethod = SmtpDeliveryMethod.Network,
                                   UseDefaultCredentials = false,
                                   Credentials = new NetworkCredential(fromAddress.Address, fromPassword)
                               };

                    foreach (string email in QueueKeeper.Instance.emails)
                    {
                        var toAddress = new MailAddress(email, email);
                        foreach (Article article in articles)
                        {
                            string subject = "RSS " + article.title.Substring(0, Math.Min(article.title.Length, 50));
                            string body = "<table width=\"100%\" align=\"center\" border=1>\n" + article.to_html() + "</table>";

                            using (var message = new MailMessage(fromAddress, toAddress)
                                                 {
                                                     Subject = subject,
                                                     Body = body,
                                                     IsBodyHtml = true
                                                 })
                            {
                                smtp.Send(message);
                            }
                        }
                    }
                }
                catch (Exception ex)
                {
                    Console.Error.WriteLine(ex.Message);
                }
            }
        }

        private Queue<List<Article>> _in_queue;
        private bool doExit;
    }
}
