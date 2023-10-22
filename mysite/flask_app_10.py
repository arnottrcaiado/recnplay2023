#
# Projeto Desfile Maker 2 - Rec N Play 2023
#
#
# Data: 6/10/2023
# Autor: Arnott Ramos Caiado
#
from flask import Flask, jsonify, render_template, redirect
import datetime

import os
import time

os.environ["TZ"] = "America/Recife"
time.tzset()

header_key = '*'


app = Flask(__name__)

class GerenciadorCores:
    def __init__(self):
        self.votos = {'vermelho': 0, 'azul': 0, 'verde': 0,'ultimo': None, 'ledgrande': True}
        self.registro_votos = []

    def incrementar_voto(self, cor):
        if cor in self.votos:
            self.votos[cor] += 1
            self.votos['ultimo'] = cor
            momento_voto = datetime.datetime.now()
            self.registro_votos.append({'cor': cor, 'momento_voto': momento_voto})
            return True
        return False

    def obter_registro_votos(self):
        return self.registro_votos

    def obter_ultimo_voto(self):
        return self.votos['ultimo']

    def obter_votos(self):
        return self.votos

    def mudarEstadoLed(self) :
        if self.votos['ledgrande'] == True :
            self.votos['ledgrande'] = False
        else :
            self.votos['ledgrande'] = True


gerenciador = GerenciadorCores()

@app.route('/')
def principal():
    global generciador
    return render_template("index.html", red=gerenciador.votos['vermelho'], blue=gerenciador.votos['azul'], green=gerenciador.votos['verde'])

# endpoint para realizar votacao
# https://recnplay2023.pythonanywhere.com/votar/"cor"
# exemplo
# https://recnplay2023.pythonanywhere.com/votar/vermelho
#
#@app.route('/votar/<cor>', methods=['POST','GET'])
#def votar(cor):
#    if gerenciador.incrementar_voto(cor):
##        # return jsonify({'mensagem': f'Voto registrado para a cor {cor}'}), 200
#        return redirect('/')
#    else:
#        return jsonify({'mensagem': 'Cor inválida'}), 400

@app.route('/votar/<cor>', methods=['POST'])
def votar(cor):
    global gerenciador
    if cor in ['vermelho', 'azul', 'verde']:
        if gerenciador.incrementar_voto(cor):
            return redirect('/')
    return jsonify({'mensagem': 'Cor inválida'}), 400


# endpoint para mostrar resultados gerais
# https://recnplay2023.pythonanywhere.com/resultados
#
@app.route('/resultados', methods=['GET'])
def obter_resultados():
    global gerenciador
    return jsonify(gerenciador.obter_votos()), 200

@app.route('/ledgrande', methods=['POST', 'GET'])
def ledGrande():
    global gerenciador
    gerenciador.mudarEstadoLed()
    return redirect('/')

# endpoint para mostrar ultimo voto
# https://recnplay2023.pythonanywhere.com/ultima_escolha
#
@app.route('/ultima_escolha', methods=['GET'])
def obter_ultima_escolha():
    global gerenciador
    return jsonify({'ultima_escolha': gerenciador.obter_ultimo_voto()}), 200

# endpoint para mostrar todos os votos - lista
# https://recnplay2023.pythonanywhere.com/registro_votos
#
@app.route('/registro_votos', methods=['GET'])
def obter_registro_votos():
    global gerenciador
    return jsonify(gerenciador.obter_registro_votos()), 200


if __name__ == '__main__':
    app.run()