from flask import Flask, request
import sqlite3

app = Flask(__name__)

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
conn.commit()


@app.route('/moisture', methods=['POST'])
def add_moisture_level():
    # Get the JSON data from the request
    data = request.get_json()

    # Extract the moisture level from the JSON data
    moisture_level = data.get('moisture_level')

    # Insert the moisture level into the database
    cursor.execute('INSERT INTO moisture (level) VALUES (?)',
                   (moisture_level, ))
    conn.commit()

    return 'Moisture level added successfully'


@app.route('/moisture', methods=['GET'])
def get_moisture_levels():
    # Get the moisture levels from the database
    cursor.execute('SELECT * FROM moisture')
    moisture_levels = cursor.fetchall()

    # Convert the moisture levels to a list of dictionaries
    moisture_levels_dicts = []
    for moisture_level in moisture_levels:
        moisture_levels_dicts.append({
            'id': moisture_level[0],
            'level': moisture_level[1],
            'created_at': moisture_level[2]
        })

    return {'moisture_levels': moisture_levels_dicts}


if __name__ == '__main__':
    app.run()
