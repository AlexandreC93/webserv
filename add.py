# multiplication.py

from http.server import BaseHTTPRequestHandler, HTTPServer
from urllib.parse import urlparse, parse_qs

# Classe de gestion des requêtes HTTP
class RequestHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        # Analyse de l'URL
        parsed_path = urlparse(self.path)
        query_params = parse_qs(parsed_path.query)

        # Vérification des paramètres requis
        if 'number1' in query_params and 'number2' in query_params:
            # Récupération des nombres et calcul de la multiplication
            number1 = int(query_params['number1'][0])
            number2 = int(query_params['number2'][0])
            result = number1 * number2

            # Envoi de la réponse HTTP
            self.send_response(200)
            self.send_header('Content-type', 'text/html')
            self.end_headers()

            # Corps de la réponse
            self.wfile.write(bytes("<html><head><title>Résultat de la multiplication</title></head>", "utf-8"))
            self.wfile.write(bytes("<body>", "utf-8"))
            self.wfile.write(bytes("<h2>Résultat de la multiplication en Python</h2>", "utf-8"))
            self.wfile.write(bytes(f"<p>Résultat de {number1} * {number2} : {result}</p>", "utf-8"))
            self.wfile.write(bytes("</body></html>", "utf-8"))
        else:
            # En cas de paramètres manquants, renvoyer une erreur 400 (Bad Request)
            self.send_error(400, 'Bad Request')

# Fonction principale pour démarrer le serveur
def run(server_class=HTTPServer, handler_class=RequestHandler, port=8080):
    server_address = ('', port)
    httpd = server_class(server_address, handler_class)
    print(f'Starting server on port {port}...')
    httpd.serve_forever()

if __name__ == '__main__':
    run()
