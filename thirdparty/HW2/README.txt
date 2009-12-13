== Compilation ==
For this homework, I will try to use OO-design where it makes sense. There are a few functions that do not fit into OO-notion so I left them in the Misc module. All my functions and classes are under the creatively-named dc namespace.

For this homework, I use OpenCV 2.0.0a for Windows and Linux. This means that you will need to install and compile OpenCV 2.0.0 on your own. The provided CMakeLists.txt file should allow you to configure this project to link to your compiled version of OpenCV 2.

Please note that it is NOT possible to skip compiling of OpenCV on Windows unless you are using MinGW since the dll shipped with Windows version of OpenCV is compiled for MinGW.

It is also NOT possible to use OpenCV 1.1a (and older) with this code since I make extensive use of the new C++ binding for OpenCV.

You will need to use CMake to create the project solution/Makefile necessary to compile the code.

== Running ==
All of my project executable have clear instructions on how to run them. Please run the executable without any parameter for a usage instruction. I include them here for your convenience.

I recommend that you set the working directory to the folder '''images''' that is included in this zip package. That folder will have the masks used in RetinalImage project.

=== HistogramShaping ===
usage: ./HistogramShaping input_image.tif triangle|gaussian [gaussian_std]

You only need to specify standard deviation if you choose to use a target Gaussian histogram.

=== ToeplitzStripe ===
usage: ./ToeplitzStripe

=== LogPolar ===
usage: ./LogPolar Boats.tif 64

You can choose a different size for theta using the 2nd parameter.

=== RetinalImage ===
Similar to LogPolar