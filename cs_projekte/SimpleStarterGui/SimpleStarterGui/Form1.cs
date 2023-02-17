using System.Diagnostics;
using Timer = System.Windows.Forms.Timer;

namespace SimpleStarterGui
{
    public sealed partial class Form1 : Form
    {
        private readonly Button _bLogView;
        private readonly Button _bShowConfig;

        public Form1()
        {
            InitializeComponent();

            _bLogView = new Button
            {
                Text = @"show log...",
                TextAlign = ContentAlignment.MiddleLeft,
                Font = new Font(DefaultFont, FontStyle.Italic),
                Anchor = AnchorStyles.Bottom | AnchorStyles.Left,
                FlatStyle = FlatStyle.Flat
            };
            
            var toolTips = new ToolTip();
            var logger = DebugLoggerFactory.GetOrCreate();

            var startInfo = Utils.ReadStartInfo();
            var editCommand = new CommandlineParser(Environment.GetCommandLineArgs()).FindOption("editor", "notepad");

            if (startInfo?.Name != null)
                Text += @$": {startInfo.Name}";

            TopMost = true;
            MaximizeBox = false;
            MinimizeBox = false;
            var yPos = 10;
            const int starterButtonHeight = 25;
            Width = 200;

            if (startInfo?.StartInfos != null)
            {
                // Height = 50 + (startInfo.StartInfos.Length + 2) * starterButtonHeight;
                Height = 50 + starterButtonHeight;

                Dictionary<string, Button> dictStarterButtons = new Dictionary<string, Button>();

                foreach (var info in startInfo.StartInfos)
                {
                    if (dictStarterButtons.ContainsKey(info.Title))
                    {
                        logger.Error($"Duplicate start information found: [{info.Title}]");
                        SetError(true);
                        continue;
                    }

                    var x = new Button
                    {
                        Text = info.Title,
                        Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right
                    };
                    dictStarterButtons.Add(info.Title, x);

                    x.SetBounds(5, yPos, 175, starterButtonHeight);
                    if (info.Execution != null)
                    {
                        toolTips.SetToolTip(x,
                            $"{info.Description}\nStart: [{info.Execution.Executable}]\nWith args: [{info.Execution.Arguments}]\nIn: [{info.Execution.WorkingDirectory}]");
                        x.Click += (_, _) =>
                        {
                            if (x.Tag is Process runningProcess)
                            {
                                if (MessageBox.Show(
                                        @$"Killing {info.Title} (pid: {runningProcess.Id})...{Environment.NewLine}Are you sure?",
                                        @"Warning", MessageBoxButtons.OKCancel) != DialogResult.OK)
                                    return;

                                logger.Information($"killing {info.Title} (pid: {runningProcess.Id})...");
                                try
                                {
                                    runningProcess.Kill(true);
                                }
                                catch (Exception ex)
                                {
                                    logger.Error(ex);
                                    SetError(true);
                                }

                                return;
                            }

                            logger.Information($"starting {info.Title}: {info.Execution.Executable}...");
                            var p = new Process();
                            p.StartInfo = new ProcessStartInfo
                            {
                                FileName = info.Execution.Executable,
                                Arguments = info.Execution.Arguments,
                                WorkingDirectory = info.Execution.WorkingDirectory,
                                UseShellExecute = info.Execution.UseShellExecute
                            };
                            try
                            {
                                p.Start();
                                var pid = p.Id;
                                x.Text += @$" ({pid})";
                                x.Tag = p;
                                x.BackColor = Color.LawnGreen;
                            }
                            catch (Exception ex)
                            {
                                logger.Error(ex);
                                SetError(true);
                            }
                        };
                    }
                    else if (info.Executions != null && info.Executions.Any())
                    {
                        toolTips.SetToolTip(x, $"Start [{string.Join(", ", info.Executions)}]");
                        x.Font = new Font(DefaultFont, FontStyle.Bold);
                        x.Click += (_, _) =>
                        {
                            foreach (var execution in info.Executions)
                            {
                                if (dictStarterButtons.TryGetValue(execution, out var button))
                                {
                                    button.PerformClick();
                                }
                            }
                        };
                    }
                    else
                    {
                        // used as separator
                        x.Visible = false;
                        x.Enabled = false;
                        x.Height = 10;
                    }

                    Controls.Add(x);
                    yPos += x.Height;
                    Height += x.Height;
                }

                if (dictStarterButtons.Any())
                {
                    var t = new Timer();
                    t.Interval = 100;
                    t.Tick += (_, _) =>
                    {
                        foreach (var kv in dictStarterButtons)
                        {
                            if (kv.Value.Tag is not Process process || !process.HasExited) continue;

                            kv.Value.ResetBackColor();
                            kv.Value.Tag = null;
                            kv.Value.Text = kv.Key;
                        }
                    };
                    t.Start();
                }

            }
            else
            {
                logger.Error("No/Invalid configuration");
                SetError(true);
            }

            Height += starterButtonHeight;
            yPos += starterButtonHeight;
            _bLogView.SetBounds(5, yPos, 85, 25);
            _bLogView.Click += (_, _) =>
            {
                var p = new Process();
                p.StartInfo = new ProcessStartInfo
                {
                    FileName = (logger as DebugLogger)!.LogFullPath,
                    UseShellExecute = true
                };
                try
                {
                    p.Start();
                    SetError(false);
                }
                catch (Exception ex)
                {
                    logger.Error(ex);
                    SetError(true);
                }
            };
            Controls.Add(_bLogView);

            if (startInfo?.FullConfigPath != null)
            {
                _bShowConfig = new Button
                {
                    Text = @"edit config...",
                    TextAlign = ContentAlignment.MiddleLeft,
                    Font = new Font(DefaultFont, FontStyle.Italic),
                    Anchor = AnchorStyles.Bottom | AnchorStyles.Right,
                    FlatStyle = FlatStyle.Flat
                };

                _bShowConfig.SetBounds(_bLogView.Width + 10, yPos, 85, 25);
                _bShowConfig.Click += (_, _) =>
                {
                    var p = new Process();
                    p.StartInfo = new ProcessStartInfo
                    {
                        FileName = editCommand,
                        Arguments = startInfo.FullConfigPath
                    };
                    try
                    {
                        p.Start();
                        SetError(false);
                    }
                    catch (Exception ex)
                    {
                        logger.Error(ex);
                        SetError(true);
                    }
                };
                Controls.Add(_bShowConfig);
            }
        }

        private void SetError(bool iSet)
        {
            _bLogView.BackColor = iSet ? Color.Firebrick : DefaultBackColor;
        }
    }
}