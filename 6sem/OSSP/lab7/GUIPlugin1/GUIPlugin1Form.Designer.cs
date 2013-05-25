namespace GUIPlugin1
{
    partial class GUIPlugin1Form
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
            this.btnHW = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // btnHW
            // 
            this.btnHW.Location = new System.Drawing.Point(29, 23);
            this.btnHW.Name = "btnHW";
            this.btnHW.Size = new System.Drawing.Size(227, 77);
            this.btnHW.TabIndex = 0;
            this.btnHW.Text = "Click me!";
            this.btnHW.UseVisualStyleBackColor = true;
            this.btnHW.Click += new System.EventHandler(this.btnHW_Click);
            // 
            // GUIPlugin1Form
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(284, 120);
            this.Controls.Add(this.btnHW);
            this.Name = "GUIPlugin1Form";
            this.Text = "Hello world";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button btnHW;
    }
}

