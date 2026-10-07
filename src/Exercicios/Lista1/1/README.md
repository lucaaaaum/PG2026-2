# Lista 1 — Exercício 1: Dois triângulos

## Descrição do Projeto

Desenho de 2 triângulos na tela, formando uma "gravata-borboleta" que se encontra na origem, com três formas de desenho:

- **a)** apenas o polígono preenchido;
- **b)** apenas o contorno;
- **c)** apenas os pontos (vértices);
- **d)** as 3 formas juntas.

Cada forma de desenho pode ser ligada ou desligada pelo teclado, o que permite visualizar cada item separadamente ou combinados. O programa inicia com as 3 formas ligadas (item **d**).

### Controles

| Tecla | Ação                                         |
|-------|----------------------------------------------|
| `1`   | Liga/desliga o polígono preenchido (azul)    |
| `2`   | Liga/desliga o contorno (vermelho)           |
| `3`   | Liga/desliga os vértices (verde)             |
| `ESC` | Fecha o programa                             |

Para ver cada item isoladamente:

| Item                  | Teclas a pressionar (a partir do estado inicial) |
|-----------------------|--------------------------------------------------|
| a) Apenas preenchido  | `2` e `3`                                        |
| b) Apenas contorno    | `1` e `3`                                        |
| c) Apenas pontos      | `1` e `2`                                        |
| d) As 3 juntas        | nenhuma (estado inicial)                         |

A cada mudança, o estado da forma de desenho é exibido no terminal.

---

## Estrutura do Projeto

| Arquivo                   | Descrição |
|---------------------------|-----------|
| `Lista_1_Exercicio_1.cpp` | Programa completo: inicialização da janela e do contexto OpenGL, shaders, geometria (VBO/VAO com os 6 vértices dos 2 triângulos), callback de teclado e loop de renderização. |

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
./build/1
```

---

## Checklist de Requisitos

- [x] Criação de janela e contexto OpenGL
- [x] Configuração de shaders e pipeline programável
- [x] Desenho de 2 triângulos com o polígono preenchido
- [x] Desenho de 2 triângulos apenas com contorno
- [x] Desenho de 2 triângulos apenas como pontos
- [x] Desenho com as 3 formas juntas

---

## Referências e/ou créditos

- Código base: exemplo *Hello Triangle* da disciplina, adaptado por Rossana Baptista Queiroz — [repositório da turma](https://github.com/fellowsheep/PG2026-2)
- LearnOpenGL — [Hello Triangle](https://learnopengl.com/Getting-started/Hello-Triangle)
- Anton Gerdelan — [OpenGL context](https://antongerdelan.net/opengl/glcontext2.html)
- Documentação do GLFW — [Input guide](https://www.glfw.org/docs/latest/input_guide.html)
