# TextSearcher_3

基于 Qt 的 Windows 桌面文件搜索工具，支持目录树浏览、文件内容关键字检索与按扩展名过滤，并可一键调用外部程序（默认 QuickLook）预览文件。

## 功能特性

- **目录树浏览**：输入路径后递归（DFS）展开文件夹，子目录懒加载，单击节点加载下一层，节点带目录/文件图标
- **文件内容搜索**：在指定根目录下递归扫描所有匹配扩展名的文件，统计关键字在文件中出现的次数，结果显示在表格中
- **扩展名过滤**：默认支持 `text / go / c / cpp / java / m / py / jsp / html / css / js`，可在界面中自定义
- **一键预览**：双击文件行调用配置的外部程序（默认 `QuickLook.exe`）打开；双击表格第二列可跳转至文件所在目录
- **目录层级导航**：`上一层` 按钮快速返回上级目录
- **配置持久化**：搜索的扩展名列表与打开方式保存在 `TOTOO\TextSearcher\TextSearcherConfig.txt`，下次启动自动加载
- **KMP 字符串匹配**：核心查找使用手写 KMP 算法（见 `stringOP::findPlace` 与 `getNextArray`），性能优于朴素匹配
- **后台线程搜索**：搜索任务运行在独立线程（`std::thread` + `detach`），避免阻塞 UI

## 技术栈

| 类别 | 说明 |
|------|------|
| 语言 | C++11 |
| 框架 | Qt（core、gui、widgets、svg 模块） |
| 平台 | Windows（依赖 `_findfirst/_findnext`、`<Windows.h>`、`<direct.h>`） |
| 算法 | KMP 字符串匹配、DFS 目录遍历 |
| 多线程 | `std::thread` |

## 目录结构

```
TextSearcher_3/
├── TextSearcher_3.pro          # Qt 工程文件
├── TextSearcher_3.rc           # Windows 资源文件（图标等）
├── main.cpp                    # 程序入口 + 控制台版 DFS 扫描示例
├── mainwindow.h / .cpp         # 主窗口：树+表格+输入区
├── mainwindow.ui               # Qt Designer 界面文件
├── ui_mainwindow.h             # 由 .ui 自动生成的头
├── mysearch.h / .cpp           # 早期独立搜索类（基本未用）
├── stringop.h / .cpp           # 字符串工具：KMP 查找/替换/分割/读文件
├── vectorop.h / vectorOP.cpp   # vector/map 模板工具：打印/转换/切片
├── loadingconfig.h / .cpp      # 配置文件读写（扩展名、外部程序）
├── treewidgetitemwrapper.h/.cpp # QTreeWidgetItem 的 Qt 信号包装
├── resource.h                  # Windows 资源头
├── logo.qrc                    # Qt 资源：目录/文件图标
└── map/                        # 图标资源（PNG/JPG/SVG）
    ├── dir_*.png                # 目录图标
    ├── file_*.png              # 文件图标
    └── totoo.jpg                # 应用图标
```

## 构建与运行

### 依赖
- Qt 5.x 或 Qt 6.x（需包含 `widgets` 与 `svg` 模块）
- Windows 平台 + MSVC 编译器（代码使用了 Windows 特有的 `_finddata_t` 与 `_findfirst`）

### 步骤
1. 使用 **Qt Creator** 打开 [TextSearcher_3.pro](TextSearcher_3.pro)
2. 选择合适的 Kit（MSVC 构建套件）
3. 点击 **Build → Run** 即可编译并运行
4. 命令行构建：
   ```powershell
   qmake TextSearcher_3.pro
   nmake          # 或 mingw32-make，取决于编译器
   ```

### 运行时
- 首次启动若 `TOTOO\TextSearcher\TextSearcherConfig.txt` 不存在，会自动创建并写入默认配置
- 如需修改默认打开程序，可在界面"双击文件（夹）打开方式"输入框中填写可执行文件完整路径，关闭程序时自动保存

## 使用说明

界面分为三栏（左右分屏 + 右下表格）：

1. **左栏输入区**
   - `请输入文件夹路径`：填写要扫描的根目录，点击 `Open 文件夹` 加载到目录树
   - `所点击文件（夹）路径`：显示当前在目录树中选中节点的全路径
   - `输入文件后缀`：以 `.` 分隔的扩展名，如 `.text.go.c.cpp.java.`，影响搜索范围
   - `输入搜索内容`：要查找的关键字（默认 `.*`）
   - `双击文件（夹）打开方式`：双击文件时调用的外部程序路径
   - `上一层`：返回当前目录的上一级
2. **中栏目录树**：单击节点展开下一层，双击节点用外部程序打开
3. **右栏结果表格**：`filename / filepath / displaynum` 三列，双击第 1 列打开文件，双击第 2 列在目录树中定位所在文件夹

点击 `search file` 按钮启动后台搜索，完成后结果填入右栏表格。

## 核心实现说明

- **KMP 匹配**：[stringop.cpp](stringop.cpp) 中的 `stringOP::findPlace` 通过预计算 `next` 数组实现 O(n+m) 匹配，`op=2` 时还会合并相邻重叠命中。
- **DFS 目录遍历**：[mainwindow.cpp](mainwindow.cpp) 中的 `MainWindow::DfsListFolderFiles`（模板函数，支持以 `QTreeWidget` 或 `QTreeWidgetItem` 为父节点）使用 `_findfirst/_findnext/_findclose` 完成递归遍历。
- **内容搜索**：`MainWindow::DfsSearchFiles` 递归扫描，对符合扩展名的文件调用 `stringOP::readTxtByChar` 读全文，再用 `findPlace` 统计关键字出现次数，结果填入 `QTableWidget`。
- **配置管理**：[loadingconfig.h](loadingconfig.h) 中 `LoadingConfig` 类在构造时读取本地配置；`~MainWindow` 析构时调用 `writeConfig` 回写界面中的扩展名与打开方式。
- **中文编码**：`main.cpp` 使用 `QTextCodec::setCodecForLocale("GB2312")`，源文件以 `fromLocal8Bit` 与 `QString` 互转处理中文路径。

## 已知限制

- 仅支持 Windows（依赖 `_finddata_t` 系列 API）
- 文件内容采用一次性整体读取（`readTxtByChar`），超大文件可能占用较多内存
- `mysearch.cpp` / `treewidgetitemwrapper.cpp` 基本为空文件，属于历史残留
- `main.cpp` 中含有调试用的硬编码绝对路径（已注释）

## 开发者

个人项目。应用图标见 `map/totoo.jpg`。
