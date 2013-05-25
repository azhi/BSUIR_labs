using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ModuleGuiAttribute
{
    [AttributeUsage(AttributeTargets.All)]
    public class PositionAttribute : System.Attribute
    {
        public PositionAttribute(bool goesInTab)
        {
            this.goesInTab = goesInTab;
        }

        public readonly bool goesInTab;
    }
}
