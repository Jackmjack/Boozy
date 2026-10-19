# Boozy Engine · 酒鬼引擎

> 一个用 C++20 从零手写的游戏引擎学习项目。

**这是一个个人学习仓库**，目的不是产出可用的引擎，而是把「引擎是怎么搭起来的」这件事亲手走一遍。

- **2D 部分**参考 「The Cherno 的 Hazel 引擎教程」 逐集实现。
- **3D 部分**是在 AI 辅助下完成的 —— 阴影部分前 AI 仅为我提供建议及验收结果，阴影部分及之后设计取舍、代码、以及验收程序都有 AI 参与。

---

## 环境要求

- **Windows 10/11 x64** 或 **Linux x86_64**
- **xmake ≥ 3.1.2**（`xmake.lua` 中通过 `set_xmakever("3.1.2")` 要求）
- **编译器**
  - Windows：MSVC 工具链，推荐安装 Visual Studio 2026 Build Tools 的「使用 C++ 的桌面开发」工作负载即可（不需要完整 VS IDE；xmake 会自动探测 MSVC v145）
  - Linux：GCC ≥ 13 或 Clang ≥ 17（需要支持 C++20 的 `std::format`）
- **VS Code**：推荐安装 `xmake` 扩展 + `C/C++` 扩展（clangd 可选）
- **OpenGL 4.6 Core**（通过 GLFW 请求 4.6 core profile + 24 位深度缓冲；Linux 下需要能提供 4.6 的显卡驱动）
- **首次构建需要联网**：xmake-repo 会下载依赖源码并在本地编译

## 构建

### Windows

```bat
:: 1. 配置（Debug）并编译
xmake f -p windows -a x64 -m debug -y
xmake -y

:: 2. 运行（工作目录会自动设为 Sandbox/，assets 相对路径才能加载）
xmake run Sandbox
```

Release:

```bat
xmake f -p windows -a x64 -m release -y && xmake -y
```

### Linux

```bash
# 1. 配置（Debug）并编译
xmake f -p linux -a x86_64 -m debug -y
xmake -y

# 2. 运行（工作目录同样会自动设为 Sandbox/）
xmake run Sandbox
```

Release：

```bash
xmake f -p linux -a x86_64 -m release -y && xmake -y
```

构建产物在 `build/<plat>/<arch>/<mode>/`（例如 `build/linux/x86_64/debug/Sandbox`），日志写在 exe 同级的 `logs/` 目录。

> **注意**：`Sandbox` 的启动层在 `Sandbox/src/SandboxApp.cpp` 里写死（当前是 `PushLayer(new Sandbox3D())`，2D 那行被注释掉了）。想跑 2D 就把这两行对调。

### 依赖说明

- 第三方库全部通过 **xmake-repo** 获取（见下方表格）
- `xmake-requires.lock` 锁定了 xmake-repo 的 commit 与各包版本（含 `windows|x64` 与 `linux|x86_64` 两套锁）；即使本地包缓存被清掉，也能按同一版本复现

## 项目结构

```
Boozy/
├── Boozy/                          # 引擎（StaticLib）
│   └── src/
│       ├── Boozy.h                 # 对外总头文件
│       ├── bzpch.h/.cpp            # 预编译头
│       ├── StbImageImpl.cpp        # stb_image 实现单元（header-only 库需要实现一次）
│       ├── Boozy/
│       │   ├── Core/               # Application / Layer / LayerStack / Log
│       │   │                       # Window / Input / Transform / Timestep
│       │   ├── Events/             # 事件基类 + 窗口 / 键盘 / 鼠标事件
│       │   ├── ImGui/              # ImGuiLayer（含编辑器式 DockSpace）
│       │   └── Renderer/           # 渲染抽象层（后端无关）
│       └── Platform/
│           ├── OpenGL/             # OpenGL 后端实现
│           └── Windows/            # 基于 GLFW 的窗口与输入（目录名为历史遗留，实际跨平台）
├── Sandbox/                        # 示例程序（ConsoleApp）
│   ├── src/                        # Sandbox2D / Sandbox3D / SandboxApp
│   └── assets/                     # 着色器、贴图、模型
├── xmake.lua                       # 构建脚本
├── xmake-requires.lock             # 依赖版本锁定
└── README.md
```

## 第三方库

全部通过 `xmake-repo` 以 `add_requires` 引入（版本写在 `xmake.lua`，并锁定在 `xmake-requires.lock`）：

