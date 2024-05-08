/*****************************************************************//**
 * \file   Table.h
 * \brief  Declarações das funções e variaveis relacionadas á mesa de bilhar
 *
 * \authors André Cerqueira, Alexandre Marques, Nuno Fernandes e Tomás Carvalho
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
