# iCamera Project Documentation

## Overview

iCamera is a smart camera project using open-source libraries for video processing, MQTT communication, and SSL/TLS encryption.

## Project Structure

```
iCamera/
├── docs/                 # Documentation
│   ├── README.md        # Project overview
│   └── BUILD_GUIDE.md   # Build instructions
├── scripts/             # Build scripts
│   └── build.sh         # Dependency build script
├── dependencies/        # Source code dependencies
│   ├── openssl-1.1.1w/  # OpenSSL source
│   └── paho.mqtt.c-1.3.13/ # Paho MQTT source
├── include/             # Header files
│   ├── openssl/         # OpenSSL headers
│   ├── paho/           # Paho MQTT headers
│   └── nlohmann/       # nlohmann/json headers
├── lib/                 # Libraries
│   ├── libssl.a/so     # OpenSSL libraries
│   ├── libcrypto.a/so  # OpenSSL crypto libraries
│   └── libpaho-mqtt3a.a/so # Paho MQTT libraries
├── source/              # Main source code
├── config/              # Configuration files
└── CMakeLists.txt       # CMake configuration
```

## Dependencies

### Core Libraries
- **OpenSSL 1.1.1w** - Cryptography and SSL/TLS
- **Paho MQTT C 1.3.13** - MQTT client
- **nlohmann/json v3.12.0** - JSON parsing

### Build Tools
- **CMake 3.10+** - Build system
- **GCC/G++** - Compiler (native)
- **ARM Toolchain** - Cross-compilation

## Quick Start

### 1. Clone Repository
```bash
git clone <repository-url>
cd iCamera
```

### 2. Build Dependencies
```bash
# Native build
./scripts/build.sh

# Cross-compilation
./scripts/build.sh cross
```

### 3. Build Project
```bash
mkdir build && cd build
cmake ..
make
```

## Documentation

- **[Build Guide](BUILD_GUIDE.md)** - Detailed build instructions
- **[API Reference]** - API documentation (coming soon)
- **[Configuration]** - Configuration guide (coming soon)

## Features

### Completed
- ✅ Build system for native and cross-compilation
- ✅ OpenSSL integration
- ✅ Paho MQTT client
- ✅ nlohmann/json support
- ✅ Basic documentation

### In Development
- 🔄 Camera integration
- 🔄 Video processing
- 🔄 MQTT communication
- 🔄 SSL/TLS security

## Platform Support

### Native
- Ubuntu 18.04+
- CentOS 7+
- Debian 9+

### Cross-compilation
- ARM Linux (uClibc)
- Rockchip RK830 series
- Luckfox Pico

## Contributing

1. Fork repository
2. Create feature branch
3. Commit changes
4. Push to branch
5. Create Pull Request

## License

[License information to be added]

## Contact

[Contact information to be added]

---

**Note**: Project is under development. Documentation will be updated regularly. 