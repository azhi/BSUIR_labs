namespace OSSP_lab2
{
    partial class MainForm
    {
        /// <summary>
        /// Требуется переменная конструктора.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Освободить все используемые ресурсы.
        /// </summary>
        /// <param name="disposing">истинно, если управляемый ресурс должен быть удален; иначе ложно.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Код, автоматически созданный конструктором форм Windows

        /// <summary>
        /// Обязательный метод для поддержки конструктора - не изменяйте
        /// содержимое данного метода при помощи редактора кода.
        /// </summary>
        private void InitializeComponent()
        {
            this.btnResize = new System.Windows.Forms.Button();
            this.btnFlip = new System.Windows.Forms.Button();
            this.btnLoad = new System.Windows.Forms.Button();
            this.pictureBox = new System.Windows.Forms.PictureBox();
            this.btnMirror = new System.Windows.Forms.Button();
            this.btnBright = new System.Windows.Forms.Button();
            this.btnCont = new System.Windows.Forms.Button();
            this.btnSave = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox)).BeginInit();
            this.SuspendLayout();
            // 
            // btnResize
            // 
            this.btnResize.Location = new System.Drawing.Point(405, 70);
            this.btnResize.Name = "btnResize";
            this.btnResize.Size = new System.Drawing.Size(75, 23);
            this.btnResize.TabIndex = 1;
            this.btnResize.Text = "Resize";
            this.btnResize.UseVisualStyleBackColor = true;
            this.btnResize.Click += new System.EventHandler(this.btnResize_Click);
            // 
            // btnFlip
            // 
            this.btnFlip.Location = new System.Drawing.Point(405, 99);
            this.btnFlip.Name = "btnFlip";
            this.btnFlip.Size = new System.Drawing.Size(75, 23);
            this.btnFlip.TabIndex = 2;
            this.btnFlip.Text = "Flip";
            this.btnFlip.UseVisualStyleBackColor = true;
            this.btnFlip.Click += new System.EventHandler(this.btnFlip_Click);
            // 
            // btnLoad
            // 
            this.btnLoad.Location = new System.Drawing.Point(405, 12);
            this.btnLoad.Name = "btnLoad";
            this.btnLoad.Size = new System.Drawing.Size(75, 23);
            this.btnLoad.TabIndex = 3;
            this.btnLoad.Text = "Load";
            this.btnLoad.UseVisualStyleBackColor = true;
            this.btnLoad.Click += new System.EventHandler(this.btnLoad_Click);
            // 
            // pictureBox
            // 
            this.pictureBox.Location = new System.Drawing.Point(12, 12);
            this.pictureBox.Name = "pictureBox";
            this.pictureBox.Size = new System.Drawing.Size(387, 403);
            this.pictureBox.TabIndex = 4;
            this.pictureBox.TabStop = false;
            this.pictureBox.Paint += new System.Windows.Forms.PaintEventHandler(this.pictureBox_Paint);
            this.pictureBox.MouseDown += new System.Windows.Forms.MouseEventHandler(this.pictureBox_MouseDown);
            this.pictureBox.MouseMove += new System.Windows.Forms.MouseEventHandler(this.pictureBox_MouseMove);
            this.pictureBox.MouseUp += new System.Windows.Forms.MouseEventHandler(this.pictureBox_MouseUp);
            // 
            // btnMirror
            // 
            this.btnMirror.Location = new System.Drawing.Point(405, 128);
            this.btnMirror.Name = "btnMirror";
            this.btnMirror.Size = new System.Drawing.Size(75, 23);
            this.btnMirror.TabIndex = 5;
            this.btnMirror.Text = "Mirror";
            this.btnMirror.UseVisualStyleBackColor = true;
            this.btnMirror.Click += new System.EventHandler(this.btnMirror_Click);
            // 
            // btnBright
            // 
            this.btnBright.Location = new System.Drawing.Point(405, 157);
            this.btnBright.Name = "btnBright";
            this.btnBright.Size = new System.Drawing.Size(75, 23);
            this.btnBright.TabIndex = 6;
            this.btnBright.Text = "Brightness";
            this.btnBright.UseVisualStyleBackColor = true;
            this.btnBright.Click += new System.EventHandler(this.btnBright_Click);
            // 
            // btnCont
            // 
            this.btnCont.Location = new System.Drawing.Point(405, 186);
            this.btnCont.Name = "btnCont";
            this.btnCont.Size = new System.Drawing.Size(75, 23);
            this.btnCont.TabIndex = 7;
            this.btnCont.Text = "Contrast";
            this.btnCont.UseVisualStyleBackColor = true;
            this.btnCont.Click += new System.EventHandler(this.btnCont_Click);
            // 
            // btnSave
            // 
            this.btnSave.Location = new System.Drawing.Point(405, 41);
            this.btnSave.Name = "btnSave";
            this.btnSave.Size = new System.Drawing.Size(75, 23);
            this.btnSave.TabIndex = 8;
            this.btnSave.Text = "Save";
            this.btnSave.UseVisualStyleBackColor = true;
            this.btnSave.Click += new System.EventHandler(this.btnSave_Click);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(529, 427);
            this.Controls.Add(this.btnSave);
            this.Controls.Add(this.btnCont);
            this.Controls.Add(this.btnBright);
            this.Controls.Add(this.btnMirror);
            this.Controls.Add(this.pictureBox);
            this.Controls.Add(this.btnLoad);
            this.Controls.Add(this.btnFlip);
            this.Controls.Add(this.btnResize);
            this.Name = "MainForm";
            this.Text = "Picture editor";
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button btnResize;
        private System.Windows.Forms.Button btnFlip;
        private System.Windows.Forms.Button btnLoad;
        private System.Windows.Forms.PictureBox pictureBox;
        private System.Windows.Forms.Button btnMirror;
        private System.Windows.Forms.Button btnBright;
        private System.Windows.Forms.Button btnCont;
        private System.Windows.Forms.Button btnSave;
    }
}

