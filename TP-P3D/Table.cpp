/*****************************************************************//**
 * \file   Table.cpp
 * \brief  Implementa��o da mesa de bilhar
 *
 * \authors Andr� Cerqueira, Alexandre Marques, Nuno Fernandes e Tom�s Carvalho
 * \date    Junho 2024
 *********************************************************************/

// Includes
#include "table.h"


/**
* \brief Inicializar os v�rtices da mesa de bilhar
*/
Table::Table() {
    vertices = {
        // Frente
        glm::vec3(-15.0f, -1.0f,  9.0f),
        glm::vec3(15.0f, -1.0f,  9.0f),
        glm::vec3(15.0f,  1.0f,  9.0f),
        glm::vec3(-15.0f,  1.0f,  9.0f),

        // Tr�s
        glm::vec3(-15.0f, -1.0f, -9.0f),
        glm::vec3(-15.0f,  1.0f, -9.0f),
        glm::vec3(15.0f,  1.0f, -9.0f),
        glm::vec3(15.0f, -1.0f, -9.0f),

        // Direita
        glm::vec3(15.0f, -1.0f,  9.0f),
        glm::vec3(15.0f, -1.0f, -9.0f),
        glm::vec3(15.0f,  1.0f, -9.0f),
        glm::vec3(15.0f,  1.0f,  9.0f),

        // Esquerda
        glm::vec3(-15.0f, -1.0f,  9.0f),
        glm::vec3(-15.0f,  1.0f,  9.0f),
        glm::vec3(-15.0f,  1.0f, -9.0f),
        glm::vec3(-15.0f, -1.0f, -9.0f),

        // Cima
        glm::vec3(-15.0f,  1.0f,  9.0f),
        glm::vec3(15.0f,  1.0f,  9.0f),
        glm::vec3(15.0f,  1.0f, -9.0f),
        glm::vec3(-15.0f,  1.0f, -9.0f),

        // Baixo
        glm::vec3(-15.0f, -1.0f,  9.0f),
        glm::vec3(-15.0f, -1.0f, -9.0f),
        glm::vec3(15.0f, -1.0f, -9.0f),
        glm::vec3(15.0f, -1.0f,  9.0f)
    };

    colors = {
        glm::vec3(0.0f, 0.5f, 0.0f), // Frente - verde escuro
        glm::vec3(0.0f, 0.7f, 0.0f), // Esquerda - verde claro
        glm::vec3(0.0f, 0.7f, 0.0f), // Direita - verde m�dio
        glm::vec3(0.0f, 0.5f, 0.0f), // Esquerda - verde claro
        glm::vec3(0.0f, 1.0f, 0.0f), // Cima - verde escuro
        glm::vec3(0.0f, 0.7f, 0.0f)  // Baixo - verde m�dio
    };
}


Table::~Table() {
    // Limpar mem�ria, se necess�rio
}


std::vector<glm::vec3> Table::getVertices() const {
    return vertices;
}

std::vector<glm::vec3> Table::getColors() const {
    return colors;
}

float Table::getHeight() const {
    // Altura m�xima da mesa
    float max_height = 0.0f;

    for (const auto& vertex : vertices) {
        if (vertex.y > max_height) {
            max_height = vertex.y;
        }
    }

    return max_height;
}

glm::vec3 Table::getRandomPositionOnTable() const {
    // Largura e comprimento da mesa
    float tableWidth = 26.0f;
    float tableLength = 15.0f;

    // Gerar coordenadas aleat�rias dentro dos limites da mesa
    float randomX = getRandomValue(-tableWidth / 2.0f, tableWidth / 2.0f);
    float randomZ = getRandomValue(-tableLength / 2.0f, tableLength / 2.0f);

    // A altura da posi��o ser� a altura m�xima da mesa
    float randomY = getHeight();

    return glm::vec3(randomX, randomY, randomZ);
}
