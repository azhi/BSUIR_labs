IF OBJECT_ID ('dbo.vw_EmployeeAndPayHistory', 'V') IS NOT NULL DROP VIEW dbo.vw_EmployeeAndPayHistory;
IF OBJECT_ID('dbo.EmployeeAndPayHistoryManageTrig', 'TR') IS NOT NULL DROP TRIGGER dbo.EmployeeAndPayHistoryManageTrig
GO

CREATE VIEW dbo.vw_EmployeeAndPayHistory
AS
  SELECT e.BusinessEntityID, e.JobTitle, p.RateChangeDate, p.Rate, p.PayFrequency
    FROM HumanResources.Employee e
    INNER JOIN HumanResources.EmployeePayHistory p ON e.BusinessEntityID = p.BusinessEntityID
GO

CREATE TRIGGER dbo.EmployeeAndPayHistoryManageTrig
ON dbo.vw_EmployeeAndPayHistory
INSTEAD OF INSERT, UPDATE, DELETE
AS
BEGIN
  IF EXISTS(SELECT * FROM DELETED)
  BEGIN
    IF EXISTS(SELECT * FROM INSERTED)
	BEGIN
	  -- Update Action.

      UPDATE HumanResources.Employee
        SET BusinessEntityID = i.BusinessEntityID,
		    JobTitle = i.JobTitle
		  FROM HumanResources.Employee e
	      JOIN INSERTED i ON e.BusinessEntityID = i.BusinessEntityID

	  UPDATE HumanResources.EmployeePayHistory
        SET RateChangeDate = i.RateChangeDate,
		    Rate = i.Rate,
			PayFrequency = i.PayFrequency
		  FROM HumanResources.EmployeePayHistory p
	      JOIN INSERTED i ON p.BusinessEntityID = i.BusinessEntityID
    END
	ELSE
	BEGIN
	  -- Delete Action
      DELETE HumanResources.EmployeePayHistory
	    FROM HumanResources.EmployeePayHistory p
		JOIN DELETED d ON p.BusinessEntityID = d.BusinessEntityID
		WHERE p.RateChangeDate = d.RateChangeDate AND
		      p.BusinessEntityID = d.BusinessEntityID
    END
  END
  ELSE
    IF EXISTS(SELECT * FROM INSERTED)
	BEGIN
	  -- Insert Action

	  INSERT INTO HumanResources.Employee(BusinessEntityID, JobTitle)
	    SELECT i.BusinessEntityID, i.JobTitle
		FROM INSERTED i
		LEFT JOIN HumanResources.Employee e ON e.BusinessEntityID = i.BusinessEntityID
		WHERE e.BusinessEntityID IS NULL

	  INSERT INTO HumanResources.EmployeePayHistory(BusinessEntityID, RateChangeDate, Rate, PayFrequency)
        SELECT i.BusinessEntityID, i.RateChangeDate, i.Rate, i.PayFrequency
        FROM INSERTED i
		JOIN HumanResources.Employee e ON e.BusinessEntityID = i.BusinessEntityID
	END
  END
GO
