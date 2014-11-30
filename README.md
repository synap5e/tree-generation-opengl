comp-308-project
================

My project for COMP 308 - Introduction to Computer Graphics at Victoria University of Wellington.

This project aims to create visually pleasing procedurally generated trees using OpenGL 4. 

Space colonization is used for branch growth along with raytracing for adaption to shadows cast by other trees.

![Alt text](/../screenshots/Picture1.png)

## Running

Grab the latest release for your system from [releases](https://github.com/synap5e/comp-308-project/releases) and run the executable.

Command line arguments are provided in the relese information.

Alternately compile as described in the following section. 

To run the following files must be present in the working directory:
- tree.json
- bark-normal.png
- shaders/*

## Compiling

Clone the repository making sure to also fetch the submodules (e.g. `git clone --recursive`).

Make sure all the required files are in the working directory when attempting to run of debug the executable.

### cmake + make
Simply use cmake to generate a makefile with the relevant options (e.g. `-DCMAKE_BUILD_TYPE=Release` if you want a release build).
Running cmake from a subdirectory is reccomended to keep the source directory clean(ish).

Compile the makefile using `make`

### cmake + Visual Studio
Use cmake to generate the solution then load it in Visual Studio.

Set 'tree' to the default project and set the debug working directory to `..` if you plan to run the program through Visual Studio for debugging.

## License

The code in the root of the reposity is liscened under MIT to Simon Pinfold (me). A table of the liscenses of the required libraries follows.


|    Library     |    Website                       |    Licence               |
|----------------|----------------------------------|--------------------------|
|    GLFW        |    [www.glfw.org](www.glfw.org)                  |    zlib/libpng           |
|    SOIL        |    [www.lonesock.net/soil.html](www.lonesock.net/soil.html)    |    Public   domain       |
|    glm         |    [glm.g-truc.net](glm.g-truc.net)                |    MIT                   |
|    GLEW        |    [glew.sourceforge.net](glew.sourceforge.net)         |    Modified BSD & MIT    |
|    muparser    |    [muparser.beltoforion.de](muparser.beltoforion.de)       |    MIT                   |
|    PicoJSON    |    [github.com/kazuho/picojson](github.com/kazuho/picojson)    |    2-clause BSD          |



