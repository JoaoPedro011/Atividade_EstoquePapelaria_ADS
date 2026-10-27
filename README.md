# Projeto Final - Sistema de Estoque (Linguagem C)

Sistema de estoque em linha de comando, feito em C, com cadastro de produtos e categorias e persistencia em arquivos de texto.

## Como compilar e executar

```
gcc estoque.c -o estoque
./estoque          (no Windows PowerShell: .\estoque.exe)
```

Execute sempre de dentro da pasta do projeto: os arquivos de dados sao criados nela.

## Menu

```
1 - Cadastrar Produto
2 - Listar Produtos
3 - Pesquisar Produto (por codigo ou por nome)
4 - Categorias (Cadastrar, Listar, Voltar)
5 - Sair
```

## Regras principais

- Produto: codigo, nome, prateleira (A, B, C ou D), preco, quantidade, categoria e disponibilidade (calculada pelo sistema).
- Categoria: codigo e nome. Todo produto precisa de uma categoria que ja exista.
- Limites: 50 produtos e 20 categorias.
- Codigos de produto e de categoria nao podem se repetir.

## Arquivos de dados

- `categorias.txt` - uma categoria por linha: `codigo;nome`
- `produtos.txt` - um produto por linha: `codigo;nome;prateleira;preco;quantidade;disponivel;categoria`

Os dados sao carregados ao iniciar o programa e salvos a cada cadastro e ao sair. Se os arquivos nao existirem (primeira execucao), o sistema comeca com as listas vazias. Por isso os nomes nao podem conter `;`.
