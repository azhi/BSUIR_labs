namespace SystemLoadGUI
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
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.lblCPU = new System.Windows.Forms.Label();
            this.lblMEM = new System.Windows.Forms.Label();
            this.cpuUsageText = new System.Windows.Forms.Label();
            this.memUsageText = new System.Windows.Forms.Label();
            this.btnClose = new System.Windows.Forms.Button();
            this.tmrUpdate = new System.Windows.Forms.Timer(this.components);
            this.notifyIcon = new System.Windows.Forms.NotifyIcon(this.components);
            this.SuspendLayout();
            // 
            // lblCPU
            // 
            this.lblCPU.AutoSize = true;
            this.lblCPU.Location = new System.Drawing.Point(12, 9);
            this.lblCPU.Name = "lblCPU";
            this.lblCPU.Size = new System.Drawing.Size(67, 13);
            this.lblCPU.TabIndex = 0;
            this.lblCPU.Text = "CPU usage: ";
            // 
            // lblMEM
            // 
            this.lblMEM.AutoSize = true;
            this.lblMEM.Location = new System.Drawing.Point(12, 35);
            this.lblMEM.Name = "lblMEM";
            this.lblMEM.Size = new System.Drawing.Size(82, 13);
            this.lblMEM.TabIndex = 1;
            this.lblMEM.Text = "Memory usage: ";
            // 
            // cpuUsageText
            // 
            this.cpuUsageText.AutoSize = true;
            this.cpuUsageText.Location = new System.Drawing.Point(100, 9);
            this.cpuUsageText.Name = "cpuUsageText";
            this.cpuUsageText.Size = new System.Drawing.Size(0, 13);
            this.cpuUsageText.TabIndex = 2;
            this.cpuUsageText.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // memUsageText
            // 
            this.memUsageText.AutoSize = true;
            this.memUsageText.Location = new System.Drawing.Point(100, 35);
            this.memUsageText.Name = "memUsageText";
            this.memUsageText.Size = new System.Drawing.Size(0, 13);
            this.memUsageText.TabIndex = 3;
            this.memUsageText.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // btnClose
            // 
            this.btnClose.Location = new System.Drawing.Point(12, 65);
            this.btnClose.Name = "btnClose";
            this.btnClose.Size = new System.Drawing.Size(75, 23);
            this.btnClose.TabIndex = 4;
            this.btnClose.Text = "Exit";
            this.btnClose.UseVisualStyleBackColor = true;
            this.btnClose.Click += new System.EventHandler(this.btnClose_Click);
            // 
            // tmrUpdate
            // 
            this.tmrUpdate.Enabled = true;
            this.tmrUpdate.Interval = 1000;
            this.tmrUpdate.Tick += new System.EventHandler(this.tmrUpdate_Tick);
            // 
            // notifyIcon
            // 
            this.notifyIcon.Icon = ((System.Drawing.Icon)(resources.GetObject("notifyIcon.Icon")));
            this.notifyIcon.Text = "System Load Monitor";
            this.notifyIcon.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.notifyIcon_MouseDoubleClick);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(211, 94);
            this.Controls.Add(this.btnClose);
            this.Controls.Add(this.memUsageText);
            this.Controls.Add(this.cpuUsageText);
            this.Controls.Add(this.lblMEM);
            this.Controls.Add(this.lblCPU);
            this.Name = "MainForm";
            this.Text = "SystemLoad";
            this.Resize += new System.EventHandler(this.MainForm_Resize);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label lblCPU;
        private System.Windows.Forms.Label lblMEM;
        private System.Windows.Forms.Label cpuUsageText;
        private System.Windows.Forms.Label memUsageText;
        private System.Windows.Forms.Button btnClose;
        private System.Windows.Forms.Timer tmrUpdate;
        private System.Windows.Forms.NotifyIcon notifyIcon;
    }
}

