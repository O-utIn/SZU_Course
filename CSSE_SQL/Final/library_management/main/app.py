from flask import Flask, request, jsonify, render_template, redirect, url_for, session
import mysql.connector
import datetime
from flask_cors import CORS

# 配置数据库连接信息
config = {
    'user': 'root',  # 替换为你的MySQL用户名
    'password': 'Zqm532146789',  # 替换为你的MySQL密码
    'host': 'localhost',  # 替换为你的MySQL主机地址，如localhost
    'database': 'library_management_system',
}

app = Flask(__name__, template_folder='templates', static_folder='static')
# 配置CORS，允许来自http://127.0.0.1:5000的请求，可根据实际情况调整 origins 参数
CORS(app)


# 辅助函数：用于将数据库查询结果转换为字典列表形式
def row_to_dict(rows, cursor):
    result = []
    for row in rows:
        row_dict = {}
        for index, col in enumerate(cursor.column_names):
            row_dict[col] = row[index]
        result.append(row_dict)
    return result


@app.route('/', methods=['GET'])
def index():
    """
    首页路由，根据URL中'page'参数决定跳转到登录页面或图书页面，默认跳转到登录页面
    """
    target_page = request.args.get('page', 'login_page')  # 默认跳转到 login_page
    # print(f"当前请求的目标页面参数: {target_page}")
    if target_page == 'login_page':
        return redirect(url_for('login_page'))
    elif target_page == 'reader_page':
        return redirect(url_for('reader_page'))
    elif target_page == 'librarian_page':
        return redirect(url_for('librarian_page'))
    else:
        return redirect(url_for('complex_search_page'))


@app.route('/login_page')
def login_page():
    """
    登录页面路由，返回登录页面的HTML模板
    """
    return render_template('login.html')


@app.route('/register_page')
def register_page():
    """
    注册页面路由，返回注册页面的HTML模板
    """
    return render_template('register.html')


@app.route('/reader_page')
def reader_page():
    """
    图书页面路由，需验证用户是否已登录，若已登录则返回图书页面的HTML模板，否则重定向到登录页面
    """
    return render_template('reader_page.html')


@app.route('/librarian_page')
def librarian_page():
    return render_template('librarian_page.html')


@app.route('/login', methods=['POST'])
def login():
    data = request.get_json()
    username = data.get('username')
    password = data.get('password')
    print(f"Received login request with username: {username} and password: {password}")

    try:
        # 连接到数据库
        cnx = mysql.connector.connect(**config)
        cursor = cnx.cursor()

        # 查询数据库中的用户名和密码
        query = "SELECT user_id, username, user_type FROM users WHERE username = %s AND password = %s"
        cursor.execute(query, (username, password))
        result = cursor.fetchone()

        print(f"Database query result: {result}")

        cursor.close()
        cnx.close()

        # 检查是否找到了该用户
        if result:
            session['user_id'] = result[0]  # 将 user_id 存储到 session 中
            session['username'] = result[1]  # 将 username 存储到 session 中
            session['user_type'] = result[2]  # 将 user_type 存储到 session 中

            # 根据 user_type 跳转到不同页面
            if result[2] == 'reader':
                return jsonify({"message": "登录成功", "username": result[1], "redirect": "/reader_page"}), 200
            elif result[2] == 'librarian':
                return jsonify({"message": "登录成功", "username": result[1], "redirect": "/librarian_page"}), 200
            else:
                return jsonify({"message": "登录成功", "username": result[1], "redirect": "/reader_page"}), 200
        else:
            return jsonify({"message": "用户名或密码错误"}), 401  # 用户名或密码错误

    except mysql.connector.Error as err:
        print(f"Database error: {err}")
        return jsonify({"message": f"登录失败: {err}"}), 500  # 数据库错误


# 用户注册功能
@app.route('/register', methods=['POST'])
def register():
    """
    用户注册路由，接收包含用户名、密码、邮箱、电话号码、用户类型的JSON数据，将用户信息插入到数据库中，若成功则返回注册成功消息，否则返回相应错误消息
    """
    data = request.get_json()
    username = data.get('username')
    password = data.get('password')
    email = data.get('email')
    phone_number = data.get('phone_number')
    user_type = data.get('user_type')

    try:
        cnx = mysql.connector.connect(**config)
        cursor = cnx.cursor()
        query = "INSERT INTO users (username, password, email, phone_number, user_type) VALUES (%s, %s, %s, %s, %s)"
        cursor.execute(query, (username, password, email, phone_number, user_type))
        cnx.commit()
        cursor.close()
        cnx.close()
        return jsonify({"message": "注册成功"}), 201
    except mysql.connector.Error as err:
        return jsonify({"message": f"注册失败: {err}"}), 500


