using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace SystemLoadGUI
{
    class SystemLoadMeasurer
    {
        [DllImport("SystemLoadMeasurer.dll")]
        public static extern long getCpuUsage();

        [DllImport("SystemLoadMeasurer.dll")]
        public static extern long getRamUsage();
    }
}
