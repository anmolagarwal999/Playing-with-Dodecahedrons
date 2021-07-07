#include "main.h"
#include "timer.h"
#include "ball.h"
#include "stb_image.h"

using namespace std;

GLMatrices Matrices;
GLuint programID;
GLFWwindow *window;

bool load_texture = false;
/**************************
* Customizable functions *
**************************/

// const int choice = 2;

Geometric_Obj geo_obj;

int choice_id;
int master_texture_id = -1;

//-------------CONFIGURE-----------------------------//
bool stop_spinning_on_object_movement = true;
bool stop_spinning_on_camera_movement = false;

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
//-------------CONFIGURE-END-----------------------------//

/////////////////////////////////////////////////////////////////
// camera
const glm::vec3 camera_initial_Pos = glm::vec3(0.0f, 0.0f, 10.0f);
glm::vec3 cameraPos = camera_initial_Pos;
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -3.0f);
const glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 camera_finalized = glm::vec3(0.0f, 1.0f, 0.0f);

// glm::mat4 view;
/////////////////////////////////////////////////////////
//-----------------------------------------------------------------

bool should_object_rotate = false;
bool is_rotate = false;
bool is_rotate_2 = false;

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

//-------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;

//////////////////////////////////////////////////////////////////////////////

Timer t60(1.0 / 1000);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw()
{
    // clear the color and depth in the frame buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram(programID);
    if (load_texture)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, master_texture_id);
    }

    // Eye - Location of camera. Don't change unless you are sure!!
    // glm::vec3 eye(5 * cos(camera_rotation_angle * M_PI / 180.0f), 0, 5 * sin(camera_rotation_angle * M_PI / 180.0f));
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    // glm::vec3 target(0, 0, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    // glm::vec3 up(0, 1, 0);

    if (is_rotate)
    {

        if (false)
        {
            glm::vec3 dir_move = glm::normalize(glm::cross(geo_obj.position - cameraPos, cameraUp));
            float cameraSpeed = (glm::length(geo_obj.position - cameraPos) * 10 * deltaTime) / 3;
            cameraPos += dir_move * cameraSpeed;
            camera_finalized = geo_obj.position;
            // std::cout << "dir tangent is ";
            // print_vec3(dir_move);
            // part;
        }

        glm::vec3 obj_to_cam = cameraPos - geo_obj.position;
        // glm::vec3 new_obj_to_cam = glm::rotate((float)(5.0f * M_PI / 180.0f), cameraUp);
        // cameraPos += new_obj_to_cam + geo_obj.position;
        // camera_finalized = geo_obj.position;

        glm::mat4 model = glm::mat4(1.0f);
        //  model = glm::translate(model, cubePositions[i]);
        float angle = 2;
        std::cout << "obj to can before is  is ";
        print_vec3(obj_to_cam);
        model = glm::rotate(model, glm::radians(angle), cameraUp);
        glm::vec3 new_obj_to_cam = model * glm::vec4(obj_to_cam, 1.0);
        cameraPos = new_obj_to_cam + geo_obj.position;
        camera_finalized = geo_obj.position;
        std::cout << "new obj to can before is  is ";
        print_vec3(new_obj_to_cam);

        part;
    }
    // Compute Camera matrix (view)
    // Matrices.view = glm::lookAt(eye, target, up); // Rotating Camera for 3D
    Matrices.view = glm::lookAt(cameraPos, camera_finalized, cameraUp);

    // Don't change unless you are sure!!
    // Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    Matrices.projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP; // MVP = Projection * View * Model

    // Scene render
    geo_obj.draw(VP);
}

void tick_input(GLFWwindow *window)
{
    int left = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    if (left)
    {
        // Do something
    }
}

void tick_elements()
{
    if (should_object_rotate)
    {
        geo_obj.tick();
    }
    //  camera_rotation_angle += 1;
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height)
{
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    geo_obj = Geometric_Obj(0, 0, COLOR_RED, choice_id);

    // Create and compile our GLSL program from the shaders

    if (load_texture == false)
    {
        programID = LoadShaders("../source/shaders/shader.vert", "../source/shaders/shader.frag");
    }
    else
    {
        programID = LoadShaders("../source/shaders/shader.vert", "../source/shaders/shader_texture.frag");
        // load and create a texture
        // -------------------------
        unsigned int texture1, texture2;
        // texture 1
        // ---------
        GLCall(glGenTextures(1, &texture1));
        GLCall(glBindTexture(GL_TEXTURE_2D, texture1));
        master_texture_id = texture1;
        // set the texture wrapping parameters
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
        // set texture filtering parameters
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        // load image, create texture and generate mipmaps
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
        std::string use_path = "../source/wall.png";
        std::cout << "GOinf to load tetxure 1 " << std::endl;
        unsigned char *data = stbi_load(use_path.c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
            GLCall(glGenerateMipmap(GL_TEXTURE_2D));
        }
        else
        {
            std::cout << "1) Failed to load texture" << std::endl;
        }

        GLCall(stbi_image_free(data));
        cout << "1) prog id is " << programID << endl;

        glUniform1i(glGetUniformLocation(programID, "texture1"), 0);
    }
    // Get a handle for our "MVP" uniform
    cout << "2) prog id is " << programID << endl;
    ;
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////////////////////////

    reshapeWindow(window, width, height);

    // Background color of the scene
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    GLCall(glClearDepth(1.0f));

    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glDepthFunc(GL_LEQUAL));

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}

