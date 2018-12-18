namespace Viewer
{
    partial class GuiForm
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
            this.XPositionField = new System.Windows.Forms.NumericUpDown();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.YPositionField = new System.Windows.Forms.NumericUpDown();
            this.HeightField = new System.Windows.Forms.NumericUpDown();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.WidthField = new System.Windows.Forms.NumericUpDown();
            this.label5 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.XPositionField)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.YPositionField)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.HeightField)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.WidthField)).BeginInit();
            this.SuspendLayout();
            // 
            // XPositionField
            // 
            this.XPositionField.Location = new System.Drawing.Point(12, 61);
            this.XPositionField.Maximum = new decimal(new int[] {
            3840,
            0,
            0,
            0});
            this.XPositionField.Minimum = new decimal(new int[] {
            3840,
            0,
            0,
            -2147483648});
            this.XPositionField.Name = "XPositionField";
            this.XPositionField.Size = new System.Drawing.Size(120, 20);
            this.XPositionField.TabIndex = 3;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 42);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(14, 13);
            this.label1.TabIndex = 4;
            this.label1.Text = "X";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(139, 42);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(14, 13);
            this.label2.TabIndex = 5;
            this.label2.Text = "Y";
            // 
            // YPositionField
            // 
            this.YPositionField.Location = new System.Drawing.Point(142, 61);
            this.YPositionField.Maximum = new decimal(new int[] {
            2160,
            0,
            0,
            0});
            this.YPositionField.Minimum = new decimal(new int[] {
            2160,
            0,
            0,
            -2147483648});
            this.YPositionField.Name = "YPositionField";
            this.YPositionField.Size = new System.Drawing.Size(120, 20);
            this.YPositionField.TabIndex = 6;
            // 
            // HeightField
            // 
            this.HeightField.Location = new System.Drawing.Point(142, 108);
            this.HeightField.Maximum = new decimal(new int[] {
            2160,
            0,
            0,
            0});
            this.HeightField.Name = "HeightField";
            this.HeightField.Size = new System.Drawing.Size(120, 20);
            this.HeightField.TabIndex = 10;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(139, 89);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(38, 13);
            this.label3.TabIndex = 9;
            this.label3.Text = "Height";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(13, 89);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(35, 13);
            this.label4.TabIndex = 8;
            this.label4.Text = "Width";
            // 
            // WidthField
            // 
            this.WidthField.Location = new System.Drawing.Point(12, 108);
            this.WidthField.Maximum = new decimal(new int[] {
            3840,
            0,
            0,
            0});
            this.WidthField.Name = "WidthField";
            this.WidthField.Size = new System.Drawing.Size(120, 20);
            this.WidthField.TabIndex = 7;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(13, 13);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(83, 13);
            this.label5.TabIndex = 11;
            this.label5.Text = "SunriseKingdom";
            // 
            // GuiForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(284, 261);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.HeightField);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.WidthField);
            this.Controls.Add(this.YPositionField);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.XPositionField);
            this.Name = "GuiForm";
            this.Text = "GuiForm";
            this.Load += new System.EventHandler(this.GuiForm_Load);
            ((System.ComponentModel.ISupportInitialize)(this.XPositionField)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.YPositionField)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.HeightField)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.WidthField)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.NumericUpDown XPositionField;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.NumericUpDown YPositionField;
        private System.Windows.Forms.NumericUpDown HeightField;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.NumericUpDown WidthField;
        private System.Windows.Forms.Label label5;
    }
}