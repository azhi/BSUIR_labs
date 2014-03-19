UPDATE p
  SET p.Title = case
                  when e.Gender = 'M' then 'Mr.'
				  when e.Gender = 'F' then 'Ms.'
                  else null
                 end
  FROM dbo.Person p
  JOIN HumanResources.Employee e ON p.BusinessEntityID = e.BusinessEntityID

ALTER TABLE dbo.Person
  ADD FullName varchar(100)
GO

UPDATE dbo.Person
  SET FullName = Title + ' ' + FirstName + ' ' + LastName


DELETE FROM dbo.Person
WHERE LEN(FullName) > 20

ALTER TABLE dbo.Person DROP CONSTRAINT pk_id
ALTER TABLE dbo.Person DROP CONSTRAINT title_mr_or_ms
DECLARE @ConstraintName nvarchar(255)
SELECT @ConstraintName = Name FROM sys.default_constraints WHERE PARENT_OBJECT_ID = OBJECT_ID('dbo.Person') AND PARENT_COLUMN_ID = (SELECT column_id FROM sys.columns WHERE NAME = N'Suffix' AND object_id = OBJECT_ID(N'dbo.Person'))
IF @ConstraintName IS NOT NULL
    EXEC('ALTER TABLE dbo.Person DROP CONSTRAINT ' + @ConstraintName)
ALTER TABLE dbo.Person DROP COLUMN ID
GO

SELECT TOP 10 * FROM dbo.Person

DROP TABLE dbo.Person

