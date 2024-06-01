# Simulador de Mesa de Bilhar (Pool Table)

Este projeto implementa um simulador 3D de uma mesa de bilhar utilizando OpenGL, GLFW e GLEW. Ele permite visualizar e interagir com uma mesa de bilhar virtual, incluindo as bolas, e oferece recursos básicos de iluminação e controle de câmera.

## Funcionalidades

- **Visualização 3D**: Renderiza uma mesa de bilhar e 16 bolas em um ambiente 3D.
- **Iluminação**: Suporta diferentes tipos de luzes (ambiente, direcional, pontual e spot) que podem ser ativadas/desativadas individualmente.
- **Controle de Câmera**: Permite mover a câmera em torno da mesa clicando e arrastando com o botão esquerdo do mouse, e ajustar o zoom usando o scroll do mouse.
- **Movimento da Bola**: A barra de espaço inicia o movimento da bola 9.
- **Colisões**: Detecta colisões entre as bolas e entre as bolas e as paredes da mesa.

## Estrutura do Projeto

O projeto está organizado em vários arquivos de código-fonte:

- **main.cpp**: Arquivo principal do projeto, responsável por inicializar a aplicação, configurar o OpenGL, carregar os shaders, criar os objetos da cena e executar o loop principal do jogo.
- **Ball.h/Ball.cpp**: Define e implementa a classe Ball, que representa uma bola de bilhar, incluindo suas propriedades (posição, velocidade, textura) e métodos para carregamento, renderização e atualização.
- **Table.h/Table.cpp**: Define e implementa a classe Table, que representa a mesa de bilhar, incluindo sua geometria, material e métodos para carregamento e renderização.
- **Camera.h/Camera.cpp**: Define e implementa a classe Camera, que controla a posição, orientação e zoom da câmera, e responde aos eventos de mouse e scroll para manipulação da câmera.
- **Lights.h/Lights.cpp**: Define e implementa a classe Lights, que gerencia os diferentes tipos de luzes no jogo (ambiente, direcional, pontual e spot).
- **LoadShaders.h/LoadShaders.cpp**: Contém funções auxiliares para carregar, compilar e vincular shaders.

## Como Compilar e Executar

1. Certifique-se de ter as bibliotecas GLFW, GLEW e GLM instaladas em seu sistema.
2. Compile o projeto usando um compilador C++ compatível com OpenGL.
3. Execute o executável gerado.

## Controles

- Clique e arraste com o botão esquerdo do mouse para mover a câmera.
- Use o scroll do mouse para ajustar o zoom.
- Pressione a barra de espaço para iniciar o movimento da bola 9.
- Pressione as teclas `1`, `2`, `3` e `4` para alternar as luzes ambiente, direcional, pontual e spot, respectivamente.
