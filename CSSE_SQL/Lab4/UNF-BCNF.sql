use mysql;

-- UNF
DROP TABLE IF EXISTS Invoice_UNF;
CREATE TABLE IF NOT EXISTS Invoice_UNF (
    hospital_name VARCHAR(100),
    invoice_date VARCHAR(20),
    invoice_number VARCHAR(20),
    customer_name VARCHAR(100),
    customer_address VARCHAR(255),
    pet_name VARCHAR(50),
    procedure_name VARCHAR(100),
    amount DECIMAL(10, 2),
    total DECIMAL(10, 2),
    tax_percentage DECIMAL(5, 2),
    amount_owing DECIMAL(10, 2)
);

INSERT INTO Invoice_UNF (hospital_name, invoice_date, invoice_number, customer_name, customer_address, pet_name, procedure_name, amount, total, tax_percentage, amount_owing)
VALUES
('HILLTOP ANIMAL HOSPITAL', 'JAN 13/2002', '987', 'MR. RICHARD COOK', '123 THIS STREET, MY CITY, ONTARIO, Z5Z 6G6', 'ROVER', 'RABIES VACCINATION', 30.00, 54.00, 8.00, 58.32),
('HILLTOP ANIMAL HOSPITAL', 'JAN 13/2002', '987', 'MR. RICHARD COOK', '123 THIS STREET, MY CITY, ONTARIO, Z5Z 6G6', 'MORRIS', 'RABIES VACCINATION', 24.00, 54.00, 8.00, 58.32);

select * from Invoice_UNF;


-- 1NF
DROP TABLE IF EXISTS Invoices_1NF;
CREATE TABLE IF NOT EXISTS Invoices_1NF (
    InvoiceID INT AUTO_INCREMENT PRIMARY KEY,
    InvoiceNumber VARCHAR(20),
    InvoiceDate DATE,
    CustomerFirstName VARCHAR(50),
    CustomerLastName VARCHAR(50),
    CustomerAddress VARCHAR(255),
    CustomerCity VARCHAR(100),
    CustomerProvince VARCHAR(50),
    CustomerPostalCode VARCHAR(10),
    PetName VARCHAR(50),
    ProcedureName VARCHAR(100),
    Amount DECIMAL(10, 2),
    Total DECIMAL(10, 2),
    TaxPercentage DECIMAL(5, 2),
    AmountOwing DECIMAL(10, 2)
);
INSERT INTO Invoices_1NF (InvoiceNumber, InvoiceDate, CustomerFirstName, CustomerLastName, CustomerAddress, CustomerCity, CustomerProvince, CustomerPostalCode, PetName, ProcedureName, Amount, Total, TaxPercentage, AmountOwing)
VALUES ('987', '2002-01-13', 'RICHARD', 'COOK', '123 THIS STREET, MY CITY, ONTARIO, Z5Z 6G6', 'MY CITY', 'ONTARIO', 'Z5Z 6G6', 'ROVER', 'RABIES VACCINATION', 30.00, 54.00, 8.00, 58.32),
('987', '2002-01-13', 'RICHARD', 'COOK', '123 THIS STREET, MY CITY, ONTARIO, Z5Z 6G6', 'MY CITY', 'ONTARIO', 'Z5Z 6G6', 'MORRIS', 'RABIES VACCINATION', 24.00, 54.00, 8.00, 58.32);
SELECT * FROM Invoices_1NF;


-- 2NF
DROP TABLE IF EXISTS Customers_2NF;
CREATE TABLE IF NOT EXISTS Customers_2NF (
    CustomerID INT AUTO_INCREMENT PRIMARY KEY,
    FirstName VARCHAR(50),
    LastName VARCHAR(50),
    Address VARCHAR(255),
    City VARCHAR(100),
    Province VARCHAR(50),
    PostalCode VARCHAR(10)
);
INSERT INTO Customers_2NF (FirstName, LastName, Address, City, Province, PostalCode)
VALUES ('RICHARD', 'COOK', '123 THIS STREET, MY CITY, ONTARIO, Z5Z 6G6', 'MY CITY', 'ONTARIO', 'Z5Z 6G6');
SELECT * FROM Customers_2NF;


