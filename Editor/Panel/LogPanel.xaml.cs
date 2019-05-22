﻿using System;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using Ghurund.Controls.Workspace;
using Ghurund.Managed.Core;
using static Ghurund.Controls.LogsView;

namespace Ghurund.Editor {
    public interface ILogPanel : IDockablePanel {
    }

    public partial class LogPanel : UserControl, ILogPanel {

        private bool disposed = false;

        public LogPanel() {
            InitializeComponent();

            Logger.Init();
            Logger.OnLog += Logger_OnLog;
        }

        ~LogPanel() {
            Dispose(false);
        }

        public void Dispose() {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing) {
            if (disposed)
                return;

            Logger.Uninit();

            disposed = true;
        }

        private void Logger_OnLog(string logText) {
            string messageWithType = logText.Substring(logText.IndexOf(']') + 1).Trim();
            string type = messageWithType.Substring(0,messageWithType.IndexOf(' '));
            string message = messageWithType.Substring(messageWithType.IndexOf(' ')+1);
            logs.Logs.Add(new Log() {
                Time = DateTime.UtcNow.ToString("HH:mm:ssZ"),
                Type = type,
                Message = message
            });
        }

        public ImageSource Icon { get; } = new BitmapImage(new Uri("pack://application:,,,/Resources/icons/logs32.png", UriKind.Absolute));
        public Control Control { get => this; }
        public Title Title { get; } = new Title("Logs");

        private void ClearLogs_Click(object sender, System.Windows.RoutedEventArgs e) {
            logs.Logs.Clear();
        }

        private void ScrollToBottom_Click(object sender, System.Windows.RoutedEventArgs e) {
            logs.ScrollToBottom();
        }
    }
}
