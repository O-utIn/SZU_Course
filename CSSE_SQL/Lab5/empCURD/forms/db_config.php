<?php
$host = 'localhost';
$username = 'root';
$password = '03062813';
$dbname = 'abc_company';

$conn = new mysqli($host, $username, $password, $dbname);

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}
?>