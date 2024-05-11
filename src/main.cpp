
// Bibliotecas incluídas
#include <iostream>
#include "glad.h"
#include "glfw3.h"
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <vector>

// Constantes
const float radius = 0.5f;
const float center_x = 0.0f;
const float center_y = 0.0f;
const int windowHeight = 600;
const int windowWidth = 800;
const int portHeight = 600;
const int portWidth = 800;

// seletor (seta para cima altera para o proximo) (seta para baixo altera para o anterior)
int seletor = 0;

// Funções de compilação de shader
unsigned int make_module(const std::string& filepath, unsigned int module_type)
{
    std::ifstream file;
    std::stringstream bufferedLines;
    std::string line;

    file.open(filepath);
    while (std::getline(file,line))
    {
        bufferedLines << line << "\n";
    }
    std::string shaderSource = bufferedLines.str();
    const char * shaderSrc = shaderSource.c_str();
    bufferedLines.str("");
    file.close();

    unsigned int shaderModule = glCreateShader(module_type);
    glShaderSource(shaderModule,1,&shaderSrc,NULL);
    glCompileShader(shaderModule);

    int success;
    glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char errorLog[1024];
        glGetShaderInfoLog(shaderModule, 1024, NULL, errorLog);
        std::cout << "make module error:\n" << errorLog << std::endl;
    }
    return shaderModule;
}
unsigned int make_shader(const std::string& vertex_filepath,const std::string& fragment_filepath)
{
    std::vector<unsigned int> modules;
    modules.push_back(make_module(vertex_filepath, GL_VERTEX_SHADER));
    modules.push_back(make_module(fragment_filepath, GL_FRAGMENT_SHADER));

    unsigned int shader = glCreateProgram();
    for (unsigned int shaderModule : modules)
    {
        glAttachShader(shader, shaderModule);
    }
    glLinkProgram(shader);

    int success;
    glGetShaderiv(shader, GL_LINK_STATUS, &success);
    if (!success)
    {
        char errorLog[1024];
        glGetShaderInfoLog(shader, 1024, NULL, errorLog);
        std::cout << "make Shader error:\n" << errorLog << std::endl;
    }

    for (unsigned int shaderModule : modules)
    {
        glDeleteShader(shaderModule);
    }

    return shader;
}


// Função para lidar com entrada do teclado
void processInput(GLFWwindow *window)
{
    static bool downPressed = false;
    static bool upPressed = false;

    // Funções para fechar a janela
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && !upPressed or glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && !downPressed)
    {
        std::cout << seletor << std::endl;
    }


    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Função para trocar desenho (seta para cima = proximo) (seta para baixo = anterior)
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && !downPressed)
    {
        seletor -= 1;
        downPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && !upPressed)
    {
        seletor += 1;
        upPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE)
        downPressed = false;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE)
        upPressed = false;

    if (seletor < 0)
        seletor = 0;
    else if (seletor > 14)
        seletor = 14;
}

// Função para desenhar um círculo
void redondo(int lados, float Completude,bool pizza, float espiral)
{
    glBindVertexArray(0); // Unbind any existing VAO

    // Generate vertices for the circle
    std::vector<float> circleVertices;
if (pizza){
    circleVertices.push_back(center_x);
    circleVertices.push_back(center_y);
    circleVertices.push_back(0.0f);
}  
    float raio = .5;
    for (int i = 0; i < lados*Completude; ++i)
    {
        float theta = 2.0f * 3.1415926f * float(i) / float(lados);
        float x = raio * std::cos(theta);
        float y = raio * std::sin(theta);
        circleVertices.push_back(x + center_x);
        circleVertices.push_back(y + center_y);
        circleVertices.push_back(0.0f); // z-coordinate (assuming 2D)
        raio = raio - radius*(espiral/lados);
    }
    

    // Create and bind a VAO
    GLuint circleVAO;
    glGenVertexArrays(1, &circleVAO);
    glBindVertexArray(circleVAO);

    // Create a VBO and buffer data
    GLuint circleVBO;
    glGenBuffers(1, &circleVBO);
    glBindBuffer(GL_ARRAY_BUFFER, circleVBO);
    glBufferData(GL_ARRAY_BUFFER, circleVertices.size() * sizeof(float), circleVertices.data(), GL_STATIC_DRAW);

    // Enable vertex attribute array and specify vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Draw the circle
    if (espiral == 0)
    {
        glBindVertexArray(circleVAO);
        glDrawArrays(GL_LINE_LOOP, 0, lados*Completude);
        glBindVertexArray(0);
    }
    else
    {
        glBindVertexArray(circleVAO);
        glDrawArrays(GL_LINE_STRIP, 0, lados*Completude);
        glBindVertexArray(0);
    }
    
    

    // Clean up VBO and VAO
    glDeleteBuffers(1, &circleVBO);
    glDeleteVertexArrays(1, &circleVAO);
}

