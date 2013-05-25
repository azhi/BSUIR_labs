namespace RSSReader
{
    partial class MainForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.webBrowser = new System.Windows.Forms.WebBrowser();
            this.loadButton = new System.Windows.Forms.Button();
            this.URLlistBox = new System.Windows.Forms.ListBox();
            this.URLtextBox = new System.Windows.Forms.TextBox();
            this.addButton = new System.Windows.Forms.Button();
            this.groupCheckBox = new System.Windows.Forms.CheckBox();
            this.SuspendLayout();
            // 
            // webBrowser
            // 
            this.webBrowser.Location = new System.Drawing.Point(0, 0);
            this.webBrowser.Margin = new System.Windows.Forms.Padding(0, 0, 0, 3);
            this.webBrowser.MinimumSize = new System.Drawing.Size(20, 20);
            this.webBrowser.Name = "webBrowser";
            this.webBrowser.Size = new System.Drawing.Size(593, 432);
            this.webBrowser.TabIndex = 0;
            // 
            // loadButton
            // 
            this.loadButton.Location = new System.Drawing.Point(12, 452);
            this.loadButton.Name = "loadButton";
            this.loadButton.Size = new System.Drawing.Size(75, 23);
            this.loadButton.TabIndex = 1;
            this.loadButton.Text = "Load";
            this.loadButton.UseVisualStyleBackColor = true;
            this.loadButton.Click += new System.EventHandler(this.button1_Click);
            // 
            // URLlistBox
            // 
            this.URLlistBox.FormattingEnabled = true;
            this.URLlistBox.Items.AddRange(new object[] {
            "http://news.tut.by/rss/politics.rss"});
            this.URLlistBox.Location = new System.Drawing.Point(595, 1);
            this.URLlistBox.Name = "URLlistBox";
            this.URLlistBox.Size = new System.Drawing.Size(252, 433);
            this.URLlistBox.TabIndex = 2;
            // 
            // URLtextBox
            // 
            this.URLtextBox.Location = new System.Drawing.Point(595, 440);
            this.URLtextBox.Name = "URLtextBox";
            this.URLtextBox.Size = new System.Drawing.Size(184, 20);
            this.URLtextBox.TabIndex = 3;
            // 
            // addButton
            // 
            this.addButton.Location = new System.Drawing.Point(785, 440);
            this.addButton.Name = "addButton";
            this.addButton.Size = new System.Drawing.Size(54, 23);
            this.addButton.TabIndex = 4;
            this.addButton.Text = "Add";
            this.addButton.UseVisualStyleBackColor = true;
            this.addButton.Click += new System.EventHandler(this.addButton_Click);
            // 
            // groupCheckBox
            // 
            this.groupCheckBox.AutoSize = true;
            this.groupCheckBox.Location = new System.Drawing.Point(493, 440);
            this.groupCheckBox.Name = "groupCheckBox";
            this.groupCheckBox.Size = new System.Drawing.Size(81, 17);
            this.groupCheckBox.TabIndex = 5;
            this.groupCheckBox.Text = "sort by date";
            this.groupCheckBox.UseVisualStyleBackColor = true;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(846, 487);
            this.Controls.Add(this.groupCheckBox);
            this.Controls.Add(this.addButton);
            this.Controls.Add(this.URLtextBox);
            this.Controls.Add(this.URLlistBox);
            this.Controls.Add(this.loadButton);
            this.Controls.Add(this.webBrowser);
            this.Name = "MainForm";
            this.Text = "RssReader";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.WebBrowser webBrowser;
        private System.Windows.Forms.Button loadButton;
        private System.Windows.Forms.ListBox URLlistBox;
        private System.Windows.Forms.TextBox URLtextBox;
        private System.Windows.Forms.Button addButton;
        private System.Windows.Forms.CheckBox groupCheckBox;
    }
}