@app.route('/complex_search_page')
def complex_search_page():
    """
    新页面路由，展示复杂的搜索查询
    """
    try:
        # 连接数据库
        cnx = mysql.connector.connect(**config)
        cursor = cnx.cursor()

        # 单表查询：查询所有书籍
        cursor.execute("SELECT * FROM books")
        all_books = row_to_dict(cursor.fetchall(), cursor)

        # 联接查询 (INNER JOIN)：获取每本书的作者及出版社
        cursor.execute("""
            SELECT books.title, authors.author_name, publishers.publisher_name
                FROM books
                INNER JOIN authors ON books.author = authors.author_name
                INNER JOIN publishers ON books.publisher = publishers.publisher_name;
        """)
        inner_join_books = row_to_dict(cursor.fetchall(), cursor)

        # 使用 GROUP BY 和 ORDER BY 聚合函数：按出版社统计每个出版社的书籍数量
        cursor.execute("""
            SELECT publishers.publisher_name, COUNT(books.book_id) AS book_count
                FROM books
                INNER JOIN publishers ON books.publisher = publishers.publisher_name
                GROUP BY publishers.publisher_name
                ORDER BY book_count DESC;
        """)
        grouped_books = row_to_dict(cursor.fetchall(), cursor)

        # 日期和时间函数：查询最近三年发布的书籍
        cursor.execute("""
            SELECT * FROM books
            WHERE publication_year >= YEAR(CURDATE()) - 1;
        """)
        recent_books = row_to_dict(cursor.fetchall(), cursor)

        # 子查询：查询拥有最多书籍的出版社
        cursor.execute("""
            SELECT publisher, COUNT(*) AS book_count
            FROM books
            LEFT JOIN borrowing_records ON books.book_id = borrowing_records.book_id
            GROUP BY publisher
            ORDER BY book_count DESC
            LIMIT 1;
        """)
        publisher_with_most_books = cursor.fetchone()

        # 相关子查询：查询所有书籍中，借阅次数大于该书籍平均借阅次数的书籍
        cursor.execute("""
            SELECT title
            FROM books
            WHERE book_id IN (
                SELECT book_id
                FROM borrowing_records
                GROUP BY book_id
                HAVING COUNT(*) > (
                    SELECT AVG(borrow_count) 
                    FROM (
                        SELECT book_id, COUNT(*) AS borrow_count
                        FROM borrowing_records
                        GROUP BY book_id
                    ) AS borrow_stats
                )
            );
        """)
        books_above_avg_borrow = row_to_dict(cursor.fetchall(), cursor)

        # 集合操作：UNION：查询所有图书的标题和借阅记录的书名
        cursor.execute("""
            SELECT title FROM books
            UNION
            SELECT b.title FROM borrowing_records br
            JOIN books b ON br.book_id = b.book_id
        """)
        union_books = row_to_dict(cursor.fetchall(), cursor)

        # 多表联接查询：查询每本书的类别及借阅记录
        cursor.execute("""
            SELECT 
                u.username,
                b.title,
                bc.category_name,  
                br.borrow_date,
                br.due_date,
                br.return_date
            FROM
                users u
            JOIN
                borrowing_records br ON u.user_id = br.user_id
            JOIN
                books b ON br.book_id = b.book_id
            JOIN
                book_categories bc ON b.category = bc.category_name;
        """)
        multi_table_books = row_to_dict(cursor.fetchall(), cursor)

        # 除法查询：查询那些所有借阅记录中都被借阅过的书籍
        cursor.execute("""
            SELECT 
                b.title
            FROM
                books b
            JOIN
                (
                    SELECT 
                        publisher,
                        AVG(borrow_count_per_book) AS avg_borrow_count
                    FROM
                        (
                            SELECT 
                                b.publisher,
                                COUNT(br.record_id) AS borrow_count_per_book
                            FROM
                                books b
                            LEFT JOIN
                                borrowing_records br ON b.book_id = br.book_id
                            GROUP BY
                                b.publisher, b.book_id
                        ) subquery
                    GROUP BY
                        publisher
                    ORDER BY
                        avg_borrow_count DESC
                    LIMIT 1
                ) top_publisher ON b.publisher = top_publisher.publisher;
        """)
        books_borrowed_by_all = row_to_dict(cursor.fetchall(), cursor)

        # 关闭数据库连接
        cursor.close()
        cnx.close()

        # 返回渲染页面并传递查询结果
        return render_template('complex_search_page.html',
                               all_books=all_books,
                               inner_join_books=inner_join_books,
                               grouped_books=grouped_books,
                               recent_books=recent_books,
                               publisher_with_most_books=publisher_with_most_books,
                               books_above_avg_borrow=books_above_avg_borrow,
                               union_books=union_books,
                               multi_table_books=multi_table_books,
                               books_borrowed_by_all=books_borrowed_by_all)

    except mysql.connector.Error as err:
        return jsonify({"message": f"获取书籍数据失败: {err}"}), 500



