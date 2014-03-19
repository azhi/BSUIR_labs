IF OBJECT_ID('dbo.Person', 'U') IS NOT NULL DROP TABLE dbo.Person

CREATE TABLE dbo.Person
(
  ID bigint identity(10, 10),
  BusinessEntityID int not null,
  PersonType nchar(2) not null,
  NameStyle NameStyle not null,
  Title nvarchar(8) null,
  FirstName Name not null,
  MiddleName Name null,
  LastName Name not null,
  Suffix nvarchar(10) null DEFAULT 'N/A',
  EmailPromotion int not null,
  ModifiedDate datetime not null,

  CONSTRAINT pk_id PRIMARY KEY (ID),
  CONSTRAINT title_mr_or_ms CHECK(Title = 'Mr.' OR Title = 'Ms.'),
)

INSERT INTO dbo.Person
  SELECT p.BusinessEntityID, p.PersonType, p.NameStyle, p.Title,
         p.FirstName, p.MiddleName, p.LastName, p.Suffix, p.EmailPromotion, p.ModifiedDate
  FROM Person.Person p
  INNER JOIN HumanResources.Employee e ON e.BusinessEntityID = p.BusinessEntityID
  INNER JOIN HumanResources.EmployeeDepartmentHistory dh ON dh.BusinessEntityID = e.BusinessEntityID
  INNER JOIN HumanResources.Department d ON d.DepartmentID = dh.DepartmentID
  WHERE dh.EndDate IS NULL AND d.Name != 'Executive'


 ALTER TABLE dbo.Person
   ALTER COLUMN Suffix nvarchar(5) null

SELECT TOP 10 * FROM dbo.Person
