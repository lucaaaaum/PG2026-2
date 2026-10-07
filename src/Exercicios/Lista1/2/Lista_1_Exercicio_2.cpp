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
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// Protótipo da função de callback de teclado
void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mode);

// Protótipos das funções
int setupShader();
GLuint setupGeometry(vector<GLfloat> vertices);
vector<GLfloat> gerarVerticesParametricosContinuos(int quantidade, float raio,
                                                   float centroX,
                                                   float centroY);
vector<GLfloat> gerarVerticesSetor(int quantidade, float raio, float centroX,
                                   float centroY, float anguloInicialGraus,
                                   float anguloFinalGraus);

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 800, HEIGHT = 600;

enum ModoDesenho {
  CIRCULO,
  OCTOGONO,
  PENTAGONO,
  PACMAN,
  PIZZA
};
enum ModoDesenho modo = CIRCULO;

// Total de modos: como o enum começa em 0, é o último + 1
const int QUANTIDADE_DE_MODOS = PIZZA + 1;

// Nome legível de cada modo, para as mensagens de debug
const char *nomeDoModo(ModoDesenho m) {
  switch (m) {
  case CIRCULO:
    return "Círculo";
  case OCTOGONO:
    return "Octógono";
  case PENTAGONO:
    return "Pentágono";
  case PACMAN:
    return "Pac-man";
  case PIZZA:
    return "Fatia de pizza";
  }
  return "Desconhecido";
}

// Código fonte do Vertex Shader (em GLSL): ainda hardcoded
const GLchar *vertexShaderSource = R"glsl(
 #version 400
 layout (location = 0) in vec3 position;
 void main()
 {
	 gl_Position = vec4(position.x, position.y, position.z, 1.0);
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

  // Criação da janela GLFW
  GLFWwindow *window = glfwCreateWindow(
      WIDTH, HEIGHT, "Ola Triangulo! -- Rossana", nullptr, nullptr);
  if (!window) {
    std::cerr << "Falha ao criar a janela GLFW" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // Fazendo o registro da função de callback para a janela GLFW
  glfwSetKeyCallback(window, key_callback);

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

  // Gerando os vértices de cada forma. Cada lista fica na posição do seu modo
  // (ex.: listas[OCTOGONO] são os vértices do octógono)
  vector<GLfloat> listas[QUANTIDADE_DE_MODOS];
  listas[CIRCULO] = gerarVerticesParametricosContinuos(100, 0.5f, 0.0f, 0.0f);
  listas[OCTOGONO] = gerarVerticesParametricosContinuos(8, 0.5f, 0.0f, 0.0f);
  listas[PENTAGONO] = gerarVerticesParametricosContinuos(5, 0.5f, 0.0f, 0.0f);
  // Pac-man: o círculo inteiro menos a boca (de 30° a 330°, boca para a
  // direita). Pizza: exatamente a "boca" do pac-man (de -30° a 30°)
  listas[PACMAN] = gerarVerticesSetor(100, 0.5f, 0.0f, 0.0f, 30.0f, 330.0f);
  listas[PIZZA] = gerarVerticesSetor(20, 0.5f, 0.0f, 0.0f, -30.0f, 30.0f);

  // Enviando cada forma para a GPU: um VAO por forma, e guardando quantos
  // vértices cada uma tem (cada vértice ocupa 3 floats: x, y, z)
  GLuint VAOs[QUANTIDADE_DE_MODOS];
  int contagens[QUANTIDADE_DE_MODOS];
  for (int i = 0; i < QUANTIDADE_DE_MODOS; i++) {
    VAOs[i] = setupGeometry(listas[i]);
    contagens[i] = listas[i].size() / 3;
  }

  // Enviando a cor desejada (vec4) para o fragment shader
  // Utilizamos a variáveis do tipo uniform em GLSL para armazenar esse tipo de
  // info que não está nos buffers
  GLint colorLoc = glGetUniformLocation(shaderID, "inputColor");

  glUseProgram(
      shaderID); // Reseta o estado do shader para evitar problemas futuros

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

    // Conectando ao buffer de geometria da forma do modo atual
    glBindVertexArray(VAOs[modo]);

    glUniform4f(colorLoc, 0.0f, 0.0f, 1.0f,
                1.0f); // enviando cor para variável uniform inputColor

    // Chamada de desenho - drawcall
    // Poligono Preenchido - GL_TRIANGLE_FAN
    glDrawArrays(GL_TRIANGLE_FAN, 0, contagens[modo]);

    // glBindVertexArray(0); // Desnecessário aqui, pois não há múltiplos VAOs

    // Troca os buffers da tela
    glfwSwapBuffers(window);
  }
  // Pede pra OpenGL desalocar os buffers
  glDeleteVertexArrays(QUANTIDADE_DE_MODOS, VAOs);
  // Finaliza a execução da GLFW, limpando os recursos alocados por ela
  glfwTerminate();
  return 0;
}

