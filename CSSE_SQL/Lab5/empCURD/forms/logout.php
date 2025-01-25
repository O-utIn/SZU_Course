<?php
// Start the session
session_start();

// Unset the session variables
unset($_SESSION['logged_in']);
unset($_SESSION['username']);

// Destroy the session
session_destroy();

// Check if the session is still active
if (session_status() === PHP_SESSION_ACTIVE) {
    echo "Session not destroyed!";
    exit;
}

// Redirect the user to the login page
header('Location: http://localhost/empCURD/index.php');
exit;
?>