// Função main
int main(int, char **)
{
    // Inicialização do GLFW
    GLFWwindow *window;
    if (!glfwInit())
    {
        return -1;
    }

    // Configurar versão OpenGL e perfil de compatibilidade
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Criação da janela GLFW
    window = glfwCreateWindow(windowWidth, windowHeight, "OpenGL Triângulo", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Carregamento das funções OpenGL via GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Não foi possível carregar a biblioteca OpenGL." << std::endl;
        glfwTerminate();
        return -1;
    }

    // Define as dimensões da viewport para a janela criada
    glViewport(0, 0, portWidth, portHeight);

    unsigned int shader = make_shader("../src/shaders/vertex.txt", "../src/shaders/fragment.txt");

    // Configura a matriz de projeção ortográfica para coordenadas normalizadas de -1 a 1.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    // Configura a matriz de visualização do modelo para a identidade
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(2.0f);

    // Define os vértices para desenhar triângulos e pontos
    float vertices[] = {
        // (x, y, z) - posição
        // (r, g, b) - cor
        // (x, y, z, r, g, b) - posição > cor
        -0.4f, -0.9f, 0.0f, 0.0f, 0.0f, 0.0f, //00
        +0.4f, -0.9f, 0.0f, 0.0f, 0.0f, 0.0f, //01
        +0.0f, -0.1f, 0.0f, 0.0f, 0.0f, 0.0f, //02
        +0.4f, +0.9f, 0.0f, 0.0f, 0.0f, 0.0f, //03
        -0.4f, +0.9f, 0.0f, 0.0f, 0.0f, 0.0f, //04
        +0.0f, +0.1f, 0.0f, 0.0f, 0.0f, 0.0f, //05
        
        -0.4f, -0.9f, 0.0f, 1.0f, 0.0f, 0.0f, //06
        +0.4f, -0.9f, 0.0f, 0.0f, 1.0f, 0.0f, //07
        +0.0f, -0.1f, 0.0f, 0.0f, 0.0f, 1.0f, //08
        +0.4f, +0.9f, 0.0f, 1.0f, 0.0f, 0.0f, //09
        -0.4f, +0.9f, 0.0f, 0.0f, 1.0f, 0.0f, //10
        +0.0f, +0.1f, 0.0f, 0.0f, 0.0f, 1.0f, //11

        +0.0f, +0.6f, 0.0f, 0.0f, 0.0f, 1.0f, //12
        -0.6f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, //13
        +0.6f, -0.3f, 0.0f, 0.0f, 0.0f, 1.0f, //14

        +0.0f, +0.6f, 0.0f, 1.0f, 0.0f, 0.0f, //15
        -0.6f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, //16
        +0.6f, -0.3f, 0.0f, 0.0f, 0.0f, 1.0f, //17

        -1.0f, -0.8f, 0.0f, 1.0f, 0.5f, 0.0f, //18
        +1.0f, -0.8f, 0.0f, 1.0f, 0.5f, 0.0f, //19
        +0.4f, -0.8f, 0.0f, 0.0f, 0.0f, 0.0f, //20
        +0.4f, +0.2f, 0.0f, 0.0f, 0.0f, 0.0f, //21
        +0.0f, +0.8f, 0.0f, 0.0f, 0.0f, 0.0f, //22
        -0.4f, +0.2f, 0.0f, 0.0f, 0.0f, 0.0f, //23
        -0.4f, -0.8f, 0.0f, 0.0f, 0.0f, 0.0f, //24
        +0.4f, +0.2f, 0.0f, 0.0f, 0.0f, 0.0f, //25
        -0.4f, +0.2f, 0.0f, 0.0f, 0.0f, 0.0f, //26
        -0.1f, -0.8f, 0.0f, 0.0f, 0.0f, 0.0f, //27
        -0.1f, -0.4f, 0.0f, 0.0f, 0.0f, 0.0f, //28
        +0.1f, -0.4f, 0.0f, 0.0f, 0.0f, 0.0f, //29
        +0.1f, -0.8f, 0.0f, 0.0f, 0.0f, 0.0f, //30
        -0.3f, -0.3f, 0.0f, 0.0f, 0.0f, 0.0f, //31
        +0.0f, -0.3f, 0.0f, 0.0f, 0.0f, 0.0f, //32
        +0.0f, +0.1f, 0.0f, 0.0f, 0.0f, 0.0f, //33
        -0.3f, +0.1f, 0.0f, 0.0f, 0.0f, 0.0f, //34
        -0.3f, -0.1f, 0.0f, 0.0f, 0.0f, 0.0f, //35
        +0.0f, -0.1f, 0.0f, 0.0f, 0.0f, 0.0f, //36
        -0.15f, -0.3f, 0.0f, 0.0f, 0.0f, 0.0f, //37
        -0.15f, +0.1f, 0.0f, 0.0f, 0.0f, 0.0f, //38
        +0.4f, +0.2f, 0.0f, 0.5f, 0.0f, 0.0f, //39
        +0.0f, +0.8f, 0.0f, 0.5f, 0.0f, 0.0f, //40
        -0.4f, +0.2f, 0.0f, 0.5f, 0.0f, 0.0f, //41
        -0.3f, -0.3f, 0.0f, 0.8f, 0.8f, 0.0f, //42
        +0.0f, -0.3f, 0.0f, 0.8f, 0.8f, 0.0f, //43
        +0.0f, +0.1f, 0.0f, 0.8f, 0.8f, 0.0f, //44
        -0.3f, +0.1f, 0.0f, 0.8f, 0.8f, 0.0f, //45
        -0.3f, -0.3f, 0.0f, 0.8f, 0.8f, 0.0f, //46
    };
    
    // Define os indices para os vértices
    float indices[] =
    {

    };

    // Criação dos buffers e arrays de vértices
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Configuração dos buffers e arrays de vértices
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Configura atributos de Posição
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Configura atributos de Cor
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

    // Configura a cor do fundo
    glClearColor(0.2f, 0.3f, 0.4f, 1.0f);

    // Loop principal
    while (!glfwWindowShouldClose(window))
    {
        // Verifica se alguma tecla foi pressionada
        processInput(window);

        // Limpa o buffer de cor
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shader);

        if (seletor == 1) // Desenha apenas o preenchimento (5 A)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        if (seletor == 2) // Desenha apenas as Linhas (5 B)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        if (seletor == 3) // Desenha apenas os pontos (5 C)
        {
            glPointSize(10.0f); // tamanho dos pontos
            glBindVertexArray(VAO);
            glDrawArrays(GL_POINTS, 0, 6);
        }

        if (seletor == 4) // Desenha o preenchimento, as Linhas e os pontos (5 D)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            glPointSize(10.0f);
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            glBindVertexArray(VAO);
            glDrawArrays(GL_POINTS, 0, 6);
        }

        if (seletor == 5) // Desenha o triangulo com borda colorida (5 I)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 6, 6);
        }

        if (seletor == 6) // Desenha o circulo (6)
        {
            redondo(100,1,false,0);
        }

        if (seletor == 7) // Desenha o octágono (6 A)
        {
            redondo(8,1,false,0);
        }

        if (seletor == 8) // Desenha o pentágono (6 B)
        {
            redondo(5,1,false,0);
        }
        
        if (seletor == 9) // Desenha o pac-man! (6 C)
        {
            redondo(100,.75,true,0);
        }
        
        if (seletor == 10) // Desenha a fatia de pizza (6 D)
        {
            redondo(100,.15,true,0);
        }

        if (seletor == 11) // Desenha a estrela (6 E)
        {
            // ainda nao :(
        }
         
        if (seletor == 12) // Desenha a espiral (7)
        {
            redondo(100,3,false,.3);
        }

        if (seletor == 13) // Reproducao do triangulo (8)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 12, 3);
            glPointSize(10.0f);
            glDrawArrays(GL_POINTS, 14, 3);
        }

        if (seletor == 14) // Reproducao do desenho (8)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 39, 6);
            glDrawArrays(GL_TRIANGLES, 44, 3);

            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glBindVertexArray(VAO);
            glDrawArrays(GL_LINE_STRIP, 20, 5);
            glDrawArrays(GL_LINE_STRIP, 25, 2);
            glDrawArrays(GL_LINE_STRIP, 27, 4);
            glDrawArrays(GL_LINE_LOOP, 31, 4);
            glDrawArrays(GL_LINE_STRIP, 35, 2);
            glDrawArrays(GL_LINE_STRIP, 37, 2);
            glDrawArrays(GL_LINE_STRIP, 18, 2);
        }

        // Troca os buffers da janela
        glfwSwapBuffers(window);

        // Verifica e chama eventos
        glfwPollEvents();
    }

    // Deleta os buffers e finaliza o GLFW
    glDeleteProgram(shader);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}