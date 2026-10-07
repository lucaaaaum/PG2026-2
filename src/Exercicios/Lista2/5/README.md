# Lista 2 — Exercício 5: A mesma cena nos 4 quadrantes

## Descrição do Projeto

Desenho da mesma cena nos 4 quadrantes da janela, usando uma *viewport* diferente para cada um.

A cena é a mesma dos exercícios 3 e 4: um triângulo em coordenadas de pixels, com a projeção ortográfica de 0 a 800 (x) e de 600 a 0 (y).

### Viewports usadas

Todos os quadrantes têm o mesmo tamanho (`width / 2` × `height / 2`). Só muda o canto inferior esquerdo de cada um. Na viewport, a origem (0, 0) é o canto **inferior** esquerdo da janela.

| Quadrante          | x           | y            |
|--------------------|-------------|--------------|
| Superior esquerdo  | `0`         | `height / 2` |
| Superior direito   | `width / 2` | `height / 2` |
| Inferior esquerdo  | `0`         | `0`          |
| Inferior direito   | `width / 2` | `0`          |

### Por que desenhar 4 vezes

O OpenGL não guarda uma imagem da câmera para mostrar depois. Cada `glDrawArrays` pinta os pixels direto na janela, dentro da viewport ativa naquele momento, e cada `glViewport` substitui o anterior.

Por isso, a cada quadro, o loop de renderização faz 4 pares de chamadas:

```cpp
glViewport(0, height / 2, width / 2, height / 2);         // superior esquerdo
glDrawArrays(GL_TRIANGLES, 0, 3);
glViewport(width / 2, height / 2, width / 2, height / 2); // superior direito
glDrawArrays(GL_TRIANGLES, 0, 3);
glViewport(0, 0, width / 2, height / 2);                  // inferior esquerdo
glDrawArrays(GL_TRIANGLES, 0, 3);
glViewport(width / 2, 0, width / 2, height / 2);          // inferior direito
glDrawArrays(GL_TRIANGLES, 0, 3);
```

Essas chamadas ficam dentro do loop porque a tela é limpa (`glClear`) no começo de cada quadro.

### Controles

| Tecla | Ação             |
|-------|------------------|
| `ESC` | Fecha o programa |

---

## Estrutura do Projeto

| Arquivo                   | Descrição |
|---------------------------|-----------|
| `Lista_2_Exercicio_5.cpp` | Programa completo: inicialização da janela e do contexto OpenGL, shaders com matriz de projeção, geometria do triângulo em pixels e loop de renderização com 4 viewports. |

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
./build/Lista2_5
```

---

## Checklist de Requisitos

- [x] Mesma cena desenhada nos 4 quadrantes da janela
- [x] Uma viewport por quadrante, definida antes de cada chamada de desenho

---

## Referências e/ou créditos

- Código base: exemplo *Hello Triangle* da disciplina, adaptado por Rossana Baptista Queiroz — [repositório da turma](https://github.com/fellowsheep/PG2026-2)
- Slides da disciplina: *Sistemas de Coordenadas*
- Documentação do OpenGL — [glViewport](https://registry.khronos.org/OpenGL-Refpages/gl4/html/glViewport.xhtml)
