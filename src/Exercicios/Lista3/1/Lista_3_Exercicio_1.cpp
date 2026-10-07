/*
 * Hello Triangle - Código adaptado de:
 *   - https://learnopengl.com/#!Getting-started/Hello-Triangle
 *   - https://antongerdelan.net/opengl/glcontext2.html
 *
 * Adaptado por: Rossana Baptista Queiroz
 *
 * Disciplinas:
 *   - Processamento Gráfico (Ciência da Computação - Híbrido)
 *   - Processamento Gráfico: Fundamentos (Ciência da Computação - Presencial)
 *   - Fundamentos de Computação Gráfica (Jogos Digitais)
 *
 * Descrição:
 *   Este código é o "Olá Mundo" da Computação Gráfica, utilizando OpenGL
 * Moderna. No pipeline programável, o desenvolvedor pode implementar as etapas
 * de Processamento de Geometria e Processamento de Pixel utilizando shaders. Um
 * programa de shader precisa ter, obrigatoriamente, um Vertex Shader e um
 * Fragment Shader, enquanto outros shaders, como o de geometria, são opcionais.
 *
 * Histórico:
 *   - Versão inicial: 07/04/2017
 *   - Última atualização: 05/08/2025
 *
 */

#include <assert.h>
#include <iostream>
#include <vector>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Protótipo da função de callback de teclado
void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mode);

void mouse_callback(GLFWwindow *window, int key, int action, int mode);

// Protótipo da função chamada quando o tamanho da imagem da janela muda
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

// Protótipos das funções
int setupShader();
int setupGeometry(vector<GLfloat> vertices);

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 800, HEIGHT = 600;

// Vértices dos triângulos (x, y, z), em pixels. Fica fora das funções para que
// a função de clique do mouse também consiga acessar
vector<GLfloat> vertices;

// Buffer (VBO) com os vértices na GPU. Fica fora das funções para que o loop
// de renderização consiga reenviar os vértices a cada quadro
GLuint VBO;

// Cores dos triângulos (RGB). O triângulo i usa a cor i % 6, então as cores se
// repetem a partir do sétimo triângulo
const GLfloat colors[][3] = {
    {1.0f, 0.0f, 0.0f}, // vermelho
    {0.0f, 1.0f, 0.0f}, // verde
    {0.0f, 0.0f, 1.0f}, // azul
    {1.0f, 1.0f, 0.0f}, // amarelo
    {1.0f, 0.0f, 1.0f}, // magenta
    {0.0f, 1.0f, 1.0f}, // ciano
};
const int NUM_COLORS = sizeof(colors) / sizeof(colors[0]);

// Código fonte do Vertex Shader (em GLSL): ainda hardcoded
const GLchar *vertexShaderSource = R"glsl(
 #version 400
 layout (location = 0) in vec3 position;
 uniform mat4 projection;
 void main()
 {
	 gl_Position = projection * vec4(position.x, position.y, position.z, 1.0);
 }
 )glsl";

// Código fonte do Fragment Shader (em GLSL): ainda hardcoded
const GLchar *fragmentShaderSource = R"glsl(
 #version 400
 uniform vec4 inputColor;
 out vec4 color;
 void main()
 {
	 color = inputColor;
 }
 )glsl";

