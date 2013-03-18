using System;
using System.Windows.Forms;
using System.Drawing;

using Sort;

public class MainForm : Form
{
	static public void Main ()
	{
		Application.Run (new MainForm ());
	}

	public MainForm()
	{
		Text = "File Sorter";
    Size = new Size(230, 100);

    Button button1 = new Button();
    button1.Text = "SortAlph";
    button1.Location = new Point(30, 20);
    button1.Click += new EventHandler(OnAlphClick);
    button1.Parent = this;

    Button button2 = new Button();
    button2.Text = "SortLength";
    button2.Location = new Point(120, 20);
    button2.Click += new EventHandler(OnLengthClick);
    button2.Parent = this;
	}

  public void OnAlphClick(object sender, EventArgs e)
  {
    OpenFileDialog dialog = new OpenFileDialog();
    dialog.Filter = "txt files (*.txt)|*.txt";

    if (dialog.ShowDialog(this) == DialogResult.OK) {
      FileSorter.SortByAlphabet(dialog.FileName);
    }
  }

  public void OnLengthClick(object sender, EventArgs e)
  {
    OpenFileDialog dialog = new OpenFileDialog();
    dialog.Filter = "txt files (*.txt)|*.txt";

    if (dialog.ShowDialog(this) == DialogResult.OK) {
      FileSorter.SortByLength(dialog.FileName);
    }
  }
}
