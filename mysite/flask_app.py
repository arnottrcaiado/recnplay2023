#
# Projeto Desfile Maker 2 - Rec N Play 2023
#
#
# Data: 6/10/2023
# Autor: Arnott Ramos Caiado
#
from flask import Flask, request, jsonify
import datetime
import threading

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
        self.registro_votos = []
        self.contagem_por_minuto = {'vermelho': 0, 'azul': 0, 'verde': 0}

        # Inicia um cronômetro para calcular os votos por minuto
        self.iniciar_cronometro()

    def incrementar_voto(self, cor):
        if cor in self.votos:
            self.votos[cor] += 1
            self.ultima_escolha = cor
            momento_voto = datetime.datetime.now()
            self.registro_votos.append({'cor': cor, 'momento_voto': momento_voto})
            return True
        return False

    def obter_registro_votos(self):
        return self.registro_votos

    def calcular_votos_por_minuto(self):
        while True:
            # Obtém o momento atual
            momento_atual = datetime.datetime.now()

            # Inicializa a contagem por minuto
            contagem_por_minuto = {'vermelho': 0, 'azul': 0, 'verde': 0}

            # Calcula a contagem por minuto com base nos votos no último minuto
            for voto in self.registro_votos:
                diferenca_tempo = momento_atual - voto['momento_voto']
                if diferenca_tempo.total_seconds() <= 60:
                    contagem_por_minuto[voto['cor']] += 1

            # Atualiza a contagem por minuto
            self.contagem_por_minuto = contagem_por_minuto

            # Aguarde 1 minuto antes de verificar novamente
            threading.Event().wait(60)

    def iniciar_cronometro(self):
        # Inicia uma thread para calcular os votos por minuto
        cronometro_thread = threading.Thread(target=self.calcular_votos_por_minuto)
        cronometro_thread.daemon = True
        cronometro_thread.start()

gerenciador = GerenciadorCores()

@app.route('/votar/<cor>', methods=['GET'])
def votar(cor):
    if gerenciador.incrementar_voto(cor):
        return jsonify({'mensagem': f'Voto registrado para a cor {cor}'}), 200
    else:
        return jsonify({'mensagem': 'Cor inválida'}), 400

@app.route('/resultados', methods=['GET'])
def obter_resultados():
    return jsonify(gerenciador.votos), 200

@app.route('/ultima_escolha', methods=['GET'])
def obter_ultima_escolha():
    return jsonify({'ultima_escolha': gerenciador.ultima_escolha}), 200

@app.route('/registro_votos', methods=['GET'])
def obter_registro_votos():
    return jsonify(gerenciador.obter_registro_votos()), 200

@app.route('/votos_por_minuto', methods=['GET'])
def obter_votos_por_minuto():
    return jsonify(gerenciador.contagem_por_minuto), 200

if __name__ == '__main__':
    app.run(debug=True)
