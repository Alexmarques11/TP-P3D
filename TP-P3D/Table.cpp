/*****************************************************************//**
 * \file   Table.cpp
 * \brief  Implementação da mesa de bilhar
 *
 * \authors André Cerqueira, Alexandre Marques, Nuno Fernandes e Tomás Carvalho
 * \date    Junho 2024
 *********************************************************************/

// Includes
#include "table.h"


/**
* \brief Inicializar os vértices da mesa de bilhar
*/
Table::Table() {
    vertices = {
        // Frente
        glm::vec3(-9.0f, -0.5f,  5.5f),
        glm::vec3(9.0f, -0.5f,  5.5f),
        glm::vec3(9.0f,  0.5f,  5.5f),
        glm::vec3(-9.0f,  0.5f,  5.5f),

        // Trás
        glm::vec3(-9.0f, -0.5f, -5.5f),
        glm::vec3(-9.0f,  0.5f, -5.5f),
        glm::vec3(9.0f,  0.5f, -5.5f),
        glm::vec3(9.0f, -0.5f, -5.5f),

        // Direita
        glm::vec3(9.0f, -0.5f,  5.5f),
        glm::vec3(9.0f, -0.5f, -5.5f),
        glm::vec3(9.0f,  0.5f, -5.5f),
        glm::vec3(9.0f,  0.5f,  5.5f),

        // Esquerda
        glm::vec3(-9.0f, -0.5f,  5.5f),
        glm::vec3(-9.0f,  0.5f,  5.5f),
        glm::vec3(-9.0f,  0.5f, -5.5f),
        glm::vec3(-9.0f, -0.5f, -5.5f),

        // Cima
        glm::vec3(-9.0f,  0.5f,  5.5f),
        glm::vec3(9.0f,  0.5f,  5.5f),
        glm::vec3(9.0f,  0.5f, -5.5f),
        glm::vec3(-9.0f,  0.5f, -5.5f),

        // Baixo
        glm::vec3(-9.0f, -0.5f,  5.5f),
        glm::vec3(-9.0f, -0.5f, -5.5f),
        glm::vec3(9.0f, -0.5f, -5.5f),
        glm::vec3(9.0f, -0.5f,  5.5f)
    };

    colors = {
        glm::vec3(0.0f, 0.5f, 0.0f), // Frente - verde escuro
        glm::vec3(0.0f, 0.7f, 0.0f), // Esquerda - verde claro
        glm::vec3(0.0f, 0.7f, 0.0f), // Direita - verde médio
        glm::vec3(0.0f, 0.5f, 0.0f), // Esquerda - verde claro
        glm::vec3(0.0f, 1.0f, 0.0f), // Cima - verde escuro
        glm::vec3(0.0f, 0.7f, 0.0f)  // Baixo - verde médio
    };
}


Table::~Table() {
    // Limpar memória, se necessário
}


std::vector<glm::vec3> Table::getVertices() const {
    return vertices;
}

std::vector<glm::vec3> Table::getColors() const {
    return colors;
}