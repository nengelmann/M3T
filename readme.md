<p align="center">
  <h2 align="center">M3T</h2>
</p>
<p align="center">
    This repo provides examples, tools and installation instructions as well as modified source code of the <a href="https://github.com/DLR-RM/3DObjectTracking/tree/master">3DObjectTracking</a> library, provided by Manuel Stoiber and the <a href="https://github.com/DLR-RM">German Aerospace Center</a>. 
</p>

# _Work in progess.._

### 3D Object Tracking

Tracking objects and kinematic structures in 3D space and determining their poses and configurations is an essential task in computer vision. Its application ranges from augmented reality to robotic perception. Given consecutive image frames, as well as 3D meshes and kinematic information, the goal is to robustly estimate the rotation and translation of all bodies relative to a camera. While the problem has been thoroughly studied, many challenges such as partial occlusions, appearance changes, motion blur, background clutter, object ambiguity, and real-time requirements still exist.

## Installation

The following installation instruction is for Ubuntu 22.04, but might work for other Ubuntu versions as well.
All commands in need of root privileges are preceded by `sudo`. Consider removing it if you run with root privileges already.

The installation covers Microsofts [Azure Kinect DK](https://azure.microsoft.com/en-us/products/kinect-dk) camera but not Intels [RealSense](https://www.intelrealsense.com/) cameras.
In future this repo will be extended with Luxonis [OAK](https://shop.luxonis.com/collections/oak-cameras-1) camera series.

### Prerequisites

First clone this repo and cd into this directory.

```
git clone https://github.com/nengelmann/M3T.git && cd ./M3T
```

Then create a variable for the current working directory.

```
cwd=$(pwd)
```

Update apt and apt-get.

```
sudo apt update && sudo apt-get update
```

And then install the following dependencies.

```
sudo apt install libeigen3-dev libglew-dev libglfw3-dev libopencv-dev
sudo apt-get install doxygen
```

### Installation of Azure Kinect camera packages

If you are running a Ubuntu version other than 22.04, please refer to the [original installation instructions](https://github.com/microsoft/Azure-Kinect-Sensor-SDK/blob/develop/docs/usage.md).

Azure Kinect is **not officially supported for Ubuntu 22.04**.
The following instructions will enable you to install it.

```
sudo apt-add-repository -y -n 'deb http://archive.ubuntu.com/ubuntu focal main'
sudo apt-add-repository -y 'deb http://archive.ubuntu.com/ubuntu focal universe'
sudo apt-get install -y libsoundio1
sudo apt-add-repository -r -y -n 'deb http://archive.ubuntu.com/ubuntu focal universe'
sudo apt-add-repository -r -y 'deb http://archive.ubuntu.com/ubuntu focal main'

if ! dpkg -s libk4a1.3 > /dev/null; then
	curl -sSL https://packages.microsoft.com/ubuntu/18.04/prod/pool/main/libk/libk4a1.3/libk4a1.3_1.3.0_amd64.deb > /tmp/libk4a1.3_1.3.0_amd64.deb
	echo 'libk4a1.3 libk4a1.3/accepted-eula-hash string 0f5d5c5de396e4fee4c0753a21fee0c1ed726cf0316204edda484f08cb266d76' | sudo debconf-set-selections
	sudo dpkg -i /tmp/libk4a1.3_1.3.0_amd64.deb
fi
if ! dpkg -s libk4a1.3-dev > /dev/null; then
	curl -sSL https://packages.microsoft.com/ubuntu/18.04/prod/pool/main/libk/libk4a1.3-dev/libk4a1.3-dev_1.3.0_amd64.deb > /tmp/libk4a1.3-dev_1.3.0_amd64.deb
	sudo dpkg -i /tmp/libk4a1.3-dev_1.3.0_amd64.deb
fi
if ! dpkg -s libk4abt1.0 > /dev/null; then
	curl -sSL https://packages.microsoft.com/ubuntu/18.04/prod/pool/main/libk/libk4abt1.0/libk4abt1.0_1.0.0_amd64.deb > /tmp/libk4abt1.0_1.0.0_amd64.deb
	echo 'libk4abt1.0	libk4abt1.0/accepted-eula-hash	string	03a13b63730639eeb6626d24fd45cf25131ee8e8e0df3f1b63f552269b176e38' | sudo debconf-set-selections
	sudo dpkg -i /tmp/libk4abt1.0_1.0.0_amd64.deb
fi
if ! dpkg -s libk4abt1.0-dev > /dev/null; then
	curl -sSL https://packages.microsoft.com/ubuntu/18.04/prod/pool/main/libk/libk4abt1.0-dev/libk4abt1.0-dev_1.0.0_amd64.deb > /tmp/libk4abt1.0-dev_1.0.0_amd64.deb
	sudo dpkg -i /tmp/libk4abt1.0-dev_1.0.0_amd64.deb
fi
if ! dpkg -s k4a-tools > /dev/null; then
	curl -sSL https://packages.microsoft.com/ubuntu/18.04/prod/pool/main/k/k4a-tools/k4a-tools_1.3.0_amd64.deb > /tmp/k4a-tools_1.3.0_amd64.deb
	sudo dpkg -i /tmp/k4a-tools_1.3.0_amd64.deb
fi
```

_This installation script was original provided [here](https://github.com/microsoft/Azure-Kinect-Sensor-SDK/issues/1790)._

The last step is to add udev rules for the Azure Kinect. This way you'll be able to run the Kinect without root privileges.

```
bash << EOF
echo '\
# Bus 002 Device 116: ID 045e:097a Microsoft Corp.  - Generic Superspeed USB Hub
# Bus 001 Device 015: ID 045e:097b Microsoft Corp.  - Generic USB Hub
# Bus 002 Device 118: ID 045e:097c Microsoft Corp.  - Azure Kinect Depth Camera
# Bus 002 Device 117: ID 045e:097d Microsoft Corp.  - Azure Kinect 4K Camera
# Bus 001 Device 016: ID 045e:097e Microsoft Corp.  - Azure Kinect Microphone Array

BUS!="usb", ACTION!="add", SUBSYSTEM!=="usb_device", GOTO="k4a_logic_rules_end"

ATTRS{idVendor}=="045e", ATTRS{idProduct}=="097a", MODE="0666", GROUP="plugdev"
ATTRS{idVendor}=="045e", ATTRS{idProduct}=="097b", MODE="0666", GROUP="plugdev"
ATTRS{idVendor}=="045e", ATTRS{idProduct}=="097c", MODE="0666", GROUP="plugdev"
ATTRS{idVendor}=="045e", ATTRS{idProduct}=="097d", MODE="0666", GROUP="plugdev"
ATTRS{idVendor}=="045e", ATTRS{idProduct}=="097e", MODE="0666", GROUP="plugdev"

LABEL="k4a_logic_rules_end"' \
| sudo tee /etc/udev/rules.d/99-k4a.rules
EOF
sudo udevadm control --reload-rules && sudo udevadm trigger
```

The udev rules for the Kinect are now added to the system (`/etc/udev/rules.d/99-k4a.rules`).

### Installation of OAK camera packages

#todo

### Build OpenCV4 (4.3.0) from source:

The command `sudo apt install libopencv-dev` in the prerequisites section should have installed OpenCV and the contrib modules already, also `xfeatures2d` should be available within this installation.
However it might be that it is not installed, lets check if you can compile a simple file with `xfeatures2d`.

```
g++ $cwd/M3T/test/xfeatures2d_test.cpp -I /usr/local/include/opencv4 -L/usr/local/lib -lopencv_core -lopencv_xfeatures2d -o xfeatures2d_test && rm xfeatures2d_test
```

If this command fails with a `xfeatures2d` compilation error you can install OpenCV from source as follows.
If the compilation and linking run without any error, feel free to skip this section.

```
sudo apt-get install libopenexr-dev
```

```
cvVersion=4.3.0
git clone https://github.com/opencv/opencv.git && git clone https://github.com/opencv/opencv_contrib.git
cd $cwd/opencv && git checkout $cvVersion && cd $cwd
cd $cwd/opencv_contrib && git checkout $cvVersion && cd $cwd
cd $cwd/opencv && mkdir build && cd build
cmake -DOPENCV_EXTRA_MODULES_PATH=$cwd/opencv_contrib/modules -DCMAKE_BUILD_TYPE=Release ..
make -j6
sudo make install
cd $cwd
```

### Build googletest from source:

```
git clone https://github.com/google/googletest.git -b v1.14.0
cd $cwd/googletest && mkdir build && cd build
cmake ..
make
sudo make install
cd $cwd
```

### Build M3T:

```
cd $cwd/M3T && mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
sudo make install
cd $cwd
```

**If any of the installation steps fails, please open a descriptive issue**

## Test the installation

### Build and run evaluation example

Example for **RBOT dataset evaluation**.

1. Download the RBOT dataset from [here](https://www.mi.hs-rm.de/~schwan/research/RBOT/) and unzip.
2. You need to create two folders, one for external files or generated files and one for the results, e.g. `./RBOT_evaluation/external/` and `./RBOT_evaluation/result/`
3. Open `$cwd/M3T/examples/evaluate_rbot_dataset.cpp` and edit the file paths according to your setup
   e.g.

   ```
   std::filesystem::path dataset_directory{"/home/full/path/to/RBOT_dataset/"};
   std::filesystem::path external_directory{"/home/full/path/to/RBOT_evaluation/external/"};
   std::filesystem::path result_directory{"/home/full/path/to/RBOT_evaluation/result/"};
   ```

4. Also set `evaluator.set_visualize_all_results(false);` to `evaluator.set_visualize_all_results(true);` to visualize the results. (optional)

5. Set the current working directory variable

   ```
   cwd=$(pwd)
   ```

6. Link evaluate_rbot_dataset.cpp and rbot_evaluator.cpp

   ```
   g++ -fopenmp -lGLEW -lglfw -I /usr/include/eigen3 -I /usr/local/include/opencv4 -I /usr/local/include/m3t -I $cwd/M3T/examples/ -c $cwd/M3T/examples/evaluate_rbot_dataset.cpp -o $cwd/M3T/examples/evaluate_rbot_dataset.o

   g++ -fopenmp -lGLEW -lglfw -I /usr/include/eigen3 -I /usr/local/include/opencv4 -c $cwd/M3T/examples/rbot_evaluator.cpp -o $cwd/M3T/examples/rbot_evaluator.o
   ```

7. Build

   ```
   g++ -fopenmp -lGLEW -lglfw $cwd/M3T/examples/evaluate_rbot_dataset.o $cwd/M3T/examples/rbot_evaluator.o -L /usr/local/lib -lm3t -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs -lopencv_calib3d -lopencv_features2d -lopencv_xfeatures2d -lGLEW -lglfw -lGL -o $cwd/M3T/examples/evaluate_rbot_dataset
   ```

8. To run it:

   ```
   $cwd/M3T/examples/evaluate_rbot_dataset
   ```

   If you have set the flag for visualizing the results (step 4.) then you should see the evaluation running in a viewer. \

## Usage example

#todo

## Tracking custom objects

#todo

## Attributions

### Original work / library

[**M3T**](https://github.com/DLR-RM/3DObjectTracking/tree/master/M3T)
Stoiber M (2023) Closing the Loop: 3D Object Tracking for Advanced Robotic Manipulation. Dissertation submitted to the Technical University of Munich

### References

Preceding work, as well es more details about the original work ([3DObjectTracking](https://github.com/DLR-RM/3DObjectTracking/tree/master), Manuel Stoiber) is listed in the following:

- **Closing the Loop: 3D Object Tracking for Advanced Robotic Manipulation**
  Manuel Stoiber
  Dissertation submitted to the Technical University of Munich

- **A Multi-body Tracking Framework - From Rigid Objects to Kinematic Structures**
  Manuel Stoiber, Martin Sundermeyer, Wout Boerdijk, and Rudolph Triebel
  Submitted to IEEE Transactions on Pattern Analysis and Machine Intelligence: [paper](https://arxiv.org/abs/2208.01502)

- **Fusing Visual Appearance and Geometry for Multi-Modality 6DoF Object Tracking**
  Manuel Stoiber, Mariam Elsayed, Anne E. Reichert, Florian Steidle, Dongheui Lee, and Rudolph Triebel
  Submitted to IEEE/RSJ International Conference on Intelligent Robots 2023: [paper](https://arxiv.org/abs/2302.11458)

- **Iterative Corresponding Geometry: Fusing Region and Depth for Highly Efficient 3D Tracking of Textureless Objects**
  Manuel Stoiber, Martin Sundermeyer, and Rudolph Triebel
  IEEE/CVF Conference on Computer Vision and Pattern Recognition 2022: [paper](https://arxiv.org/abs/2203.05334)

- **SRT3D: A Sparse Region-Based 3D Object Tracking Approach for the Real World**
  Manuel Stoiber, Martin Pfanne, Klaus H. Strobl, Rudolph Triebel, and Alin Albu-Schäffer
  International Journal of Computer Vision: [paper](https://arxiv.org/abs/2110.12715)

- **A Sparse Gaussian Approach to Region-Based 6DoF Object Tracking**
  Manuel Stoiber, Martin Pfanne, Klaus H. Strobl, Rudolph Triebel, and Alin Albu-Schäffer
  [Best Paper] Asian Conference on Computer Vision 2020: [paper](https://openaccess.thecvf.com/content/ACCV2020/papers/Stoiber_A_Sparse_Gaussian_Approach_to_Region-Based_6DoF_Object_Tracking_ACCV_2020_paper.pdf), [supplementary](https://openaccess.thecvf.com/content/ACCV2020/supplemental/Stoiber_A_Sparse_Gaussian_ACCV_2020_supplemental.zip)
