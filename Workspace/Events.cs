﻿using System.Windows;
using System.Windows.Controls;

namespace Ghurund.Controls.Workspace {
    public class ControlEventArgs {
        public IDockablePanel[] Controls { get; set; }
    }

    public delegate void ControlEventHandler(object sender, ControlEventArgs args);

    public class RoutedControlEventArgs : RoutedEventArgs {
        public IDockablePanel[] Controls { get; set; }

        public RoutedControlEventArgs(RoutedEvent routedEvent, IDockablePanel[] controls) : base(routedEvent) {
            Controls = controls;
        }
    }

    public delegate void RoutedControlEventHandler(object sender, RoutedControlEventArgs args);

    public class WindowEventArgs : RoutedEventArgs {
        public double Left { get; }
        public double Top { get; }

        public WindowEventArgs(RoutedEvent routedEvent, double left, double top) : base(routedEvent) {
            Left = left;
            Top = top;
        }
    }

    public delegate void WindowEventHandler(object sender, WindowEventArgs args);

    public enum WindowAction {
        Close, Minimize, Maximize, Restore, Undock
    }

    public class WindowActionEventArgs : RoutedEventArgs {

        public WindowAction Action { get; }
        public bool ActionHandled { get; set; } = false;
        public DockableGroup DockableControls { get; set; }
        public Point? Location { get; }
        public PeekSide? Side { get; }

        public WindowActionEventArgs(RoutedEvent routedEvent, WindowAction action, Point? location = null, DockableGroup controls = null, PeekSide? side = null) : base(routedEvent) {
            Action = action;
            Location = location;
            DockableControls = controls;
            Side = side;
        }

    }

    public delegate void WindowActionEventHandler(object sender, WindowActionEventArgs args);

    public class PanelActionEventArgs : RoutedEventArgs {
        public EditorPanel Panel { get; }
        public TabControl TabControl { get; }

        public PanelActionEventArgs(EditorPanel control, TabControl tabControl, RoutedEvent routedEvent) : base(routedEvent) {
            Panel = control;
            TabControl = tabControl;
        }
    }

    public delegate void PanelActionEventHandler(object sender, PanelActionEventArgs args);
}