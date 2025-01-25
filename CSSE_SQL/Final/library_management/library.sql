-- 先删除可能已存在的同名数据库（谨慎操作，会删除库及其中所有数据）
DROP DATABASE IF EXISTS library_management_system;

-- 创建数据库（如果数据库不存在的话）
CREATE DATABASE IF NOT EXISTS library_management_system;
USE library_management_system;

-- 2. 逻辑数据模型（LDM）中的实体对应的表创建（这里包含了至少10个实体）

-- 2.1 用户表（users）
CREATE TABLE `users` (
    `user_id` int(11) NOT NULL AUTO_INCREMENT,
    `username` varchar(50) NOT NULL,
    `password` varchar(100) NOT NULL,
    `email` varchar(100),
    `phone_number` varchar(20),
    `user_type` enum('reader', 'librarian', 'admin') NOT NULL,
    PRIMARY KEY (`user_id`)
);

-- 用户资料
CREATE TABLE `user_profiles` (
    `profile_id` int(11) NOT NULL AUTO_INCREMENT,
    `user_id` int(11) NOT NULL UNIQUE,
    `address` varchar(200),
    `birth_date` date,
    PRIMARY KEY (`profile_id`),
    FOREIGN KEY (`user_id`) REFERENCES `users` (`user_id`)
);

-- 2.2 图书表（books）
CREATE TABLE `books` (
    `book_id` int(11) NOT NULL AUTO_INCREMENT,
    `title` varchar(100) NOT NULL,
    `author` varchar(100) NOT NULL,
    `publisher` varchar(100),
    `publication_year` int(4),
    `isbn` varchar(20),
    `category` varchar(50),
    `total_copies` int(11) NOT NULL DEFAULT 0,
    `available_copies` int(11) NOT NULL DEFAULT 0,
    PRIMARY KEY (`book_id`)
);

-- 2.3 借阅记录表（borrowing_records）
CREATE TABLE `borrowing_records` (
    `record_id` int(11) NOT NULL AUTO_INCREMENT,
    `user_id` int(11) NOT NULL,
    `book_id` int(11) NOT NULL,
    `borrow_date` datetime NOT NULL,
    `due_date` datetime NOT NULL,
    `return_date` datetime,
    PRIMARY KEY (`record_id`),
    FOREIGN KEY (`user_id`) REFERENCES `users` (`user_id`),
    FOREIGN KEY (`book_id`) REFERENCES `books` (`book_id`)
);


-- 2.4 图书分类表（book_categories）
CREATE TABLE `book_categories` (
    `category_id` int(11) NOT NULL AUTO_INCREMENT,
    `category_name` varchar(50) NOT NULL,
    PRIMARY KEY (`category_id`)
);

-- 2.5 创建图书分类表中category_name列的索引，便于后续添加外键约束
CREATE INDEX `idx_book_categories_name` ON `book_categories` (`category_name`);


-- 2.5 出版社表（publishers）
CREATE TABLE `publishers` (
    `publisher_id` int(11) NOT NULL AUTO_INCREMENT,
    `publisher_name` varchar(100) NOT NULL,
    `address` varchar(200),
    `contact_number` varchar(20),
    PRIMARY KEY (`publisher_id`)
);

-- 2.6 创建出版社表中publisher_name列的索引，便于后续添加外键约束
CREATE INDEX `idx_publishers_name` ON `publishers` (`publisher_name`);

-- 2.6 作者表（authors）
CREATE TABLE `authors` (
    `author_id` int(11) NOT NULL AUTO_INCREMENT,
    `author_name` varchar(100) NOT NULL,
    `country` varchar(50),
    PRIMARY KEY (`author_id`)
);

-- 2.7 创建作者表中author_name列的索引，便于后续添加外键约束
CREATE INDEX `idx_authors_name` ON `authors` (`author_name`);

