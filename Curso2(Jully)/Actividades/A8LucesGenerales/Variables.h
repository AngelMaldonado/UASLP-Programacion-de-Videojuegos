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
};

int expan[] = {
    2,
    2,
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
int totalP = 4;
vec3 lightPos(0.5f, 0.2f, 1.0f);
vec3 lightDirect(-0.2f, -1.0f, -0.3f);

unsigned int diffuseMap;
unsigned int specularMap;
float cutOffValue = 12.5f;
float outerCutOffValue = 17.5f;

float constantV = 1.0f;
float linearV = 0.09f;
float quadraticV = 0.032f;

vec3 pointLightPositions[] = {
    vec3(0.7f, 0.2f, 2.0f),
    vec3(2.3f, -3.3f, -4.0f),
    vec3(-4.0f, 2.0f, -12.0f),
    vec3(0.0f, 0.0f, -3.0f)
};
