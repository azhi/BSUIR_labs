-- COMMON
SELECT e.BusinessEntityID, e.JobTitle, max(p.Rate) AS MaxRate
  FROM HumanResources.Employee e
  INNER JOIN HumanResources.EmployeePayHistory p ON e.BusinessEntityID = p.BusinessEntityID
  GROUP BY e.BusinessEntityID, e.JobTitle


-- CTE
WITH EmployeesMaxRate_CTE AS
(
  SELECT e.BusinessEntityID, e.JobTitle, max(p.Rate) AS MaxRate
    FROM HumanResources.Employee e
    INNER JOIN HumanResources.EmployeePayHistory p ON e.BusinessEntityID = p.BusinessEntityID
    GROUP BY e.BusinessEntityID, e.JobTitle
)
SELECT * FROM EmployeesMaxRate_CTE


-- TABLE VARIABLE
DECLARE @EmployeesMaxRate_T TABLE
(
  BusinessEntityID int,
  JobTitle nvarchar(50),
  MaxRate money
)

INSERT INTO @EmployeesMaxRate_T (BusinessEntityID, JobTitle, MaxRate)
  SELECT e.BusinessEntityID, e.JobTitle, max(p.Rate) AS MaxRate
    FROM HumanResources.Employee e
    INNER JOIN HumanResources.EmployeePayHistory p ON e.BusinessEntityID = p.BusinessEntityID
    GROUP BY e.BusinessEntityID, e.JobTitle

SELECT * FROM @EmployeesMaxRate_T

-- TEMPORARY TABLE
CREATE TABLE #EmployeesMaxRate_T
(
  BusinessEntityID int,
  JobTitle nvarchar(50),
  MaxRate money
)

INSERT INTO #EmployeesMaxRate_T (BusinessEntityID, JobTitle, MaxRate)
  SELECT e.BusinessEntityID, e.JobTitle, max(p.Rate) AS MaxRate
    FROM HumanResources.Employee e
    INNER JOIN HumanResources.EmployeePayHistory p ON e.BusinessEntityID = p.BusinessEntityID
    GROUP BY e.BusinessEntityID, e.JobTitle

SELECT * FROM #EmployeesMaxRate_T
