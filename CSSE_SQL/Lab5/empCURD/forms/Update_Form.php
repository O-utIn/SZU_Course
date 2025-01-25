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

if (isset($_POST['populate']) && $_POST['populate'] == 'true') {
    $empno = $_POST['empno'] ?? '';
    $sqlq = "SELECT * FROM EMP WHERE EMPNO = '$empno'";
    $result = $conn->query($sqlq);
    if ($result->num_rows > 0) {
        $row = $result->fetch_assoc();
        $data = array(
            'ename' => $row['ENAME'],
            'job' => $row['JOB'],
            'sal' => $row['SAL'],
            'comm' => $row['COMM'],
            'hiredate' => $row['HIREDATE'],
            'deptno' => $row['DEPTNO'],
            'mgr' => $row['MGR']
        );
        echo json_encode($data);
        exit;
    } else {
        echo json_encode(array('error' => 'Employee not found'));
        exit;
    }
}

if (isset($_POST["update_DATA"])) {
    $empno = $_POST["empno"]  ?? '';
    $ename = $_POST["ename"] ?? '';
    $job = $_POST["job"] ?? '';
    $sal = $_POST["sal"] ?? '';
    $comm = $_POST["comm"] ?? '';
    $hiredate = $_POST["hiredate"] ?? '';
    $deptno = $_POST["deptno"] ?? '';
    $mgr = $_POST["mgr"] ?? '';
    if (updateEmployee($conn, $empno, $ename, $job, $sal, $comm, $hiredate, $deptno, $mgr)) {
        echo "Employee updated successfully";
    } else {
        echo "Error updating employee";
    }
}

$empno_list = array();
$sqlq = "SELECT EMPNO FROM EMP";
$result = $conn->query($sqlq);
if ($result->num_rows > 0) {
    while($row = $result->fetch_assoc()) {
        $empno_list[] = $row["EMPNO"];
    }
}
?>

<form action="<?php echo $_SERVER['PHP_SELF']; ?>" method="post">
    <h2>Update Employee</h2>
    <table>
        <tr>
            <td>EMPNO:</td>
            <td>
                <select name="empno" id="empno" onchange="populateForm(this.value)">
                    <option value="">Select EMPNO</option>
                    <?php foreach ($empno_list as $empno) { ?>
                        <option value="<?php echo $empno; ?>"><?php echo $empno; ?></option>
                    <?php } ?>
                </select>
            </td>
        </tr>
        <tr>
            <td>ENAME:</td>
            <td><input type="text" name="ename" id="ename"></td>
        </tr>
        <tr>
            <td>JOB:</td>
            <td><input type="text" name="job" id="job"></td>
        </tr>
        <tr>
            <td>SAL:</td>
            <td><input type="number" name="sal" id="sal"></td>
        </tr>
        <tr>
            <td>COMM:</td>
            <td><input type="number" name="comm" id="comm"></td>
        </tr>
        <tr>
            <td>HIREDATE:</td>
            <td><input type="date" name="hiredate" id="hiredate"></td>
        </tr>
        <tr>
            <td>DEPTNO:</td>
            <td><input type="number" name="deptno" id="deptno"></td>
        </tr>
        <tr>
            <td>MGR:</td>
            <td><input type="number" name="mgr" id="mgr"></td>
        </tr>
        <tr>
            <td><input type="submit" name="update_DATA" value="Update Employee"></td>
        </tr>
    </table>
</form>

<script>
    function populateForm(empno) {
        var xhr = new XMLHttpRequest();
        xhr.open('POST', '<?php echo $_SERVER['PHP_SELF']; ?>', true);
        xhr.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
        xhr.onload = function() {
            if (xhr.status === 200) {
                var response = JSON.parse(xhr.responseText);
                if (response.error) {
                    alert(response.error);
                } else {
                    document.getElementById('ename').value = response.ename;
                    document.getElementById('job').value = response.job;
                    document.getElementById('sal').value = response.sal;
                    document.getElementById('comm').value = response.comm;
                    document.getElementById('hiredate').value = response.hiredate;
                    document.getElementById('deptno').value = response.deptno;
                    document.getElementById('mgr').value = response.mgr;
                }
            }
        };
        xhr.send('empno=' + empno + '&populate=true');
    }
</script>