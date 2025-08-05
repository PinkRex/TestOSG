
# Setting Up OsgEarth with Qt QML on Ubuntu

This guide provides step-by-step instructions to set up and run **OsgEarth** inside a **Qt QML** application on Ubuntu-based systems.

# Table of Contents

1. [Install Dependencies](#1-install-dependencies)  
   - [Method 1: Auto Install](#method-1-auto-install-recommended)  
   - [Method 2: Manual Installation](#method-2-manual-installation)  
     - [System Dependencies](#install-system-dependencies)  
     - [Legacy Repository for Jasper](#add-legacy-repository-for-jasper-support)  
     - [Additional Libraries](#additional-libraries)

2. [Install Libraries to Fix Common Errors](#2-install-libraries-to-fix-common-errors)  
   - [2.1. Install FFmpeg](#21-install-ffmpeg)  
   - [2.2. Install SQLite3](#22-install-sqlite3)  
   - [2.3. Install GEOS 3.12.3](#23-install-geos-3123)  
   - [2.4. Install GDAL 3.9.2](#24-install-gdal-392)

3. [Install OSG + OSGEarth](#3-install-osg--osgearth)  
   - [3.1. Install OSG 3.6.5](#31-install-osg-365)  
   - [3.2. Install OSGEarth 3.7](#32-install-osgearth-37)

4. [Common Build Errors and Fixes](#4-common-build-errors-and-fixes)  
   - [4.1. ASIO Placeholder Errors When Building OSG](#41-asio-placeholder-errors-when-building-osg)  
   - [4.2. Libzip Error When Building OsgEarth](#42-libzip-error-when-building-osgearth)

5. [Implement in Qt QML](#5-implement-in-qt-qml)


---

## 1. Install Dependencies

You can install required dependencies in **two ways**:

### Method 1: Auto Install (Recommended)

1. Download the installation script:

    ```bash
    wget https://your-download-url/install_dependencies.sh
    chmod +x install_dependencies.sh
    ./install_dependencies.sh
    ```

> Replace `https://your-download-url/` with the actual URL hosting your script.

---

### Method 2: Manual Installation

#### Install System Dependencies

```bash
sudo apt install -y \
wget build-essential autoconf locate apt-file libspdlog-dev \
git-all fuse libgl1-mesa-dev psmisc libpq-dev libssl-dev openssl libffi-dev \
zlib1g-dev libdbus-1-3 desktop-file-utils \
libxcb-icccm4 libxcb-image0 libxcb-keysyms1 libilmbase-dev \
libxcb-render-util0 libxcb-xinerama0 libxcb-composite0 libxcb-cursor0 \
libxcb-damage0 libxcb-dpms0 libxcb-dri2-0 libxcb-dri3-0 libxcb-ewmh2 \
libxcb-glx0 libxcb-present0 libxcb-randr0 libxcb-record0 libxcb-render0 \
libxcb-res0 libxcb-screensaver0 libxcb-shape0 libxcb-shm0 libxcb-sync1 \
libxcb-util1 libfontconfig libfontconfig1 libxcb-xkb1 libxkbcommon-x11-0 \
libegl1-mesa-dev unixodbc-dev curl unzip tar libnss3 libxcomposite1 \
libxcursor-dev libxtst-dev libxrandr-dev libgtk3.0-cil-dev libcurl4-openssl-dev \
libomp-dev libstdc++6 ninja-build libboost-all-dev librocksdb-dev \
freeglut3-dev libglu1-mesa-dev libx11-xcb-dev 'libxcb.*-dev' libxrender-dev \
libxi-dev libxkbcommon-dev libglew-dev libxkbcommon-x11-dev libxinerama1 \
libxrandr2 protobuf-compiler libprotobuf-dev libavformat-dev libavdevice-dev \
libavcodec-dev libavutil-dev libswscale-dev libsdl1.2-dev libsdl2-dev \
libgtkgl2.0-dev libgtkglext1-dev libasio-dev dcmtk libvncserver-dev \
xfonts-scalable libocct-data-exchange-dev libocct-draw-dev libocct-foundation-dev \
libocct-modeling-algorithms-dev libocct-modeling-data-dev libocct-ocaf-dev \
libocct-visualization-dev libzip-dev libblosc-dev libpoco-dev libgl-dev \
libsqlite3-dev nano libgomp1 \
--fix-missing
```

#### Add Legacy Repository (For Jasper Support)

```bash
sudo add-apt-repository "deb http://security.ubuntu.com/ubuntu xenial-security main"
sudo apt update
sudo apt install libjasper1 libjasper-dev
```

#### Additional Libraries

```bash
sudo apt-get install zlib1g
sudo apt-get install libxml2-dev libxml2
```

---





## 2. Install Libraries to Fix Common Errors

The dependencies above should be installed in `/usr`. The libraries below should be installed in a custom folder. Here is the recommended structure:

```
Setup/
 ├── FIX/
 │   ├── FFmpeg/
 │   └── sqlite3/
 └── VISG/
     ├── GEOS/
     ├── GDAL/
     └── OSG/
         ├── OSG-3.6.5/
         └── OSGEarth-3.7/
```

---

### 2.1. Install FFmpeg

**Common Error**: `invalid conversion from 'const AVInputFormat*' to 'AVInputFormat'`  
**Fix**: Build FFmpeg manually as below.

```bash
sudo apt install -y nasm yasm
git clone https://github.com/FFmpeg/FFmpeg.git
cd FFmpeg
git checkout release/4.1
./configure --enable-pic --enable-shared
make -j$(nproc)
sudo make install
```

Verifying Installed Versions

```bash
ffmpeg -version
```

---

### 2.2. Install SQLite3

**Common Error**: `/usr/bin/ld: /lib/libgdal.so1.26: undefined reference to 'sqlite3_column_origin_name'`  
**Fix**: Build SQLite3 manually as below

```bash
wget https://www.sqlite.org/2025/sqlite-src-3500400.zip
unzip sqlite-src-3500400.zip
cd sqlite-src-3500400
CFLAGS="-fPIC -DSQLITE_ENABLE_COLUMN_METADATA=1 -DSQLITE_ENABLE_RTREE=1" ./configure
make -j$(nproc)
sudo make install
```

> If the link doesn't work, visit: [https://www.sqlite.org/download.html](https://www.sqlite.org/download.html) and download manually.

Verifying Installed Versions

```bash
sqlite3 --version
```

---

### 2.3. Install GEOS 3.12.3

```bash
wget https://download.osgeo.org/geos/geos-3.12.3.tar.bz2
tar xjf geos-3.12.3.tar.bz2
cd geos-3.12.3
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local
make -j$(nproc)
ctest
sudo make install
```

>If the link doesn't work, visit: [https://libgeos.org/usage/download/](https://libgeos.org/usage/download/) and download manually.

**Update** `~/.bashrc`:
Replace `GEOS_DIR` with the actual path to your GEOS install directory 

```bash
# GEOS
export GEOS_DIR=/your/path/Setup/VISG/GEOS/install
export PATH=${PATH}:${GEOS_DIR}/bin
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${GEOS_DIR}/lib
export GEOS_INCLUDE_DIR=${GEOS_DIR}/include
export CMAKE_MODULE_PATH=${CMAKE_MODULE_PATH}:${GEOS_DIR}/lib/cmake/GEOS
export PKG_CONFIG_PATH=${PKG_CONFIG_PATH}:${GEOS_DIR}/lib/pkgconfig
```

Reload `~/.bashrc`:
```bash
source ~/.bashrc
```

Verifying Installed Versions:

```bash
geos-config --version
```

---

### 2.4. Install GDAL 3.9.2

```bash
wget https://download.osgeo.org/gdal/3.9.2/gdal-3.9.2.tar.gz
tar zxvf gdal-3.9.2.tar.gz
cd gdal-3.9.2
mkdir build && cd build
cmake ../gdal -DCMAKE_INSTALL_PREFIX=../install
make -j$(nproc)
sudo make install
```

> If the link doesn't work, visit: [https://github.com/OSGeo/gdal/releases/tag/v3.9.2](https://github.com/OSGeo/gdal/releases/tag/v3.9.2) and download manually.

**Update** `~/.bashrc`:
Replace `GDAL_DIR` with the actual path to your GDAL install directory 


```bash
# GDAL
export GDAL_DIR=/your/path/Setup/VISG/GDAL/install
export PATH=${PATH}:${GDAL_DIR}/bin
export GDAL_INCLUDE_DIR=${GDAL_DIR}/include
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${GDAL_DIR}/lib
export CMAKE_MODULE_PATH=${CMAKE_MODULE_PATH}:${GDAL_DIR}/lib/cmake/gdal
export PKG_CONFIG_PATH=${PKG_CONFIG_PATH}:${GDAL_DIR}/lib/pkgconfig
```

Reload `~/.bashrc`:
```bash
source ~/.bashrc
```

Verifying Installed Versions

```bash
gdalinfo --version
```

---





## 3. Install OSG + OSGEarth

We will maintain the folder structure outlined above for consistency and clarity throughout the setup process.

---

### 3.1. Install OSG 3.6.5

```bash
git clone https://github.com/openscenegraph/OpenSceneGraph.git
cd OpenSceneGraph
git checkout OpenSceneGraph-3.6.5
mkdir build install
cd build
cmake \
  -DOSG_GL3_AVAILABLE=ON \
  -DOSG_GL1_AVAILABLE=OFF \
  -DOSG_GL2_AVAILABLE=OFF \
  -DOSG_GLES1_AVAILABLE=OFF \
  -DOSG_GLES2_AVAILABLE=OFF \
  -DOSG_GLES3_AVAILABLE=OFF \
  -DOSG_GL_DISPLAYLISTS_AVAILABLE=OFF \
  -DOSG_GL_FIXED_FUNCTION_AVAILABLE=OFF \
  -DOSG_GL_MATRICES_AVAILABLE=OFF \
  -DOSG_GL_VERTEX_ARRAY_FUNCS_AVAILABLE=OFF \
  -DOSG_GL_VERTEX_FUNCS_AVAILABLE=OFF \
  -DOPENGL_PROFILE=GL3 \
  -DOSG_GL_CONTEXT_VERSION=4.6 \
  -DCMAKE_INSTALL_PREFIX=../install \
  -DCMAKE_BUILD_TYPE=Release \
  -DBUILD_OSG_EXAMPLES=ON \
  ..
cmake --build . --parallel 20 --target install
```

**Update** `~/.bashrc`:
Replace `OSG_DIR` with the actual path to your OSG install directory 

```bash
# OSG
export OSG_DIR=/your/path/Setup/VISG/OSG-3.6.5/install
export OSG_BIN_PATH=$OSG_DIR/bin
export OSG_INCLUDE_PATH=$OSG_DIR/include
export OSG_INCLUDE_DIR=$OSG_DIR/include
export OSG_LIB_PATH=$OSG_DIR/lib
export PATH=$PATH:$OSG_DIR/bin
export LD_LIBRARY_PATH=$OSG_DIR/lib:${LD_LIBRARY_PATH}
export OSGEARTH_INCLUDE_DIR=$OSG_DIR/include
export OPENTHREADS_LIB_DIR=$OSG_DIR/lib
export PATH=$OPENTHREADS_LIB_DIR:${PATH}
export PKG_CONFIG_PATH=$OSG_DIR/lib/pkgconfig:${PKG_CONFIG_PATH}
```

Reload `~/.bashrc`:
```bash
source ~/.bashrc
```

Verifying Installed Versions

```bash
osgversion
```

---

### 3.2. Install OSGEarth 3.7

```bash
git clone https://github.com/gwaldron/osgearth.git
cd osgearth
git checkout osgearth-3.7
mkdir build install 
cd build
cmake .. \
  -DCMAKE_INSTALL_PREFIX=../install
cmake --build . --parallel 20 --target install
```

**Update** `~/.bashrc`:
Replace `OSGEARTH_DIR` with the actual path to your OSGEarth install directory

```bash
# OSG
# OSGEarth
export OSGEARTH_DIR=/your/path/Setup/VISG/OSGEarth-3.7/install
export LD_LIBRARY_PATH=${OSGEARTH_DIR}/lib64:${LD_LIBRARY_PATH}
export PATH=${OSGEARTH_DIR}/bin:$PATH
export OSGEARTH_INCLUDE_DIR=${OSGEARTH_DIR}/include
```

Reload `~/.bashrc`:
```bash
source ~/.bashrc
```

Verifying Installed Versions

```bash
osgearth_version
```

To further verify that all dependencies and runtime libraries are properly configured, run the sample viewer with any valid `.earth` file from the tests/ directory inside the cloned repository:

```bash
cd ../tests
osgearth_viewer [your_earth_file.earth]
```

For example:

```bash
osgearth_viewer boston.earth
```

---





## 4. Common Build Errors and Fixes

Even with all dependencies properly installed, some issues may still occur during the build process. Below are some real-world problems and how to resolve them.

---

### 4.1. ASIO Placeholder Errors When Building OSG

**Error message:**  

```text
error: ‘error’ is not a member of ‘asio::placeholders’
error: ‘bytes_transferred’ is not a member of ‘asio::placeholders’
```

**Cause:**  
This error occurs when there's a version mismatch between Boost.Asio and standalone Asio being used in the OSG plugin build.  

**Solution:**  
You can either manually fix each occurrence of the error, or the simpler solution is to remove the plugin causing the issue.  

In most cases, the issue originates from the **RestHttpDevice** plugin. To disable it:

1. Locate the plugin folder:  

```bash
OpenSceneGraph/src/osgPlugins/RestHttpDevice
```

2. Delete the folder, or comment out/remove the line referencing it in the `CMakeLists.txt` of the parent osgPlugins directory.  

This will prevent the plugin from being built, resolving the error.

---

### 4.2. Libzip Error When Building OsgEarth

If you encounter a linking or symbol error related to libzip, it may be due to a system-installed version that's outdated or incompatible.

**Solution:**

```bash
git clone https://github.com/nih-at/libzip.git
cd libzip
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local
make -j$(nproc)
sudo make install
```

**Update** `~/.bashrc`: Replace `/your/path` with the actual path to your install directory

```bash
# System lib
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH

# OSGEarth
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/your/path/Setup/OSG/osgearth/install/lib64

# OSG
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/your/path/Setup/OSG/OpenSceneGraph/install/lib

# GDAL
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/your/path/Setup/GDAL/install/lib

# Qt (Mine is 6.9.1)
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/your/path/Qt/6.9.1/gcc_64/lib
```

Reload `~/.bashrc`:
```bash
source ~/.bashrc
```

---





## 5. Implement In Qt QML

To integrate **osgEarth** with a **Qt QML** application, modify your `CMakeLists.txt`

```cmake
# Set paths to your OSG and osgEarth installation directories
set(OSGEARTH_ROOT /your/path/Setup/OSG/osgearth/install)
set(OSG_ROOT /your/path/Setup/OSG/OpenSceneGraph/install)

# Include header files
include_directories(${OSGEARTH_ROOT}/include)
include_directories(${OSG_ROOT}/include)

# Link the libraries
link_directories(${OSGEARTH_ROOT}/lib64)
link_directories(${OSG_ROOT}/lib)
link_directories(/your/path/Setup/GDAL/install/lib/)

# Link libraries to your Qt QML app
target_link_libraries(your_app
    PRIVATE Qt6::Quick

    # OpenSceneGraph core modules
    osg
    osgDB
    osgGA
    osgViewer
    osgUtil
    osgText
    
    # osgEarth
    osgEarth

    # GIS dependencies
    gdal
    proj
    sqlite3
)
```

> **Replace** `/your/path` with the actual path to your local installation directories.
