IF OBJECT_ID('dbo.Person', 'U') IS NOT NULL DROP TABLE dbo.SalesPerson
IF OBJECT_ID ('dbo.vw_SalesPerson', 'V') IS NOT NULL DROP VIEW dbo.vw_SalesPerson;

SELECT TOP 0 sp.*, spqh.SalesQuota AS HistSalesQuota, st.CountryRegionCode INTO dbo.SalesPerson
  FROM Sales.SalesPerson sp
  JOIN Sales.SalesPersonQuotaHistory spqh ON sp.BusinessEntityID = spqh.BusinessEntityID
  LEFT JOIN Sales.SalesTerritory st ON sp.TerritoryID = st.TerritoryID

ALTER TABLE dbo.SalesPerson
  ADD CONSTRAINT pk_business_entity_id PRIMARY KEY (BusinessEntityID)
ALTER TABLE dbo.SalesPerson
  ADD SalesQuotaDiff AS SalesQuota - HistSalesQuota
GO

CREATE VIEW dbo.vw_SalesPerson
AS
  SELECT * FROM dbo.SalesPerson
GO

INSERT INTO dbo.vw_SalesPerson(BusinessEntityID, TerritoryID, SalesQuota, Bonus,
                               CommissionPct, SalesYTD, SalesLastYear, rowguid,
							   ModifiedDate, HistSalesQuota, CountryRegionCode)
  VALUES (290, 7, 250000, 985,
		   0.016, 3121616.3202, 2396539.7601, 'F509E3D4-76C8-42AA-B353-90B7B8DB08DE',
		   '20060624', 908000, 'FR'),
         (276, 4, 250000, 2000,
		  0.015, 4251368.5497, 1439156.0291, '4DD9EEE4-8E81-4F8C-AF97-683394C1F7C0',
		  '20050624', 1124000.00, 'BY'),
         (100500, 50, 100, 50,
	      25, 12.5, 11, '1E0A7233-3064-4F58-88BA-4C6586C17169',
          '20130707', 100, 'US')

SELECT * FROM dbo.vw_SalesPerson

MERGE dbo.vw_SalesPerson AS [dest]
USING (SELECT sp.BusinessEntityID, sp.TerritoryID, sp.SalesQuota, Bonus,
              CommissionPct, sp.SalesYTD, sp.SalesLastYear, sp.rowguid,
		      sp.ModifiedDate, spqh.SalesQuota AS HistSalesQuota, st.CountryRegionCode
		 FROM Sales.SalesPerson sp
         JOIN Sales.SalesPersonQuotaHistory spqh ON sp.BusinessEntityID = spqh.BusinessEntityID
         LEFT JOIN Sales.SalesTerritory st ON sp.TerritoryID = st.TerritoryID
         WHERE spqh.QuotaDate = (
           SELECT MAX(QuotaDate) FROM Sales.SalesPersonQuotaHistory
	         WHERE BusinessEntityID = sp.BusinessEntityID
         )
		 AND
         (
	       SELECT Count(*) FROM Sales.SalesPerson spc
             JOIN Sales.SalesOrderHeader soh ON soh.SalesPersonID = spc.BusinessEntityID
	       WHERE spc.BusinessEntityID = sp.BusinessEntityID AND soh.SubTotal > 20000
	     ) > 0
	  ) AS src
ON src.BusinessEntityID = dest.BusinessEntityID
WHEN MATCHED AND src.SalesYTD > 3000000 THEN
  UPDATE SET BusinessEntityID = src.BusinessEntityID,
             TerritoryID = src.TerritoryID,
             SalesQuota = src.SalesQuota,
             Bonus = src.Bonus,
             CommissionPct = src.CommissionPct,
             SalesYTD = src.SalesYTD,
             SalesLastYear = src.SalesLastYear,
             rowguid = src.rowguid,
             ModifiedDate = src.ModifiedDate,
			 HistSalesQuota = src.HistSalesQuota,
			 CountryRegionCode = src.CountryRegionCode
WHEN NOT MATCHED BY SOURCE THEN
  DELETE
WHEN NOT MATCHED BY TARGET THEN
  INSERT (BusinessEntityID, TerritoryID, SalesQuota, Bonus,
                         CommissionPct, SalesYTD, SalesLastYear, rowguid,
						 ModifiedDate, HistSalesQuota, CountryRegionCode)
    VALUES(src.BusinessEntityID, src.TerritoryID, src.SalesQuota, src.Bonus,
           src.CommissionPct, src.SalesYTD, src.SalesLastYear, src.rowguid,
		   src.ModifiedDate, src.HistSalesQuota, src.CountryRegionCode);

SELECT * FROM dbo.vw_SalesPerson
