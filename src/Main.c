/*
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <stdlib.h>
#include <stdio.h>

Display *display;
Window root;
XVisualInfo *vi;
Colormap cmap;
XSetWindowAttributes swa;
Window win;
GLXContext glc;

void init_openGL() {
    // OpenGL-Kontext initialisieren
    glClearColor(0.0, 0.0, 0.0, 0.0); // Hintergrundfarbe auf Schwarz setzen
}

void draw_scene() {
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0); // Rot
    glVertex2f(-0.6f, -0.6f);
    glColor3f(0.0, 1.0, 0.0); // Grün
    glVertex2f( 0.6f, -0.6f);
    glColor3f(0.0, 0.0, 1.0); // Blau
    glVertex2f( 0.0f,  0.6f);
    glEnd();

    glXSwapBuffers(display, win); // Doppelpuffer verwenden, um das Bild zu zeigen
}

int main() {
    // X11-Verbindung aufbauen
    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Konnte keine Verbindung zum X-Server herstellen.\n");
        exit(1);
    }

    root = DefaultRootWindow(display);
    
    // Visual auswählen
    static int attributes[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
    vi = glXChooseVisual(display, 0, attributes);
    if (vi == NULL) {
        fprintf(stderr, "Kein passendes Visual für OpenGL gefunden.\n");
        exit(1);
    }

    // Colormap und Fensterattribute festlegen
    cmap = XCreateColormap(display, root, vi->visual, AllocNone);
    swa.colormap = cmap;
    swa.event_mask = ExposureMask | KeyPressMask;

    // X11-Fenster erstellen
    win = XCreateWindow(display, root, 0, 0, 800, 600, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
    XMapWindow(display, win);
    XStoreName(display, win, "OpenGL + X11 Beispiel");

    // OpenGL-Kontext erstellen
    glc = glXCreateContext(display, vi, NULL, GL_TRUE);
    glXMakeCurrent(display, win, glc);

    init_openGL();

    // Ereignisschleife
    XEvent event;
    while (1) {
        XNextEvent(display, &event);

        if (event.type == Expose) {
            draw_scene();
        }

        if (event.type == KeyPress)
            break; // Beenden bei Tastendruck
    }

    // Aufräumen
    glXMakeCurrent(display, None, NULL);
    glXDestroyContext(display, glc);
    XDestroyWindow(display, win);
    XCloseDisplay(display);

    return 0;
}
*/
/*
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "/home/codeleaded/System/Static/Library/Stb_Image.h"

// Fenster-Größe
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// Shader Quellcode (Vertex- und Fragment-Shader)
const char* vertexShaderSource = NULL;
const char* fragmentShaderSource = NULL;

// Vertex-Daten (Würfel)
float vertices[] = {
    // Position               // Texture-Koordinaten
    -0.5f, -0.5f, -0.5f,      0.0f, 0.0f,  // Rückseite
     0.5f, -0.5f, -0.5f,      1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,      1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,      0.0f, 1.0f,

    -0.5f, -0.5f,  0.5f,      0.0f, 0.0f,  // Vorderseite
     0.5f, -0.5f,  0.5f,      1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,      1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,      0.0f, 1.0f,

    -0.5f, -0.5f, -0.5f,      0.0f, 0.0f,  // Linke Seite
    -0.5f, -0.5f,  0.5f,      1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,      1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,      0.0f, 1.0f,

     0.5f, -0.5f, -0.5f,      0.0f, 0.0f,  // Rechte Seite
     0.5f, -0.5f,  0.5f,      1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,      1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,      0.0f, 1.0f,

    -0.5f, -0.5f, -0.5f,      0.0f, 0.0f,  // Unterseite
     0.5f, -0.5f, -0.5f,      1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,      1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,      0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,      0.0f, 0.0f,  // Oberseite
     0.5f,  0.5f, -0.5f,      1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,      1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,      0.0f, 1.0f,
};

// Indizes für die Dreiecke (6 Seiten, 2 Dreiecke pro Seite)
unsigned int indices[] = {
    0, 1, 2, 2, 3, 0,  // Rückseite
    4, 5, 6, 6, 7, 4,  // Vorderseite
    8, 9, 10, 10, 11, 8, // Linke Seite
    12, 13, 14, 14, 15, 12, // Rechte Seite
    16, 17, 18, 18, 19, 16, // Unterseite
    20, 21, 22, 22, 23, 20  // Oberseite
};

// Texturen
GLuint textureID[6]; // 6 Texturen für die 6 Seiten des Blocks

// Kamera und Matrices
glm::mat4 projection;
glm::mat4 view;
glm::mat4 model;
GLuint shaderProgram;
GLuint VAO, VBO, EBO;

// Funktion zum Laden einer Textur aus einem Sprite-Sheet
GLuint loadTextureFromSubImage(const char *path, int x_offset, int y_offset, int width, int height) {
    int width_img, height_img, nrChannels;
    unsigned char *data = stbi_load(path, &width_img, &height_img, &nrChannels, 0);
    if (data) {
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        // Textur-Parameter setzen
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Sub-Image extrahieren und Textur laden
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                     data + (y_offset * width_img + x_offset) * nrChannels);

        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
        return texture;
    } else {
        printf("Fehler beim Laden der Textur!\n");
        return 0;
    }
}

// Shader kompilieren
GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("Shader Compilation Error: %s\n", infoLog);
    }
    return shader;
}

// Shader Programm erstellen
GLuint createShaderProgram() {
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("Program Linking Error: %s\n", infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return shaderProgram;
}

void processInput(GLFWwindow *window) {
    // Kamera-Bewegung (W, A, S, D)
    float cameraSpeed = 2.5f * 0.1f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

int main() {
    // GLFW Initialisieren
    if (!glfwInit()) {
        printf("GLFW Initialisierung fehlgeschlagen!\n");
        return -1;
    }

    // Fenster erstellen
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "3D Camera & Block", NULL, NULL);
    if (!window) {
        printf("Fenster konnte nicht erstellt werden!\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // GLEW initialisieren
    if (glewInit() != GLEW_OK) {
        printf("GLEW konnte nicht initialisiert werden!\n");
        return -1;
    }

    // Shader programm erstellen
    shaderProgram = createShaderProgram();

    // Vertex Array und Buffer erstellen
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texturen laden
    textureID[0] = loadTextureFromSubImage("texture.png", 0, 0, 16, 16); // Beispiel-Sprite-Sheet
    // Füge hier für jede Seite des Blocks andere Texturen hinzu

    // Haupt-Rendering-Schleife
    while (!glfwWindowShouldClose(window)) {
        // Eingaben verarbeiten
        processInput(window);

        // Rendern
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Kamera und View setzen
        projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        model = glm::mat4(1.0f);

        glUseProgram(shaderProgram);

        // Matrizen an Shader übergeben
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

        // Block rendern
        glBindVertexArray(VAO);
        for (int i = 0; i < 6; ++i) {
            glBindTexture(GL_TEXTURE_2D, textureID[i]);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(i * 6 * sizeof(unsigned int)));
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
*/

