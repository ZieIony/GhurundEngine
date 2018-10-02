﻿using System.Windows.Controls;
using System.Windows.Media;
using Ghurund.Controls.Workspace;
using Ghurund.Managed.Game;
using Ghurund.Managed.Resource;
using Ninject;

namespace Ghurund.Editor.ResourceEditor {
    public interface ISceneEditor : IDockableControl {
        Scene Scene { get; set; }
    }

    public partial class SceneEditorPanel : UserControl, ISceneEditor, IStateControl {
        [Inject]
        public ResourceManager ResourceManager { get; set; }

        public SceneEditorPanel() {
            InitializeComponent();

            EditorKernel.Instance.Inject(this);
        }

        private Scene scene;
        public Scene Scene {
            get => scene;
            set {
                scene = value;
                surfaceView.Scene = scene;
            }
        }

        public ImageSource Icon { get; }
        public Control Control { get => this; }
        public string Title { get => "Scene"; }

        public object Save() {
            return Scene?.FileName;
        }

        public void Restore(object state) {
            if (state != null) {
                string fileName = state as string;
                Scene = new Scene();
                ResourceManager.StartLoading();
                Scene.Load(ResourceManager, fileName);
                ResourceManager.FinishLoading();
            }
        }
    }
}