// Função MAIN
int main() {
  // Inicialização da GLFW
  glfwInit();

  // Muita atenção aqui: alguns ambientes não aceitam essas configurações
  // Você deve adaptar para a versão do OpenGL suportada por sua placa
  // Sugestão: comente essas linhas de código para desobrir a versão e
  // depois atualize (por exemplo: 4.5 com 4 e 5)
  // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Ativa a suavização de serrilhado (MSAA) com 8 amostras por pixel
  // glfwWindowHint(GLFW_SAMPLES, 8);

  // Essencial para computadores da Apple
  // #ifdef __APPLE__
  //	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  // #endif

  // Janela com tamanho fixo: a projeção é de 800x600, então a janela não pode
  // mudar de tamanho para que cada unidade continue sendo 1 pixel
  // (gerenciadores de janela lado a lado costumam deixar janelas de tamanho
  // fixo flutuantes)
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  // Criação da janela GLFW
  GLFWwindow *window = glfwCreateWindow(
      WIDTH, HEIGHT, "Ola Triangulo! -- Rossana", nullptr, nullptr);
  if (!window) {
    std::cerr << "Falha ao criar a janela GLFW" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // Reforça o tamanho de 800x600: alguns gerenciadores de janela ignoram o
  // tamanho pedido na criação quando a janela fica flutuante
  glfwSetWindowSize(window, WIDTH, HEIGHT);

  // Fazendo o registro da função de callback para a janela GLFW
  glfwSetKeyCallback(window, key_callback);
  glfwSetMouseButtonCallback(window, mouse_callback);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // GLAD: carrega todos os ponteiros d funções da OpenGL
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Falha ao inicializar GLAD" << std::endl;
    return -1;
  }

  // Obtendo as informações de versão
  const GLubyte *renderer = glGetString(GL_RENDERER); /* get renderer string */
  const GLubyte *version = glGetString(GL_VERSION);   /* version as a string */
  cout << "Renderer: " << renderer << endl;
  cout << "OpenGL version supported " << version << endl;

  // Definindo as dimensões da viewport com as mesmas dimensões da janela da
  // aplicação
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);

  // Compilando e buildando o programa de shader
  GLuint shaderID = setupShader();

  // Gerando um buffer simples, com a geometria de um triângulo
  GLuint VAO = setupGeometry(vertices);

  // Enviando a cor desejada (vec4) para o fragment shader
  // Utilizamos a variáveis do tipo uniform em GLSL para armazenar esse tipo de
  // info que não está nos buffers
  GLint colorLoc = glGetUniformLocation(shaderID, "inputColor");

  glUseProgram(
      shaderID); // Reseta o estado do shader para evitar problemas futuros

  // Projeção ortográfica do tamanho da janela, em pixels, com o (0, 0) no canto
  // superior esquerdo (o mesmo sistema de coordenadas do mouse)
  glm::mat4 projection =
      glm::ortho(0.0f, (float)WIDTH, (float)HEIGHT, 0.0f, -1.0f, 1.0f);
  GLint projLoc = glGetUniformLocation(shaderID, "projection");
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

  double prev_s = glfwGetTime(); // Define o "tempo anterior" inicial.
  double title_countdown_s =
      0.1; // Intervalo para atualizar o título da janela com o FPS.

  // Loop da aplicação - "game loop"
  while (!glfwWindowShouldClose(window)) {
    // Este trecho de código é totalmente opcional: calcula e mostra a contagem
    // do FPS na barra de título
    {
      double curr_s = glfwGetTime(); // Obtém o tempo atual.
      double elapsed_s =
          curr_s - prev_s; // Calcula o tempo decorrido desde o último frame.
      prev_s = curr_s;     // Atualiza o "tempo anterior" para o próximo frame.

      // Exibe o FPS, mas não a cada frame, para evitar oscilações excessivas.
      title_countdown_s -= elapsed_s;
      if (title_countdown_s <= 0.0 && elapsed_s > 0.0) {
        double fps =
            1.0 / elapsed_s; // Calcula o FPS com base no tempo decorrido.

        // Cria uma string e define o FPS como título da janela.
        char tmp[256];
        sprintf(tmp, "Ola Triangulo! -- Rossana\tFPS %.2lf", fps);
        glfwSetWindowTitle(window, tmp);

        title_countdown_s = 0.1; // Reinicia o temporizador para atualizar o
                                 // título periodicamente.
      }
    }

    // Checa se houveram eventos de input (key pressed, mouse moved etc.) e
    // chama as funções de callback correspondentes
    glfwPollEvents();

    // Limpa o buffer de cor
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // cor de fundo
    glClear(GL_COLOR_BUFFER_BIT);

    glLineWidth(10);
    glPointSize(20);

    glBindVertexArray(VAO); // Conectando ao buffer de geometria

    // Reenvia os vértices para a GPU, já que os cliques podem ter adicionado
    // novos vértices desde o último quadro
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat),
                 vertices.data(), GL_DYNAMIC_DRAW);

    // Um desenho por triângulo completo (cada triângulo tem 3 vértices de 3
    // floats), cada um com a sua cor. Vértices que ainda não formam um
    // triângulo não são desenhados
    int numTriangles = vertices.size() / 9;
    for (int i = 0; i < numTriangles; i++) {
      const GLfloat *c = colors[i % NUM_COLORS];
      glUniform4f(colorLoc, c[0], c[1], c[2], 1.0f);
      glDrawArrays(GL_TRIANGLES, i * 3, 3);
    }

    // glBindVertexArray(0); // Desnecessário aqui, pois não há múltiplos VAOs

    // Troca os buffers da tela
    glfwSwapBuffers(window);
  }
  // Pede pra OpenGL desalocar os buffers
  glDeleteVertexArrays(1, &VAO);
  // Finaliza a execução da GLFW, limpando os recursos alocados por ela
  glfwTerminate();
  return 0;
}