int main(int argc, char **argv)
{
    srand(time(0));
    // int width = 600;
    // int height = 600;

    //-----------------------------------------------
    cout << "Currently, if the object or camera moves, then the camera, if rotating around the objec, stops rotating.\n\
    This can be changed in the config variables at the top of main.cpp\n"
         << endl;
    cout << "Enter corresponding choice to see the SOLID ON THE SCREEN" << endl;
    cout << "Enter 1 for Elonagated Square Dipyramid" << endl;
    cout << "Enter 2 for Hexagonal dipyramid" << endl;
    cout << "Enter 3 for Undecagonal pyramid" << endl;
    cin >> choice_id;

    cout << "\nDo you want texture or not ?" << endl;
    cout << "Enter 1 for COLOR Interpolation shade on figures" << endl;
    cout << "Enter 2 for wall texture on solids\n"
         << endl;
    int ch;
    cin >> ch;
    if (ch == 1)
    {
        load_texture = false;
    }
    else
    {
        load_texture = true;
    }
    // load_texture=false;

    //----------------------------------------------------

    window = initGLFW(SCR_WIDTH, SCR_HEIGHT);

    initGL(window, SCR_WIDTH, SCR_HEIGHT);
    camera_finalized = geo_obj.position;

    /* Draw in loop */
    while (!glfwWindowShouldClose(window))
    {
        // Process timers

        if (t60.processTick())
        {
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            //TAKING INPUT, NEWLY ADDED
            processInput(window);

            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            GLCall(glfwSwapBuffers(window));

            tick_elements();
            tick_input(window);
        }

        // Poll for Keyboard and mouse events
        GLCall(glfwPollEvents());
    }

    quit(window);
}