-- 图书作者关联表
CREATE TABLE `book_authors` (
    `book_author_id` int(11) NOT NULL AUTO_INCREMENT,
    `book_id` int(11) NOT NULL,
    `author_id` int(11) NOT NULL,
    PRIMARY KEY (`book_author_id`),
    FOREIGN KEY (`book_id`) REFERENCES `books` (`book_id`),
    FOREIGN KEY (`author_id`) REFERENCES `authors` (`author_id`)
);

-- 2.7 罚款表（fines）
CREATE TABLE `fines` (
    `fine_id` int(11) NOT NULL AUTO_INCREMENT,
    `user_id` int(11) NOT NULL,
    `record_id` int(11) NOT NULL,
    `fine_amount` decimal(10, 2) NOT NULL,
    `payment_date` datetime,
    PRIMARY KEY (`fine_id`),
    FOREIGN KEY (`user_id`) REFERENCES `users` (`user_id`),
    FOREIGN KEY (`record_id`) REFERENCES `borrowing_records` (`record_id`)
);

-- 2.8 预约表（reservations）
CREATE TABLE `reservations` (
    `reservation_id` int(11) NOT NULL AUTO_INCREMENT,
    `user_id` int(11) NOT NULL,
    `book_id` int(11) NOT NULL,
    `reservation_date` datetime NOT NULL,
    PRIMARY KEY (`reservation_id`),
    FOREIGN KEY (`user_id`) REFERENCES `users` (`user_id`),
    FOREIGN KEY (`book_id`) REFERENCES `books` (`book_id`)
);

-- 2.9 图书评论表（book_reviews）
CREATE TABLE `book_reviews` (
    `review_id` int(11) NOT NULL AUTO_INCREMENT,
    `user_id` int(11) NOT NULL,
    `book_id` int(11) NOT NULL,
    `review_text` text NOT NULL,
    `rating` int(11) NOT NULL,
    `review_date` datetime NOT NULL,
    PRIMARY KEY (`review_id`),
    FOREIGN KEY (`user_id`) REFERENCES `users` (`user_id`),
    FOREIGN KEY (`book_id`) REFERENCES `books` (`book_id`)
);

-- 2.10 管理员操作日志表（admin_operation_logs）
CREATE TABLE `admin_operation_logs` (
    `log_id` int(11) NOT NULL AUTO_INCREMENT,
    `admin_id` int(11) NOT NULL,
    `operation_type` varchar(100) NOT NULL,
    `operation_time` datetime NOT NULL,
    `description` text,
    PRIMARY KEY (`log_id`),
    FOREIGN KEY (`admin_id`) REFERENCES `users` (`user_id`)
);

-- 3. 添加其他必要的约束和索引

-- 3.1 为图书表（books）的分类字段添加外键约束关联图书分类表（book_categories）
ALTER TABLE `books`
ADD CONSTRAINT `fk_books_categories` FOREIGN KEY (`category`) REFERENCES `book_categories` (`category_name`);

-- 3.2 为图书表（books）的出版社字段添加外键约束关联出版社表（publishers）
ALTER TABLE `books`
ADD CONSTRAINT `fk_books_publishers` FOREIGN KEY (`publisher`) REFERENCES `publishers` (`publisher_name`);

-- 3.3 为图书表（books）的作者字段添加外键约束关联作者表（authors）
ALTER TABLE `books`
ADD CONSTRAINT `fk_books_authors` FOREIGN KEY (`author`) REFERENCES `authors` (`author_name`);

-- 3.4 创建索引提升查询性能
-- 为用户表（users）的用户名创建索引（方便登录等按用户名查询操作）
CREATE INDEX `idx_users_username` ON `users` (`username`);
-- 为图书表（books）的书名创建索引（方便按书名查找图书）
CREATE INDEX `idx_books_title` ON `books` (`title`);
-- 为借阅记录表（borrowing_records）的用户ID和图书ID创建联合索引（常用于查询某个用户的借阅情况或者某本图书的借阅情况）
CREATE INDEX `idx_borrowing_records_user_book` ON `borrowing_records` (`user_id`, `book_id`);