# 获取用户信息功能（根据用户ID获取单个用户信息）
@app.route('/users/<int:user_id>', methods=['GET'])
def get_user(user_id):
    """
    根据用户ID获取单个用户信息的路由，若用户存在则返回用户信息的字典格式数据，否则返回用户不存在的错误消息
    """
    try:
        cnx = mysql.connector.connect(**config)
        cursor = cnx.cursor()
        query = "SELECT * FROM users WHERE user_id = %s"
        cursor.execute(query, (user_id,))
        result = cursor.fetchone()
        if result:
            user_dict = {}
            for index, col in enumerate(cursor.column_names):
                user_dict[col] = result[index]
            cursor.close()
            cnx.close()
            return jsonify(user_dict), 200
        else:
            cursor.close()
            cnx.close()
            return jsonify({"message": "用户不存在"}), 404
    except mysql.connector.Error as err:
        return jsonify({"message": f"获取用户信息失败: {err}"}), 500


# 获取所有图书信息功能
@app.route('/books', methods=['GET'])
def get_books():
    """
    获取所有图书信息的路由，查询数据库中的图书信息并转换为字典列表格式后返回，若查询失败则返回相应错误消息
    """
    try:
        cnx = mysql.connector.connect(**config)
        cursor = cnx.cursor()
        query = "SELECT * FROM books"
        cursor.execute(query)
        books = cursor.fetchall()
        book_list = row_to_dict(books, cursor)
        cursor.close()
        cnx.close()
        return jsonify(book_list), 200
    except mysql.connector.Error as err:
        return jsonify({"message": f"获取图书信息失败: {err}"}), 500


@app.route('/books/search', methods=['GET'])
def search_books():
    """
    根据书名搜索图书信息的路由，查询数据库中的图书，并根据书名返回匹配的图书信息
    """
    query = request.args.get('title', '')  # 获取查询的书名

    try:
        cnx = mysql.connector.connect(**config)
        cursor = cnx.cursor()

        # 使用LIKE进行模糊查询
        search_query = "SELECT * FROM books WHERE title LIKE %s"
        cursor.execute(search_query, ('%' + query + '%',))

        books = cursor.fetchall()
        book_list = row_to_dict(books, cursor)

        cursor.close()
        cnx.close()

        return jsonify(book_list), 200
    except mysql.connector.Error as err:
        return jsonify({"message": f"搜索图书失败: {err}"}), 500


# 添加图书功能
@app.route('/books', methods=['POST'])
def add_book():
    """
    添加图书路由，接收包含图书各项信息的JSON数据，将图书信息插入到数据库中，若成功则返回图书添加成功消息，否则返回相应错误消息
    """
    data = request.get_json()
    title = data.get('title')
    author = data.get('author')
    publisher = data.get('publisher')
    publication_year = data.get('publication_year')
    isbn = data.get('isbn')
    category = data.get('category')
    total_copies = data.get('total_copies')
    available_copies = data.get('available_copies')

    try:
        cnx = mysql.connector.connect(**config)
        cursor = cnx.cursor()

        # 检查类别是否存在
        cursor.execute("SELECT * FROM book_categories WHERE category_name = %s", (category,))
        result = cursor.fetchone()
        if not result:
            # 如果分类不存在，可以选择插入该分类
            cursor.execute("INSERT INTO book_categories (category_name) VALUES (%s)", (category,))
            cnx.commit()

        # 检查出版社是否存在
        cursor.execute("SELECT * FROM publishers WHERE publisher_name = %s", (publisher,))
        result = cursor.fetchone()
        if not result:
            # 如果出版社不存在，可以选择插入该出版社
            cursor.execute("INSERT INTO publishers (publisher_name) VALUES (%s)", (publisher,))
            cnx.commit()

        # 检查作者是否存在
        cursor.execute("SELECT * FROM authors WHERE author_name = %s", (author,))
        result = cursor.fetchone()
        if not result:
            # 如果作者不存在，可以选择插入该作者
            cursor.execute("INSERT INTO authors (author_name) VALUES (%s)", (author,))
            cnx.commit()

        # 插入图书数据
        query = "INSERT INTO books (title, author, publisher, publication_year, isbn, category, total_copies, available_copies) VALUES (%s, %s, %s, %s, %s, %s, %s, %s)"
        cursor.execute(query,
                       (title, author, publisher, publication_year, isbn, category, total_copies, available_copies))
        cnx.commit()

        cursor.close()
        cnx.close()
        return jsonify({"message": "图书添加成功"}), 201
    except mysql.connector.Error as err:
        return jsonify({"message": f"图书添加失败: {err}"}), 500


