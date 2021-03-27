
#define GLEW_STATIC
#include <open3d/Open3D.h>

#include <open3d/visualization/gui/Window.h>
#include <open3d/Open3DConfig.h>
#include <open3d/geometry/BoundingVolume.h>
#include <open3d/geometry/Image.h>
#include <open3d/geometry/LineSet.h>
#include <open3d/geometry/PointCloud.h>
#include <open3d/geometry/TriangleMesh.h>
#include <open3d/io/FileFormatIO.h>
#include <open3d/io/ImageIO.h>
#include <open3d/io/ModelIO.h>
#include <open3d/io/PointCloudIO.h>
#include <open3d/io/TriangleMeshIO.h>
#include <open3d/utility/Console.h>
#include <open3d/utility/FileSystem.h>
#include <open3d/visualization/gui/Application.h>
#include <open3d/visualization/gui/Button.h>
#include <open3d/visualization/gui/Checkbox.h>
#include <open3d/visualization/gui/Color.h>
#include <open3d/visualization/gui/ColorEdit.h>
#include <open3d/visualization/gui/Combobox.h>
#include <open3d/visualization/gui/Dialog.h>
#include <open3d/visualization/gui/FileDialog.h>
#include <open3d/visualization/gui/Label.h>
#include <open3d/visualization/gui/Layout.h>
#include <open3d/visualization/gui/ProgressBar.h>
#include <open3d/visualization/gui/SceneWidget.h>
#include <open3d/visualization/gui/Slider.h>
#include <open3d/visualization/gui/Theme.h>
#include <open3d/visualization/gui/VectorEdit.h>
#include <open3d/visualization/rendering/Camera.h>
#include <open3d/visualization/rendering/Material.h>
#include <open3d/visualization/rendering/Model.h>
#include <open3d/visualization/rendering/Open3DScene.h>
#include <open3d/visualization/rendering/RenderToBuffer.h>
#include <open3d/visualization/rendering/RendererHandle.h>
#include <open3d/visualization/rendering/RendererStructs.h>
#include <open3d/visualization/rendering/Scene.h>
#include <open3d/visualization/rendering/filament/FilamentResourceManager.h>
#include <open3d/visualization/visualizer/GuiSettingsModel.h>
#include <open3d/visualization/visualizer/GuiSettingsView.h>
#include <open3d/visualization/visualizer/GuiWidgets.h>
#include <open3d/visualization/visualizer/Receiver.h>
