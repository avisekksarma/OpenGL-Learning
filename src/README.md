### Opengl hello triangle
##### Short writeup for confusing parts only rather than like note

### Note: The comments in code is done by me, where some imp tips might be missed , so atleast quickly see the code+comment of original learnopengl too from learnopengl.com site.


1. Opengl works in 3d but our screen is 2d of pixels, so a large part of OpenGL's work is about transforming all 3D coordinates to 2D pixels that fit on your screen. The process of transforming 3D coordinates to 2D pixels is managed by the graphics pipeline of OpenGL

2. Graphics pipeline - two parts: 
    a. first transforms 3d to 2d
    b. in second part transforms 2d coordinates to actual rendered pixels.

3. Input to graphics pipeline here is  called vertex data, as it is collection of vertices, where one vertex is collection of data per 3d coordinate, where data is collection of vertex attributes ,where say in our triangle case say vertex attributes are 3d coordinate and color only