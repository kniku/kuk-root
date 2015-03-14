namespace testServer
{
	partial class FormMain
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
			this.components = new System.ComponentModel.Container();
			System.Windows.Forms.Label label1;
			System.Windows.Forms.Label label2;
			this.butStartStop = new System.Windows.Forms.Button();
			this.textInfo = new System.Windows.Forms.TextBox();
			this.timer = new System.Windows.Forms.Timer(this.components);
			this.listClients = new System.Windows.Forms.ListBox();
			this.listLog = new System.Windows.Forms.ListBox();
			label1 = new System.Windows.Forms.Label();
			label2 = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// label1
			// 
			label1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			label1.AutoSize = true;
			label1.Location = new System.Drawing.Point(602, 265);
			label1.Name = "label1";
			label1.Size = new System.Drawing.Size(40, 13);
			label1.TabIndex = 1;
			label1.Text = "clients:";
			// 
			// label2
			// 
			label2.AutoSize = true;
			label2.Location = new System.Drawing.Point(12, 18);
			label2.Name = "label2";
			label2.Size = new System.Drawing.Size(28, 13);
			label2.TabIndex = 6;
			label2.Text = "Log:";
			// 
			// butStartStop
			// 
			this.butStartStop.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.butStartStop.Location = new System.Drawing.Point(685, 83);
			this.butStartStop.Name = "butStartStop";
			this.butStartStop.Size = new System.Drawing.Size(123, 23);
			this.butStartStop.TabIndex = 2;
			this.butStartStop.Text = "start tcp server";
			this.butStartStop.UseVisualStyleBackColor = true;
			this.butStartStop.Click += new System.EventHandler(this.butStartStop_Click);
			// 
			// textInfo
			// 
			this.textInfo.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.textInfo.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.textInfo.Location = new System.Drawing.Point(629, 34);
			this.textInfo.Multiline = true;
			this.textInfo.Name = "textInfo";
			this.textInfo.ReadOnly = true;
			this.textInfo.Size = new System.Drawing.Size(179, 43);
			this.textInfo.TabIndex = 3;
			// 
			// timer
			// 
			this.timer.Tick += new System.EventHandler(this.timer_Tick);
			// 
			// listClients
			// 
			this.listClients.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.listClients.FormattingEnabled = true;
			this.listClients.Location = new System.Drawing.Point(602, 281);
			this.listClients.Name = "listClients";
			this.listClients.Size = new System.Drawing.Size(206, 108);
			this.listClients.TabIndex = 4;
			// 
			// listLog
			// 
			this.listLog.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.listLog.Font = new System.Drawing.Font("Courier New", 8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.listLog.FormattingEnabled = true;
			this.listLog.ItemHeight = 14;
			this.listLog.Location = new System.Drawing.Point(15, 34);
			this.listLog.Name = "listLog";
			this.listLog.Size = new System.Drawing.Size(581, 354);
			this.listLog.TabIndex = 5;
			// 
			// FormMain
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(820, 401);
			this.Controls.Add(label2);
			this.Controls.Add(this.listLog);
			this.Controls.Add(this.listClients);
			this.Controls.Add(this.textInfo);
			this.Controls.Add(this.butStartStop);
			this.Controls.Add(label1);
			this.Location = new System.Drawing.Point(100, 20);
			this.MinimumSize = new System.Drawing.Size(526, 405);
			this.Name = "FormMain";
			this.StartPosition = System.Windows.Forms.FormStartPosition.Manual;
			this.Text = "tcp Server";
			this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.FormMain_FormClosing);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Button butStartStop;
		private System.Windows.Forms.TextBox textInfo;
		private System.Windows.Forms.Timer timer;
		private System.Windows.Forms.ListBox listClients;
		private System.Windows.Forms.ListBox listLog;
	}
}

