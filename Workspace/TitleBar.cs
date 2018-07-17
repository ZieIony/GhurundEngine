﻿using System;
using System.ComponentModel;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;

namespace Ghurund.Controls.Workspace {
    public partial class TitleBar : Control {

        Point mousePos;
        double left, top;
        private bool lmbDown, dragging;

        public bool DragWindow { get; set; } = true;

        [Bindable(true)]
        public String Title {
            get { return (String)GetValue(TitleProperty); }
            set { SetValue(TitleProperty, value); }
        }

        public static readonly DependencyProperty TitleProperty =
            DependencyProperty.Register("Title", typeof(String), typeof(TitleBar), new PropertyMetadata(null));

        public bool IsParentFocused {
            get { return (bool)GetValue(IsParentFocusedProperty); }
            set { SetValue(IsParentFocusedProperty, value); }
        }

        public static readonly DependencyProperty IsParentFocusedProperty =
            DependencyProperty.Register("IsParentFocused", typeof(bool), typeof(TitleBar), new PropertyMetadata(null));

        [Bindable(true)]
        public ImageSource Icon {
            get { return (ImageSource)GetValue(IconProperty); }
            set { SetValue(IconProperty, value); }
        }

        public static readonly DependencyProperty IconProperty =
            DependencyProperty.Register("Icon", typeof(ImageSource), typeof(TitleBar), new PropertyMetadata(null));

        static TitleBar() {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(TitleBar), new FrameworkPropertyMetadata(typeof(TitleBar)));
        }

        public override void OnApplyTemplate() {
            base.OnApplyTemplate();

            var closeButton = GetTemplateChild("closeButton") as Button;
            closeButton.Click += CloseButton_Click;
            var minimizeButton = GetTemplateChild("minimizeButton") as Button;
            minimizeButton.Click += MinimizeButton_Click;

            MenuItem closeItem = GetTemplateChild("closeMenuItem") as MenuItem;
            closeItem.Click += close_Click;
            MenuItem undockItem = GetTemplateChild("undockMenuItem") as MenuItem;
            undockItem.Click += undock_Click;
        }

        private void MinimizeButton_Click(object sender, RoutedEventArgs e) {
            RaiseEvent(new WindowActionEventArgs(WindowActionEvent, Workspace.WindowAction.Minimize, new Point()));
        }

        private void close_Click(object sender, RoutedEventArgs e) {
            RaiseEvent(new WindowActionEventArgs(WindowActionEvent, Workspace.WindowAction.Close, new Point()));
        }

        private void undock_Click(object sender, RoutedEventArgs e) {
            RaiseEvent(new WindowActionEventArgs(WindowActionEvent, Workspace.WindowAction.Undock, new Point()));
        }


        protected override void OnMouseDown(MouseButtonEventArgs e) {
            base.OnMouseDown(e);

            StartDrag();

            e.Handled = true;
        }

        public void StartDrag() {
            mousePos = PointToScreen(Mouse.GetPosition(this));
            var window = Window.GetWindow(this);
            left = window.Left;
            top = window.Top;
            CaptureMouse();
            lmbDown = true;
            dragging = true;
        }

        protected override void OnMouseMove(MouseEventArgs e) {
            base.OnMouseMove(e);

            if (!lmbDown)
                return;

            try {
                var pos = PointToScreen(e.GetPosition(this));
                if (!dragging &&
                    (Math.Abs(pos.X - mousePos.X) >= SystemParameters.MinimumHorizontalDragDistance ||
                    Math.Abs(pos.Y - mousePos.Y) >= SystemParameters.MinimumVerticalDragDistance)) {

                    dragging = true;
                    RaiseEvent(new WindowActionEventArgs(WindowActionEvent, Workspace.WindowAction.Undock, e.GetPosition(this)));
                }

                if (dragging)
                    RaiseEvent(new WindowEventArgs(WindowDraggedEvent, left + pos.X - mousePos.X, top + pos.Y - mousePos.Y));
            } catch (Exception) {
                lmbDown = false;
                dragging = false;
            }

            e.Handled = true;
        }

        protected override void OnMouseUp(MouseButtonEventArgs e) {
            base.OnMouseUp(e);

            ReleaseMouseCapture();
            lmbDown = false;
            dragging = false;

            var pos = PointToScreen(e.GetPosition(this));
            RaiseEvent(new WindowEventArgs(WindowDroppedEvent, left + pos.X - mousePos.X, top + pos.Y - mousePos.Y));

            e.Handled = true;
        }
    }
}
