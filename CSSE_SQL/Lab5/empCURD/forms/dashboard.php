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

include_once('db_config.php');
include_once('db_functions.php');

if (isset($_POST["add"])) {
    include("add_form.php");
} elseif (isset($_POST["browse_data"])) {
    include("Browse_employee.php");
} elseif (isset($_POST["view"])) {
    displayEmployees($conn);
} elseif (isset($_POST["update_DATA"])) {
    include("update_form.php");
} elseif (isset($_POST["delete"])) {
    include("delete_form.php");
} elseif (isset($_POST["logout"])) {
    include("logout.php");
}
?>
<head>
<link href="style.css" rel="stylesheet" type="text/css">
</head>
<form action="" method="post">
    <h2>Employee Management System</h2>
    <input type="hidden" name="action" value="main">
	<div class="dashboard">
	<h1>Dashboard</h1>
    <table>
        <tr>
            <td><input type="submit" name="add" value="Add Employee"></td>
            <td><input type="submit" name="view" value="View Employees"></td>
			<td><input type="submit" name="browse_data" value="Browse Employees Data"></td>
            <td><input type="submit" name="update_DATA" value="Update Employee"></td>
            <td><input type="submit" name="delete" value="Delete Employee"></td>
			<td><input type="submit" name="logout" value="Logout"></td>
        </tr>
    </table>
</form>

  <table>
    <!-- table content -->
  </table>
 </div>
