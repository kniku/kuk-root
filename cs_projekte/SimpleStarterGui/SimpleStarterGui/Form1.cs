using System.Diagnostics;
using System.Text.RegularExpressions;
using Timer = System.Windows.Forms.Timer;

namespace SimpleStarterGui
{
    public sealed partial class Form1 : Form
    {
        private readonly Button _bLogView;
        private readonly Button _bShowConfig;
        private const int starterButtonHeight = 25;
        private ToolTip toolTips = new ();
        IDebugLogger logger = DebugLoggerFactory.GetOrCreate();

        public Form1()
        {
            InitializeComponent();

            var selfExecutable = Utils.GetFullPathOfExecutingAssemblyWithNewExtension("exe");
            
            _bLogView = new Button
            {
                Text = @"log",
                TextAlign = ContentAlignment.MiddleLeft,
                Font = new Font(DefaultFont, FontStyle.Italic),
                Anchor = AnchorStyles.Bottom | AnchorStyles.Left,
                FlatStyle = FlatStyle.Flat
            };
            
            var startInfo = Utils.ReadStartInfo();
            var editCommand = new CommandlineParser(Environment.GetCommandLineArgs()).FindOption("editor", "notepad");
            
            if (startInfo?.Name != null)
                Text += @$": {startInfo.Name}";

            TopMost = true;
            MaximizeBox = false;
            MinimizeBox = false;
            var yPos = 10;
            Width = 200;

            if (startInfo?.StartInfos != null)
            {
                startInfo.Tokens ??= new Dictionary<string, string>();
                if (!startInfo.Tokens.ContainsKey("editor"))
                    startInfo.Tokens.Add("editor", editCommand!);
                editCommand = startInfo.Tokens["editor"];

                startInfo.Tokens.Add("self", selfExecutable);
                
                Height = 50 + starterButtonHeight;

                var dictStarterButtons = new Dictionary<string, Button>();

                foreach (var info in startInfo.StartInfos)
                {
                    ExpandAllTokens(info, startInfo.Tokens);
                    
                    if (dictStarterButtons.ContainsKey(info.Title))
                    {
                        logger.Error($"Duplicate start information found: [{info.Title}]");
                        SetError(true);
                        continue;
                    }

                    var newControl = CreateControl(info, dictStarterButtons, yPos);
                    if (!info.ShowControl) continue;

                    Controls.Add(newControl);
                    yPos += newControl.Height;
                    Height += newControl.Height;
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

                            if (process.ExitCode == 0) kv.Value.ResetBackColor();
                            else kv.Value.BackColor = Color.LightCoral;
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
            _bLogView.SetBounds(5, yPos, 57, 25);
            _bLogView.Click += (_, _) =>
            {
                var p = new Process();
                p.StartInfo = new ProcessStartInfo
                {
                    FileName = editCommand,
                    Arguments = (logger as DebugLogger)!.LogFullPath
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
                    Text = @"config",
                    TextAlign = ContentAlignment.MiddleLeft,
                    Font = new Font(DefaultFont, FontStyle.Italic),
                    Anchor = AnchorStyles.Bottom | AnchorStyles.Right,
                    FlatStyle = FlatStyle.Flat
                };

                _bShowConfig.SetBounds(_bLogView.Width + 5 + 1, yPos, 57, 25);
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
                
                var _bRestart = new Button
                {
                    Text = @"restart",
                    TextAlign = ContentAlignment.MiddleLeft,
                    Font = new Font(DefaultFont, FontStyle.Italic),
                    Anchor = AnchorStyles.Bottom | AnchorStyles.Right,
                    FlatStyle = FlatStyle.Flat
                };

                _bRestart.SetBounds(_bLogView.Width + 5 + 1 + _bShowConfig.Width + 1, yPos, 57, 25);
                _bRestart.Click += (_, _) =>
                {
                    var p = new Process();
                    p.StartInfo = new ProcessStartInfo
                    {
                        FileName = selfExecutable,
                        WorkingDirectory = Path.GetDirectoryName(startInfo.FullConfigPath)
                    };
                    try
                    {
                        p.Start();
                        Application.Exit();
                    }
                    catch (Exception ex)
                    {
                        logger.Error(ex);
                        SetError(true);
                    }
                };
                Controls.Add(_bRestart);
                
            }
        }


        private readonly Regex _rxTokens = new (@"(%)(\w+)(%)");
        private string? ExpandTokens(string? iText, Dictionary<string, string>? mappings)
        {
            if (string.IsNullOrEmpty(iText) || mappings?.Any() != true)
                return iText;

            var replaced = _rxTokens.Replace(iText,
                m => mappings.TryGetValue(m.Groups[2].Value, out var replacement)
                    ? replacement
                    : m.Groups[0].Value);
            
            return replaced;
        }

        private void ExpandAllTokens(StartInfoEntryModel info, Dictionary<string, string> mappings)
        {
            info.Title = ExpandTokens(info.Title, mappings)!;
            info.Description = ExpandTokens(info.Description, mappings);
            if (info.Execution != null)
            {
                info.Execution.Executable = ExpandTokens(info.Execution.Executable, mappings)!;
                info.Execution.Arguments = ExpandTokens(info.Execution.Arguments, mappings)!;
                info.Execution.WorkingDirectory = ExpandTokens(info.Execution.WorkingDirectory, mappings)!;
            }

            if (info.Executions?.Any() == true)
                info.Executions = info.Executions.Select(e => ExpandTokens(e, mappings)).ToArray()!;
        }
        
        private Control CreateControl(StartInfoEntryModel info, Dictionary<string, Button> dictStarterButtons, int yPos)
        {
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

                    logger.Information($"starting {info.Title} in {info.Execution.WorkingDirectory}: {info.Execution.Executable} {info.Execution.Arguments}");
                    var p = new Process();
                    p.StartInfo = new ProcessStartInfo
                    {
                        FileName = info.Execution.Executable,
                        Arguments = info.Execution.Arguments,
                        WorkingDirectory = info.Execution.WorkingDirectory,
                        UseShellExecute = info.Execution.UseShellExecute
                    };
                    if (info.Execution.Environment != null)
                        foreach (var kv in info.Execution.Environment)
                            p.StartInfo.Environment[kv.Key] = kv.Value;
                       
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

            return x;
        }
        
        private void SetError(bool iSet)
        {
            _bLogView.BackColor = iSet ? Color.Firebrick : DefaultBackColor;
        }
    }
}