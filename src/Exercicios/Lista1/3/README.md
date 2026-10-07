# Lista 1 — Exercício 3: Triângulo com cor por vértice

## Descrição do Projeto

Desenho de um triângulo formado pelos vértices P1, P2 e P3, respectivamente com as cores vermelho, verde e azul. A cor é um atributo de cada vértice (e não uma cor única para o desenho todo), então o OpenGL interpola as três cores no interior do triângulo, gerando um degradê.

| Vértice | Posição (x, y, z)  | Cor (r, g, b)        |
|---------|--------------------|----------------------|
| P1      | (0.0, 0.6, 0.0)    | (1, 0, 0) — vermelho |
| P2      | (−0.6, −0.5, 0.0)  | (0, 1, 0) — verde    |
| P3      | (0.6, −0.3, 0.0)   | (0, 0, 1) — azul     |

### Controles

| Tecla | Ação             |
|-------|------------------|
| `ESC` | Fecha o programa |

---

## Respostas

### a) Configuração dos buffers (VBO e VAO)

Foi usado **um único VBO** com os atributos **intercalados**: cada vértice ocupa 6 floats seguidos, os 3 primeiros com a posição e os 3 seguintes com a cor.

```
VBO: [ x y z r g b | x y z r g b | x y z r g b ]
        P1              P3              P2
```

O **VAO** guarda como ler esse VBO, com dois atributos:

| Atributo (location) | Floats por vértice | Pulo entre vértices (*stride*) | Início (*offset*) |
|---------------------|--------------------|--------------------------------|-------------------|
| 0 — posição         | 3                  | `6 * sizeof(GLfloat)`          | `0`               |
| 1 — cor             | 3                  | `6 * sizeof(GLfloat)`          | `3 * sizeof(GLfloat)` |

```cpp
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
glEnableVertexAttribArray(0);
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
glEnableVertexAttribArray(1);
```

Outra configuração possível seria usar dois VBOs separados (um só com posições e outro só com cores), ambos ligados ao mesmo VAO.

### b) Identificação dos atributos no vertex shader

Cada atributo é identificado pelo mesmo número (*location*) usado no `glVertexAttribPointer`:

```glsl
layout (location = 0) in vec3 position; // atributo 0: posição
layout (location = 1) in vec3 color;    // atributo 1: cor
```

Como quem pinta os pixels é o fragment shader, o vertex shader repassa a cor para ele por uma variável `out`, recebida no fragment shader por uma `in` de mesmo nome:

```glsl
// vertex shader
out vec3 corDoVertice;
...
corDoVertice = color;

// fragment shader
in vec3 corDoVertice;
...
color = vec4(corDoVertice, 1.0);
```

---

## Estrutura do Projeto

| Arquivo                   | Descrição |
|---------------------------|-----------|
| `Lista_1_Exercicio_3.cpp` | Programa completo: inicialização da janela e do contexto OpenGL, shaders (com cor por vértice), geometria (1 VBO com posição e cor intercaladas, 1 VAO com 2 atributos) e loop de renderização. |

---

## Informações Técnicas

- **Linguagem:** C++17
- **API Gráfica:** OpenGL 4.0+ (shaders GLSL `#version 400`)
- **Dependências:** GLFW, GLAD
- **Compilador / Build:** GCC + CMake (ambiente via Nix flake)
- **Plataforma-alvo:** Linux

### Como compilar e executar

A partir da raiz do repositório:

```sh
cmake -S . -B build
cmake --build build
./build/Lista1_3
```

---

## Checklist de Requisitos

- [x] Criação de janela e contexto OpenGL
- [x] Configuração de shaders e pipeline programável
- [x] a) Descrição da configuração dos buffers (VBO, VAO)
- [x] b) Descrição da identificação dos atributos no vertex shader
- [x] Implementação do triângulo com cor por vértice

---

## Referências e/ou créditos

- Código base: exemplo *Hello Triangle* da disciplina, adaptado por Rossana Baptista Queiroz — [repositório da turma](https://github.com/fellowsheep/PG2026-2)
- Material da disciplina: slides *Introdução à OpenGL Moderna e Shaders* (variáveis `in`/`out` entre estágios do pipeline)
- LearnOpenGL — [Shaders](https://learnopengl.com/Getting-started/Shaders)
