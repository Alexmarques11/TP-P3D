#include "Utils.h"

float getRandomValue(float min, float max) {
    // Cria um gerador de números aleatórios
    std::random_device rd;  // Obtém uma semente aleatória do dispositivo de entropia do sistema operacional
    std::mt19937 gen(rd()); // Mersenne Twister padrão inicializado com rd()

    // Cria uma distribuição uniforme para números reais no intervalo [min, max]
    std::uniform_real_distribution<float> dis(min, max);

    // Gera e retorna um valor aleatório dentro do intervalo
    return dis(gen);
}