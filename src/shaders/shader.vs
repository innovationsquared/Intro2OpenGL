//This is GLSL, shader lingo similar to C

//make sure version is >= 3.3
#version 330 core

//all input starts with 'in'
// our input has 3d coord, so vec3 (3d vec) named aPos
//set location of input var
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;

void main()
{
  //set output of vertex shader, whatever we set gl_position to will be output of vertex shader
  gl_Position = vec4(aPos, 1.0);
  ourColor = aColor; 
}
