/*****************************************************************//**
 * \file   Table.h
 * \brief  Declara��es das fun��es e variaveis relacionadas � mesa de bilhar
 *
 * \authors Andr� Cerqueira, Alexandre Marques, Nuno Fernandes e Tom�s Carvalho
 * \date    Junho 2024
 *********************************************************************/


#ifndef TABLE_H
#define TABLE_H

#include <vector>
#include <glm/glm.hpp>

class Table {
public:
    Table();
    ~Table();

    std::vector<glm::vec3> getVertices() const;
    std::vector<glm::vec3> getColors() const;

private:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> colors;
};

#endif
