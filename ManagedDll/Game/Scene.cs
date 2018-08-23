﻿using System;
using System.ComponentModel;
using System.Runtime.InteropServices;

namespace Ghurund.Managed.Game {
    public class Scene : Entity {
        [DllImport(@"NativeDll.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr Scene_new();

        protected override void newObject() => NativePtr = Scene_new();


        public Scene() {
            Entities = new EntityList(Scene_getEntities(NativePtr));
        }

        public Scene(IntPtr ptr) : base(ptr) {
            Entities = new EntityList(Scene_getEntities(NativePtr));
        }


        [DllImport(@"NativeDll.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr Scene_getEntities(IntPtr _this);

        [Browsable(false)]
        public EntityList Entities {
            get; internal set;
        }

    }
}
