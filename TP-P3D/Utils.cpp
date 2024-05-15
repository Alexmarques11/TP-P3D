#include "Utils.h"

float getRandomValue(float min, float max) {
    // Cria um gerador de n�meros aleat�rios
    std::random_device rd;  // Obt�m uma semente aleat�ria do dispositivo de entropia do sistema operacional
    std::mt19937 gen(rd()); // Mersenne Twister padr�o inicializado com rd()

    // Cria uma distribui��o uniforme para n�meros reais no intervalo [min, max]
    std::uniform_real_distribution<float> dis(min, max);

    // Gera e retorna um valor aleat�rio dentro do intervalo
    return dis(gen);
}