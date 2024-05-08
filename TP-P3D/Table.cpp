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
        glm::vec3(-9.0f, -0.5f,  5.5f),
        glm::vec3(9.0f, -0.5f,  5.5f),
        glm::vec3(9.0f,  0.5f,  5.5f),
        glm::vec3(-9.0f,  0.5f,  5.5f),

        // Tr�s
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