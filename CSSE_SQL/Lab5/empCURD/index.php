<?php
// Start the session
session_start();

// Check if the user has submitted the login form
if (isset($_POST['username']) && isset($_POST['password'])) {
    // Connect to the database
    $conn = new mysqli('localhost', 'root', '03062813', 'abc_company');

    // Check if the connection was successful
    if ($conn->connect_error) {
        die("Connection failed: " . $conn->connect_error);
    }
	
	// Prepare the query
	$stmt = $conn->prepare("SELECT * FROM login WHERE username = ? AND password = ?");

	// Bind the parameters
	$stmt->bind_param("ss", $_POST['username'], $_POST['password']);

	// Execute the query
	$stmt->execute();
   
    // Get the result
    $result = $stmt->get_result();

    // Check if the query was successful
    if ($result->num_rows == 1) {
        // If the query was successful, create a session variable to store the user's login information
        $_SESSION['logged_in'] = true;
        $_SESSION['username'] = $_POST['username'];

        // Redirect the user to the dashboard page
        header('Location: http://localhost/empCURD/forms/dashboard.php');
        exit;
    } else {
        // If the query was not successful, display an error message
        echo 'Invalid username or password';
    }

    // Close the database connection
    $conn->close();
}

// If the user has not submitted the login form, display the login form
?>

<!DOCTYPE html>
<html>
<head>
    <title>Login Page</title>
    <link href="style.css" rel="stylesheet" type="text/css">
</head>
<body>
    <div id="main">
        <h1> Please Login to Access the Employee Information</h1>
        <div id="login">
            <h2>Login Form</h2>
            <form action="<?php echo $_SERVER['PHP_SELF']; ?>" method="post">
                <label>UserName :</label>
                <input id="username" name="username" placeholder="username" type="text">
                <label>Password :</label>
                <input id="password" name="password" placeholder="******" type="password">
                <input name="submit" type="submit" value=" Login ">
            </form>
        </div>
    </div>
</body>
</html>
