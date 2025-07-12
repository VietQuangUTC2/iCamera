# Build Guide

Complete guide for building iCamera project for both native and cross-compilation.

## Overview

The project uses `scripts/build.sh` to automatically build dependencies:
- **OpenSSL** - Cryptography and SSL/TLS library
- **Paho MQTT C** - MQTT client library
- **nlohmann/json** - JSON library for C++

## System Requirements

### Native Build
- Ubuntu 18.04+ or equivalent
- GCC/G++ compiler
- CMake 3.10+
- Make
- wget

### Cross-compilation
- ARM Toolchain: `arm-rockchip830-linux-uclibcgnueabihf`
- Corresponding sysroot
- CMake 3.10+

## Usage

### 1. Native Build (x86-64)

```bash
# Build for current system
./scripts/build.sh native

# Or simply (default is native)
./scripts/build.sh
```

### 2. Cross-compilation (ARM)

```bash
# Build for ARM target
./scripts/build.sh cross
```

## Output Structure

After successful build, files will be created in:

```
iCamera/
├── include/
│   ├── openssl/    # OpenSSL headers
│   ├── paho/       # Paho MQTT headers
│   └── nlohmann/   # nlohmann/json headers
└── lib/
    ├── libssl.a            # OpenSSL static library
    ├── libssl.so           # OpenSSL shared library
    ├── libcrypto.a         # OpenSSL crypto static library
    ├── libcrypto.so        # OpenSSL crypto shared library
    ├── libpaho-mqtt3a.a    # Paho MQTT static library
    └── libpaho-mqtt3a.so   # Paho MQTT shared library
```

## Toolchain Configuration

### Cross-compilation Settings

File `scripts/build.sh` contains toolchain configuration:

```bash
# Toolchain prefix
CROSS_PREFIX="arm-rockchip830-linux-uclibcgnueabihf-"

# Sysroot path
SYSROOT="/home/quangtv98/workspace/camera/rockchip/sdk/luckfox-pico/tools/linux/toolchain/arm-rockchip830-linux-uclibcgnueabihf/arm-rockchip830-linux-uclibcgnueabihf/sysroot"
```

**Note**: Update the `SYSROOT` path according to your toolchain.

## Troubleshooting

### Common Issues

#### 1. "cannot find -lanl" Error
```
/home/quangtv98/workspace/camera/rockchip/sdk/luckfox-pico/tools/linux/toolchain/arm-rockchip830-linux-uclibcgnueabihf/bin/../lib/gcc/arm-rockchip830-linux-uclibcgnueabihf/8.3.0/../../../../arm-rockchip830-linux-uclibcgnueabihf/bin/ld.bfd: cannot find -lanl
```

**Cause**: uClibc doesn't have `libanl` library

**Solution**: Script automatically removes this dependency:
```bash
sed -i '/anl/d' "$PAHO_SRC/src/CMakeLists.txt"
```

#### 2. Building x86-64 instead of ARM
```
libpaho-mqtt3as.so.1.3.13: ELF 64-bit LSB shared object, x86-64
```

**Cause**: Old CMake cache or toolchain not used correctly

**Solution**: 
- Remove old build directory: `rm -rf build/`
- Ensure using `-DCMAKE_C_COMPILER=${CROSS_PREFIX}gcc`

#### 3. "same file" Copy Error
```
cp: '/path/to/file' and '/path/to/file' are the same file
```

**Cause**: Copying file to itself

**Solution**: Check and fix paths in script

### Verify Results

#### Native Build
```bash
file lib/*.so*
# Expected: ELF 64-bit LSB shared object, x86-64
```

#### Cross-compilation
```bash
file lib/*.so*
# Expected: ELF 32-bit LSB shared object, ARM
```

## Clean Build

To rebuild from scratch:

```bash
# Remove all build artifacts
rm -rf dependencies/*/build/
rm -rf lib/*
rm -rf include/openssl include/paho include/nlohmann

# Rebuild
./scripts/build.sh cross
```

## Versions

Dependencies versions used:

- **OpenSSL**: 1.1.1w
- **Paho MQTT C**: 1.3.13
- **nlohmann/json**: v3.12.0

Update versions in `scripts/build.sh`:

```bash
PAHO_VERSION="1.3.13"
OPENSSL_VERSION="1.1.1w"
NLOHMANN_JSON_VERSION="v3.12.0"
```

## CMake Integration

To use libraries in CMake project:

```cmake
# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Library directories
link_directories(${PROJECT_SOURCE_DIR}/lib)

# Link libraries
target_link_libraries(your_target
    ssl
    crypto
    paho-mqtt3a
)
```

## Notes

1. **Permissions**: Ensure script is executable: `chmod +x scripts/build.sh`
2. **Network**: Internet connection required to download dependencies
3. **Disk space**: Approximately 500MB needed for build process
4. **Time**: Cross-compilation may take 10-15 minutes

## Support

If you encounter issues, check:
1. Toolchain path is correct
2. Sysroot exists
3. Sufficient disk space
4. Stable network connection 