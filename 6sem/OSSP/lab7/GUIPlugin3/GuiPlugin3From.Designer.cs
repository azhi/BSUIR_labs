namespace GUIPlugin3
{
    partial class GuiPlugin3From
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
            this.btnSleep = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // btnSleep
            // 
            this.btnSleep.Location = new System.Drawing.Point(26, 12);
            this.btnSleep.Name = "btnSleep";
            this.btnSleep.Size = new System.Drawing.Size(236, 54);
            this.btnSleep.TabIndex = 0;
            this.btnSleep.Text = "Sleep";
            this.btnSleep.UseVisualStyleBackColor = true;
            this.btnSleep.Click += new System.EventHandler(this.btnSleep_Click);
            // 
            // GuiPlugin3From
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(284, 88);
            this.Controls.Add(this.btnSleep);
            this.Name = "GuiPlugin3From";
            this.Text = "Sleep form";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button btnSleep;
    }
}

