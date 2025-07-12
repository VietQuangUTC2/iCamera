#!/bin/bash
set -e

# Argument: native or cross
MODE=${1:-native}

# Output directories
PROJECT_ROOT="$(cd "$(dirname "$0")/.." && pwd)"
INCLUDE_DIR="$PROJECT_ROOT/include"
LIB_DIR="$PROJECT_ROOT/lib"

# Versions
PAHO_VERSION="1.3.13"
OPENSSL_VERSION="1.1.1w"
NLOHMANN_JSON_VERSION="v3.12.0"

# Dependencies directory
DEPS_DIR="$PROJECT_ROOT/dependencies"

# Toolchain for cross-compilation
CROSS_PREFIX="arm-rockchip830-linux-uclibcgnueabihf-"
# Build OpenSSL (for both native and cross)
build_openssl() {
    echo "[+] Build OpenSSL $OPENSSL_VERSION"
    
    OPENSSL_TAR="$DEPS_DIR/openssl-$OPENSSL_VERSION.tar.gz"
    OPENSSL_SRC="$DEPS_DIR/openssl-$OPENSSL_VERSION"

    if [ ! -f "$OPENSSL_TAR" ]; then
        wget -O "$OPENSSL_TAR" https://www.openssl.org/source/openssl-$OPENSSL_VERSION.tar.gz
    fi
    if [ ! -d "$OPENSSL_SRC" ]; then
        tar -C "$DEPS_DIR" -xf "$OPENSSL_TAR"
    fi
    cd "$OPENSSL_SRC"
    
    OPENSSL_INSTALL_DIR="$OPENSSL_SRC/build"
    rm -rf $OPENSSL_INSTALL_DIR
    mkdir -p "$OPENSSL_INSTALL_DIR"
    
    export CFLAGS="-fPIC"
    export CXXFLAGS="-fPIC"
    if [ "$MODE" = "cross" ]; then
        export CROSS_COMPILE=$CROSS_PREFIX
        ./Configure linux-generic32 --cross-compile-prefix=$CROSS_PREFIX --prefix="$OPENSSL_INSTALL_DIR" no-tests no-afalgeng no-hw-padlock
    else
        ./config --prefix="$OPENSSL_INSTALL_DIR" no-tests no-afalgeng no-hw-padlock
    fi

    make clean
    make -j$(nproc)
    make install

    cd -
    # Copy OpenSSL libraries
    cp -a "$OPENSSL_INSTALL_DIR/lib/"*.a "$LIB_DIR/" 2>/dev/null || true
    cp -a "$OPENSSL_INSTALL_DIR/lib/"*.so* "$LIB_DIR/" 2>/dev/null || true

    # Copy OpenSSL headers
    cp -a "$OPENSSL_INSTALL_DIR/include/openssl" "$INCLUDE_DIR/" 2>/dev/null || true
}

# Build Paho MQTT C/C++
build_paho() {
    echo "[+] Build Paho MQTT C/C++..."
    PAHO_TAR="$DEPS_DIR/paho.mqtt.c-$PAHO_VERSION.tar.gz"
    PAHO_SRC="$DEPS_DIR/paho.mqtt.c-$PAHO_VERSION"
    if [ ! -f "$PAHO_TAR" ]; then
        wget -O "$PAHO_TAR" https://github.com/eclipse/paho.mqtt.c/archive/refs/tags/v$PAHO_VERSION.tar.gz
    fi
    if [ ! -d "$PAHO_SRC" ]; then
        tar -C "$DEPS_DIR" -xf "$PAHO_TAR"
    fi
    cd "$PAHO_SRC"

    rm -rf build
    mkdir -p build && cd build
    if [ "$MODE" = "cross" ]; then
        # uClibc not have -lanl -> remove it in CMakeLists.txt
        sed -i '/anl/d' "$PAHO_SRC/src/CMakeLists.txt"
        cmake .. -DPAHO_WITH_SSL=ON \
            -DPAHO_BUILD_STATIC=ON \
            -DPAHO_BUILD_SHARED=ON \
            -DOPENSSL_ROOT_DIR="$OPENSSL_INSTALL_DIR" \
            -DOPENSSL_INCLUDE_DIR="$OPENSSL_INSTALL_DIR/include" \
            -DOPENSSL_LIBRARIES="$OPENSSL_INSTALL_DIR/lib" \
            -DCMAKE_C_COMPILER=${CROSS_PREFIX}gcc \
            -DCMAKE_INSTALL_PREFIX=$PAHO_SRC/build
    else
        cmake .. -DPAHO_WITH_SSL=ON \
            -DPAHO_BUILD_STATIC=ON \
            -DPAHO_BUILD_SHARED=ON \
            -DOPENSSL_ROOT_DIR="$OPENSSL_INSTALL_DIR" \
            -DOPENSSL_INCLUDE_DIR="$OPENSSL_INSTALL_DIR/include" \
            -DOPENSSL_LIBRARIES="$OPENSSL_INSTALL_DIR/lib" \
            -DCMAKE_INSTALL_PREFIX=$PAHO_SRC/build
    fi
    make -j$(nproc)
    make install
    cd -

    # Copy Paho MQTT libraries
    PAHO_INSTALL_DIR=$PAHO_SRC/build
    cp -a $PAHO_INSTALL_DIR/lib/*.a $LIB_DIR 2>/dev/null || true
    cp -a $PAHO_INSTALL_DIR/lib/*.so* $LIB_DIR 2>/dev/null || true
    
    # Copy Paho MQTT headers
    PAHO_HEADERS=$PROJECT_ROOT/include/paho
    mkdir -p $PAHO_HEADERS
    cp -a $PAHO_INSTALL_DIR/include/*.h $PAHO_HEADERS 2>/dev/null || true
}

# Copy nlohmann/json
nlohmann_json() {
    echo "[+] Copy nlohmann/json..."
    
    NLOHMANN_DIR="$INCLUDE_DIR/nlohmann"
    JSON_HEADER="$NLOHMANN_DIR/json.hpp"

    if [ ! -f "$JSON_HEADER" ]; then
        mkdir -p $NLOHMANN_DIR
        wget -O $JSON_HEADER https://github.com/nlohmann/json/releases/download/$NLOHMANN_JSON_VERSION/json.hpp
    fi
}

# Main
build_openssl
build_paho
nlohmann_json

echo "[+] DONE. All libraries are in $LIB_DIR and all headers are in $INCLUDE_DIR." 