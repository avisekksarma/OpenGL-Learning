#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
out vec3 ourColor;
uniform float height;
uniform float offset;
out vec3 position;
void main()
{
    vec3 pos = aPos;
    // if(pos.y < 0){
    //     pos.y = pos.y + height;
    // }else{
      //   pos.y = pos.y - height;
    // }

    // if else is bad in shader unless really important to do.
   // gl_Position = vec4(pos.x , pos.y, pos.z , 1.0);
   gl_Position = vec4(aPos.x + offset, aPos.y, aPos.z , 1.0);
   ourColor = aColor;
   position = aPos;
}