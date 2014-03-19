IF OBJECT_ID('dbo.Person', 'U') IS NOT NULL DROP TABLE dbo.SalesPerson
IF OBJECT_ID('dbo.SalesPersonTrig', 'TR') IS NOT NULL DROP TRIGGER dbo.SalesPersonTrig
IF OBJECT_ID('dbo.SalesPersonInsTrig', 'TR') IS NOT NULL DROP TRIGGER dbo.SalesPersonInsTrig
IF OBJECT_ID('dbo.SalesPersonUpdateTrig', 'TR') IS NOT NULL DROP TRIGGER dbo.SalesPersonUpdateTrig
IF OBJECT_ID('dbo.SalesPersonDeleteTrig', 'TR') IS NOT NULL DROP TRIGGER dbo.SalesPersonDeleteTrig

SELECT TOP 0 sp.*, spqh.SalesQuota AS HistSalesQuota, st.CountryRegionCode INTO dbo.SalesPerson
  FROM Sales.SalesPerson sp
  JOIN Sales.SalesPersonQuotaHistory spqh ON sp.BusinessEntityID = spqh.BusinessEntityID
  LEFT JOIN Sales.SalesTerritory st ON sp.TerritoryID = st.TerritoryID
GO

CREATE TRIGGER dbo.SalesPersonTrig
ON dbo.SalesPerson
AFTER INSERT, UPDATE, DELETE
AS
BEGIN
  DECLARE @pk_iter int

  IF EXISTS(SELECT * FROM DELETED)
  BEGIN
    IF EXISTS(SELECT * FROM INSERTED)
	BEGIN
	  -- Update Action.

	  SELECT @pk_iter = MIN(BusinessEntityID) FROM INSERTED
	  WHILE (ISNULL(@pk_iter, 0) <> 0)
	  BEGIN
	    DECLARE @i_SalesYTD money
		SELECT @i_SalesYTD = SalesYTD FROM INSERTED WHERE BusinessEntityID = @pk_iter

		DECLARE @d_SalesYTD money
		SELECT @d_SalesYTD = SalesYTD FROM DELETED WHERE BusinessEntityID = @pk_iter

	    IF ABS(@i_SalesYTD - @d_SalesYTD) > 1000
	      UPDATE dbo.SalesPerson SET SalesYTD = @d_SalesYTD
		                         WHERE BusinessEntityID = @pk_iter

		SELECT @pk_iter = MIN(BusinessEntityID)
		  FROM INSERTED
		  WHERE BusinessEntityID > @pk_iter
      END
    END
	ELSE
	BEGIN
	  -- Delete Action

	  DECLARE @del_count bigint
	  DECLARE @rows_count bigint
	  SELECT @del_count = count(*) FROM DELETED
	  SELECT @rows_count = count(*) FROM dbo.SalesPerson

	  PRINT CONCAT('USER: ', CURRENT_USER,
	               CHAR(10), 'DATE: ', getdate(),
				   CHAR(10), 'DEL COUNT: ', @del_count,
				   CHAR(10), 'ROWS LEFT: ', @rows_count)
    END
  END
  ELSE
    IF EXISTS(SELECT * FROM INSERTED)
	BEGIN
	  -- Insert Action

	  SELECT @pk_iter = MIN(BusinessEntityID) FROM INSERTED
	  WHILE (ISNULL(@pk_iter, 0) <> 0)
	  BEGIN

	    DECLARE @i_Bonus money
		SELECT @i_Bonus = Bonus FROM INSERTED WHERE BusinessEntityID = @pk_iter

	    IF @i_Bonus < 100
	      UPDATE dbo.SalesPerson SET Bonus = @i_Bonus + 10
		                         WHERE BusinessEntityID = @pk_iter

	    SELECT @pk_iter = MIN(BusinessEntityID)
		  FROM INSERTED
		  WHERE BusinessEntityID > @pk_iter

      END
	END
END
GO

INSERT INTO dbo.SalesPerson
  VALUES (1, 2, 500000, 100, 50, 50000, 30000, '1E0A7233-3064-4F58-88BA-4C6586C17168', '20131107', 400000, 'US'),
         (2, 2, 200000, 20, 30, 20000, 10000, '1E0A7233-3064-4F58-88BA-4C6586C17169', '20131107', 300000, 'FR'),
		 (3, 3, 600000, 40, 80, 10000, 10000, '1E0A7233-3064-4F58-88BA-4C6586C17170', '20131107', 100000, 'FR')

UPDATE dbo.SalesPerson
  SET SalesYTD = 50500

SELECT * FROM dbo.SalesPerson

DELETE dbo.SalesPerson
  WHERE TerritoryID = 2

SELECT * FROM dbo.SalesPerson
