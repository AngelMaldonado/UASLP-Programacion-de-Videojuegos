
float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 
    0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 
    - 0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 
    - 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 
    - 0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 
    - 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 
    - 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 
    - 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 
    0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
    - 0.5f,  0.5f,  0.5f,  0.0f, 0.0f
};


unsigned int indices[]{
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4, 
    8, 9, 10, 10, 4, 8,
    11, 2, 12, 12, 13, 11,
    10, 14, 5, 5, 4, 10,
    3, 2, 11, 11, 15, 3
};

vec3 cubePositions[] = {
        vec3(0.0f,  0.0f,  0.0f),
        vec3(2.0f,  5.0f, -15.0f),
        vec3(-1.5f, -2.2f, -2.5f),
        vec3(-3.8f, -2.0f, -12.3f),
        vec3(2.4f, -0.4f, -3.5f),
        vec3(-1.7f,  3.0f, -7.5f),
        vec3(1.3f, -2.0f, -2.5f),
        vec3(1.5f,  2.0f, -2.5f),
        vec3(1.5f,  0.2f, -1.5f),
        vec3(-1.3f,  1.0f, -1.5f)
};