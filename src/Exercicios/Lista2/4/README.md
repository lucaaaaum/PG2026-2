# Lista 2 — Exercício 4: Viewport no quadrante superior direito

## Descrição do Projeto

Modificação da *viewport* para desenhar a cena apenas no quadrante superior direito da janela da aplicação.

A cena é a mesma do exercício 3: um triângulo em coordenadas de pixels, com a projeção ortográfica de 0 a 800 (x) e de 600 a 0 (y).

### Projeção × viewport

- **Projeção:** define qual parte do mundo é vista e em que unidade (aqui, pixels de 0 a 800 × 600 a 0). Funciona como a câmera.
- **Viewport:** define em qual retângulo da janela a imagem da câmera é desenhada. Por padrão, é a janela inteira.

As duas são independentes: a projeção e os vértices continuam iguais aos do exercício 3, e a cena inteira é encolhida para caber na viewport.

### Viewport usada

```cpp
glViewport(width / 2, height / 2, width / 2, height / 2);
```

Os parâmetros são `(x, y, largura, altura)` do retângulo, em pixels da janela. Na viewport, a origem (0, 0) é o canto **inferior** esquerdo da janela, e o y cresce para cima, sem relação com a projeção usada.

| Parâmetro | Valor        | Motivo |
|-----------|--------------|--------|
| `x`       | `width / 2`  | o quadrante começa no meio da janela, na horizontal |
| `y`       | `height / 2` | o quadrante começa no meio da janela, na vertical (contando de baixo) |
| largura   | `width / 2`  | do meio até a borda direita |
| altura    | `height / 2` | do meio até a borda de cima |

`width` e `height` vêm de `glfwGetFramebufferSize`, que informa o tamanho real da janela em pixels (pode ser maior que 800×600 em telas de alta densidade).

O fundo continua pintado na janela inteira, porque o `glClear` não é limitado pela viewport.

### Controles

| Tecla | Ação             |
|-------|------------------|
| `ESC` | Fecha o programa |

---

## Estrutura do Projeto

| Arquivo                   | Descrição |
|---------------------------|-----------|
| `Lista_2_Exercicio_4.cpp` | Programa completo: inicialização da janela e do contexto OpenGL, viewport no quadrante superior direito, shaders com matriz de projeção, geometria do triângulo em pixels e loop de renderização. |

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
./build/Lista2_4
```

---

## Checklist de Requisitos

- [x] Viewport limitada ao quadrante superior direito da janela
- [x] Cena do exercício anterior desenhada dentro da viewport

---

## Referências e/ou créditos

- Código base: exemplo *Hello Triangle* da disciplina, adaptado por Rossana Baptista Queiroz — [repositório da turma](https://github.com/fellowsheep/PG2026-2)
- Slides da disciplina: *Sistemas de Coordenadas*
- LearnOpenGL — [Hello Window](https://learnopengl.com/Getting-started/Hello-Window) (viewport)
- Documentação do OpenGL — [glViewport](https://registry.khronos.org/OpenGL-Refpages/gl4/html/glViewport.xhtml)
