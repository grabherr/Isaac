# Isaac
Isaac
(c) Manfred G. Grabherr, Licensed under the GPL

Isaac is a 3D visualization framework for complex systems. Its goal is to be easy to use, while giving you access to very powerful abilities provided by state-of-the-art 3d rendering engines. Isaac uses the Irrlicht engine (http://irrlicht.sourceforge.net/) by defualt.


1. Installation and build

Isaac currently only supports the Linux operating system. It does run on Mac OS, but you will have to manually install the irrlicht engine.

To set up the system, get the latest version of the source code from github via

git clone https://github.com/grabherr/Isaac.git

Next, install the following development libraries:

libx11-dev mesa-common-dev freeglut3-dev libxcursor-dev subversion libasound2-dev

e.g. via

sudo apt-get install libx11-dev mesa-common-dev freeglut3-dev libxcursor-dev subversion libasound2-dev

on Ubuntu or Mint. Type 'make' to build Isaac and verify that there are no compilation or linking errors.

Third, install the irrlicht engine into a directory 'irrlicht-code' within the Isaac directory. To get the latest version of irrlicht and compile it, run

./getIrrlicht

which will download and compile the Irrlicht engine, followed by

./compileIrrlicht

Which will compile the irrlicht client. Note that you will have to run ./compileIrrlicht any time you modify the Irrlicht client, and that this script will also re-compile the Isaac code.

To test your installation, run:

./TestGameEngine -i data/empty.cfg > tmp

which brings up a fairly empty map in full screen mode. To navigate, use the arrow keys and mouse. To exit irrlicht, press 'z'. Note that you will have to manually stop or kill the executable TestGameEngine as well.


2. Using Isaac: the Makefile system

To add a a new executable, simply create a file that has the extension .cc and contains the exact line

int main(int argc,char** argv)

then type 'make'. It will be automatically recognized. To add new code, add files with the extensions .cc and .h, making sure that they have the same name and are located in the same directory. Including the .h file will cause the .cc file to be compiled and linked in.