// Função chamada quando o tamanho da imagem da janela (em pixels reais) muda.
// Em telas com escala (ex.: 1.5), a imagem tem mais pixels que a janela, e esse
// tamanho só é conhecido depois que a janela aparece: a viewport é atualizada
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

// Função de callback de teclado - só pode ter uma instância (deve ser estática
// se estiver dentro de uma classe) - É chamada sempre que uma tecla for
// pressionada ou solta via GLFW
void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mode) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}

void mouse_callback(GLFWwindow *window, int key, int action, int mode) {
  if (key == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    vertices.push_back(xpos);
    vertices.push_back(ypos);
    vertices.push_back(0.0f);
    cout << "Adicionando vértice: (" << xpos << ", " << ypos << ", 0.0)"
         << endl;
    if ((vertices.size() / 3) % 3 == 0) {
      cout << "Triângulo completo!" << endl;
    }
  }
}

// Esta função está bastante hardcoded - objetivo é compilar e "buildar" um
// programa de
//  shader simples e único neste exemplo de código
//  O código fonte do vertex e fragment shader está nos arrays
//  vertexShaderSource e fragmentShader source no iniçio deste arquivo A função
//  retorna o identificador do programa de shader
int setupShader() {
  // Vertex shader
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  // Checando erros de compilação (exibição via log no terminal)
  GLint success;
  GLchar infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }
  // Fragment shader
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  // Checando erros de compilação (exibição via log no terminal)
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }
  // Linkando os shaders e criando o identificador do programa de shader
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  // Checando por erros de linkagem
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
  }
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return shaderProgram;
}

// Esta função está bastante harcoded - objetivo é criar os buffers que
// armazenam a geometria de um triângulo Apenas atributo coordenada nos vértices
// 1 VBO com as coordenadas, VAO com apenas 1 ponteiro para atributo
// A função retorna o identificador do VAO
int setupGeometry(vector<GLfloat> vertices) {
  GLuint VAO;
  // Geração do identificador do VBO
  glGenBuffers(1, &VBO);
  // Faz a conexão (vincula) do buffer como um buffer de array
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // Envia os dados do array de floats para o buffer da OpenGl
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat),
               vertices.data(), GL_STATIC_DRAW);

  // Geração do identificador do VAO (Vertex Array Object)
  glGenVertexArrays(1, &VAO);
  // Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s)
  // de vértices e os ponteiros para os atributos
  glBindVertexArray(VAO);
  // Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o
  // atributo), indicando:
  //  Localização no shader * (a localização dos atributos devem ser
  //  correspondentes no layout especificado no vertex shader) Numero de valores
  //  que o atributo tem (por ex, 3 coordenadas xyz) Tipo do dado Se está
  //  normalizado (entre zero e um) Tamanho em bytes Deslocamento a partir do
  //  byte zero
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                        (GLvoid *)0);
  glEnableVertexAttribArray(0);

  // Observe que isso é permitido, a chamada para glVertexAttribPointer
  // registrou o VBO como o objeto de buffer de vértice atualmente vinculado -
  // para que depois possamos desvincular com segurança
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array
  // para evitar bugs medonhos)
  glBindVertexArray(0);

  return VAO;
}
