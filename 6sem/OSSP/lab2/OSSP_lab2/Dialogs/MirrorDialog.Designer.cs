namespace OSSP_lab2.Dialogs
{
    partial class MirrorDialog
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
            this.radioGroupBox = new System.Windows.Forms.GroupBox();
            this.checkBoxVert = new System.Windows.Forms.CheckBox();
            this.checkBoxHoriz = new System.Windows.Forms.CheckBox();
            this.btnOK = new System.Windows.Forms.Button();
            this.btnCancel = new System.Windows.Forms.Button();
            this.radioGroupBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // radioGroupBox
            // 
            this.radioGroupBox.Controls.Add(this.checkBoxVert);
            this.radioGroupBox.Controls.Add(this.checkBoxHoriz);
            this.radioGroupBox.Location = new System.Drawing.Point(12, 12);
            this.radioGroupBox.Name = "radioGroupBox";
            this.radioGroupBox.Size = new System.Drawing.Size(107, 72);
            this.radioGroupBox.TabIndex = 1;
            this.radioGroupBox.TabStop = false;
            this.radioGroupBox.Text = "Angle";
            // 
            // checkBoxVert
            // 
            this.checkBoxVert.AutoSize = true;
            this.checkBoxVert.Location = new System.Drawing.Point(6, 42);
            this.checkBoxVert.Name = "checkBoxVert";
            this.checkBoxVert.Size = new System.Drawing.Size(61, 17);
            this.checkBoxVert.TabIndex = 1;
            this.checkBoxVert.Text = "Vertical";
            this.checkBoxVert.UseVisualStyleBackColor = true;
            // 
            // checkBoxHoriz
            // 
            this.checkBoxHoriz.AutoSize = true;
            this.checkBoxHoriz.Location = new System.Drawing.Point(6, 19);
            this.checkBoxHoriz.Name = "checkBoxHoriz";
            this.checkBoxHoriz.Size = new System.Drawing.Size(73, 17);
            this.checkBoxHoriz.TabIndex = 0;
            this.checkBoxHoriz.Text = "Horizontal";
            this.checkBoxHoriz.UseVisualStyleBackColor = true;
            // 
            // btnOK
            // 
            this.btnOK.Location = new System.Drawing.Point(14, 90);
            this.btnOK.Name = "btnOK";
            this.btnOK.Size = new System.Drawing.Size(75, 23);
            this.btnOK.TabIndex = 2;
            this.btnOK.Text = "OK";
            this.btnOK.UseVisualStyleBackColor = true;
            this.btnOK.Click += new System.EventHandler(this.btnOK_Click);
            // 
            // btnCancel
            // 
            this.btnCancel.Location = new System.Drawing.Point(95, 90);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(75, 23);
            this.btnCancel.TabIndex = 3;
            this.btnCancel.Text = "Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // MirrorDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(190, 128);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnOK);
            this.Controls.Add(this.radioGroupBox);
            this.Name = "MirrorDialog";
            this.Text = "MirrorDialog";
            this.radioGroupBox.ResumeLayout(false);
            this.radioGroupBox.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox radioGroupBox;
        private System.Windows.Forms.Button btnOK;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.CheckBox checkBoxVert;
        private System.Windows.Forms.CheckBox checkBoxHoriz;
    }
}