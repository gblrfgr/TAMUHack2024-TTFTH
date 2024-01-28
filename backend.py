from flask import Flask, request, after_this_request, jsonify
import sqlite3
from flask_cors import CORS

app = Flask(__name__)
CORS(app)

# Create a SQLite database and table
conn = sqlite3.connect('moisture_levels.db')
cursor = conn.cursor()
cursor.execute('''
    CREATE TABLE IF NOT EXISTS moisture (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        level REAL NOT NULL,
        created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
    )
''')
for i in range(100):
    conn.execute('INSERT INTO moisture (level) VALUES (?)', (i / 100, ))
conn.commit()
conn.close()


def get_db_connection():
    conn = sqlite3.connect('moisture_levels.db')
    conn.row_factory = sqlite3.Row
    return conn


@app.route('/moisture', methods=['POST'])
def add_moisture_level():

    @after_this_request
    def add_header(response):
        response.headers.add('Access-Control-Allow-Origin', '*')
        return response

    # Get the JSON data from the request
    data = request.get_json()

    # Extract the moisture level from the JSON data
    moisture_level = data.get('moisture_level')

    # Insert the moisture level into the database
    conn = get_db_connection()
    conn.execute('INSERT INTO moisture (level) VALUES (?)', (moisture_level, ))
    conn.commit()
    conn.close()

    return 'Moisture level added successfully'


@app.route('/moisture', methods=['GET'])
def get_moisture_levels():

    @after_this_request
    def add_header(response):
        response.headers.add('Access-Control-Allow-Origin', '*')
        return response

    # Get the moisture levels from the database
    conn = get_db_connection()
    moisture_levels = conn.execute('SELECT * FROM moisture').fetchall()
    conn.close()

    # Convert the moisture levels to a list of dictionaries
    moisture_levels_dicts = []
    for moisture_level in moisture_levels:
        moisture_levels_dicts.append({
            'id': moisture_level[0],
            'level': moisture_level[1],
            'created_at': moisture_level[2]
        })

    return jsonify({'moisture_levels': moisture_levels_dicts})


if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0')
