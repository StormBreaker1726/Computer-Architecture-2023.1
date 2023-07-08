# Trabalho de Arquitetura de Computadores

Código referente ao trabalho da disciplina DCC057 - 2023.1-A - Arquitetura,
implementando um simulador MIPs com o método de Tomasulo.

## Como executar
```
$ make                   # gera o binário 'taca'.
$ ./taca                 # Conteúdo dos registradores, memória, etc. serão imprimidos na saída padrão.
$ ./taca <arquivo_saida> # Conteúdo dos registradores, memória, etc. será imprimido no arquivo de saída passado como argumento.
```

## Arquivos de teste
Arquivos de teste podem ser gerados a partir de um aquivo contendo código em
RISC-V através do script `scripts/asm.py`.


```
$ python scripts/asm.py -i <arquivo_risc_v> -o <arquivo_gerado>
```
