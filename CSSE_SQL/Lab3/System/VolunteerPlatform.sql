CREATE DATABASE IF NOT EXISTS VolunteerPlatform;
USE VolunteerPlatform;

CREATE TABLE IF NOT EXISTS User (
  UserID INT AUTO_INCREMENT PRIMARY KEY,
  Username VARCHAR(50) NOT NULL UNIQUE,
  Password VARCHAR(255) NOT NULL,
  Email VARCHAR(100) NOT NULL UNIQUE,
  Role ENUM('User', 'Admin') NOT NULL DEFAULT 'User'
);

CREATE TABLE IF NOT EXISTS Activity (
  ActivityID INT AUTO_INCREMENT PRIMARY KEY,
  Title VARCHAR(100) NOT NULL,
  Description TEXT,
  Location VARCHAR(255),
  Time DATETIME NOT NULL,
  MaxParticipants INT NOT NULL,
  Requirements TEXT,
  Status ENUM('Open', 'Closed') NOT NULL DEFAULT 'Open',
  CurrentParticipants INT DEFAULT 0 -- 添加这一行
);

CREATE TABLE IF NOT EXISTS Application (
  ApplicationID INT AUTO_INCREMENT PRIMARY KEY,
  UserID INT NOT NULL,
  ActivityID INT NOT NULL,
  ApplyTime DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
  Status ENUM('Pending', 'Accepted', 'Rejected') NOT NULL DEFAULT 'Pending',
  FOREIGN KEY (UserID) REFERENCES User(UserID),
  FOREIGN KEY (ActivityID) REFERENCES Activity(ActivityID)
);

CREATE TABLE IF NOT EXISTS Review (
  ReviewID INT AUTO_INCREMENT PRIMARY KEY,
  ApplicationID INT NOT NULL,
  ReviewTime DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
  ReviewerID INT NOT NULL,
  Comments TEXT,
  FOREIGN KEY (ApplicationID) REFERENCES Application(ApplicationID),
  FOREIGN KEY (ReviewerID) REFERENCES User(UserID)
);

CREATE TABLE IF NOT EXISTS Notification (
  NotificationID INT AUTO_INCREMENT PRIMARY KEY,
  UserID INT NOT NULL,
  Content TEXT NOT NULL,
  SendTime DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (UserID) REFERENCES User(UserID)
);

CREATE TABLE IF NOT EXISTS AdminLog (
  LogID INT AUTO_INCREMENT PRIMARY KEY,
  AdminID INT NOT NULL,
  Action TEXT NOT NULL,
  Time DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (AdminID) REFERENCES User(UserID)
);

CREATE TABLE IF NOT EXISTS Feedback (
  FeedbackID INT AUTO_INCREMENT PRIMARY KEY,
  UserID INT NOT NULL,
  ActivityID INT NOT NULL,
  Content TEXT NOT NULL,
  Time DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (UserID) REFERENCES User(UserID),
  FOREIGN KEY (ActivityID) REFERENCES Activity(ActivityID)
);

CREATE TABLE IF NOT EXISTS Certificate (
  CertificateID INT AUTO_INCREMENT PRIMARY KEY,
  UserID INT NOT NULL,
  ActivityID INT NOT NULL,
  IssueTime DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
  FOREIGN KEY (UserID) REFERENCES User(UserID),
  FOREIGN KEY (ActivityID) REFERENCES Activity(ActivityID)
);

-- 创建索引
CREATE INDEX idx_activity_time ON Activity(Time);
CREATE INDEX idx_application_status ON Application(Status);

-- 创建视图
CREATE VIEW UserApplications AS
SELECT u.Username, a.Title, ap.ApplyTime, ap.Status
FROM User u
JOIN Application ap ON u.UserID = ap.UserID
JOIN Activity a ON ap.ActivityID = a.ActivityID;

-- 创建触发器
DELIMITER //
CREATE TRIGGER update_activity_status
AFTER INSERT ON Application
FOR EACH ROW
BEGIN
  UPDATE Activity
  SET Status = 'Closed'
  WHERE ActivityID = NEW.ActivityID AND (SELECT COUNT(*) FROM Application WHERE ActivityID = NEW.ActivityID) >= (SELECT MaxParticipants FROM Activity WHERE ActivityID = NEW.ActivityID);
END; //
DELIMITER ;

-- 创建存储过程
DELIMITER //
CREATE PROCEDURE GetApplicationStatus(IN appID INT)
BEGIN
  SELECT Status FROM Application WHERE ApplicationID = appID;
END; //
DELIMITER ;

-- 检查 Activity 表结构
DESCRIBE Activity;
ALTER TABLE Activity ADD COLUMN CurrentParticipants INT DEFAULT 0;