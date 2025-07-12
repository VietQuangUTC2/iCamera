# iCamera Documentation Index

## 📚 Main Documentation

### [README.md](README.md)
- Project overview
- Directory structure
- Quick start guide
- Features and roadmap

### [BUILD_GUIDE.md](BUILD_GUIDE.md)
- Detailed build instructions
- Native and cross-compilation
- Troubleshooting
- Toolchain configuration

## 🛠️ Usage Guide

### Build Dependencies
```bash
# Native build
./scripts/build.sh

# Cross-compilation
./scripts/build.sh cross
```

### Verify Results
```bash
# Check architecture
file lib/*.so*

# Check dependencies
ls -la lib/
ls -la include/
```

## 🔧 Troubleshooting

### Common Issues
1. **"cannot find -lanl"** → See [BUILD_GUIDE.md#cannot-find-lanl-error](BUILD_GUIDE.md#cannot-find-lanl-error)
2. **Building x86-64 instead of ARM** → See [BUILD_GUIDE.md#building-x86-64-instead-of-arm](BUILD_GUIDE.md#building-x86-64-instead-of-arm)
3. **"same file" error** → See [BUILD_GUIDE.md#same-file-copy-error](BUILD_GUIDE.md#same-file-copy-error)

## 📁 Project Structure

```
iCamera/
├── docs/           # 📚 Documentation
├── scripts/        # 🔧 Build scripts
├── dependencies/   # 📦 Source dependencies
├── include/        # 📄 Headers
├── lib/            # 📚 Libraries
├── source/         # 💻 Source code
└── config/         # ⚙️ Configuration
```

## 🚀 Quick Links

- **[Build Script](../scripts/build.sh)** - Main build script
- **[CMakeLists.txt](../CMakeLists.txt)** - CMake configuration
- **[.gitignore](../.gitignore)** - Git ignore rules

## 📝 Notes

- All documentation is written in English
- Format: Markdown (.md)
- Updates: When build system changes

---

**Need help?** See the [Support](BUILD_GUIDE.md#support) section in BUILD_GUIDE.md 