#include "/home/codeleaded/System/Static/Library/WindowEngineGL1.0.h"
#include "/home/codeleaded/System/Static/Library/PerlinNoise.h"
#include "/home/codeleaded/System/Static/Library/Files.h"

char* vertexShaderSource = NULL;
char* fragmentShaderSource = NULL;

float vertices[] = {
    // positions        // texture coords
     0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, 0.0f,  0.0f, 1.0f
};
unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3
};
GLuint VBO, VAO, EBO;
GLuint texture;

GLuint vertexShader;
GLuint fragmentShader;
GLuint shaderProgram;

void checkCompileErrors(GLuint shader, const char* type) {
    GLint success;
    GLchar infoLog[512];
    if (strcmp(type, "PROGRAM") != 0) {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            printf("ERROR: Shader Compilation [%s]\n%s\n", type, infoLog);
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 512, NULL, infoLog);
            printf("ERROR: Program Linking\n%s\n", infoLog);
        }
    }
}

void Setup(AlxWindow* w){
    vertexShaderSource = Files_Read("./src/vertexShader.glsl");
    if(!vertexShaderSource){
        printf("[vertexShader]: Error\n");
        window.Running = 0;
        return;
    }

    fragmentShaderSource = Files_Read("./src/fragmentShader.glsl");
    if(!fragmentShaderSource){
        printf("[fragmentShader]: Error\n");
        window.Running = 0;
        return;
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, (const GLchar * const*)&vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    checkCompileErrors(vertexShader, "VERTEX");

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (const GLchar * const*)&fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    checkCompileErrors(fragmentShader, "FRAGMENT");

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    checkCompileErrors(shaderProgram, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    Image_Enable_FlipV();
    Sprite sp = Sprite_Load_BGR("./data/Atlas.png");
    if(sp.img){
        GLenum format = GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, sp.w, sp.h, 0, format, GL_UNSIGNED_BYTE, sp.img);
        glGenerateMipmap(GL_TEXTURE_2D);
        Sprite_Free(&sp);
    }
}
void Update(AlxWindow* w){
    glClearColor(0.5f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
void Delete(AlxWindow* w){
    if(vertexShaderSource)      free(vertexShaderSource);
    if(fragmentShaderSource)    free(fragmentShaderSource);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
}

int main(){
    if(Create("OpenGL Test",2500,1300,1,1,Setup,Update,Delete))
        Start();
    return 0;
}
