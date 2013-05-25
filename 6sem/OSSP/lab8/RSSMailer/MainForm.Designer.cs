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
            this.goButton = new System.Windows.Forms.Button();
            this.URLlistBox = new System.Windows.Forms.ListBox();
            this.URLtextBox = new System.Windows.Forms.TextBox();
            this.addRssButton = new System.Windows.Forms.Button();
            this.addEmailButton = new System.Windows.Forms.Button();
            this.emailTextBox = new System.Windows.Forms.TextBox();
            this.emailsListBox = new System.Windows.Forms.ListBox();
            this.regexTextBox = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // goButton
            // 
            this.goButton.Location = new System.Drawing.Point(473, 437);
            this.goButton.Name = "goButton";
            this.goButton.Size = new System.Drawing.Size(75, 23);
            this.goButton.TabIndex = 1;
            this.goButton.Text = "Go";
            this.goButton.UseVisualStyleBackColor = true;
            this.goButton.Click += new System.EventHandler(this.button1_Click);
            // 
            // URLlistBox
            // 
            this.URLlistBox.FormattingEnabled = true;
            this.URLlistBox.Items.AddRange(new object[] {
            "http://news.tut.by/rss/politics.rss"});
            this.URLlistBox.Location = new System.Drawing.Point(12, 12);
            this.URLlistBox.Name = "URLlistBox";
            this.URLlistBox.Size = new System.Drawing.Size(252, 381);
            this.URLlistBox.TabIndex = 2;
            // 
            // URLtextBox
            // 
            this.URLtextBox.Location = new System.Drawing.Point(12, 399);
            this.URLtextBox.Name = "URLtextBox";
            this.URLtextBox.Size = new System.Drawing.Size(184, 20);
            this.URLtextBox.TabIndex = 3;
            // 
            // addRssButton
            // 
            this.addRssButton.Location = new System.Drawing.Point(210, 397);
            this.addRssButton.Name = "addRssButton";
            this.addRssButton.Size = new System.Drawing.Size(54, 23);
            this.addRssButton.TabIndex = 4;
            this.addRssButton.Text = "Add";
            this.addRssButton.UseVisualStyleBackColor = true;
            this.addRssButton.Click += new System.EventHandler(this.addButton_Click);
            // 
            // addEmailButton
            // 
            this.addEmailButton.Location = new System.Drawing.Point(494, 397);
            this.addEmailButton.Name = "addEmailButton";
            this.addEmailButton.Size = new System.Drawing.Size(54, 23);
            this.addEmailButton.TabIndex = 7;
            this.addEmailButton.Text = "Add";
            this.addEmailButton.UseVisualStyleBackColor = true;
            this.addEmailButton.Click += new System.EventHandler(this.addEmailButton_Click);
            // 
            // emailTextBox
            // 
            this.emailTextBox.Location = new System.Drawing.Point(296, 399);
            this.emailTextBox.Name = "emailTextBox";
            this.emailTextBox.Size = new System.Drawing.Size(184, 20);
            this.emailTextBox.TabIndex = 6;
            // 
            // emailsListBox
            // 
            this.emailsListBox.FormattingEnabled = true;
            this.emailsListBox.Location = new System.Drawing.Point(296, 12);
            this.emailsListBox.Name = "emailsListBox";
            this.emailsListBox.Size = new System.Drawing.Size(252, 381);
            this.emailsListBox.TabIndex = 5;
            // 
            // regexTextBox
            // 
            this.regexTextBox.Location = new System.Drawing.Point(12, 437);
            this.regexTextBox.Name = "regexTextBox";
            this.regexTextBox.Size = new System.Drawing.Size(455, 20);
            this.regexTextBox.TabIndex = 8;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(576, 487);
            this.Controls.Add(this.regexTextBox);
            this.Controls.Add(this.addEmailButton);
            this.Controls.Add(this.emailTextBox);
            this.Controls.Add(this.emailsListBox);
            this.Controls.Add(this.addRssButton);
            this.Controls.Add(this.URLtextBox);
            this.Controls.Add(this.URLlistBox);
            this.Controls.Add(this.goButton);
            this.Name = "MainForm";
            this.Text = "RSS Mailer";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.MainForm_FormClosed);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button goButton;
        private System.Windows.Forms.ListBox URLlistBox;
        private System.Windows.Forms.TextBox URLtextBox;
        private System.Windows.Forms.Button addRssButton;
        private System.Windows.Forms.Button addEmailButton;
        private System.Windows.Forms.TextBox emailTextBox;
        private System.Windows.Forms.ListBox emailsListBox;
        private System.Windows.Forms.TextBox regexTextBox;
    }
}

