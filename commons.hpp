#include <cstddef>
#include <string>

#define FIRST_RAIL 0
#define LAST_RAIL (railSize - 1)

#define DECODE 0
#define ENCODE 1

constexpr size_t BUFF_SIZE = 1024;
constexpr size_t MIN_RAIL_SIZE = 3;
constexpr size_t MAX_RAIL_SIZE = 32;

/**
 * @brief Le o conteudo de arqRead e o retorna em um buffer
 * @param[in] arqRead Arquivo com conteudo a ser lido
 * @param[in] size Tamanho do buffer retornado
 * @return Ponteiro para buffer de bytes com o conteúdo
 */
unsigned char* readFileToBuffer(FILE* arqRead, size_t* size);

/**
 * @brief Avisa o usuário sobre a forma correta de usar o programa.
 * @param[in] encode Booleano que indica qual programa está sendo executado.
 *            TRUE para codificação, FALSE para decodificação.
 */
void printCorrectUse(bool encode);

/**
 * @brief Busca a primeira ocorrência de uma sequência de caractéres sem espaços
 *        em uma string.
 * @param[in] s String a ser analizada.
 * @return String contendo uma sequência de caractéres sem espaços.
 */
std::string first_word(const std::string& s);

/**
 * @brief Busca a primeira ocorrência de uma sequência de caractéres sem espaços
 *        em uma string.
 * @param[in] buf String a ser analizada.
 * @param[in] len Tamanho da string @buf
 * @return String contendo uma sequência de caractéres sem espaços.
 */
std::string first_word(const unsigned char* buf, size_t len);
