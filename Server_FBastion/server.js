// server.js

// --- Импорт необходимых модулей ---
const WebSocket = require('ws');
const crypto = require('crypto');
const { Pool } = require('pg');

// --- Настройки ---
const port = 8080;

// --- Подключение к базе данных ---
const pool = new Pool({
    user: 'postgres',
    host: 'localhost', // Используем localhost для подключения к БД на этой же машине
    database: 'bastion_db',
    password: '532681034', // <-- ВАЖНО: Убедитесь, что это ваш актуальный пароль
    port: 5432,
});

// Проверяем соединение с базой данных при старте
pool.query('SELECT NOW()', (err, res) => {
  if (err) {
    console.error('❌ Ошибка подключения к базе данных!', err.stack);
  } else {
    console.log('✅ База данных успешно подключена.');
  }
});

// --- WebSocket Сервер ---
// Убираем параметр 'host', чтобы сервер слушал все доступные IP-адреса (0.0.0.0)
// Это позволит подключаться как по localhost, так и по локальному IP (например, 192.168.1.10)
const wss = new WebSocket.Server({ port });

console.log(`🚀 Сервер запущен на порту ${port} и доступен в локальной сети.`);

// --- Логика WebSocket-сервера ---
wss.on('connection', ws => {
    console.log('✅ Клиент подключился.');

    ws.on('message', async (message) => {
        try {
            const data = JSON.parse(message);
            console.log('📥 Получена команда:', data);

            switch (data.action) {
                // --- Логика создания нового аккаунта ---
                case 'create_account': {
                    const login = `Player${Math.floor(100000 + Math.random() * 900000)}`;
                    const token = crypto.randomBytes(16).toString('hex');
                    // ... здесь могут быть другие стартовые данные

                    // Используем полный SQL-запрос
                    const newUserQuery = 'INSERT INTO players(player_id, username, hero_name, token, last_login) VALUES($1, $2, $3, $4, NOW()) RETURNING player_id';
                    const values = [login, login, login, token];
                    
                    const result = await pool.query(newUserQuery, values);
                    
                    console.log(`[DB] Создан новый игрок: ${result.rows[0].player_id}`);

                    ws.send(JSON.stringify({
                        action: 'create_account_success',
                        player_id: login,
                        player_name: login,
                        token: token
                    }));
                    break;
                }
                
                // --- Логика входа по токену ---
                case 'login_with_token': {
                    const { player_id, token } = data;
                    const loginQuery = 'SELECT * FROM players WHERE player_id = $1 AND token = $2';
                    const result = await pool.query(loginQuery, [player_id, token]);

                    if (result.rows.length > 0) {
                        console.log(`[Auth] Игрок ${player_id} успешно вошел.`);
                        ws.send(JSON.stringify({
                            action: 'login_success',
                            player_data: result.rows[0],
                        }));
                    } else {
                        console.log(`[Auth] Неудачная попытка входа для: ${player_id}`);
                        ws.send(JSON.stringify({ action: 'login_failed' }));
                    }
                    break;
                }

                default:
                    console.log('Неизвестная команда:', data.action);
                    ws.send(JSON.stringify({ action: 'error', message: 'Unknown action' }));
                    break;
            }
        } catch (error) {
            console.error('Ошибка обработки сообщения:', error);
            ws.send(JSON.stringify({ action: 'error', message: 'Invalid message format' }));
        }
    });

    ws.on('close', () => {
        console.log('❌ Клиент отключился.');
    });
});