DROP TABLE IF EXISTS Invoices_2NF;
CREATE TABLE IF NOT EXISTS Invoices_2NF (
    InvoiceID INT AUTO_INCREMENT PRIMARY KEY,
    InvoiceNumber VARCHAR(20),
    InvoiceDate DATE,
    CustomerID INT,
    TotalAmount DECIMAL(10, 2),
    FOREIGN KEY (CustomerID) REFERENCES Customers_2NF(CustomerID)
);
INSERT INTO Invoices_2NF (InvoiceNumber, InvoiceDate, CustomerID, TotalAmount)
VALUES ('987', '2002-01-13', 1, 54.00);
SELECT * FROM Invoices_2NF;


DROP TABLE IF EXISTS Pets_2NF;
CREATE TABLE IF NOT EXISTS Pets_2NF (
    PetID INT AUTO_INCREMENT PRIMARY KEY,
    PetName VARCHAR(50),
    CustomerID INT,
    FOREIGN KEY (CustomerID) REFERENCES Customers_2NF(CustomerID)
);
INSERT INTO Pets_2NF (PetName, CustomerID)
VALUES ('ROVER', 1), ('MORRIS', 1);
SELECT * FROM Pets_2NF;


DROP TABLE IF EXISTS Procedures_2NF;
CREATE TABLE IF NOT EXISTS Procedures_2NF (
    ProcedureID INT AUTO_INCREMENT PRIMARY KEY,
    ProcedureName VARCHAR(100)
);
INSERT INTO Procedures_2NF (ProcedureName)
VALUES ('RABIES VACCINATION');
SELECT * FROM Procedures_2NF;


DROP TABLE IF EXISTS InvoiceDetails_2NF;
CREATE TABLE IF NOT EXISTS InvoiceDetails_2NF (
    InvoiceDetailID INT AUTO_INCREMENT PRIMARY KEY,
    InvoiceID INT,
    PetID INT,
    ProcedureID INT,
    Amount DECIMAL(10, 2),
    CONSTRAINT fk_invoice_details_invoice_id FOREIGN KEY (InvoiceID) REFERENCES Invoices_2NF(InvoiceID),
    CONSTRAINT fk_invoice_details_pet_id FOREIGN KEY (PetID) REFERENCES Pets_2NF(PetID),
    CONSTRAINT fk_invoice_details_procedure_id FOREIGN KEY (ProcedureID) REFERENCES Procedures_2NF(ProcedureID)
);
INSERT INTO InvoiceDetails_2NF (InvoiceID, PetID, ProcedureID, Amount)
VALUES (987, 1, 1, 30.00), (987, 2, 1, 24.00);
SELECT * FROM InvoiceDetails_2NF;
-- 3NF
DROP TABLE IF EXISTS Customers_3NF;
CREATE TABLE IF NOT EXISTS Customers_3NF (
    CustomerID INT AUTO_INCREMENT PRIMARY KEY,
    FirstName VARCHAR(50),
    LastName VARCHAR(50),
    Address VARCHAR(255),
    City VARCHAR(100),
    Province VARCHAR(50),
    PostalCode VARCHAR(10)
);
INSERT INTO Customers_3NF (FirstName, LastName, Address, City, Province, PostalCode)
VALUES ('RICHARD', 'COOK', '123 THIS STREET, MY CITY, ONTARIO, Z5Z 6G6', 'MY CITY', 'ONTARIO', 'Z5Z 6G6');
SELECT * FROM Customers_3NF;


DROP TABLE IF EXISTS Invoices_3NF;
CREATE TABLE IF NOT EXISTS Invoices_3NF (
    InvoiceID INT AUTO_INCREMENT PRIMARY KEY,
    InvoiceNumber VARCHAR(20),
    InvoiceDate DATE,
    CustomerID INT,
    FOREIGN KEY (CustomerID) REFERENCES Customers_3NF(CustomerID)
);
INSERT INTO Invoices_3NF (InvoiceNumber, InvoiceDate, CustomerID)
VALUES ('987', '2002-01-13', 1);
SELECT * FROM Invoices_3NF;

DROP TABLE IF EXISTS Pets_3NF;
CREATE TABLE IF NOT EXISTS Pets_3NF (
    PetID INT AUTO_INCREMENT PRIMARY KEY,
    PetName VARCHAR(50),
    CustomerID INT,
    FOREIGN KEY (CustomerID) REFERENCES Customers_3NF(CustomerID)
);
INSERT INTO Pets_3NF (PetName, CustomerID)
VALUES ('ROVER', 1), ('MORRIS', 1);
SELECT * FROM Pets_3NF;


