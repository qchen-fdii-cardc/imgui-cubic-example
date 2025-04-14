# OpenGL 骨架程序

[Github Repository](https://github.com/qchen-fdii-cardc/imgui-cubic-example)

一个具有着色器管理和UI控制的现代OpenGL应用程序骨架。

## 功能特点

- 现代OpenGL (3.3+) 渲染
- 支持热重载的着色器管理系统
- 基于ImGui的UI控制
- 相机控制（轨道、平移、缩放）
- 通过INI文件配置着色器参数
- 实时更新着色器参数

## 依赖项

### 必需的库

- OpenGL
- GLEW
- GLFW3
- GLM
- ImGui
- SimpleIni
- ICU (用于SimpleIni的Unicode支持)

### 安装说明 (Ubuntu/WSL2)

```bash
# 更新软件包列表
sudo apt-get update

# 安装OpenGL开发文件
sudo apt-get install libgl1-mesa-dev

# 安装GLEW
sudo apt-get install libglew-dev

# 安装GLFW3
sudo apt-get install libglfw3-dev

# 安装GLM
sudo apt-get install libglm-dev

# 安装ImGui
sudo apt-get install libimgui-dev

# 安装SimpleIni
sudo apt-get install libsimpleini-dev

# 安装ICU (用于SimpleIni的Unicode支持)
sudo apt-get install libicu-dev
```

## 构建说明

```bash
# 创建构建目录，使用CMake配置
cmake -B build

# 构建
cmake --build build

# 运行
./build/opengl_skeleton
```

## 使用说明

1. 运行程序：

```bash
./build/opengl_skeleton
```

2. 控制方式：
   - 鼠标左键：轨道相机
   - 鼠标右键：平移相机
   - 鼠标滚轮：缩放相机
   - UI控制：实时调整着色器参数

3. 着色器配置：
   - 编辑 `shader_config.ini` 修改着色器参数
   - 更改实时生效
   - 着色器文件位于 `shaders/vertex` 和 `shaders/fragment` 目录

## 库文件查找方法

在CMake中，有两种主要的方法来查找和链接外部库：`find_package` 和 `pkg-config`。本项目同时使用了这两种方法，下面详细介绍它们的区别和使用场景。

### find_package 方法

`find_package` 是CMake原生的库查找机制，它通过查找和加载CMake模块来定位库文件。

#### 工作原理

1. CMake会在预定义的路径中查找名为 `Find<包名>.cmake` 的模块文件
2. 如果找到，则执行该模块文件，该模块会设置一系列变量（如 `<包名>_INCLUDE_DIRS`、`<包名>_LIBRARIES` 等）
3. 如果没找到，则尝试使用 `<包名>Config.cmake` 文件（通常由库的开发者提供）

#### 使用示例

```cmake
# 查找OpenGL库
find_package(OpenGL REQUIRED)

# 查找GLEW库
find_package(GLEW REQUIRED)

# 查找GLFW3库
find_package(glfw3 REQUIRED)

# 查找GLM库
find_package(glm REQUIRED)
```

#### 优点

- 是CMake原生的查找机制
- 可以处理复杂的依赖关系
- 支持版本要求、组件选择等高级功能
- 可以自定义查找逻辑

#### 缺点

- 需要库提供CMake支持（通过Find模块或Config文件）
- 不同库的变量命名可能不一致
- 某些库可能没有提供CMake支持

### pkg-config 方法

`pkg-config` 是一个用于从已安装的库中检索编译和链接参数的工具，在Linux系统上广泛使用。

#### 工作原理

1. pkg-config读取 `.pc` 文件（通常位于 `/usr/lib/x86_64-linux-gnu/pkgconfig`）
2. 这些文件包含了库的编译和链接信息
3. CMake通过 `pkg_check_modules` 命令调用pkg-config获取这些信息

#### 使用示例

```cmake
# 启用pkg-config支持
find_package(PkgConfig REQUIRED)

# 查找ImGui库
pkg_check_modules(IMGUI REQUIRED imgui)

# 查找SimpleIni库
pkg_check_modules(SIMPLEINI REQUIRED simpleini)
```

#### 优点

- 在Linux系统上广泛支持
- 标准化的库信息格式
- 可以处理没有CMake支持的库
- 自动处理依赖关系

#### 缺点

- 主要限于Linux系统
- 需要库提供 `.pc` 文件
- 功能相对简单，不支持复杂的配置选项
- 还需要显式增加路径支持，例如：

```cmake
target_include_directories(${PROJECT_NAME} PRIVATE
    ${IMGUI_INCLUDE_DIRS}  # 添加IMGUI头文件目录
    ${SIMPLEINI_INCLUDE_DIRS}  # 添加SimpleIni头文件目录
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${IMGUI_LIBRARIES}  # 添加IMGUI库
    ${SIMPLEINI_LIBRARIES}  # 添加SimpleIni库
)
```

### 两种方法的结合使用

在本项目中，我们结合使用了这两种方法：

- 使用 `find_package` 查找OpenGL、GLEW、GLFW3和GLM等提供CMake支持的库
- 使用 `pkg-config` 查找ImGui和SimpleIni等通过pkg-config提供信息的库

这种组合使用可以充分利用两种方法的优点，提高项目的兼容性和可移植性。

## CMakeLists.txt 函数详解

### cmake_minimum_required

- **含义**：指定CMake最低版本要求
- **语法**：`cmake_minimum_required(VERSION major.minor)`
- **作用**：确保项目使用足够新版本的CMake
- **配合**：通常放在CMakeLists.txt的最开始

### project

- **含义**：定义项目名称和使用的语言
- **语法**：`project(项目名 [语言列表])`
- **作用**：设置项目名称，可选指定使用的编程语言
- **配合**：在cmake_minimum_required之后使用

### set

- **含义**：设置CMake变量
- **语法**：`set(变量名 值 [CACHE 类型 描述])`
- **作用**：定义或修改CMake变量
- **配合**：用于设置C++标准、编译选项等

### cmake_policy

- **含义**：设置CMake策略
- **语法**：`cmake_policy(SET 策略名 值)`
- **作用**：控制CMake的行为
- **配合**：用于处理不同版本CMake的兼容性

### find_package

- **含义**：查找外部库
- **语法**：`find_package(包名 [REQUIRED] [COMPONENTS 组件列表])`
- **作用**：查找并配置外部依赖
- **配合**：与target_link_libraries配合使用

### pkg_check_modules

- **含义**：使用pkg-config查找包
- **语法**：`pkg_check_modules(变量前缀 包名)`
- **作用**：通过pkg-config查找系统安装的库
- **配合**：用于查找没有CMake配置文件的库

### add_executable

- **含义**：创建可执行文件目标
- **语法**：`add_executable(目标名 源文件列表)`
- **作用**：定义项目的可执行文件
- **配合**：与target_link_libraries配合使用

### add_definitions

- **含义**：添加预处理器定义
- **语法**：`add_definitions(-D定义名)`
- **作用**：添加编译时的预处理器定义
- **配合**：用于条件编译和特性开关

### target_include_directories

- **含义**：设置包含目录
- **语法**：`target_include_directories(目标名 PRIVATE|PUBLIC|INTERFACE 目录列表)`
- **作用**：指定头文件搜索路径
- **配合**：与add_executable配合使用

### target_link_libraries

- **含义**：链接库
- **语法**：`target_link_libraries(目标名 PRIVATE|PUBLIC|INTERFACE 库列表)`
- **作用**：指定需要链接的库
- **配合**：与add_executable配合使用

### add_custom_command

- **含义**：添加自定义命令
- **语法**：`add_custom_command(TARGET 目标名 POST_BUILD COMMAND 命令)`
- **作用**：在构建过程中执行自定义命令
- **配合**：用于文件复制、后处理等操作

## 许可证

本项目采用 MIT 许可证 - 详见 LICENSE 文件
