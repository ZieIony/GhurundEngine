﻿using System;
using System.ComponentModel;
using System.Runtime.InteropServices;
using Ghurund.Managed.Resource;

namespace Ghurund.Managed.Game {
    public class Scene : Entity {
        [DllImport(@"NativeDll.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr Scene_new();

        protected override IntPtr NewObject() => Scene_new();


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


        [DllImport(@"NativeDll.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr Scene_getFormats();

        public static ResourceFormatArray Formats { get; } = new ResourceFormatArray(Scene_getFormats());

    }

    public static class Scenes {

        [DllImport(@"NativeDll.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr Scenes_makeEditor(
            [MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(NativeClassMarshaler))] ResourceManager resourceManager,
            [MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(NativeClassMarshaler))] ResourceContext resourceContext);

        public static Scene MakeEditor(ResourceManager resourceManager, ResourceContext resourceContext) {
            Scene scene = new Scene(Scenes_makeEditor(resourceManager, resourceContext));
            scene.Release();
            return scene;
        }
    }
}
