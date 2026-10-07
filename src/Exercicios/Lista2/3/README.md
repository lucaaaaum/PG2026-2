# Lista 2 — Exercício 3: Desenho com a câmera em pixels

## Descrição do Projeto

Desenho de um triângulo usando a câmera 2D do exercício 2:

| xmin | xmax | ymin | ymax |
|------|------|------|------|
| 0    | 800  | 600  | 0    |

Com esses limites, cada unidade corresponde a 1 pixel da janela (800×600). O ponto (0, 0) fica no canto superior esquerdo, e o y cresce para baixo.

Os vértices do *Hello Triangle* foram convertidos para pixels, mantendo o triângulo na mesma posição da tela:

| Vértice | Hello Triangle | Em pixels  |
|---------|----------------|------------|
| v0      | (−0.5, −0.5)   | (200, 450) |
| v1      | (0.5, −0.5)    | (600, 450) |
| v2      | (0.0, 0.5)     | (400, 150) |

A conversão usada foi:

```
x_pixel = (x + 1) · 400
y_pixel = (1 - y) · 300
```

### O que acontece quando posicionamos os objetos?

As coordenadas dos vértices passam a ser posições em pixels na janela:

- (0, 0) é o canto superior esquerdo e (800, 600) é o canto inferior direito;
- o y cresce **para baixo**, ao contrário do padrão do OpenGL. A ponta do triângulo (v2) tem o **menor** y (150) e aparece em cima;
- um objeto fora do intervalo de 0 a 800 (x) ou de 0 a 600 (y) fica fora da tela.

### Por que essa configuração é útil?

- É o mesmo sistema de coordenadas da tela, das imagens e do mouse. O GLFW informa a posição do cursor em pixels, com o (0, 0) no canto superior esquerdo, então um clique pode ser usado como vértice sem conversão.
- Tamanhos e posições ficam fáceis de pensar: um quadrado de 100×100 tem 100 pixels de lado.
- Como a projeção tem o mesmo tamanho da janela, o desenho não fica esticado (o que acontecia no exercício 1, com uma projeção quadrada numa janela retangular).

### Controles

| Tecla | Ação             |
|-------|------------------|
| `ESC` | Fecha o programa |

---

## Estrutura do Projeto

| Arquivo                   | Descrição |
|---------------------------|-----------|
| `Lista_2_Exercicio_3.cpp` | Programa completo: inicialização da janela e do contexto OpenGL, shaders com matriz de projeção, envio da matriz, geometria do triângulo em pixels e loop de renderização. |

Funções principais:

| Função            | Descrição |
|-------------------|-----------|
| `setupGeometry()` | Envia os vértices do triângulo para a GPU (VBO) e retorna o VAO correspondente. |
| `setupShader()`   | Compila os shaders e retorna o programa de shader. |

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
./build/Lista2_3
```

---

## Checklist de Requisitos

- [x] Uso da câmera 2D do exercício 2 (0 a 800, 600 a 0)
- [x] Desenho com coordenadas em pixels
- [x] Resposta: o que acontece quando posicionamos os objetos
- [x] Resposta: por que essa configuração é útil

---

## Referências e/ou créditos

- Código base: exemplo *Hello Triangle* da disciplina, adaptado por Rossana Baptista Queiroz — [repositório da turma](https://github.com/fellowsheep/PG2026-2)
- Slides da disciplina: *Sistemas de Coordenadas* (projeção ortográfica com proporção 1:1 com as coordenadas de tela)
- LearnOpenGL — [Coordinate Systems](https://learnopengl.com/Getting-started/Coordinate-Systems)
- GLFW — [Input guide](https://www.glfw.org/docs/latest/input_guide.html) (coordenadas do cursor)
