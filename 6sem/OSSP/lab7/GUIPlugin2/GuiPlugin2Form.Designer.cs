namespace GUIPlugin2
{
    partial class GuiPlugin2Form
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
            this.btnShout = new System.Windows.Forms.Button();
            this.textBox = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // btnShout
            // 
            this.btnShout.Location = new System.Drawing.Point(87, 38);
            this.btnShout.Name = "btnShout";
            this.btnShout.Size = new System.Drawing.Size(126, 31);
            this.btnShout.TabIndex = 0;
            this.btnShout.Text = "Shout out";
            this.btnShout.UseVisualStyleBackColor = true;
            this.btnShout.Click += new System.EventHandler(this.btnShout_Click);
            // 
            // textBox
            // 
            this.textBox.Location = new System.Drawing.Point(12, 12);
            this.textBox.Name = "textBox";
            this.textBox.Size = new System.Drawing.Size(267, 20);
            this.textBox.TabIndex = 1;
            // 
            // GuiPlugin2Form
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(291, 82);
            this.Controls.Add(this.textBox);
            this.Controls.Add(this.btnShout);
            this.Name = "GuiPlugin2Form";
            this.Text = "Shout out";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnShout;
        private System.Windows.Forms.TextBox textBox;
    }
}

