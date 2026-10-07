# Lista 2 — Exercício 6: Triângulos criados pelo clique do mouse

## Descrição do Projeto

Criação de triângulos a partir de cliques do mouse:

- cada clique com o botão esquerdo cria **1 vértice** na posição do cursor;
- a cada **3 vértices** criados, forma-se um triângulo;
- cada triângulo novo recebe uma **cor nova**.

### Projeção do tamanho da tela

A projeção ortográfica tem as mesmas dimensões da janela (800×600), com o (0, 0) no canto superior esquerdo e o y para baixo:

```cpp
glm::ortho(0.0f, (float)WIDTH, (float)HEIGHT, 0.0f, -1.0f, 1.0f);
```

Esse é o mesmo sistema de coordenadas que o GLFW usa para o cursor, então a posição do clique é usada como vértice sem nenhuma conversão.

### Clique do mouse

A função `mouse_callback` é registrada com `glfwSetMouseButtonCallback` e chamada pelo GLFW a cada clique. Ela recebe apenas o botão e a ação (apertar ou soltar), não a posição. Por isso, quando o botão esquerdo é apertado, a posição é obtida com `glfwGetCursorPos` e adicionada ao `vector<GLfloat> vertices` como `x, y, 0`.

O `vertices` fica fora das funções para que tanto a função de clique quanto o loop de renderização consigam acessá-lo.

### Envio dos vértices para a GPU

É usado um **VAO único**. Os cliques adicionam vértices ao `vector` do C++, mas o desenho usa o que está na GPU (no VBO). Por isso, a cada quadro, o vector é reenviado ao VBO antes do desenho:

```cpp
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat),
             vertices.data(), GL_DYNAMIC_DRAW);
```

`GL_DYNAMIC_DRAW` indica que os dados mudam com frequência.

### Uma cor por triângulo

Cada triângulo é desenhado com uma chamada própria, precedida do envio da sua cor para o uniform `inputColor`:

```cpp
int numTriangles = vertices.size() / 9; // 3 vértices × 3 floats
for (int i = 0; i < numTriangles; i++) {
  const GLfloat *c = colors[i % NUM_COLORS];
  glUniform4f(colorLoc, c[0], c[1], c[2], 1.0f);
  glDrawArrays(GL_TRIANGLES, i * 3, 3);
}
```

As cores vêm de uma lista de 6 (vermelho, verde, azul, amarelo, magenta e ciano), escolhida por `i % 6`. Vértices que ainda não completam um triângulo não são desenhados.

### Janela de tamanho fixo

- `glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE)` impede que a janela mude de tamanho, o que manteria a projeção de 800×600 diferente da janela e faria o clique cair no lugar errado. Gerenciadores de janela lado a lado costumam deixar janelas de tamanho fixo flutuantes.
- `glfwSetWindowSize(window, WIDTH, HEIGHT)` reforça o tamanho de 800×600, porque alguns gerenciadores de janela ignoram o tamanho pedido na criação.
- `framebuffer_size_callback` atualiza a viewport quando o tamanho real da imagem da janela é conhecido. Em telas com escala (ex.: 1.5), uma janela de 800×600 tem 1200×900 pixels reais.

### Controles

| Entrada                 | Ação                |
|-------------------------|---------------------|
| Clique (botão esquerdo) | Cria um vértice     |
| `ESC`                   | Fecha o programa    |

### Limitações conhecidas

- As cores se repetem a partir do sétimo triângulo.
- O vector inteiro é reenviado à GPU a cada quadro, mesmo sem cliques novos. Para poucos vértices, o custo é desprezível.

---

## Estrutura do Projeto

| Arquivo                   | Descrição |
|---------------------------|-----------|
| `Lista_2_Exercicio_6.cpp` | Programa completo: janela de tamanho fixo, projeção do tamanho da tela, tratamento do clique, reenvio dos vértices à GPU e desenho de um triângulo por cor. |

Funções principais:

| Função                        | Descrição |
|-------------------------------|-----------|
| `mouse_callback()`            | Chamada a cada clique; adiciona a posição do cursor como vértice. |
| `framebuffer_size_callback()` | Atualiza a viewport com o tamanho real da imagem da janela. |
| `setupGeometry()`             | Recebe a lista de vértices, cria o VBO e retorna o VAO. |
| `setupShader()`               | Compila os shaders e retorna o programa de shader. |

---

## Informações Técnicas

- **Linguagem:** C++17
- **API Gráfica:** OpenGL 4.0+ (shaders GLSL `#version 400`)
- **Dependências:** GLFW, GLAD, GLM
- **Compilador / Build:** GCC + CMake (ambiente via Nix flake)
- **Plataforma-alvo:** Linux

### Como compilar e executar

A partir da raiz do repositório:

```sh
cmake -S . -B build
cmake --build build
./build/Lista2_6
```

---

## Checklist de Requisitos

- [x] Cada clique cria apenas 1 vértice
- [x] A cada 3 vértices, um triângulo é criado
- [x] Cada triângulo novo tem uma cor nova
- [x] Projeção ortográfica com as mesmas dimensões da tela (800×600)
- [x] Clique do mouse tratado com o GLFW

---

## Referências e/ou créditos

- Código base: exemplo *Hello Triangle* da disciplina, adaptado por Rossana Baptista Queiroz — [repositório da turma](https://github.com/fellowsheep/PG2026-2)
- Slides da disciplina: *Sistemas de Coordenadas* e *Uniform vs. Buffers* (`GL_DYNAMIC_DRAW`)
- GLFW — [Input guide](https://www.glfw.org/docs/latest/input_guide.html) (clique e posição do cursor)
- GLFW — [Window guide](https://www.glfw.org/docs/latest/window_guide.html) (tamanho da janela e do framebuffer)
