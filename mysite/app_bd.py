from flask import Flask, request, jsonify
from flask_sqlalchemy import SQLAlchemy
from datetime import datetime

app = Flask(__name__)
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///votos.db'  # Use SQLite como exemplo
db = SQLAlchemy(app)

class Voto(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    cor = db.Column(db.String(50), nullable=False)
    momento_voto = db.Column(db.DateTime, default=datetime.utcnow)

class GerenciadorCores:
    def __init__(self):
        self.votos = {'vermelho': 0, 'azul': 0, 'verde': 0}
        self.ultima_escolha = None

    def incrementar_voto(self, cor):
        if cor in self.votos:
            self.votos[cor] += 1
            self.ultima_escolha = cor
            voto = Voto(cor=cor)
            db.session.add(voto)
            db.session.commit()
            return True
        return False

    def obter_registro_votos(self):
        return Voto.query.all()

gerenciador = GerenciadorCores()

@app.route('/votar/<cor>', methods=['POST'])
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
    votos = [{'cor': voto.cor, 'momento_voto': voto.momento_voto} for voto in gerenciador.obter_registro_votos()]
    return jsonify(votos), 200

if __name__ == '__main__':
    db.create_all()
    app.run(debug=True)
