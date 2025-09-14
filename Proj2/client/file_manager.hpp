#pragma once

#include <iostream>
#include <unistd.h>
#include <string.h>

#include <openssl/sha.h>
#include <openssl/evp.h>

#include <iomanip>
#include <iostream>
#include <sstream>

#include "../message_headers.hpp"

class FileManager{

/* 
Verificar se o arquivo [Nome_Arquivo.ext] existe no servidor.
Se o arquivo existir:

    Calcular o hash SHA (ex: SHA-256) do conteúdo completo do arquivo. (O aluno deve pesquisar como implementar/usar bibliotecas padrão para cálculo de SHA).
    Enviar o arquivo para o cliente, seguindo o protocolo de aplicação definido (ver seção “Protocolo de Aplicação”).
    A transferência deve suportar arquivos grandes (> 10 MB).

*/

public:
    
    FileManager();
    ~FileManager();

    void saveFile(char* filename, char* file, int size);
    bool checkSha(char* file, int size, char sha[SHA256_DIGEST_LENGTH]);

private:

    void setFileFullName(FileName* filename);

    char* full_filename;
    int full_name_size;

};