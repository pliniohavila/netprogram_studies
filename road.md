


# Funcionalidades Básicas

Um webserver, ou servidor web, é responsável por receber solicitações HTTP de clientes (geralmente navegadores web) e enviar respostas adequadas. As funcionalidades básicas de um webserver incluem:

Escuta de requisições HTTP: O servidor deve ser capaz de aceitar conexões TCP/IP entrantes na porta padrão (geralmente 80 para HTTP ou 443 para HTTPS) e analisar as requisições HTTP recebidas.
Análise da Requisição HTTP: O servidor precisa analisar os diferentes componentes da requisição HTTP, como o método (GET, POST, etc.), cabeçalhos, URL solicitada e quaisquer dados enviados no corpo da requisição.
Manipulação de Recursos: Com base na URL solicitada, o servidor deve ser capaz de localizar e servir o recurso apropriado, como um arquivo HTML, CSS, JavaScript, imagem ou outro tipo de arquivo.
Geração de Resposta HTTP: Após obter o recurso solicitado, o servidor deve construir uma resposta HTTP adequada, incluindo o código de status apropriado, os cabeçalhos necessários e o conteúdo a ser enviado ao cliente.
Envio da Resposta HTTP: O servidor deve transmitir a resposta HTTP de volta ao cliente que originou a requisição.
Tratamento de Erros: O servidor deve ser capaz de lidar com erros, como recursos não encontrados (404), erros do servidor (500) e outros, enviando as respostas HTTP adequadas.
Suporte a protocolos web: Além do HTTP básico, um servidor web moderno geralmente suporta recursos adicionais, como HTTP/2, WebSockets, upload de arquivos, compactação de dados, entre outros.
Logging e Monitoramento: Registrar as requisições recebidas e as respostas enviadas em logs é uma prática comum para fins de depuração, análise de tráfego e segurança.
Configuração: Um servidor web geralmente oferece opções de configuração, como definir diretórios raiz, portas de escuta, permissões de acesso, redirecionamentos e outras configurações personalizadas.



Referências de código 
https://github.com/arut/htserve
kHTTPd 


https://ruslanspivak.com/lsbaws-part1/ 
https://web.mit.edu/rust-lang_v1.25/arch/amd64_ubuntu1404/share/doc/rust/html/book/second-edition/ch20-00-final-project-a-web-server.html 
Rust Servers services and apps
https://m4nnb3ll.medium.com/webserv-building-a-non-blocking-web-server-in-c-98-a-42-project-04c7365e4ec7