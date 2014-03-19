IF OBJECT_ID ('dbo.getEmployeesMaxRates', 'IF') IS NOT NULL
    DROP FUNCTION dbo.getEmployeesMaxRates;
GO

CREATE FUNCTION dbo.getEmployeesMaxRates(@Rate AS money)
RETURNS table
AS
  RETURN(
    SELECT e.BusinessEntityID, e.NationalIDNumber, e.LoginID, e.JobTitle, e.Gender
	  FROM HumanResources.EmployeePayHistory eph
	  JOIN HumanResources.Employee e ON eph.BusinessEntityID = e.BusinessEntityID
	  GROUP BY e.BusinessEntityID, e.NationalIDNumber, e.LoginID, e.JobTitle, e.Gender
	  HAVING max(eph.Rate) = @Rate
  )
GO

SELECT distinct(eph.Rate), mr.*
  FROM HumanResources.EmployeePayHistory eph
  CROSS APPLY dbo.getEmployeesMaxRates(eph.Rate) AS mr
  ORDER BY eph.Rate desc

SELECT distinct(eph.Rate), mr.*
  FROM HumanResources.EmployeePayHistory eph
  OUTER APPLY dbo.getEmployeesMaxRates(eph.Rate) AS mr
  ORDER BY eph.Rate desc
