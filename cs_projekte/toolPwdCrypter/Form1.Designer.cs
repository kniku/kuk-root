namespace toolPwdCrypter
{
	partial class Form1
	{
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		/// <param name="disposing">True, wenn verwaltete Ressourcen gelöscht werden sollen; andernfalls False.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Vom Windows Form-Designer generierter Code

		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		private void InitializeComponent()
		{
			this.textInput = new System.Windows.Forms.TextBox();
			this.label1 = new System.Windows.Forms.Label();
			this.butEncrypt = new System.Windows.Forms.Button();
			this.butDecrypt = new System.Windows.Forms.Button();
			this.label2 = new System.Windows.Forms.Label();
			this.textOutput = new System.Windows.Forms.TextBox();
			this.comboEncryptionKind = new System.Windows.Forms.ComboBox();
			this.label3 = new System.Windows.Forms.Label();
			this.butClear = new System.Windows.Forms.Button();
			this.butExit = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// textInput
			// 
			this.textInput.Location = new System.Drawing.Point(15, 25);
			this.textInput.Multiline = true;
			this.textInput.Name = "textInput";
			this.textInput.Size = new System.Drawing.Size(555, 62);
			this.textInput.TabIndex = 0;
			this.textInput.WordWrap = false;
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(12, 9);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(34, 13);
			this.label1.TabIndex = 2;
			this.label1.Text = "Input:";
			// 
			// butEncrypt
			// 
			this.butEncrypt.Location = new System.Drawing.Point(352, 93);
			this.butEncrypt.Name = "butEncrypt";
			this.butEncrypt.Size = new System.Drawing.Size(106, 23);
			this.butEncrypt.TabIndex = 4;
			this.butEncrypt.Text = "Verschlüsseln";
			this.butEncrypt.UseVisualStyleBackColor = true;
			this.butEncrypt.Click += new System.EventHandler(this.butEncrypt_Click);
			// 
			// butDecrypt
			// 
			this.butDecrypt.Location = new System.Drawing.Point(464, 93);
			this.butDecrypt.Name = "butDecrypt";
			this.butDecrypt.Size = new System.Drawing.Size(106, 23);
			this.butDecrypt.TabIndex = 5;
			this.butDecrypt.Text = "Entschlüsseln";
			this.butDecrypt.UseVisualStyleBackColor = true;
			this.butDecrypt.Click += new System.EventHandler(this.butEncrypt_Click);
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(12, 132);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(42, 13);
			this.label2.TabIndex = 7;
			this.label2.Text = "Output:";
			// 
			// textOutput
			// 
			this.textOutput.Location = new System.Drawing.Point(15, 148);
			this.textOutput.Multiline = true;
			this.textOutput.Name = "textOutput";
			this.textOutput.ReadOnly = true;
			this.textOutput.Size = new System.Drawing.Size(555, 62);
			this.textOutput.TabIndex = 6;
			// 
			// comboEncryptionKind
			// 
			this.comboEncryptionKind.FormattingEnabled = true;
			this.comboEncryptionKind.Items.AddRange(new object[] {
            "simple"});
			this.comboEncryptionKind.Location = new System.Drawing.Point(122, 95);
			this.comboEncryptionKind.Name = "comboEncryptionKind";
			this.comboEncryptionKind.Size = new System.Drawing.Size(224, 21);
			this.comboEncryptionKind.TabIndex = 8;
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(12, 98);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(104, 13);
			this.label3.TabIndex = 9;
			this.label3.Text = "Verschlüsselungsart:";
			// 
			// butClear
			// 
			this.butClear.Location = new System.Drawing.Point(352, 119);
			this.butClear.Name = "butClear";
			this.butClear.Size = new System.Drawing.Size(106, 23);
			this.butClear.TabIndex = 10;
			this.butClear.Text = "Clear";
			this.butClear.UseVisualStyleBackColor = true;
			this.butClear.Click += new System.EventHandler(this.butClear_Click);
			// 
			// butExit
			// 
			this.butExit.Location = new System.Drawing.Point(464, 119);
			this.butExit.Name = "butExit";
			this.butExit.Size = new System.Drawing.Size(106, 23);
			this.butExit.TabIndex = 11;
			this.butExit.Text = "Exit";
			this.butExit.UseVisualStyleBackColor = true;
			this.butExit.Click += new System.EventHandler(this.butExit_Click);
			// 
			// Form1
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(586, 228);
			this.Controls.Add(this.butExit);
			this.Controls.Add(this.butClear);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.comboEncryptionKind);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.textOutput);
			this.Controls.Add(this.butDecrypt);
			this.Controls.Add(this.butEncrypt);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.textInput);
			this.Name = "Form1";
			this.Text = "tool: Symmetrisches Verschlüsseln";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.TextBox textInput;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Button butEncrypt;
		private System.Windows.Forms.Button butDecrypt;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.TextBox textOutput;
		private System.Windows.Forms.ComboBox comboEncryptionKind;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Button butClear;
		private System.Windows.Forms.Button butExit;
	}
}

