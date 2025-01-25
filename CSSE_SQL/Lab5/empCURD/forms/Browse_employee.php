<?php
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

include_once('db_functions.php');

// Get the total number of employees
$conn = new mysqli('localhost', 'root', '', 'abc_company');
$sqlq = "SELECT COUNT(*) FROM EMP";
$result = $conn->query($sqlq);
$row = $result->fetch_assoc();
$total_employees = $row["COUNT(*)"];

// Display the total number of employees
echo "Total Employees: $total_employees<br><br>";

// Get the current employee ID
$current_employee_id = $_GET['empno'] ?? '';

// Get the current employee data
$sqlq = "SELECT * FROM EMP WHERE EMPNO = '$current_employee_id'";
$result = $conn->query($sqlq);

// Check if the query returned any results
if ($result->num_rows > 0) {
    $row = $result->fetch_assoc();

    // Display the current employee data
    echo "<table>";
    echo "<tr><td>EMPNO:</td><td>$row[EMPNO]</td></tr>";
    echo "<tr><td>ENAME:</td><td>$row[ENAME]</td></tr>";
    echo "<tr><td>JOB:</td><td>$row[JOB]</td></tr>";
    echo "<tr><td>SAL:</td><td>$row[SAL]</td></tr>";
    echo "<tr><td>COMM:</td><td>$row[COMM]</td></tr>";
    echo "<tr><td>HIREDATE:</td><td>$row[HIREDATE]</td></tr>";
    echo "<tr><td>DEPTNO:</td><td>$row[DEPTNO]</td></tr>";
    echo "<tr><td>MGR:</td><td>$row[MGR]</td></tr>";
    echo "</table>";
} else {
    echo "No employee data found for the given ID.";
}

// Display a link to browse all employees
echo "<br><br><a href='browse_all_employees.php'>Browse All Employees</a>";

// Display a link to search for an employee by ID
echo "<br><br><form action='browse_employee.php' method='get'>";
echo "<input type='text' name='empno' placeholder='Enter Employee ID'>";
echo "<input type='submit' value='Search'>";
echo "</form>";
?>
