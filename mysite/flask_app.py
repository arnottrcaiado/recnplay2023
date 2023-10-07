#
# Projeto Desfile Maker 2 - Rec N Play 2023
#
#
# Data: 6/10/2023
# Autor: Arnott Ramos Caiado
#
from flask import Flask, request, jsonify
from datetime import datetime
import os
import time

os.environ["TZ"] = "America/Recife"
time.tzset()

header_key = '*'


app = Flask(__name__)

class GerenciadorCores:
    def __init__(self):
        self.votos = {'vermelho': 0, 'azul': 0, 'verde': 0}
        self.ultima_escolha = None

    def incrementar_voto(self, cor):
        if cor in self.votos:
            self.votos[cor] += 1
            self.ultima_escolha = cor
            return True
        return False

gerenciador = GerenciadorCores()

@app.route('/votar/<cor>', methods=['GET'])
def votar(cor):
    if gerenciador.incrementar_voto(cor):
        return jsonify({'escolha': f'cor {cor}'}), 200
    else:
        return jsonify({'erro': 'Cor inválida'}), 400

@app.route('/resultados', methods=['GET'])
def obter_resultados():
    return jsonify(gerenciador.votos), 200

@app.route('/ultima_escolha', methods=['GET'])
def obter_ultima_escolha():
    return jsonify({'ultima_escolha': gerenciador.ultima_escolha}), 200

if __name__ == '__main__':
    app.run(debug=True)
