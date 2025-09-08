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
    { time openssl enc -aes-256-cbc -pbkdf2 -in "$file" \
    -out aes_enc -pass pass:"$key"; } 2>> "$output_file"
    echo "" >> "$output_file"

    echo "AES Decriptação:" >> "$output_file"
    { time openssl enc -d -aes-256-cbc -pbkdf2 -in aes_enc \
    -out aes_dec -pass pass:"$key"; } 2>> "$output_file"
    echo "" >> "$output_file"

    echo "Verificando integridade do AES" >> "$output_file"
    echo "Diferenças (se houver):" >> "$output_file"
    diff "$file" aes_dec >> "$output_file"
    echo "" >> "$output_file"

    rm aes_enc aes_dec

    echo "Cifra Própria Encriptação:" >> "$output_file"
    { time ./cifra "$file" cifrado "$key"; } 2>> "$output_file"
    echo "" >> "$output_file"


    echo "Cifra Própria Decriptação:" >> "$output_file"
    { time ./decifra cifrado decifrado "$key"; } 2>> "$output_file"
    echo "" >> "$output_file"

    echo "Verificando integridade da cifra própria" >> "$output_file"
    echo "Diferenças (se houver):" >> "$output_file"
    diff "$file" decifrado >> "$output_file" 
    echo "" >> "$output_file"

    rm cifrado decifrado
done

make clean

echo "Testes concluídos."









