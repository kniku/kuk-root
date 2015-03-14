namespace testClient
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
			System.Windows.Forms.Label label3;
			this.butConnect = new System.Windows.Forms.Button();
			this.listInfo = new System.Windows.Forms.ListBox();
			this.butSend = new System.Windows.Forms.Button();
			this.butHelp = new System.Windows.Forms.Button();
			this.comboCommand = new System.Windows.Forms.ComboBox();
			this.comboParams = new System.Windows.Forms.ComboBox();
			this.timer1 = new System.Windows.Forms.Timer(this.components);
			this.comboServer = new System.Windows.Forms.ComboBox();
			label1 = new System.Windows.Forms.Label();
			label2 = new System.Windows.Forms.Label();
			label3 = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// label1
			// 
			label1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			label1.AutoSize = true;
			label1.Location = new System.Drawing.Point(563, 101);
			label1.Name = "label1";
			label1.Size = new System.Drawing.Size(30, 13);
			label1.TabIndex = 7;
			label1.Text = "cmd:";
			// 
			// label2
			// 
			label2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			label2.AutoSize = true;
			label2.Location = new System.Drawing.Point(549, 128);
			label2.Name = "label2";
			label2.Size = new System.Drawing.Size(44, 13);
			label2.TabIndex = 8;
			label2.Text = "params:";
			// 
			// label3
			// 
			label3.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			label3.AutoSize = true;
			label3.Location = new System.Drawing.Point(554, 15);
			label3.Name = "label3";
			label3.Size = new System.Drawing.Size(39, 13);
			label3.TabIndex = 10;
			label3.Text = "server:";
			// 
			// butConnect
			// 
			this.butConnect.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.butConnect.Location = new System.Drawing.Point(674, 39);
			this.butConnect.Name = "butConnect";
			this.butConnect.Size = new System.Drawing.Size(107, 23);
			this.butConnect.TabIndex = 0;
			this.butConnect.Text = "connect";
			this.butConnect.UseVisualStyleBackColor = true;
			this.butConnect.Click += new System.EventHandler(this.butConnect_Click);
			// 
			// listInfo
			// 
			this.listInfo.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.listInfo.FormattingEnabled = true;
			this.listInfo.Location = new System.Drawing.Point(12, 12);
			this.listInfo.Name = "listInfo";
			this.listInfo.Size = new System.Drawing.Size(521, 394);
			this.listInfo.TabIndex = 1;
			// 
			// butSend
			// 
			this.butSend.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.butSend.Location = new System.Drawing.Point(674, 152);
			this.butSend.Name = "butSend";
			this.butSend.Size = new System.Drawing.Size(107, 23);
			this.butSend.TabIndex = 2;
			this.butSend.Text = "send";
			this.butSend.UseVisualStyleBackColor = true;
			this.butSend.Click += new System.EventHandler(this.butSend_Click);
			// 
			// butHelp
			// 
			this.butHelp.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.butHelp.Location = new System.Drawing.Point(674, 257);
			this.butHelp.Name = "butHelp";
			this.butHelp.Size = new System.Drawing.Size(107, 23);
			this.butHelp.TabIndex = 4;
			this.butHelp.Text = "handle: help";
			this.butHelp.UseVisualStyleBackColor = true;
			this.butHelp.Click += new System.EventHandler(this.butHelp_Click);
			// 
			// comboCommand
			// 
			this.comboCommand.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.comboCommand.FormattingEnabled = true;
			this.comboCommand.Items.AddRange(new object[] {
            "cwd",
            "finfo",
            "fget",
            "help"});
			this.comboCommand.Location = new System.Drawing.Point(599, 98);
			this.comboCommand.Name = "comboCommand";
			this.comboCommand.Size = new System.Drawing.Size(182, 21);
			this.comboCommand.TabIndex = 5;
			// 
			// comboParams
			// 
			this.comboParams.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.comboParams.FormattingEnabled = true;
			this.comboParams.Items.AddRange(new object[] {
            "c:\\big1\\tmp",
            "c:\\big1\\tmp\\test.txt"});
			this.comboParams.Location = new System.Drawing.Point(599, 125);
			this.comboParams.Name = "comboParams";
			this.comboParams.Size = new System.Drawing.Size(182, 21);
			this.comboParams.TabIndex = 6;
			// 
			// timer1
			// 
			this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
			// 
			// comboServer
			// 
			this.comboServer.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.comboServer.FormattingEnabled = true;
			this.comboServer.Items.AddRange(new object[] {
            "127.0.0.1:23000",
            "192.168.0.230:23000"});
			this.comboServer.Location = new System.Drawing.Point(599, 12);
			this.comboServer.Name = "comboServer";
			this.comboServer.Size = new System.Drawing.Size(182, 21);
			this.comboServer.TabIndex = 9;
			this.comboServer.Text = "127.0.0.1:23000";
			// 
			// FormMain
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(793, 416);
			this.Controls.Add(label3);
			this.Controls.Add(this.comboServer);
			this.Controls.Add(label2);
			this.Controls.Add(label1);
			this.Controls.Add(this.comboParams);
			this.Controls.Add(this.comboCommand);
			this.Controls.Add(this.butHelp);
			this.Controls.Add(this.butSend);
			this.Controls.Add(this.listInfo);
			this.Controls.Add(this.butConnect);
			this.Location = new System.Drawing.Point(100, 460);
			this.Name = "FormMain";
			this.StartPosition = System.Windows.Forms.FormStartPosition.Manual;
			this.Text = "tcp Client";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Button butConnect;
		private System.Windows.Forms.ListBox listInfo;
		private System.Windows.Forms.Button butSend;
		private System.Windows.Forms.Button butHelp;
		private System.Windows.Forms.ComboBox comboCommand;
		private System.Windows.Forms.ComboBox comboParams;
		private System.Windows.Forms.Timer timer1;
		private System.Windows.Forms.ComboBox comboServer;
	}
}

