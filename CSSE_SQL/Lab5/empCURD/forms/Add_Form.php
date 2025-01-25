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
include_ONCE('db_functions.php');

if (isset($_POST["add"])) {
    $empno = $_POST["empno"] ?? null;
    $ename = $_POST["ename"] ?? null;
    $job = $_POST["job"] ?? null;
    $sal = $_POST["sal"] ?? null;
    $comm = $_POST["comm"] ?? null;
    $hiredate = $_POST["hiredate"] ?? null;
    $deptno = $_POST["deptno"] ?? null;
    $mgr = $_POST["mgr"] ?? null;

    if ($empno !== null && $ename !== null && $job !== null && $sal !== null && $hiredate !== null && $deptno !== null && $mgr !== null) {
        addEmployee($conn, $empno, $ename, $job, $sal, $comm, $hiredate, $deptno, $mgr);
    } else {
       echo "</h2> Please fill in all required fields </h2>"; 
    }
}
?>

<!DOCTYPE html>
<html>
<head>
    <title>Add Employee </title>
    <link href="style.css" rel="stylesheet" type="text/css">
</head>
<form action="" method="post">
    <h2>Add New Employee Information</h2>
    <table>
		<tr>
			<td>EMPNO:</td>
			<td><input type="number" name="empno"></td>
		</tr>

        <tr>
            <td>ENAME:</td>
            <td><input type="text" name="ename"></td>
        </tr>
        <tr>
            <td>JOB:</td>
            <td><input type="text" name="job"></td>
        </tr>
        <tr>
            <td>SAL:</td>
            <td><input type="number" name="sal"></td>
        </tr>
        <tr>
            <td>COMM:</td>
            <td><input type="number" name="comm"></td>
        </tr>
        <tr>
            <td>HIREDATE:</td>
            <td><input type="date" name="hiredate"></td>
        </tr>
        <tr>
            <td>DEPTNO:</td>
            <td><input type="number" name="deptno"></td>
        </tr>
        <tr>
            <td>MGR:</td>
            <td><input type="number" name="mgr"></td>
        </tr>
        <tr>
            <td><input type="submit" name="add" value="Add Employee"></td>
        </tr>
    </table>
</form>

</html>
