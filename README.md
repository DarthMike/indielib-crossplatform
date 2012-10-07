# Indielib cross-platform

## Overview

**Indielib** is a C++ 2.5d library for game development and fast game prototyping in a really easy way. It was originally written for DirectX 9.0c. This project is the cross platform version of this library.

###Platforms

* Windows
* Linux
* OSX
* iOS

###Renderers
* OpenGL 1.5 desktop support (3.2 later on)
* OpenGL ES 2.0 iOS  support (3.0 later on)

### Features
Simple interface and extreme easiness: really fast game prototyping using entities for each of the graphical objects and changing their attributes. All the methods are as simple as "SetPos", "SetCollisionArea", "SetFade", "SetTransparency", etc.

* Translations, rotation, mirroring and scaling of sprites.
* Alpha blending, transparency, tinting and fading of sprites.
* Render images of any size (not only power of two). You can load images as big as 10000x10000 pixels or more, and IndieLib will cut them internally in smaller blocks. Furthermore, IndieLib will automatically discard all the blocks out of the screen, also if your sprites are transformed and the camera making a zoom or rotated, only what you see is what is going to be rendered.
* SetGrid method in order to create grids for your sprites. Later you can move each grid point in order to make a lot of different effects like waves, etc.
* 2d and 3d camera concepts, completely independent of the graphical objects. Easy methods to translate, rotate or zoom you camera.
* Easily creation of several viewports. Just create as many viewports as you want using different cameras assigned to them. This is really useful for making widescreen games, etc.
* Scrolls of any size, with automatic block discarding.
* Animation system using XML animation scripts, for defining the frames and sequences.
* Advanced collision system. Create an xml file defining your collision areas per sprite using circles, rectangles or triangles. You can define as many collision groups as you want, each one with a different name. Join this file to an entity object and check collision between the collision groups of other entities. Don't worry about translating, rotating or scaling your entities, the collision groups will always fit perfectly your entity. You can also have sprite animations in which each frame has different collision groups (imagine this feature in a "Street Fighter" type game).
* Bitmap font system, for drawing graphical texts of fonts created with MudgeFont for IndieLib. Change the alignment, char spacing and line spacing easily.
* Rendering of 3d Meshes (you can mix 2d and 3d easily). You can also apply transformations to them (rotation, scaling, fading, etc). This will allow you to make games like "New Super Mario Bros" in which the characters are 3d models and the scenario is 2d.
* Light manager, for lighting your 3d models. Enable / Disable lights, change their attributes, etc.
* Timers. Start, stop, pause them easily.
* Entity class, for both 2d and 3d objects, for managing the graphical objects in a really easy way. Just create an entity, join a graphical object to it, an change it's attributes (space transformations, color transformations, etc).
* Image class, for loading / saving different types of image files and for accessing to the bitmaps directly. You can also apply filters to the images like gaussian, noise, equalize, saturation, etc.
* Direct blitting of primitives (rectangles, circles, polygons, lines, etc).
* Mouse and keyboard input. Check if any key/button has been pressed, is being pressed or has been pressed more than n milliseconds. Check combination of keys/buttons that are pressed at the same time. 

###Features ported to other renderers/platforms
* IND_Entity2d, IND_Surface, IND_Font, IND_Image
* All manipulations of entities (effects, interactions...)
* Transparent window creation
* 2d Cameras
* Collision tracking and definitions system 

###Features NOT ported initially
* 3d Entities
* Lights
* 3d-related objects (meshes etc.)

###Dependencies
This library bases on several others to achieve some of it's functionality:

* [FreeImage](http://freeimage.sourceforge.net/)
* [SDL](http://www.libsdl.org/)
* [TinyXML](http://www.grinninglizard.com/tinyxml/)
* [Unittest++](http://unittest-cpp.sourceforge.net/)
* [GLEW](http://glew.sourceforge.net/)

# Building the library
Instructions to build the library are available on the [wiki](http://www.indielib.com/wiki/index.php?title=Building_IndieLib) page.