# 获取单个图书详细信息功能（根据图书ID获取）
@app.route('/books/<int:book_id>', methods=['GET'])
def get_book_detail(book_id):
    """
    根据图书ID获取单个图书详细信息的路由，查询数据库并返回对应图书信息，若查询失败则返回相应错误消息
    """
    try:
        cnx = mysql.connector.connect(**config)
        cursor = cnx.cursor()
        query = "SELECT * FROM books WHERE book_id = %s"
        cursor.execute(query, (book_id,))
        result = cursor.fetchone()
        if result:
            book_dict = {}
            for index, col in enumerate(cursor.column_names):
                book_dict[col] = result[index]
            cursor.close()
            cnx.close()
            return jsonify(book_dict), 200
        else:
            cursor.close()
            cnx.close()
            return jsonify({"message": "图书不存在"}), 404
    except mysql.connector.Error as err:
        return jsonify({"message": f"获取图书信息失败: {err}"}), 500


# 更新图书信息功能（根据图书ID更新）
@app.route('/books/<int:book_id>', methods=['PUT'])
def update_book(book_id):
    data = request.get_json()

    title = data.get('title')
    author = data.get('author')
    publisher = data.get('publisher')
    publication_year = data.get('publication_year')
    isbn = data.get('isbn')
    category = data.get('category')
    total_copies = data.get('total_copies')
    available_copies = data.get('available_copies')

    try:
        cnx = mysql.connector.connect(**config)
        cursor = cnx.cursor()

        # 检查author是否存在，如果不存在则插入到authors表（可根据实际业务需求调整此处逻辑，比如是否允许自动插入等）
        if author:
            cursor.execute("SELECT * FROM authors WHERE author_name = %s", (author,))
            author_result = cursor.fetchone()
            if not author_result:
                cursor.execute("INSERT INTO authors (author_name) VALUES (%s)", (author,))
                cnx.commit()

        # 用于构建 SQL 更新语句的部分
        update_fields = []
        update_values = []

        # 判断每个字段是否为空，如果不为空，则添加到更新列表中
        if title:
            update_fields.append("title = %s")
            update_values.append(title)
        if author:
            update_fields.append("author = %s")
            update_values.append(author)
        if publisher:
            update_fields.append("publisher = %s")
            update_values.append(publisher)
        if publication_year:
            update_fields.append("publication_year = %s")
            update_values.append(publication_year)
        if isbn:
            update_fields.append("isbn = %s")
            update_values.append(isbn)
        if category:
            update_fields.append("category = %s")
            update_values.append(category)
        if total_copies:
            update_fields.append("total_copies = %s")
            update_values.append(total_copies)
        if available_copies:
            update_fields.append("available_copies = %s")
            update_values.append(available_copies)

        # 如果有需要更新的字段，构建 SQL 语句并执行更新
        if update_fields:
            query = "UPDATE books SET " + ", ".join(update_fields) + " WHERE book_id = %s"
            update_values.append(book_id)

            # 打印 SQL 和参数调试信息
            print(f"Update query: {query}")
            print(f"Update values: {tuple(update_values)}")

            cursor.execute(query, tuple(update_values))
            cnx.commit()

        cursor.close()
        cnx.close()

        return jsonify({"message": "图书信息更新成功"}), 200
    except mysql.connector.Error as err:
        print(f"Database error: {err}")
        return jsonify({"message": f"图书信息更新失败: {err}"}), 500


@app.route('/books/<int:book_id>', methods=['DELETE'])
def delete_book(book_id):
    """
    根据图书ID删除图书的路由，删除数据库中对应图书的记录，若成功则返回图书删除成功消息，否则返回相应错误消息
    """
    try:
        # 连接数据库
        cnx = mysql.connector.connect(**config)
        cursor = cnx.cursor()

        # 先删除借阅记录
        delete_borrowing_query = "DELETE FROM borrowing_records WHERE book_id = %s"
        cursor.execute(delete_borrowing_query, (book_id,))

        # 删除图书记录
        delete_book_query = "DELETE FROM books WHERE book_id = %s"
        cursor.execute(delete_book_query, (book_id,))

        # 提交事务
        cnx.commit()

        # 关闭连接
        cursor.close()
        cnx.close()

        return jsonify({"message": "图书删除成功"}), 200

    except mysql.connector.Error as err:
        # 如果发生数据库错误，返回错误信息
        return jsonify({"message": f"图书删除失败: {err}"}), 500


if __name__ == '__main__':
    app.secret_key = 'your_secret_key'  # 设置一个密钥，用于加密session数据，实际应用中需替换为安全的密钥
    app.run(debug=True)
