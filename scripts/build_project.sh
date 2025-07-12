#!/bin/bash
set -e

# Argument: native or cross
MODE=${1:-native}

# Project root
PROJECT_ROOT="$(cd "$(dirname "$0")/.." && pwd)"

# Build directories
BUILD_DIR="$PROJECT_ROOT/build"
INSTALL_DIR="$PROJECT_ROOT/install"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Print colored message
print_status() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Check if dependencies are built
check_dependencies() {
    print_status "Checking dependencies..."
    
    if [ ! -d "$PROJECT_ROOT/include/openssl" ]; then
        print_error "OpenSSL headers not found. Run ./scripts/build.sh first."
        exit 1
    fi
    
    if [ ! -d "$PROJECT_ROOT/include/paho" ]; then
        print_error "Paho MQTT headers not found. Run ./scripts/build.sh first."
        exit 1
    fi
    
    if [ ! -d "$PROJECT_ROOT/include/nlohmann" ]; then
        print_error "nlohmann/json headers not found. Run ./scripts/build.sh first."
        exit 1
    fi
    
    if [ ! -f "$PROJECT_ROOT/lib/libssl.so" ] && [ ! -f "$PROJECT_ROOT/lib/libssl.a" ]; then
        print_error "OpenSSL libraries not found. Run ./scripts/build.sh first."
        exit 1
    fi
    
    if [ ! -f "$PROJECT_ROOT/lib/libpaho-mqtt3a.so" ] && [ ! -f "$PROJECT_ROOT/lib/libpaho-mqtt3a.a" ]; then
        print_error "Paho MQTT libraries not found. Run ./scripts/build.sh first."
        exit 1
    fi
    
    print_status "Dependencies check passed."
}

# Build project
build_project() {
    print_status "Building project in $MODE mode..."
    
    # Create build directory
    mkdir -p "$BUILD_DIR"
    cd "$BUILD_DIR"
    
    # Configure with CMake
    if [ "$MODE" = "cross" ]; then
        print_status "Configuring for cross-compilation..."
        cmake .. -DCMAKE_TOOLCHAIN_FILE="$PROJECT_ROOT/CMakeLists.txt.cross"
    else
        print_status "Configuring for native build..."
        cmake ..
    fi
    
    # Build
    print_status "Building project..."
    make -j$(nproc)
    
    print_status "Build completed successfully!"
}

# Install project
install_project() {
    print_status "Installing project..."
    
    cd "$BUILD_DIR"
    make install DESTDIR="$INSTALL_DIR"
    
    print_status "Installation completed to $INSTALL_DIR"
}

# Clean build
clean_build() {
    print_status "Cleaning build directory..."
    rm -rf "$BUILD_DIR"
    rm -rf "$INSTALL_DIR"
    print_status "Clean completed."
}

# Show help
show_help() {
    echo "Usage: $0 [OPTION]"
    echo ""
    echo "Options:"
    echo "  native    Build for native system (default)"
    echo "  cross     Build for ARM cross-compilation"
    echo "  clean     Clean build directory"
    echo "  install   Build and install project"
    echo "  help      Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0              # Native build"
    echo "  $0 native       # Native build"
    echo "  $0 cross        # Cross-compilation"
    echo "  $0 clean        # Clean build"
    echo "  $0 install      # Build and install"
}

# Main script
case "$MODE" in
    "native"|"cross")
        check_dependencies
        build_project
        ;;
    "clean")
        clean_build
        ;;
    "install")
        check_dependencies
        build_project
        install_project
        ;;
    "help"|"-h"|"--help")
        show_help
        ;;
    *)
        print_error "Unknown option: $MODE"
        show_help
        exit 1
        ;;
esac 