const unsigned int w = 500;
const unsigned int h = 500;
const bool poly = false;

//Buffer
unsigned int VBO, VAO, EBO;
unsigned int lightVAO;

//Texture
string nombre[] = {
    "Imagenes/caja.png",
    "Imagenes/caja_especular.png",
    "Imagenes/caja_emisiva.png"
};

int expan[] = {
    2,
    2,
    0
};

//Camera
mat4 projection, view, model;

float cameraSpeed = 0.0f, deltaTime = 0.0f, lastFrame = 0.0f;
vec3 cameraPos = vec3(0.0f, 0.0f, 3.0f);
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);

Camera camera(vec3(0.0f, 0.0f, 3.0f));
float lastX = w / 2.0f;
float lastY = h / 2.0f;
bool firstMouse = true;

//Luz
vec3 lightPos(1.2f, 1.0f, 2.0f);

unsigned int diffuseMap;
unsigned int specularMap;
unsigned int emisionMap;
