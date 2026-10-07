# Lista 1 — Exercício 2: Círculo e formas derivadas

## Descrição do Projeto

Desenho de um círculo na tela, usando a equação paramétrica do círculo para gerar os vértices, e de formas derivadas dele:

- **Círculo** (forma inicial);
- **a)** octógono;
- **b)** pentágono;
- **c)** pac-man;
- **d)** fatia de pizza.

Todas as formas saem da mesma equação:

```
x = centroX + r · cos(θ)
y = centroY + r · sin(θ)
```

- **Círculo, octógono e pentágono:** N pontos distribuídos na volta inteira (θ de 0 a 2π, passo 2π/N). Um "círculo" é só um polígono com muitos lados (N = 100); com N = 8 ou N = 5, o mesmo código gera o octógono e o pentágono.
- **Pac-man e fatia de pizza:** um *setor* do círculo, ou seja, só o arco entre um ângulo inicial e um final, ligado ao centro. O pac-man vai de 30° a 330° (boca de 60° virada para a direita) e a fatia de pizza vai de −30° a 30° (exatamente a "boca" do pac-man).

Todas as formas são desenhadas com a primitiva `GL_TRIANGLE_FAN`. Cada forma tem o seu próprio VAO, criado uma única vez antes do loop de renderização; trocar de forma é apenas escolher qual VAO é usado no desenho.

### Controles

| Tecla | Ação                                  |
|-------|---------------------------------------|
| `→`   | Próxima forma                         |
| `←`   | Forma anterior                        |
| `ESC` | Fecha o programa                      |

A cada troca, o nome da forma atual é exibido no terminal. Ao passar da última forma, volta para a primeira (e vice-versa).

### Limitações conhecidas

- A janela não é quadrada (800×600) e não há matriz de projeção, então as formas aparecem levemente achatadas na horizontal.

---

## Estrutura do Projeto

| Arquivo                   | Descrição |
|---------------------------|-----------|
| `Lista_1_Exercicio_2.cpp` | Programa completo: inicialização da janela e do contexto OpenGL, shaders, geração dos vértices de cada forma, um VBO/VAO por forma, callback de teclado e loop de renderização. |

Funções principais:

| Função                                 | Descrição |
|----------------------------------------|-----------|
| `gerarVerticesParametricosContinuos()` | Gera N pontos igualmente espaçados na volta inteira do círculo (círculo e polígonos regulares). |
| `gerarVerticesSetor()`                 | Gera o centro e os pontos do arco entre dois ângulos (pac-man e fatia de pizza). |
| `setupGeometry()`                      | Recebe uma lista de vértices, envia para a GPU (VBO) e retorna o VAO correspondente. |
| `nomeDoModo()`                         | Converte o modo atual em texto, para as mensagens no terminal. |

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
./build/2
```

---

## Checklist de Requisitos

- [x] Criação de janela e contexto OpenGL
- [x] Configuração de shaders e pipeline programável
- [x] Desenho de um círculo com a equação paramétrica
- [x] a) Octógono
- [x] b) Pentágono
- [x] c) Pac-man
- [x] d) Fatia de pizza

---

## Referências e/ou créditos

- Código base: exemplo *Hello Triangle* da disciplina, adaptado por Rossana Baptista Queiroz — [repositório da turma](https://github.com/fellowsheep/PG2026-2)
- Material da disciplina: slides *Introdução à OpenGL Moderna e Shaders* (equação paramétrica do círculo e `GL_TRIANGLE_FAN`)
- LearnOpenGL — [Hello Triangle](https://learnopengl.com/Getting-started/Hello-Triangle)
- Documentação do GLFW — [Input guide](https://www.glfw.org/docs/latest/input_guide.html)
