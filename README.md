# CompGraficaLista1
Lista de Exercícios 1 – Fundamentos de Computação Gráfica

O projeto necessita do glfw e do OpenGL OpenGL intalados no computador
CMake deve der configurado

O codigo possui um seletor que é operado com as setas do teclado
O seletor vai trocar entre as tarefas




Lista de Exercícios 1

1. O que é a GLSL? Quais os dois tipos de shaders são obrigatórios no pipeline programável da versão atual que trabalhamos em aula e o que eles processam?

    A GLSL (OpenGL Shading Language) é uma linguagem usada para escrever shaders em OpenGL, que são programas executados na GPU para controlar a renderização de gráficos 3D. Os dois tipos obrigatórios de shaders são o Vertex Shader, que processa vértices de objetos, e o Fragment Shader, que processa pixels da tela. O Vertex Shader transforma vértices, enquanto o Fragment Shader determina as cores dos pixels. Ambos trabalham juntos para produzir a imagem final renderizada.

2. O que são primitivas gráficas? Como fazemos o armazenamento dos vértices na OpenGL?

    Primitivas gráficas são formas geométricas básicas (pontos, linhas, polígonos) usadas na computação gráfica. Na OpenGL, os vértices que compõem essas primitivas são armazenados em buffers de vértices, que podem ser arrays simples, intercalados, ou buffers separados para cada atributo. Esses vértices são então associados a objetos gráficos e renderizados na cena.

3. Explique o que é VBO, VAO e EBO, e como se relacionam (se achar mais fácil, pode fazer um gráfico representando a relação entre eles).

    VBO (Vertex Buffer Object) armazena dados dos vértices, VAO (Vertex Array Object) encapsula a configuração de atributos dos vértices e EBO (Element Buffer Object) armazena índices para geometria. Eles trabalham juntos para renderizar geometria na OpenGL. O VAO mantém associações com VBOs e EBOs e fornece à OpenGL informações sobre como interpretar os dados para renderização.

4. Analise o código fonte do projeto Hello Triangle. Localize e relacione os conceitos de shaders, VBOs e VAO apresentados até então. Não precisa entregar nada neste exercício.

    OK

5. Faça o desenho de 2 triângulos na tela. Desenhe eles:
    a) Apenas com o polígono preenchido
    b) Apenas com contorno
    c) Apenas como pontos
    d) Com as 3 formas de desenho juntas
    i) Atualize o shader para receber uma cor de contorno

    (Implementado no codigo)

6. Faça o desenho de um círculo na tela, utilizando a equação paramétrica do círculo para gerar os vértices. Depois disso:
    a) Desenhe um octágono
    b) Desenhe um pentágono
    c) Desenhe um pac-man!
    d) Desenhe uma fatia de pizza
    e) DESAFIO: desenhe uma “estrela”

    (Implementado no codigo)

7. Desenhe uma espiral, assim:

    (Implementado no codigo)


8. Considerando o seguinte triângulo abaixo, formado pelos vértices P1, P2 e P3, respectivamente com as cores vermelho, verde e azul.
    a. Descreva uma possível configuração dos buffers (VBO, VAO e EBO) para representá-lo.
    b. Como estes atributos seriam identificados no vertex shader? Agora implemente!

    (Implementado no codigo)

9. Faça um desenho em um papel quadriculado (pode ser no computador mesmo) e reproduza-o utilizando primitivas em OpenGL. Neste exercício você poderá criar mais de um VAO e fazer mais de uma chamada de desenho para poder utilizar primitivas diferentes, se necessário.

    (Em falta)

10. Implemente (pode pegar do tutorial) uma classe para tratar os shaders a partir de arquivos.
    (implementado no codigo)
