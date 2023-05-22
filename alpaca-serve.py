from http.server import BaseHTTPRequestHandler, HTTPServer
import json
import socket


class SimpleServerHandler(BaseHTTPRequestHandler):
    def do_POST(self):
        if self.path == '/':
            content_length = int(self.headers['Content-Length'])
            post_data = self.rfile.read(content_length)
            try:
                json_data = json.loads(post_data)
                prompt = json_data.get('prompt')
                if prompt:
                    response = ""
                    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                        s.connect(('127.0.0.1', 65530))
                        s.sendall(prompt.encode('utf-8'))
                        response = str(s.recv(10 * 1024), encoding='utf-8')
                    self.send_response(200)
                    self.send_header('Content-type', 'application/json')
                    self.end_headers()
                    response_json = {'response': response}
                    self.wfile.write(json.dumps(response_json).encode('utf-8'))
                else:
                    self.send_response(400)
                    self.send_header('Content-type', 'text/plain')
                    self.end_headers()
                    self.wfile.write(b'Bad Request: Prompt field is missing')
            except ValueError:
                self.send_response(400)
                self.send_header('Content-type', 'text/plain')
                self.end_headers()
                self.wfile.write(b'Bad Request: Invalid JSON')
        else:
            self.send_response(404)
            self.send_header('Content-type', 'text/plain')
            self.end_headers()
            self.wfile.write(b'404 Not Found')


def main():
    host = 'localhost'
    port = 8080
    server_address = (host, port)
    httpd = HTTPServer(server_address, SimpleServerHandler)
    print(f'Starting server on {host}:{port}...')
    httpd.serve_forever()


if __name__ == '__main__':
    main()