-- 4. 可选的插入示例数据语句（这里仅插入少量示例数据用于测试，你可以根据实际需求丰富数据内容）

-- 4.1 插入一些用户数据
INSERT INTO `users` (`username`, `password`, `email`, `phone_number`, `user_type`) VALUES
('reader1', '123456', 'reader1@example.com', '13812345678', 'reader'),
('librarian1', '654321', 'librarian1@example.com', '13687654321', 'librarian'),
('admin1', 'admin123', 'admin1@example.com', '13998765432', 'admin');

INSERT INTO `user_profiles` (`user_id`, `address`, `birth_date`) VALUES
(1, '123 Main St', '1990-01-01');

-- 4.2 插入一些图书分类数据
INSERT INTO `book_categories` (`category_name`) VALUES
('Fiction'),
('Non-fiction'),
('Science'),
('History');

-- 4.3 插入一些出版社数据
INSERT INTO `publishers` (`publisher_name`, `address`, `contact_number`) VALUES
('Publisher A', 'Address A', '1234567890'),
('Publisher B', 'Address B', '0987654321');

-- 4.4 插入一些作者数据
INSERT INTO `authors` (`author_name`, `country`) VALUES
('Author 1', 'USA'),
('Author 2', 'UK'),
('Author 3', 'China');

-- 表示图书1由作者1编写
INSERT INTO `book_authors` (`book_id`, `author_id`) VALUES
(1, 1);
-- 表示图书2由作者1编写
INSERT INTO `book_authors` (`book_id`, `author_id`) VALUES
(2, 1);
-- 表示图书3由作者2编写
INSERT INTO `book_authors` (`book_id`, `author_id`) VALUES
(3, 2);

-- 4.5 插入一些图书数据
INSERT INTO `books` (`title`, `author`, `publisher`, `publication_year`, `isbn`, `category`, `total_copies`, `available_copies`) VALUES
('Book 1', 'Author 1', 'Publisher A', 2020, '1234567890123', 'Fiction', 5, 3),
('Book 2', 'Author 2', 'Publisher B', 2021, '0987654321098', 'Non-fiction', 3, 2),
('Book 3', 'Author 3', 'Publisher A', 2022, '1111111111111', 'Science', 4, 4);

-- 4.6 插入一条借阅记录示例数据
INSERT INTO `borrowing_records` (`user_id`, `book_id`, `borrow_date`, `due_date`, `return_date`) VALUES
(1, 1, '2024-12-01 09:00:00', '2024-12-15 09:00:00', NULL);











-- 批量插入更多用户数据
INSERT INTO `users` (`username`, `password`, `email`, `phone_number`, `user_type`) VALUES
('reader2', 'password2', 'reader2@example.com', '13887654321', 'reader'),
('reader3', 'password3', 'reader3@example.com', '13912345678', 'reader'),
('librarian2', 'password4', 'librarian2@example.com', '13898765432', 'librarian'),
('admin2', 'password5', 'admin2@example.com', '13712345679', 'admin'),
('reader4', 'password6', 'reader4@example.com', '13987654322', 'reader');

-- 批量插入更多图书分类数据
INSERT INTO `book_categories` (`category_name`) VALUES
('Philosophy'),
('Technology'),
('Art'),
('Mathematics'),
('Biography');

-- 批量插入更多出版社数据
INSERT INTO `publishers` (`publisher_name`, `address`, `contact_number`) VALUES
('Publisher C', 'Address C', '1112223333'),
('Publisher D', 'Address D', '4445556666'),
('Publisher E', 'Address E', '7778889999');

-- 批量插入更多作者数据
INSERT INTO `authors` (`author_name`, `country`) VALUES
('Author 4', 'Canada'),
('Author 5', 'India'),
('Author 6', 'Germany'),
('Author 7', 'Australia'),
('Author 8', 'Japan');