// Função de callback de teclado - só pode ter uma instância (deve ser estática
// se estiver dentro de uma classe) - É chamada sempre que uma tecla for
// pressionada ou solta via GLFW
void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mode) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);

  if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
    if (modo == PIZZA) {
      modo = CIRCULO;
    } else {
      modo = static_cast<ModoDesenho>(static_cast<int>(modo) + 1);
    }
    cout << "Modo: " << nomeDoModo(modo) << endl;
  }

  if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
    if (modo == CIRCULO) {
      modo = PIZZA;
    } else {
      modo = static_cast<ModoDesenho>(static_cast<int>(modo) - 1);
    }
    cout << "Modo: " << nomeDoModo(modo) << endl;
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

// Cria os buffers na GPU para UMA forma, a partir da lista de vértices recebida
// (x, y, z em sequência). Apenas atributo coordenada nos vértices
// 1 VBO com as coordenadas, VAO com apenas 1 ponteiro para atributo
// A função retorna o identificador do VAO
GLuint setupGeometry(vector<GLfloat> vertices) {
  GLuint VBO, VAO;
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

vector<GLfloat> gerarVerticesParametricosContinuos(int quantidade, float raio,
                                                   float centroX,
                                                   float centroY) {
  vector<GLfloat> vertices;
  float anguloIncremento = 2.0f * M_PI / quantidade;
  for (int i = 0; i < quantidade; i++) {
    float angulo = i * anguloIncremento;
    float x = centroX + raio * cos(angulo);
    float y = centroY + raio * sin(angulo);
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(0.0f); // eixo z
  }
  return vertices;
}

// Gera um "setor" do círculo: só o arco entre anguloInicialGraus e
// anguloFinalGraus, ligado ao centro. Para o GL_TRIANGLE_FAN, o 1º vértice é o
// centro (é dele que saem todos os triângulos, como fatias). Depois vêm
// quantidade + 1 pontos do arco: o "+ 1" inclui o ponto do ângulo final, para
// o arco terminar exatamente nele
vector<GLfloat> gerarVerticesSetor(int quantidade, float raio, float centroX,
                                   float centroY, float anguloInicialGraus,
                                   float anguloFinalGraus) {
  vector<GLfloat> vertices;

  // Centro
  vertices.push_back(centroX);
  vertices.push_back(centroY);
  vertices.push_back(0.0f); // eixo z

  // cos() e sin() usam radianos
  float anguloInicial = anguloInicialGraus * M_PI / 180.0f;
  float anguloFinal = anguloFinalGraus * M_PI / 180.0f;
  float anguloIncremento = (anguloFinal - anguloInicial) / quantidade;
  for (int i = 0; i <= quantidade; i++) {
    float angulo = anguloInicial + i * anguloIncremento;
    float x = centroX + raio * cos(angulo);
    float y = centroY + raio * sin(angulo);
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(0.0f); // eixo z
  }
  return vertices;
}
