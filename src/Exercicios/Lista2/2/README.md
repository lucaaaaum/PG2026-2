# Lista 2 — Exercício 2: Projeção ortográfica em pixels

## Descrição do Projeto

Modificação da janela do mundo (*window*/*ortho*) para os limites:

| xmin | xmax | ymin | ymax |
|------|------|------|------|
| 0    | 800  | 600  | 0    |

Esses limites têm o mesmo tamanho da janela (800×600), então cada unidade corresponde a 1 pixel. Como `ymin` é 600 e `ymax` é 0, o eixo y fica invertido em relação ao padrão do OpenGL:

- o ponto (0, 0) fica no **canto superior esquerdo**;
- o x cresce para a direita e o y cresce **para baixo**, como nas coordenadas de tela.

A conversão feita pela matriz para o intervalo de −1 a 1 do OpenGL é:

```
x_tela = x / 400 - 1
y_tela = 1 - y / 300
```

O código de envio da matriz é o mesmo do exercício 1 (`uniform mat4 projection` no vertex shader, `glGetUniformLocation` e `glUniformMatrix4fv`). Só mudam os limites do `glm::ortho`:

```cpp
glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);
```

### Controles

| Tecla | Ação             |
|-------|------------------|
| `ESC` | Fecha o programa |

### Limitações conhecidas

- Os vértices do triângulo ainda são os do *Hello Triangle* (entre −0.5 e 0.5). Em pixels, isso é menos de 1 pixel no canto superior esquerdo, então a janela aparece vazia. O desenho com coordenadas em pixels é feito no exercício 3.

---

## Estrutura do Projeto

| Arquivo                   | Descrição |
|---------------------------|-----------|
| `Lista_2_Exercicio_2.cpp` | Programa completo: inicialização da janela e do contexto OpenGL, shaders com matriz de projeção, envio da matriz, geometria do triângulo e loop de renderização. |

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
./build/Lista2_2
```

---

## Checklist de Requisitos

- [x] Matriz de projeção ortográfica no vertex shader (`uniform mat4`)
- [x] Janela do mundo com xmin = 0, xmax = 800, ymin = 600, ymax = 0

---

## Referências e/ou créditos

- Código base: exemplo *Hello Triangle* da disciplina, adaptado por Rossana Baptista Queiroz — [repositório da turma](https://github.com/fellowsheep/PG2026-2)
- Slides da disciplina: *Sistemas de Coordenadas* (passagem da matriz de projeção para o shader)
- LearnOpenGL — [Coordinate Systems](https://learnopengl.com/Getting-started/Coordinate-Systems)
