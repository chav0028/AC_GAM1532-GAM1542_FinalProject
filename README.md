# AC_GAM1532-GAM1542_FinalProject
Final Project for both GAM1532-Game Programming IV and GAM1542-Game and Graphics Computations III courses.

For this project we had to design a game engine, and its graphics (3D rendering, etc.) components, and use it to make a game. The game I made was a 2.5D side scroller, where you control a car than can invert/revert its gravity. You have to achieve the highest score possible by reaching the end of the level, collecting pick up, and avoiding enemies.

This project was made using:
- C++ - Game and Engine logic
- GLSL (OpenGL ES 2.0) - Rendering
- BOx2D - Collisions
- SDL - Audio playback

This project included a number of different features including:
- Read/Load 3D OBJ models, including their vertices positions, UVs, and normals.
- 3D Rendering code. Use Vertex Buffer Object and Index Buffer Objects in OpenGL,  alongside fragment and vertex shaders, to render 3D Models.
- Integration with [Box2D](https://box2d.org/) (physics simulations, collisions, sensors, joints, collision filtering, etc.)
- Audio playback using [SDL](https://www.libsdl.org/) library
- Ability to read JSON files and load the game scene and create objects according to the data read.
- Scene management system, used to change scenes, draw and update the scene, and apply post process effect shaders.
- Tweening functions to add game polish.
- Basic menu UI system
- Basic parenting system (position), that allows game objects to establish parent/children relationships.
- Lighting system with diffuse and specular lighting. It also has support for multiple attenuation types (constant, linear, and quadratic) and multiple light types (point, spotlight, and directional). Objects can be lit by multiple lights at once, and have different material properties.
- Post process support. The game scene can be rendered to a texture so that it can later be affected by post process such as blur or a black and white filter.
- Cube map implementation in OpenGL, including support for objects to reflect the cube map.
- Implementation of a particle system. This includes 2D billboard particles (always face the player) and 3D cube shaped particles. It also has different options for how the particles are launched.
