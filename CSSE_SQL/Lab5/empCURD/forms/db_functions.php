<?php
function addEmployee($conn, $empno, $ename, $job, $sal, $comm, $hiredate, $deptno, $mgr) {
    $sqlq = "INSERT INTO EMP (EMPNO, ENAME, JOB, SAL, COMM, HIREDATE, DEPTNO, MGR) VALUES (?, ?, ?, ?, ?, ?, ?, ?)";
    $stmt = $conn->prepare($sqlq);
    $stmt->bind_param("isddssdd", $empno, $ename, $job, $sal, $comm, $hiredate, $deptno, $mgr);
    $stmt->execute();
    $stmt->close();
}

function updateEmployee($conn, $empno, $ename, $job, $sal, $comm, $hiredate, $deptno, $mgr) {
    $sqlq = "UPDATE EMP SET ENAME = ?, JOB = ?, SAL = ?, COMM = ?, HIREDATE = ?, DEPTNO = ?, MGR = ? WHERE EMPNO = ?";
    $stmt = $conn->prepare($sqlq);
    $stmt->bind_param("ssddssdd", $ename, $job, $sal, $comm, $hiredate, $deptno, $mgr, $empno);
    $stmt->execute();
    $stmt->close();
}

function deleteEmployee($conn, $empno) {
    $sqlq = "DELETE FROM EMP WHERE EMPNO = ?";
    $stmt = $conn->prepare($sqlq);
    $stmt->bind_param("d", $empno);
    $stmt->execute();
    $stmt->close();
}

function displayEmployees($conn) {
    $sqlq = "SELECT * FROM EMP";
    $result = $conn->query($sqlq);
    if ($result->num_rows > 0) {
        while($row = $result->fetch_assoc()) {
            echo "EMPNO: " . $row["EMPNO"]. " - ENAME: " . $row["ENAME"]. " - JOB: " . $row["JOB"]. " - SAL: " . $row["SAL"]. " - COMM: " . $row["COMM"]. " - HIREDATE: " . $row["HIREDATE"]. " - DEPTNO: " . $row["DEPTNO"]. " - MGR: " . $row["MGR"]. "<br>";
        }
    } else {
        echo "0 results";
    }
}
?>