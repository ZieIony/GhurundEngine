﻿namespace Ghurund.Managed {
    public class WindowSize {
        public int Width { get; set; }

        public int Height { get; set; }

        public override string ToString() {
            return Width + " x " + Height;
        }
    }
}