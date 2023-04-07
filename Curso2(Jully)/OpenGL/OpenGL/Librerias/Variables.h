
const unsigned int w = 500;
const unsigned int h = 500;
const bool poly = false;

//Buffer
unsigned int VBO, VAO, EBO;

//Texture
unsigned int texture[1];
int limite;

string nombre[] = {
    "Imagenes/caja.jpg"
};

int tipo[] = {
    0
};

int expan[] = {
    0
};

//Camera
float cameraSpeed = 0.0f, deltaTime = 0.0f, lastFrame = 0.0f;
vec3 cameraPos = vec3(0.0f, 0.0f, 3.0f);
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);

Camera camera(vec3(0.0f, 0.0f, 3.0f));
float lastX = w / 2.0f;
float lastY = h / 2.0f;
bool firstMouse = true;
