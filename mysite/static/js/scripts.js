// Função para atualizar a contagem de votos na tela
function updateVoteCount(color, count) {
    document.getElementById(`${color.toLowerCase()}-votes`).textContent = count;
}

// Função para enviar um voto para a API POST
function sendVote(color) {
    fetch('/votar', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({ color }),
    })
    .then(response => response.json())
    .then(data => {
        if (data.success) {
            updateVoteCount(color, data.count);
        } else {
            alert('Erro ao registrar o voto.');
        }
    })
    .catch(error => {
        console.error('Erro:', error);
        alert('Erro ao conectar à API.');
    });
}

// Adicionar event listeners aos botões
document.getElementById('red-button').addEventListener('click', () => sendVote('Red'));
document.getElementById('green-button').addEventListener('click', () => sendVote('Green'));
document.getElementById('blue-button').addEventListener('click', () => sendVote('Blue'));