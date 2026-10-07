# Lista 2 — Exercício 1: Projeção ortográfica de −10 a 10

## Descrição do Projeto

Modificação da janela do mundo (*window*/*ortho*) para os limites:

| xmin | xmax | ymin | ymax |
|------|------|------|------|
| −10  | 10   | −10  | 10   |

Sem projeção, o OpenGL só mostra o que está entre −1 e 1 nos dois eixos. A matriz de projeção ortográfica, criada com `glm::ortho`, converte as coordenadas escolhidas (−10 a 10) para esse intervalo. Neste caso, a conversão divide x e y por 10:

```
x_tela = x / 10
y_tela = y / 10
```

O triângulo do *Hello Triangle* (vértices entre −0.5 e 0.5) continua com os mesmos vértices, mas passa a aparecer 10 vezes menor, no centro da janela.

### Como a matriz chega ao shader

1. O vertex shader declara a matriz como `uniform mat4 projection` e multiplica cada vértice por ela: `gl_Position = projection * vec4(position, 1.0)`.
2. No `main`, depois do `glUseProgram`, a matriz é criada com `glm::ortho(-10, 10, -10, 10, -1, 1)`. Os dois últimos valores são a profundidade.
3. `glGetUniformLocation` busca o número do uniform `projection` no programa de shader.
4. `glUniformMatrix4fv` envia os 16 floats da matriz (`glm::value_ptr`) para esse uniform.

A matriz é enviada uma vez só, porque não muda durante a execução.

### Controles

| Tecla | Ação             |
|-------|------------------|
| `ESC` | Fecha o programa |

### Limitações conhecidas

- A janela não é quadrada (800×600), mas a projeção é (20×20), então o desenho aparece esticado na horizontal.

---

## Estrutura do Projeto

| Arquivo                   | Descrição |
|---------------------------|-----------|
| `Lista_2_Exercicio_1.cpp` | Programa completo: inicialização da janela e do contexto OpenGL, shaders com matriz de projeção, envio da matriz, geometria do triângulo e loop de renderização. |

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
./build/Lista2_1
```

---

## Checklist de Requisitos

- [x] Matriz de projeção ortográfica no vertex shader (`uniform mat4`)
- [x] Janela do mundo com xmin = −10, xmax = 10, ymin = −10, ymax = 10

---

## Referências e/ou créditos

- Código base: exemplo *Hello Triangle* da disciplina, adaptado por Rossana Baptista Queiroz — [repositório da turma](https://github.com/fellowsheep/PG2026-2)
- Slides da disciplina: *Sistemas de Coordenadas* (passagem da matriz de projeção para o shader)
- LearnOpenGL — [Coordinate Systems](https://learnopengl.com/Getting-started/Coordinate-Systems)