-- 批量插入更多图书数据
INSERT INTO `books` (`title`, `author`, `publisher`, `publication_year`, `isbn`, `category`, `total_copies`, `available_copies`) VALUES
('Book 4', 'Author 4', 'Publisher C', 2019, '2222222222222', 'History', 6, 4),
('Book 5', 'Author 5', 'Publisher D', 2024, '3333333333333', 'Technology', 7, 5),
('Book 6', 'Author 6', 'Publisher E', 2017, '4444444444444', 'Art', 5, 5),
('Book 7', 'Author 7', 'Publisher A', 2023, '5555555555555', 'Mathematics', 8, 6),
('Book 8', 'Author 8', 'Publisher B', 2022, '6666666666666', 'Biography', 4, 2);

-- 批量插入更多借阅记录数据
INSERT INTO `borrowing_records` (`user_id`, `book_id`, `borrow_date`, `due_date`, `return_date`) VALUES
(2, 2, '2024-12-02 10:00:00', '2024-12-16 10:00:00', NULL),
(3, 3, '2024-12-03 11:00:00', '2024-12-17 11:00:00', '2024-12-15 10:00:00'),
(4, 4, '2024-12-04 12:00:00', '2024-12-18 12:00:00', NULL),
(5, 5, '2024-12-05 13:00:00', '2024-12-19 13:00:00', NULL);

-- 批量插入更多罚款记录数据
INSERT INTO `fines` (`user_id`, `record_id`, `fine_amount`, `payment_date`) VALUES
(1, 1, 10.00, '2024-12-20 10:00:00'),
(3, 2, 15.00, '2024-12-21 11:00:00');

-- 批量插入更多预约记录数据
INSERT INTO `reservations` (`user_id`, `book_id`, `reservation_date`) VALUES
(1, 2, '2024-12-22 09:30:00'),
(2, 3, '2024-12-23 10:30:00'),
(3, 4, '2024-12-24 11:30:00');

-- 批量插入更多图书评论数据
INSERT INTO `book_reviews` (`user_id`, `book_id`, `review_text`, `rating`, `review_date`) VALUES
(1, 1, 'Excellent book!', 5, '2024-12-01 09:30:00'),
(2, 2, 'Informative read.', 4, '2024-12-02 10:00:00'),
(3, 3, 'Not my favorite.', 3, '2024-12-03 11:00:00');

-- 批量插入更多管理员操作日志数据
INSERT INTO `admin_operation_logs` (`admin_id`, `operation_type`, `operation_time`, `description`) VALUES
(3, 'Add Book', '2024-12-01 14:00:00', 'Added new book: Book 4'),
(3, 'Update Book', '2024-12-02 15:00:00', 'Updated book details: Book 5');



-- 批量插入更多借阅记录数据
INSERT INTO `borrowing_records` (`user_id`, `book_id`, `borrow_date`, `due_date`, `return_date`) VALUES
(2, 2, '2024-12-02 10:00:00', '2024-12-16 10:00:00', NULL),
(3, 3, '2024-12-03 11:00:00', '2024-12-17 11:00:00', '2024-12-15 10:00:00'),
(4, 4, '2024-12-04 12:00:00', '2024-12-18 12:00:00', NULL),
(5, 5, '2024-12-05 13:00:00', '2024-12-19 13:00:00', NULL),
(1, 2, '2024-12-10 09:00:00', '2024-12-24 09:00:00', NULL),
(2, 1, '2024-12-11 10:00:00', '2024-12-25 10:00:00', NULL),
(3, 4, '2024-12-12 11:00:00', '2024-12-26 11:00:00', '2024-12-22 12:00:00'),
(4, 3, '2024-12-13 12:00:00', '2024-12-27 12:00:00', NULL),
(5, 6, '2024-12-14 13:00:00', '2024-12-28 13:00:00', NULL);


-- 批量插入更多图书分类数据
INSERT INTO `book_categories` (`category_name`) VALUES
('Poetry'),
('Drama'),
('Self-Help'),
('Health'),
('Travel'),
('Cookbook'),
('Children'),
('Religion'),
('Fantasy'),
('Mystery');









