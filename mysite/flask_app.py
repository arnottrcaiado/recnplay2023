#
# Projeto Desfile Maker 2 - Rec N Play 2023
#
#
# Data: 6/10/2023
# Autor: Arnott Ramos Caiado
#
from flask import Flask, jsonify, render_template, redirect
from flask_sqlalchemy import SQLAlchemy
import datetime

import os
import time

os.environ["TZ"] = "America/Recife"
time.tzset()

app = Flask(__name__)

SQLALCHEMY_DATABASE_URI = "mysql+mysqlconnector://{username}:{password}@{hostname}/{databasename}".format(
    username="recnplay2023",
    password="TtCaiado",
    hostname="recnplay2023.mysql.pythonanywhere-services.com",
    databasename="recnplay2023$default"
)

app.config["SQLALCHEMY_DATABASE_URI"] = SQLALCHEMY_DATABASE_URI
app.config["SQLALCHEMY_POOL_RECYCLE"] = 299
db = SQLAlchemy(app)


class ResultadoVotacao(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    vermelho = db.Column(db.Integer, default=0)
    azul = db.Column(db.Integer, default=0)
    verde = db.Column(db.Integer, default=0)
    ultima_cor_votada = db.Column(db.String(255), default=None)
    data_hora_ultimo_voto = db.Column(db.DateTime)
    ledgrande = db.Column(db.Boolean, default = True )

class Voto(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    cor_votada = db.Column(db.String(255))
    momento_voto = db.Column(db.DateTime, default=datetime.datetime.utcnow)

@app.route('/')
def principal():
    resultados = ResultadoVotacao.query.first()
    return render_template("index.html", red=resultados.vermelho, green=resultados.verde, blue=resultados.azul)

@app.route('/votar/<cor>', methods=['POST'])
def votar(cor):
    if cor in ['vermelho', 'azul', 'verde']:
        resultados = ResultadoVotacao.query.first()
        if resultados:
            setattr(resultados, cor, getattr(resultados, cor) + 1)
            resultados.ultima_cor_votada = cor
            resultados.data_hora_ultimo_voto = datetime.datetime.utcnow()
        else:
            resultados = ResultadoVotacao(vermelho=0, azul=0, verde=0, ultima_cor_votada=None, data_hora_ultimo_voto=None)
            setattr(resultados, cor, getattr(resultados, cor) + 1)
            resultados.ultima_cor_votada = cor
            resultados.data_hora_ultimo_voto = datetime.datetime.utcnow()

        voto = Voto(cor_votada=cor)
        db.session.add(voto)
        db.session.commit()
        return redirect('/')
    return jsonify({'mensagem': 'Cor inválida'}), 400

@app.route('/resultados', methods=['GET'])
def obter_resultados():
    resultados = ResultadoVotacao.query.first()
    if resultados:
        resultado_dict = {
            'vermelho': resultados.vermelho,
            'azul': resultados.azul,
            'verde': resultados.verde,
            'ultima_cor_votada': resultados.ultima_cor_votada,
            'data_hora_ultimo_voto': resultados.data_hora_ultimo_voto.strftime("%Y-%m-%d %H:%M:%S") if resultados.data_hora_ultimo_voto else None
        }
        return jsonify(resultado_dict), 200
    else:
        return jsonify({'mensagem': 'Nenhum voto registrado'}), 200

@app.route('/ultima_escolha', methods=['GET'])
def obter_ultima_escolha():
    ultimo_voto = Voto.query.order_by(Voto.momento_voto.desc()).first()
    if ultimo_voto:
        return jsonify({'ultima_escolha': ultimo_voto.cor_votada}), 200
    else:
        return jsonify({'mensagem': 'Nenhum voto registrado'}), 200

@app.route('/ledgrande', methods=['POST', 'GET'])
def ledGrande():
    resultados = ResultadoVotacao.query.first()
    if resultados:
        if resultados.ledgrande == True :
            resultados.ledgrande = False
        else :
            resultados.ledgrande = True
        db.session.commit()

    return redirect('/')

@app.route('/registro_votos', methods=['GET'])
def obter_registro_votos():
    votos = Voto.query.all()
    registro_votos = [{'cor': v.cor_votada, 'momento_voto': v.momento_voto} for v in votos]
    return jsonify(registro_votos), 200

if __name__ == '__main__':
    db.create_all()
      # Crie o primeiro registro para ResultadoVotacao
    primeiro_registro = ResultadoVotacao(
        vermelho=0,
        azul=0,
        verde=0,
        ultima_cor_votada=None,
        data_hora_ultimo_voto=None
    )
    db.session.add(primeiro_registro)
    db.session.commit()
    app.run()
