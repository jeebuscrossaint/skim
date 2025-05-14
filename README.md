# Skim Browser

A lightweight, minimal browser based on WebKit and GTK.

## Features

- Clean, minimal UI
- Fast and lightweight
- Lua-based configuration
- Tab support
- Keyboard shortcuts for common actions
- Cross-platform (Linux, potentially Windows and macOS)

## Building

### Requirements

- CMake 3.10 or higher
- GTK3 development libraries
- WebKit2GTK development libraries
- Lua development libraries

### Build Steps

```bash
# Clone the repository
git clone https://github.com/yourusername/skim.git
cd skim

# Create build directory
mkdir build
cd build

# Configure and build
cmake ..
make

# Run the browser
./skim
