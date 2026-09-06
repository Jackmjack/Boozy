# Boozy Engine - 酒鬼引擎

> 一个用 C++20 编写的游戏引擎学习项目，参考 [The Cherno 的 Hazel 引擎教程](https://www.youtube.com/@TheCherno)。

适合正在学习 C++ 游戏引擎开发、想理解「引擎」的人参考。

## 这是什么

Boozy 包含两个项目：

- **Boozy**（引擎，编译为 DLL）：提供 `Application` 基类、`main()` 入口点、日志系统等基础设施。
- **Sandbox**（示例游戏，编译为 exe）：继承 `Application`，编写具体游戏逻辑。

## 当前特性

- 日志系统 `Log`：
  - 六档等级：Trace / Debug / Info / Warn / Error / Critical
  - 控制台按等级着色
  - 控制台 + 文件双输出
  - 多线程安全（`std::mutex`）
  - 支持 `{}` 占位符格式化（C++20 `std::format`）与普通字符串
  - 宏封装 `BZ_*`，Warn 及以上附带文件名与行号
- 事件系统 `Event`：
  - 事件基类 + 派生事件：窗口（`Window*`）、应用（`App*`）、键盘（`Key*`）、鼠标（`Mouse*`）
  - `EVENT_CLASS_TYPE` / `EVENT_CLASS_CATEGORY` 宏自动生成类型名与种类掩码
  - `EventDispatcher::Dispatch<T>` 按事件类型分发
  - 种类掩码过滤（`IsInCategory`）
  - 支持隐式转 `std::string`，可直接 `BZ_TRACE(e)` 打印
- 层系统 `Layer` / `LayerStack`：
  - `Layer` 基类提供 `OnAttach` / `OnDetach` / `OnUpdate` / `OnEvent` 钩子
  - `LayerStack` 维护「层 + 覆盖层」堆栈，`PushLayer` / `PushOverlay` 插入位置不同
  - `Application` 从栈顶反向遍历层栈分发事件，事件被处理（`GetHandled`）后停止向下传递
- 窗口系统 `Window`：
  - `Window` 抽象接口（纯虚基类）与 `WindowProps` 配置（标题 / 宽 / 高）
  - 静态工厂 `Window::Create()` 返回平台实现，应用层只面向接口
  - Windows 平台基于 GLFW 实现（`WindowsWindow`），已接线窗口关闭 / 尺寸变化 / 键盘 / 鼠标 / 滚轮等 GLFW 回调 → 事件系统（窗口点 × 可正常退出）

## 环境要求

- Windows
- Visual Studio（支持 premake 的 `vs2026` action）
- Premake5（放在 `vendor/bin/premake/`，未纳入版本控制）
- GLFW（作为 git submodule 引入，固定 `3.4`，见下方「构建」）

## 构建

```bat
:: 1. 拉取子模块（GLFW 源码；clone 后首次构建必做）
git submodule update --init --recursive

:: 2. 生成 Visual Studio 工程
GenerateProjects.bat
:: 等价于
vendor\bin\premake\premake5.exe vs2026

:: 3. 打开 Boozy.slnx，以 Sandbox 为启动项目，Debug x64 编译运行
```

## 项目结构

```
Boozy/
├── Boozy/                          # 引擎（DLL）
│   └── src/
│       ├── Boozy.h                 # 对外总头文件
│       ├── bzpch.h/.cpp            # 预编译头
│       ├── Boozy/
│       │   ├── Application.h/.cpp  # 应用基类
│       │   ├── Core.h              # DLL 导出/导入宏
│       │   ├── EntryPoint.h        # main() 入口点
│       │   ├── Log.h/.cpp          # 日志系统
│       │   ├── Window.h            # 窗口抽象接口
│       │   ├── Layer.h/.cpp        # 层基类
│       │   ├── LayerStack.h/.cpp   # 层堆栈
│       │   └── Events/             # 事件系统
│       │       ├── Event.h             # 事件基类 + 分发器
│       │       ├── ApplicationEvent.h  # 窗口 / 应用事件
│       │       ├── KeyEvent.h          # 键盘事件
│       │       └── MouseEvent.h        # 鼠标事件
│       └── Platform/               # 平台实现
│           └── Windows/
│               ├── WindowsWindow.h     # GLFW 窗口实现声明
│               └── WindowsWindow.cpp   # GLFW 窗口实现
├── Sandbox/                        # 示例游戏（exe）
│   └── src/
│       └── SandboxApp.cpp          # 游戏逻辑 + CreateApplication
├── premake5.lua                    # 构建配置
├── GenerateProjects.bat            # 生成工程脚本
├── Boozy.slnx                      # 解决方案（由 premake 生成）
└── vendor/
    ├── GLFW/                       # GLFW 源码（git submodule，固定 3.4）
    ├── glfw_config.h               # GLFW 平台宏配置（_GLFW_WIN32）
    └── bin/premake/                # premake5（未纳入版本控制）
```

## 快速示例

待补充

## 计划

- [x] 接入 GLFW（窗口创建，git submodule 引入）
- [x] 事件系统（含 GLFW 回调接线）
- [x] 层系统（Layer / LayerStack）
- [ ] 接入 GLAD（OpenGL 上下文加载）
- [ ] 输入抽象（键盘 / 鼠标状态查询）
- [ ] 渲染抽象层（VertexBuffer / Shader / Renderer）

## License

待补充
