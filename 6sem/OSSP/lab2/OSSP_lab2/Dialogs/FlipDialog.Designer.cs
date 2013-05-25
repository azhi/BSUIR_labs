namespace OSSP_lab2.Dialogs
{
    partial class FlipDialog
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
            this.radioBtn90 = new System.Windows.Forms.RadioButton();
            this.radioBtn270 = new System.Windows.Forms.RadioButton();
            this.radioBtn180 = new System.Windows.Forms.RadioButton();
            this.btnOK = new System.Windows.Forms.Button();
            this.btnCancel = new System.Windows.Forms.Button();
            this.radioGroupBox.SuspendLayout();
            this.SuspendLayout();
            // 
            // radioGroupBox
            // 
            this.radioGroupBox.Controls.Add(this.radioBtn90);
            this.radioGroupBox.Controls.Add(this.radioBtn270);
            this.radioGroupBox.Controls.Add(this.radioBtn180);
            this.radioGroupBox.Location = new System.Drawing.Point(12, 12);
            this.radioGroupBox.Name = "radioGroupBox";
            this.radioGroupBox.Size = new System.Drawing.Size(107, 100);
            this.radioGroupBox.TabIndex = 1;
            this.radioGroupBox.TabStop = false;
            this.radioGroupBox.Text = "Angle";
            // 
            // radioBtn90
            // 
            this.radioBtn90.Checked = true;
            this.radioBtn90.Location = new System.Drawing.Point(30, 19);
            this.radioBtn90.Name = "radioBtn90";
            this.radioBtn90.Size = new System.Drawing.Size(104, 24);
            this.radioBtn90.TabIndex = 0;
            this.radioBtn90.TabStop = true;
            this.radioBtn90.Text = "90°";
            // 
            // radioBtn270
            // 
            this.radioBtn270.AutoSize = true;
            this.radioBtn270.Location = new System.Drawing.Point(30, 65);
            this.radioBtn270.Name = "radioBtn270";
            this.radioBtn270.Size = new System.Drawing.Size(47, 17);
            this.radioBtn270.TabIndex = 2;
            this.radioBtn270.Text = "270°";
            this.radioBtn270.UseVisualStyleBackColor = true;
            // 
            // radioBtn180
            // 
            this.radioBtn180.AutoSize = true;
            this.radioBtn180.Location = new System.Drawing.Point(30, 42);
            this.radioBtn180.Name = "radioBtn180";
            this.radioBtn180.Size = new System.Drawing.Size(47, 17);
            this.radioBtn180.TabIndex = 1;
            this.radioBtn180.Text = "180°";
            this.radioBtn180.UseVisualStyleBackColor = true;
            // 
            // btnOK
            // 
            this.btnOK.Location = new System.Drawing.Point(14, 118);
            this.btnOK.Name = "btnOK";
            this.btnOK.Size = new System.Drawing.Size(75, 23);
            this.btnOK.TabIndex = 2;
            this.btnOK.Text = "OK";
            this.btnOK.UseVisualStyleBackColor = true;
            this.btnOK.Click += new System.EventHandler(this.btnOK_Click);
            // 
            // btnCancel
            // 
            this.btnCancel.Location = new System.Drawing.Point(95, 118);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(75, 23);
            this.btnCancel.TabIndex = 3;
            this.btnCancel.Text = "Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // FlipDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(190, 159);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnOK);
            this.Controls.Add(this.radioGroupBox);
            this.Name = "FlipDialog";
            this.Text = "FlipDialog";
            this.radioGroupBox.ResumeLayout(false);
            this.radioGroupBox.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.RadioButton radioBtn90;
        private System.Windows.Forms.GroupBox radioGroupBox;
        private System.Windows.Forms.RadioButton radioBtn270;
        private System.Windows.Forms.RadioButton radioBtn180;
        private System.Windows.Forms.Button btnOK;
        private System.Windows.Forms.Button btnCancel;
    }
}