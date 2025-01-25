import mysql.connector
from mysql.connector import Error
from contextlib import contextmanager

@contextmanager
def get_connection():
    connection = None
    try:
        connection = mysql.connector.connect(
            host='localhost',
            user='root',
            passwd='03062813',
            database='VolunteerPlatform'
        )
        print("Connection to MySQL DB successful")
        yield connection
    except Error as e:
        print(f"The error '{e}' occurred")
    finally:
        if connection:
            connection.close()

def execute_query(query, params=None):
    with get_connection() as connection:
        cursor = connection.cursor()
        try:
            if params:
                cursor.execute(query, params)
            else:
                cursor.execute(query)
            connection.commit()
            print("Query executed successfully")
        except Error as e:
            print(f"The error '{e}' occurred")
        finally:
            cursor.close()

def fetch_data(query, params=None):
    with get_connection() as connection:
        cursor = connection.cursor(dictionary=True)
        try:
            if params:
                cursor.execute(query, params)
            else:
                cursor.execute(query)
            result = cursor.fetchall()
            return result
        except Error as e:
            print(f"The error '{e}' occurred")
        finally:
            cursor.close()