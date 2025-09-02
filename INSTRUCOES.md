# Matrix Blocker - Versão Simples

## Como usar:

1. Execute `compilar.bat` para compilar o programa
2. Execute `blocker_simples.exe` como administrador
3. O programa ficará rodando em segundo plano
4. Edite `sites_bloqueados.txt` para adicionar/remover sites e palavras

## Funcionamento:

- Monitora tudo que é digitado no computador
- Quando detecta uma palavra ou site da lista, mostra o popup da Matrix
- Fecha todos os navegadores automaticamente
- O popup só pode ser fechado após 5 minutos
- Durante os 5 minutos, impede abertura de navegadores

## Mensagem de bloqueio:

"Voce nao pode ver fora da matrix Neo. A matrix pegou voce! Site de pornografia bloqueado!"

## Arquivo de configuração:

Edite `sites_bloqueados.txt` para personalizar a lista de sites e palavras bloqueadas.

## Como fazer o programa iniciar automaticamente com o Windows:

### Opção 1: Pasta de Inicialização (Mais Simples)

**Para o usuário atual:**
- Pressione `Win + R`, digite `shell:startup` e pressione Enter
- Cole o arquivo `blocker_simples.exe` nesta pasta

**Para todos os usuários (recomendado para escolas):**
- Pressione `Win + R`, digite `shell:common startup` e pressione Enter  
- Cole o arquivo `blocker_simples.exe` nesta pasta (precisa ser administrador)
- Cole também o arquivo `sites_bloqueados.txt` na mesma pasta

### Opção 2: Registro do Windows

1. Abra o `regedit` como administrador
2. Navegue para: `HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Run`
3. Crie uma nova entrada do tipo String:
   - **Nome:** `MatrixBlocker`
   - **Valor:** `C:\caminho\completo\para\blocker_simples.exe`

### Opção 3: Instalador Automático

Crie um arquivo `instalar_startup.bat` com o seguinte conteúdo:

\`\`\`bat
@echo off
echo Instalando Matrix Blocker na inicializacao...
copy blocker_simples.exe "%APPDATA%\Microsoft\Windows\Start Menu\Programs\Startup\"
copy sites_bloqueados.txt "%APPDATA%\Microsoft\Windows\Start Menu\Programs\Startup\"
echo Instalacao concluida! O programa iniciara automaticamente na proxima reinicializacao.
pause
\`\`\`

Execute este arquivo como administrador para instalar automaticamente.

## Recomendação para Escolas:

Use a **Opção 1** com `shell:common startup` para que o bloqueador funcione para todos os usuários do computador, garantindo proteção completa independente de quem fizer login.

## Desinstalação:

Para remover o programa da inicialização automática, simplesmente delete o arquivo `blocker_simples.exe` da pasta onde foi colocado (startup ou registro).
