<?php
if (session_status() === PHP_SESSION_NONE) {
    session_start();
}
if (!isset($_SESSION['logged_in'])) {
    // If the user is not logged in, redirect them to the login page
    header('Location: http://localhost/empCURD/index.php');
	die();
	exit();
}
// Check if the user is logged in



if (isset($_POST["delete"])) {
    $empno = $_POST["empno"] ?? null;
// I used the null coalescing operator (??) to provide a default value of null if the key does not exist 
// in the $_POST array. Then, I checked if $empno is not null before calling the deleteEmployee function. 
// If $empno is null, I displayed an error message.
    if ($empno !== null) {
        deleteEmployee($conn, $empno);
    } else {
        echo "EMPNO is required";
    }
}
?>


<form action="" method="post">
    <h2>Delete Employee</h2>
    <table>
        <tr>
            <td>EMPNO:</td>
            <td><input type="number" name="empno"></td>
        </tr>
        <tr>
            <td><input type="submit" name="delete" value="Delete Employee"></td>
        </tr>
    </table>
</form>
