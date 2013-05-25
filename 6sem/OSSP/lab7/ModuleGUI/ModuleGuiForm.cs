using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Reflection;

namespace ModuleGUI
{
    public partial class ModGuiForm : Form
    {
        public ModGuiForm()
        {
            InitializeComponent();

            String pluginsPath = Directory.GetParent(Assembly.GetExecutingAssembly().Location).FullName + @"\..\..\plugins";
            if (Directory.Exists(pluginsPath))
            {
                DirectoryInfo dir = new DirectoryInfo(pluginsPath);
                foreach (FileInfo fi in dir.GetFiles())
                {
                    Assembly assembly = null;
                    try
                    {
                        assembly = Assembly.LoadFile(fi.FullName);
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show("Unable to load assembly " + fi.Name + ": " + ex.ToString(), "warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    }
                    if (assembly != null)
                    {
                        Type[] assemblyTypes = assembly.GetTypes();
                        foreach (Type type in assemblyTypes)
                        {
                            if (type.BaseType.FullName == "System.Windows.Forms.Form")
                            {
                                bool goesInTab = false;

                                Form f = (Form) type.GetConstructor(new Type[0]).Invoke(null);
                                f.Hide();

                                object[] attributes = type.GetCustomAttributes(false);
                                for (int i = 0; i < attributes.Length; i++)
                                    if (attributes[i] is ModuleGuiAttribute.PositionAttribute)
                                        goesInTab = (attributes[i] as ModuleGuiAttribute.PositionAttribute).goesInTab;

                                if (goesInTab)
                                {
                                     f.FormBorderStyle = FormBorderStyle.None;
                                     TabPage tp = new TabPage();
                                     tp.Text = f.Text;
                                     tabControl1.TabPages.Add(tp);
                                     f.TopLevel = false;
                                     f.Parent = tp;
                                     f.Show();
                                }
                                else
                                {
                                    f.Left = 0;
                                    f.Top = 0;
                                    f.StartPosition = FormStartPosition.Manual;
                                    f.Show();
                                }
                            }
                        }
                    }
                }
            }

        }

    }
}