DROP TABLE IF EXISTS Procedures_3NF;
CREATE TABLE IF NOT EXISTS Procedures_3NF (
    ProcedureID INT AUTO_INCREMENT PRIMARY KEY,
    ProcedureName VARCHAR(100)
);
INSERT INTO Procedures_3NF (ProcedureName)
VALUES ('RABIES VACCINATION');
SELECT * FROM Procedures_3NF;


DROP TABLE IF EXISTS InvoiceDetails_3NF;
CREATE TABLE IF NOT EXISTS InvoiceDetails_3NF (
    InvoiceDetailID INT AUTO_INCREMENT PRIMARY KEY,
    InvoiceID INT,
    PetID INT,
    ProcedureID INT,
    Amount DECIMAL(10, 2),
    FOREIGN KEY (InvoiceID) REFERENCES Invoices_3NF(InvoiceID),
    FOREIGN KEY (PetID) REFERENCES Pets_3NF(PetID),
    FOREIGN KEY (ProcedureID) REFERENCES Procedures_3NF(ProcedureID)
);
INSERT INTO InvoiceDetails_3NF (InvoiceID, PetID, ProcedureID, Amount)
VALUES (1, 1, 1, 30.00), (1, 2, 1, 24.00);
SELECT * FROM InvoiceDetails_3NF;


-- BCNF


DROP TABLE IF EXISTS Customers_BCNF;
CREATE TABLE IF NOT EXISTS Customers_BCNF (
    CustomerID INT AUTO_INCREMENT PRIMARY KEY,
    FirstName VARCHAR(50),
    LastName VARCHAR(50),
    Address VARCHAR(255),
    City VARCHAR(100),
    Province VARCHAR(50),
    PostalCode VARCHAR(10)
);
INSERT INTO Customers_BCNF (FirstName, LastName, Address, City, Province, PostalCode)
VALUES ('RICHARD', 'COOK', '123 THIS STREET, MY CITY, ONTARIO, Z5Z 6G6', 'MY CITY', 'ONTARIO', 'Z5Z 6G6');
SELECT * FROM Customers_BCNF;


DROP TABLE IF EXISTS Invoices_BCNF;
CREATE TABLE IF NOT EXISTS Invoices_BCNF (
    InvoiceID INT AUTO_INCREMENT PRIMARY KEY,
    InvoiceNumber VARCHAR(20),
    InvoiceDate DATE,
    CustomerID INT,
    FOREIGN KEY (CustomerID) REFERENCES Customers_BCNF(CustomerID)
);
INSERT INTO Invoices_BCNF (InvoiceNumber, InvoiceDate, CustomerID)
VALUES ('987', '2002-01-13', 1);
SELECT * FROM Invoices_BCNF;


DROP TABLE IF EXISTS Pets_BCNF;
CREATE TABLE IF NOT EXISTS Pets_BCNF (
    PetID INT AUTO_INCREMENT PRIMARY KEY,
    PetName VARCHAR(50),
    CustomerID INT,
    FOREIGN KEY (CustomerID) REFERENCES Customers_BCNF(CustomerID)
);
INSERT INTO Pets_BCNF (PetName, CustomerID)
VALUES ('ROVER', 1), ('MORRIS', 1);
SELECT * FROM Pets_BCNF;


DROP TABLE IF EXISTS Procedures_BCNF;
CREATE TABLE IF NOT EXISTS Procedures_BCNF (
    ProcedureID INT AUTO_INCREMENT PRIMARY KEY,
    ProcedureName VARCHAR(100)
);
INSERT INTO Procedures_BCNF (ProcedureName)
VALUES ('RABIES VACCINATION');
SELECT * FROM Procedures_BCNF;


DROP TABLE IF EXISTS InvoiceDetails_BCNF;
CREATE TABLE IF NOT EXISTS InvoiceDetails_BCNF (
    InvoiceDetailID INT AUTO_INCREMENT PRIMARY KEY,
    InvoiceID INT,
    PetID INT,
    ProcedureID INT,
    Amount DECIMAL(10, 2),
    CONSTRAINT fk_invoice_details_id_invoice FOREIGN KEY (InvoiceID) REFERENCES Invoices_BCNF(InvoiceID),
    CONSTRAINT fk_invoice_details_id_pet FOREIGN KEY (PetID) REFERENCES Pets_BCNF(PetID),
    CONSTRAINT fk_invoice_details_id_procedure FOREIGN KEY (ProcedureID) REFERENCES Procedures_BCNF(ProcedureID)
);
INSERT INTO InvoiceDetails_BCNF (InvoiceID, PetID, ProcedureID, Amount)
VALUES (987, 1, 1, 30.00), (987, 2, 1, 24.00);