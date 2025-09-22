#!/bin/bash

files=(
    "The_Arctic_legions.txt" 
    "Arts_and_crafts_of_old_Japan.txt"
    "Middlemarch.txt"
    )
key="leon"
output_file="tempos_execucao.txt"

make

> "$output_file"

echo "Iniciando testes"

for file in "${files[@]}"; do
    echo "Processando: $file"

    echo "--- Resultados para: $file ---" >> "$output_file"
    echo "" >> "$output_file"

    echo "AES Encriptação:" >> "$output_file"
    ./aesEncrypt "$file" encrypted "$key" >> "$output_file"
    echo "" >> "$output_file"

    echo "AES Decriptação:" >> "$output_file"
    ./aesDecrypt encrypted decrypted "$key" >> "$output_file"
    echo "" >> "$output_file"

    echo "Verificando integridade do AES" >> "$output_file"
    echo "Diferenças (se houver):" >> "$output_file"
    diff "$file" decrypted >> "$output_file"
    echo "" >> "$output_file"

    rm encrypted decrypted

    echo "Cifra Própria Encriptação:" >> "$output_file"
    ./cifra "$file" cifrado "$key" >> "$output_file"
    echo "" >> "$output_file"


    echo "Cifra Própria Decriptação:" >> "$output_file"
    ./decifra cifrado decifrado "$key" >> "$output_file"
    echo "" >> "$output_file"

    echo "Verificando integridade da cifra própria" >> "$output_file"
    echo "Diferenças (se houver):" >> "$output_file"
    diff "$file" decifrado >> "$output_file" 
    echo "" >> "$output_file"

    rm cifrado decifrado
done

make clean

echo "Testes concluídos."