void reset_screen(int new_height, int new_width)
{
    // float top = screen_center_y + 4 / screen_zoom;
    // float bottom = screen_center_y - 4 / screen_zoom;
    // float left = screen_center_x - 4 / screen_zoom;
    // float right = screen_center_x + 4 / screen_zoom;
    // Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
    // cout << "NEW HIGHT IS " << new_height << endl;
    // cout << "NEW WIDTH IS " << new_width << endl;
    // cout << "OLD HIGHT IS " << SCR_HEIGHT << endl;
    // cout << "OLD WIDTH IS " << SCR_WIDTH << endl;

    ///////////////////////////////////////
    SCR_HEIGHT = new_height;
    SCR_WIDTH = new_width;

    //due to opengl anomaly on my system
    swap(SCR_HEIGHT, SCR_WIDTH);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 5.5 * deltaTime;
    cameraFront = camera_finalized - cameraPos;

    glm::vec3 global_up = cameraUp;
    glm::vec3 actual_right = glm::normalize(glm::cross(cameraFront, global_up));
    glm::vec3 actual_up = glm::normalize(glm::cross(actual_right, cameraFront));
    // const float cameraSpeed = 0.05f; // adjust accordingly

    glm::vec3 i_cap = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 j_cap = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 k_cap = glm::vec3(0.0f, 0.0f, 1.0f);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        cameraPos += cameraSpeed * (cameraFront);
        camera_finalized += cameraSpeed * (cameraFront);
        if (stop_spinning_on_camera_movement)
        {
            is_rotate = false;
        }
        // std::cout << "Front" << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cameraPos -= cameraSpeed * (cameraFront);
        camera_finalized -= cameraSpeed * (cameraFront);
        if (stop_spinning_on_camera_movement)
        {
            is_rotate = false;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        cameraPos += cameraSpeed * (actual_right) * (-1.0f);
        camera_finalized += cameraSpeed * (actual_right) * (-1.0f);
        if (stop_spinning_on_camera_movement)
        {
            is_rotate = false;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cameraPos += cameraSpeed * (actual_right) * (+1.0f);
        camera_finalized += cameraSpeed * (actual_right) * (+1.0f);
        if (stop_spinning_on_camera_movement)
        {
            is_rotate = false;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        cameraPos += cameraSpeed * (actual_up) * (+1.0f);
        camera_finalized += cameraSpeed * (actual_up) * (+1.0f);
        if (stop_spinning_on_camera_movement)
        {
            is_rotate = false;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        cameraPos += cameraSpeed * (actual_up) * (-1.0f);
        camera_finalized += cameraSpeed * (actual_up) * (-1.0f);
        if (stop_spinning_on_camera_movement)
        {
            is_rotate = false;
        }
    }
    //------------------------------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------
    // glm::vec4 supposed_up_mid = view * glm::vec4(cameraUp, 1.0);
    //    glm::vec3 supposed_up = glm::vec3(supposed_up_mid.x, supposed_up_mid.y, supposed_up_mid.z);
    //    cameraPos += glm::normalize(supposed_up) * cameraSpeed;
    //    camera_finalized += glm::normalize(supposed_up) * cameraSpeed;
    //------------------------------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
    {
        // geo_obj.position += cameraSpeed * glm::vec3(0.0f, 0.0f, -1.0f);
        geo_obj.position += cameraSpeed * (cameraFront);
        if (stop_spinning_on_object_movement)
        {
            is_rotate = false;
        }
        // std::cout << "Front" << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        // geo_obj.position += cameraSpeed * glm::vec3(0.0f, 0.0f, 1.0f);
        geo_obj.position -= cameraSpeed * (cameraFront);
        if (stop_spinning_on_object_movement)
        {
            is_rotate = false;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
    {
        // geo_obj.position += cameraSpeed * glm::vec3(-1.0f, 0.0f, 0.0f);
        geo_obj.position += cameraSpeed * (actual_right) * (-1.0f);
        if (stop_spinning_on_object_movement)
        {
            is_rotate = false;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        // geo_obj.position += cameraSpeed * glm::vec3(1.0f, 0.0f, 0.0f);
        geo_obj.position += cameraSpeed * (actual_right) * (+1.0f);
        if (stop_spinning_on_object_movement)
        {
            is_rotate = false;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    {
        // geo_obj.position += cameraSpeed * glm::vec3(0.0f, 1.0f, 0.0f);
        geo_obj.position += cameraSpeed * (actual_up) * (+1.0f);
        if (stop_spinning_on_object_movement)
        {
            is_rotate = false;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        // geo_obj.position += cameraSpeed * glm::vec3(0.0f, -1.0f, 0.0f);
        geo_obj.position += cameraSpeed * (actual_up) * (-1.0f);
        if (stop_spinning_on_object_movement)
        {
            is_rotate = false;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
    {
        //make camera face pbject
        glm::vec3 new_focus = geo_obj.position;
        // glm::vec3 new_right =  glm::normalize(glm::cross(cameraFront, new_focus)) * (-1.0f);
        // glm::vec3 new_up =  glm::normalize(glm::cross(new_right, new_focus)) ;
        // glm::vec3 new_up =  glm::normalize(glm::cross(cameraFront, new_focus)) * (1.0f);

        // cameraFront = glm::vec3(0.0f, 1.0f, 0.0f);
        // cameraUp = glm::vec3(0.0f, 0.0f, -1.0f);

        std::cout << "new focus is ";
        print_vec3(new_focus);
        // if(new_focus==cameraFront)
        // {
        //    new_up=cameraUp;
        // }
        camera_finalized = new_focus;

        //

        // std::cout << "new up is ";
        // print_vec3(new_up);
        // cameraUp = new_up;
        part;
    }

    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
    {
        is_rotate = true;
        is_rotate_2 = false;
    }
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
    {
        is_rotate = false;
    }

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        should_object_rotate = true;
    }

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        should_object_rotate = false;
    }

    vector<int> displacement_keyboard_keys{
        GLFW_KEY_1,
        GLFW_KEY_2,
        GLFW_KEY_3,
    };

    for (int i = 0; i < 3; i++)
    {
        if (glfwGetKey(window, displacement_keyboard_keys[i]) == GLFW_PRESS)
        {
            //restoring to a fixed orientation
            geo_obj.position = glm::vec3(0, 0, 0);
            geo_obj.rotation = 0;
            is_rotate = false;
            cameraPos = geo_obj.position + geo_obj.displacement_positions[i];
            camera_finalized = geo_obj.position;
            cout << "Wanted new camera pos is ";
            print_vec3(geo_obj.position + geo_obj.displacement_positions[i]);
        }
    }

    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
    {
        cameraPos = camera_initial_Pos;
        camera_finalized = geo_obj.position;
    }
}

void print_vec3(glm::vec3 a)
{
    std::cout << a[0] << " : " << a[1] << " : " << a[2] << std::endl;
}
