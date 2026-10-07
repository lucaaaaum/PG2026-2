# Lista 1 — Exercício 4: Cabeça de creeper

## Descrição do Projeto

Reprodução, com primitivas do OpenGL, de um desenho feito em papel quadriculado: a cabeça de um *creeper* do Minecraft.

O desenho é uma grade de 8×8 quadradinhos. A cara é toda verde, e os olhos e a boca são pretos:

```
. . . . . . . .   linha 0
. . . . . . . .   linha 1
. X X . . X X .   linha 2   olhos
. X X . . X X .   linha 3
. . . X X . . .   linha 4   boca
. . X X X X . .   linha 5
. . X X X X . .   linha 6
. . X . . X . .   linha 7
```

Na tela, a cabeça vai de −0.8 a 0.8 nos dois eixos, então cada quadradinho mede 0.2:

- a coluna `c` vai de `x = -0.8 + 0.2·c` até `x = -0.8 + 0.2·(c+1)`;
- a linha `r` vai de `y = 0.8 - 0.2·r` até `y = 0.8 - 0.2·(r+1)` (o y do OpenGL cresce para cima).

Cada retângulo é formado por 2 triângulos (6 vértices) desenhados com `GL_TRIANGLES`. As partes pretas são 5 retângulos:

| Parte                 | Colunas | Linhas | x            | y             |
|-----------------------|---------|--------|--------------|---------------|
| Olho esquerdo         | 1–2     | 2–3    | −0.6 a −0.2  | 0.4 a 0.0     |
| Olho direito          | 5–6     | 2–3    | 0.2 a 0.6    | 0.4 a 0.0     |
| Boca (meio)           | 3–4     | 4–6    | −0.2 a 0.2   | 0.0 a −0.6    |
| Boca (lado esquerdo)  | 2       | 5–7    | −0.4 a −0.2  | −0.2 a −0.8   |
| Boca (lado direito)   | 5       | 5–7    | 0.2 a 0.4    | −0.2 a −0.8   |

O desenho usa **dois VAOs** e **duas chamadas de desenho**, uma para cada cor:

1. **Cabeça:** o quadrado verde.
2. **Rosto:** olhos e boca, todos numa lista só, em preto.

A cor é definida pela variável `uniform` `inputColor` do fragment shader (`glUniform4f`) antes de cada desenho. A cabeça é desenhada primeiro, porque o que é desenhado depois fica por cima.

### Controles

| Tecla | Ação             |
|-------|------------------|
| `ESC` | Fecha o programa |

### Limitações conhecidas

- A janela não é quadrada (800×600) e não há matriz de projeção, então a cabeça aparece levemente esticada na horizontal.

---

## Estrutura do Projeto

| Arquivo                   | Descrição |
|---------------------------|-----------|
| `Lista_1_Exercicio_4.cpp` | Programa completo: inicialização da janela e do contexto OpenGL, shaders, listas de vértices da cabeça e do rosto, um VBO/VAO para cada lista e loop de renderização. |

Funções principais:

| Função            | Descrição |
|-------------------|-----------|
| `setupGeometry()` | Recebe uma lista de vértices, envia para a GPU (VBO) e retorna o VAO correspondente. |
| `setupShader()`   | Compila os shaders e retorna o programa de shader. |

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
./build/Lista1_4
```

---

## Checklist de Requisitos

- [x] Criação de janela e contexto OpenGL
- [x] Configuração de shaders e pipeline programável
- [x] Desenho em papel quadriculado (grade de 8×8)
- [x] Reprodução do desenho com primitivas OpenGL
- [x] Uso de mais de um VAO e de mais de uma chamada de desenho

---

## Referências e/ou créditos

- Código base: exemplo *Hello Triangle* da disciplina, adaptado por Rossana Baptista Queiroz — [repositório da turma](https://github.com/fellowsheep/PG2026-2)
- Desenho original: rosto do *creeper*, do jogo Minecraft (Mojang Studios)
- LearnOpenGL — [Hello Triangle](https://learnopengl.com/Getting-started/Hello-Triangle)
