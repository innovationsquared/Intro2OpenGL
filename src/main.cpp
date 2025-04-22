#include "config.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
void processInput (GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
/*
* The entry point into the OpenGL experiment.
* The workflow for a triangle:
* vertex data[] --> vertex shader (makes points and assembles shape) -->
* geometry shader (assembles shapes out of triangles) --> fragment shader (coloring)                                                             
*/
int main()
{
 //create window, make sure glfw version >= 3.3
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  //create 800x600 window
  GLFWwindow* window = glfwCreateWindow(800,600,"Hello, Window!", NULL, NULL);
  if (window == NULL)
  {
    printf("Failed to open the GLFW window.");
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  //create view, and callback function which handles window resizing 
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); 
  //load up glad
  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
  {
    printf("Failed to initialize GLAD");
    return -1;
  }
  Shader ourShader("../src/shaders/shader.vs", "../src/shaders/shader.fs");
  /*create verticies for a simple triangle
  *               |(0,1)
  *               |
  *               |
  * (-1,0)-------------------(1,0)
  *               |
  *               |
  *               |(0,-1)
  * 
  *                    ^ (0,0.5)
  *                   / \
  *                  /   \ 
  *                 /     \ 
  *    (-0.5,-0.5) <-------> (0.5,-0.5)
  */
  //this will be passed into the vertex shader to form the triangle
  // float vertices[] = {
  //   //x     y      z
  //   -0.5f, -0.5f, 0.0f, //bttm left
  //    0.5f, -0.5f, 0.0f, //bttm right
  //    0.0f,  0.5f, 0.0f //top
  // };
  

  //now lets make a square (naively)
  // float vertices2[] = {
  //   //x     y      z
  //   //1st triangle
  //   -0.5f, 0.5f, 0.0f, //top right
  //    0.5f,-0.5f, 0.0f, //bottom right
  //   -0.5f, 0.5f, 0.0f, //top left
  //   //2nd triangle
  //    0.5f,-0.5f, 0.0f, //bttm right
  //   -0.5f,-0.5f, 0.0f, //bttm left 
  //   -0.5f, 0.5f, 0.0f //top left
  // };
  //to reduce overhead: Element Buffer Object
  // float vertices[] = {
  //    0.5f,  0.5f, 0.0f, // top right
  //    0.5f, -0.5f, 0.0f, // bottom right
  //   -0.5f, -0.5f, 0.0f, // bottom left
  //   -0.5f,  0.5f, 0.0f  // top left
  // };
  unsigned int indices[] = {
    0, 1, 3, // 1st triangle
    1, 2, 3 //  2nd triangle
  };



  //exercise 2
  // float vertices[] = {
  //   //x     y      z
  // //   1st triangle
  //   0.0f, -0.5f, 0.0f, //bttm left
  //    1.0f, -0.5f, 0.0f, //bttm right
  //    0.5f,  0.5f, 0.0f, //top
  //    // 2nd
  //    -1.0f, -0.5f, 0.0f, //bttm left
  //    0.0f, -0.5f, 0.0f, //bttm right
  //    -0.5f,  0.5f, 0.0f //top
  // };
  //with colors and texture coords
  float vertices[] = {
    //x     y     z     R     G     B    texture coords
    0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, //top right
    0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, //bttm right
   -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, //bttm left
   -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, //top left

  };
  //texture coords
  float texCoords[] = {
    0.0f, 0.0f, // bttm left corner
    1.0f, 0.0f, // bttm right corner
    0.5f, 1.0f //top center
  };
  //float borderColor[] = {1.0f, 1.0f, 0.0f, 1.0f};
  
  //Vertex Array Objects, Vertex Buffer Objects, and Element Buffer Objects
  //Need to create mem on GPU to store vertex data via Vertex Buffer Objects (VBOs)
  //create VBO and VAO
  unsigned int VBO, VAO, EBO;
  glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &EBO);
  //Bind it
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // any calls from now on effect our VBO
  // 0. copy verticies into buffer mem
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  //vertex buffer data (each x y z 32 bit (4byte))
  //| Vertex 1   | Vertex 2    | Vertex 3     |
  //| X | Y | Z  | X | Y | Z   |  X | Y | Z   |
//B:0   4   8   12   16  20   24   28   32   36
//1. tell opengl how to interpret vertex data
//               args: loc = 0, 3d, data type, normalized, stride, offset
  //create our EBO from above
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  //position attribute
  glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  //color
  glVertexAttribPointer(1,3,GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  //texture 
  glVertexAttribPointer(2,2,GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  //create texture
  unsigned int texture1, texture2;
  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);
  //texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  int width, height, nrChannels;
  unsigned char *data = stbi_load("../resources/textures/dirt.jpg", &width, &height, &nrChannels, 0);
  if (data)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    std::cout << "Texture load failed" << std::endl;
  }
  stbi_image_free(data);
  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);
  //texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  data = stbi_load("../resources/textures/steve.jpg", &width, &height, &nrChannels, 0);
  if (data)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    std::cout << "Texture2 load failed" << std::endl;
  }


  ourShader.use();
  glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
  ourShader.setInt("texture2", 1);

  //rendering loop!
  while (!glfwWindowShouldClose(window))
  {
    //input
    processInput(window);
    //rendering
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // float timeValue = glfwGetTime();
    // float greenValue = sin(timeValue) / 2.0f + 0.5f;
    // int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
    // glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
    //ourShader.setFloat("aPos", 1.0f);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glBindVertexArray(VAO);
    //one triangle
    //glDrawArrays(GL_TRIANGLES, 0,3);
    //square
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    //polygon mode (apply to front and back of all triangles, draw as lines)
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //to turn off polygon:
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //call events, swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  //delete resources when done
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  //glDeleteBuffers(1, &EBO);
  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0,0,width,height);
}

void processInput (GLFWwindow *window)
{
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, true);
  }
}

