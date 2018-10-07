﻿using System;
using System.Runtime.InteropServices;
using Ghurund.Managed.Graphics;

namespace Ghurund.Managed.Game {
    public class LevelManager : NativeClass {
        [DllImport(@"NativeDll.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr LevelManager_new();

        protected override void newObject() => NativePtr = LevelManager_new();


        [DllImport(@"NativeDll.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void LevelManager_setLevel(IntPtr _this, IntPtr level);

        public Level Level {
            set {
                LevelManager_setLevel(NativePtr, value.NativePtr);
            }
        }


        [DllImport(@"NativeDll.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void LevelManager_draw(IntPtr _this, IntPtr commandList, IntPtr parameterManager);

        public void draw(CommandList commandList, ParameterManager parameterManager) {
            LevelManager_draw(NativePtr, commandList.NativePtr, parameterManager.NativePtr);
        }
    }
}