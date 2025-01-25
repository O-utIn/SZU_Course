<?php

include_once('db_functions.php');
if (session_status() === PHP_SESSION_NONE) {
    session_start();
}
// Check if the user is logged in
if (!isset($_SESSION['logged_in'])) {
    // If the user is not logged in, redirect them to the login page
    header('Location: http://localhost/empCURD/index.php');
	die();
	exit();
}
echo "<!DOCTYPE html>";
echo "<html>";
echo "<head>";
echo "<title> All Employees Information </title>";
echo '<link href="style.css" rel="stylesheet" type="text/css">';
echo "</head>";
echo "<body>";

// Get all employee data
$conn = new mysqli('localhost', 'root', '', 'abc_company');
$sqlq = "SELECT * FROM EMP";
$result = $conn->query($sqlq);

// Check if the query returned any results
if ($result->num_rows > 0) {
    echo "<table>";
    echo "<tr><th>EMPNO</th><th>ENAME</th><th>JOB</th><th>SAL</th><th>COMM</th><th>HIREDATE</th><th>DEPTNO</th><th>MGR</th></tr>";
    while ($row = $result->fetch_assoc()) {
        echo "<tr>";
        echo "<td>$row[EMPNO]</td> ";
        echo "<td>$row[ENAME]</td>";
        echo "<td>$row[JOB]</td>";
        echo "<td>$row[SAL]</td>";
        echo "<td>$row[COMM]</td>";
        echo "<td>$row[HIREDATE]</td>";
        echo "<td>$row[DEPTNO]</td>";
        echo "<td>$row[MGR]</td>";
        echo "</tr>";
    }
    echo "</table>";
} else {
    echo "No employee data found.";
}

// Display a link to go back to the previous page
echo "<br><br><a href='browse_employee.php'> <button> Go Back to Search Employee</a>";
echo "<br><br><a href='dashboard.php'><button> Go Back to Dashboard</button></a>";
echo "</body>";
echo "</html>";
?>