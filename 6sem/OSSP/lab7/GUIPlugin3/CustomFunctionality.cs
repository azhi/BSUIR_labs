using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Threading;

namespace GUIPlugin3
{
    class CustomFunctionality
    {
        public static void sleep()
        {
            MessageBox.Show("Going to sleep for 5 seconds...");
            Thread.Sleep(5000);
            MessageBox.Show("Awake!");
        }
    }
}