| 库 | 版本 | 配置 | 用途 |
|---|---|---|---|
| [GLFW](https://github.com/glfw/glfw) | 3.4 | 默认 | 窗口与输入 |
| [Glad](https://github.com/Dav1dde/glad) | v2.0.8 | `gl_version = 4.6`、`profile = core` | OpenGL 函数加载 |
| [imgui](https://github.com/ocornut/imgui) | v1.92.8-docking | `glfw = true`、`opengl3 = true` | 调试界面（docking 分支） |
| [glm](https://github.com/g-truc/glm) | 1.0.3 | header-only | 数学库 |
| [assimp](https://github.com/assimp/assimp) | v6.0.5 | `no_export = true` | 模型加载 |
| [stb_image](https://github.com/nothings/stb) | 2026.03.18 | header-only | 贴图解码 |

## Linux 支持

Linux 已可构建并运行，2D / 3D 场景均正常。

---

## 已实现

引擎采用 **`RenderCommand` → `RendererAPI` → `OpenGLRendererAPI`** 三层分发。`Boozy/src/Boozy/Renderer/` 下的代码**不含任何 `gl*` 调用**，是后端无关的。

### 基础设施

- **日志**：六档等级（Trace→Critical）、控制台按等级着色、控制台 + 文件双输出、`std::mutex` 线程安全、C++20 `std::format` 占位符；宏 `BZ_*`，Warn 及以上附带文件名与行号
- **事件系统**：`Event` 基类 + 窗口 / 应用 / 键盘 / 鼠标事件；`EVENT_CLASS_TYPE` / `EVENT_CLASS_CATEGORY` 宏生成类型名与种类掩码；`EventDispatcher::Dispatch<T>` 按类型分发
- **层系统**：`Layer` 钩子（`OnAttach` / `OnDetach` / `OnUpdate` / `OnEvent` / `OnImGuiRender`）；`LayerStack` 区分「层」与「覆盖层」；事件从栈顶反向遍历，被处理即停止下传
- **窗口**：`Window` 抽象接口 + `Window::Create()` 工厂，当前基于 GLFW 实现
- **ImGui**：编辑器式固定停靠（`ImGuiLayer::BeginDockSpace` + `DockBuilder` 默认布局），布局由 `imgui.ini` 持久化

### 2D

- `Renderer2D`：批量四边形绘制，纹理与纯色两种重载
- `OrthographicCamera` / `OrthographicCameraController`
- `Shader` / `ShaderLibrary`（从 glsl 文件或字符串源码创建）

### 3D

- **相机**：`PerspectiveCamera` + `PerspectiveCameraController`（WASD 移动、按住鼠标左键转视角，俯仰限制 ±89° 以避开 `lookAt` 退化）
- **网格与模型**：`Mesh` / `Model`（assimp 加载 OBJ），顶点格式 `Position + Normal + UV`
- **材质**：`Material`（Shader + Color + Texture + 双面开关）
- **光照**：方向光 / 点光 / 聚光三类，最多 8 盏；方向光、聚光用 2D 阴影贴图，点光用 Cubemap 阴影贴图；PCF 3×3 柔化
- **阴影**：法线偏移消自遮挡；方向光的正交包围盒跟随相机视锥并做 texel 对齐，避免移动时边缘抖动
- **无限地板**：全屏三角形 + 解析求交，向外渐隐到天空盒颜色
- **天空盒**：`TextureCube` + `gl_VertexID` 生成立方体，`gl_Position = clip.xyww` 令深度恒为 1.0
- **场景**：`Scene` / `SceneObject`（Model + Material + Transform + Visible），引擎层持有，渲染在调用方遍历
- **视锥剔除**：`Frustum` 从 viewProjection 矩阵提取 6 平面（Gribb-Hartmann），包围球判据

### 渲染状态

深度测试、背面剔除、混合、多边形偏移均可在 `RenderCommand` 层开关，无需直接调用 GL。

---

## 着色器

`Sandbox/assets/shaders/` 下的 glsl 支持 `#type vertex` / `#type fragment` 分段与 `#include`：

| 文件 | 用途 |
|---|---|
| `Texture.glsl` / `FlatColor.glsl` | 2D 纹理 / 纯色 |
| `Texture3D.glsl` / `FlatColor3D.glsl` | 3D 纹理 / 纯色 |
| `Lit3D.glsl` | 3D Blinn-Phong 光照 |
| `Lighting.glsl` | 光照与阴影计算（被 `include`） |
| `ShadowMapping.glsl` | 阴影 pass |
| `Floor.glsl` | 无限地板 |
| `Skybox.glsl` | 天空盒 |

---

## 已知问题

这些是已知但**尚未处理**的，不是「待办」而是「现状」：

- **视锥剔除没有开关**，无法做「开关剔除画面逐像素一致」的对比验证
- **阴影 pass 不参与剔除** —— 若将来要做，必须用光源视锥而非相机视锥，否则画面外投影的物体会被误剔
- **地板渐隐区在部分机位下出现异常色带**，成因未定性

## 许可

学习项目，未指定许可证。第三方库各自遵循其原有许可。
